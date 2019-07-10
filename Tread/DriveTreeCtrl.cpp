// DriveTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "Tread3D.h"
#include "DriveTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ILI_HARD_DISK       0
#define ILI_FLOPPY          1
#define ILI_CD_ROM          2
#define ILI_NET_DRIVE       3
#define ILI_RAM_DRIVE       4
#define ILI_CLOSED_FOLDER   5
#define ILI_OPEN_FOLDER     6

/////////////////////////////////////////////////////////////////////////////
// CDriveTreeCtrl

CDriveTreeCtrl::CDriveTreeCtrl()
{
	bFirst = true;
	clText[0] = RGB(0, 200, 0);
	clText[1] = RGB(0, 0, 200);
	clText[2] = RGB(200, 0, 0);
}

CDriveTreeCtrl::~CDriveTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CDriveTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CDriveTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDriveTreeCtrl message handlers

void CDriveTreeCtrl::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = pnmtv->itemNew.hItem;
    CString strPathName = GetPathFromItem (hItem);
    *pResult = FALSE;

    // Reset the drive node if the drive is empty or the media changed.
    if (!IsMediaValid (strPathName))
	{
        HTREEITEM hRoot = GetDriveNode (hItem);
        Expand(hRoot, TVE_COLLAPSE);
        DeleteChildren(hRoot);
        AddDummyNode(hRoot);
        *pResult = TRUE;
        return;
    }

    // Delete the item if strPathName no longer specifies a valid path.
    if (!IsPathValid (strPathName))
	{
        DeleteItem (hItem);
        *pResult = TRUE;
        return;
    }

    // If the item is expanding, delete the dummy item attached to it
    // and add folder items. If the item is collapsing instead, delete
    // its folder items and add a dummy item if appropriate.
    if (pnmtv->action == TVE_EXPAND)
	{
        DeleteChildren (hItem);
        if (!AddDirectoryNodes (hItem, strPathName))
            *pResult = TRUE;
    }
    else
	{
        DeleteChildren(hItem);
        if (IsDriveNode(hItem))
            AddDummyNode(hItem);
        else
            SetButtonState(hItem, strPathName);
    }
}

BOOL CDriveTreeCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	/*HCURSOR cursor;
	CWinApp* prgApp = AfxGetApp();
	cursor = prgApp->LoadCursor(IDC_CURSOR_HAND);
	if(cursor == NULL)*/
		return CTreeCtrl::OnSetCursor(pWnd, nHitTest, message);

	//SetCursor(cursor);
	
	//return TRUE;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

/*----------------------GetSelectedPath
*/
CString CDriveTreeCtrl::GetSelectedPath(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = pnmtv->itemNew.hItem;
    CString sPath = GetPathFromItem (hItem);

	return sPath;
}

/*----------------------RefreshDrive
*/
void CDriveTreeCtrl::RefreshDrive (UINT nDrive)
{
    // Find the HTREEITEM that corresponds to the target drive.
    CString strDrive = "?:\\";
    strDrive.SetAt (0, 0x41 + nDrive);

    HTREEITEM hItem = FindItem (GetNextItem (NULL, TVGN_ROOT), strDrive);

    if (hItem == NULL)
        return;

    // Reset the drive node if the drive is empty or the media changed.
    if (!IsMediaValid (strDrive))
	{
        Expand (hItem, TVE_COLLAPSE);
        DeleteChildren (hItem);
        AddDummyNode (hItem);
        return;
    }

    // Save the current drive and directory.
    char szHome[MAX_PATH];
    ::GetCurrentDirectory (sizeof (szHome), szHome);

    // Change to the root directory of the target drive.
    if (!::SetCurrentDirectory ((LPCTSTR) strDrive))
        return; // Invalid drive specification

    // Refresh the drive node and all displayed subfolders.
    if (hItem != NULL)
        RefreshDirectory (hItem);

    // Return to the original drive and directory.
    ::SetCurrentDirectory (szHome);
}

/*----------------------GetPathFromItem
*/
CString CDriveTreeCtrl::GetPathFromItem (HTREEITEM hItem)
{
    CString strPathName;
    while (hItem != NULL)
	{
        CString string = GetItemText (hItem);
        if ((string.Right(1) != "\\") && !strPathName.IsEmpty ())
            string += "\\";
        strPathName = string + strPathName;
        hItem = GetParentItem (hItem);
    }
    return strPathName;
}

/*----------------------ExpandPath
*/
BOOL CDriveTreeCtrl::ExpandPath (LPCTSTR pszPath, BOOL bSelectItem)
{
    if (::lstrlen (pszPath) < 3)
        return FALSE;

    // Begin by finding the corresponding drive node.
    CString strPathName = pszPath;
    CString strDrive = strPathName.Left (3);

    HTREEITEM hItem =
        FindItem (GetNextItem (NULL, TVGN_ROOT),
        strDrive);

    if (hItem == NULL)
        return FALSE; // Invalid drive specification

    strPathName = strPathName.Right (strPathName.GetLength () - 3);

    // Now bore down through the directory structure searching for the
    // item that corresponds to the final directory name in pszPath.
    while (strPathName.GetLength () > 0) {
        Expand (hItem, TVE_EXPAND);
        hItem = GetChildItem (hItem);
        if (hItem == NULL)
            return FALSE;

        int nIndex = strPathName.Find ('\\');
        hItem = FindItem (hItem, nIndex == -1 ? strPathName :
            strPathName.Left (nIndex));
        if (hItem == NULL)
            return FALSE; // Invalid path name

        if (nIndex == -1)
            strPathName.Empty ();
        else
            strPathName = strPathName.Right (strPathName.GetLength () -
                nIndex - 1);
    }

    Expand (hItem, TVE_EXPAND);
    if (bSelectItem)
        Select (hItem, TVGN_CARET);
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////
// Protected helper functions

/*----------------------InitTree
*/
UINT CDriveTreeCtrl::InitTree ()
{
    int nPos = 0;
    UINT nCount = 0;
    CString strDrive = "?:\\";

    DWORD dwDriveList = ::GetLogicalDrives ();

    while (dwDriveList)
	{
        if(dwDriveList & 1)
		{
            strDrive.SetAt(0, 0x41 + nPos);
            if (AddDriveNode (strDrive))
                nCount++;
        }
        dwDriveList >>= 1;
        nPos++;
    }

    return nCount;
}

/*----------------------AddDriveNode
*/
BOOL CDriveTreeCtrl::AddDriveNode (CString& strDrive)
{
    HTREEITEM hItem;
	UINT nType = ::GetDriveType ((LPCTSTR) strDrive);
    UINT nDrive = (UINT) strDrive[0] - 0x41;

    switch (nType)
	{
    case DRIVE_REMOVABLE:
        hItem = InsertItem (strDrive, ILI_FLOPPY, ILI_FLOPPY);
		AddDummyNode(hItem);
        m_dwMediaID[nDrive] = GetSerialNumber (strDrive);
		break;

    case DRIVE_FIXED:
        hItem = InsertItem (strDrive, ILI_HARD_DISK,
            ILI_HARD_DISK);
        SetButtonState (hItem, strDrive);
        //CreateMonitoringThread (strDrive);
		if (bFirst) {
            SelectItem (hItem);
            Expand (hItem, TVE_EXPAND);
            bFirst = FALSE;
        }
        break;

    case DRIVE_REMOTE:
        hItem = InsertItem (strDrive, ILI_NET_DRIVE,
            ILI_NET_DRIVE);
        SetButtonState (hItem, strDrive);
        //CreateMonitoringThread (strDrive);
        break;

    case DRIVE_CDROM:
        hItem = InsertItem (strDrive, ILI_CD_ROM,
            ILI_CD_ROM);
        AddDummyNode (hItem);
        m_dwMediaID[nDrive] = GetSerialNumber (strDrive);
		break;

    case DRIVE_RAMDISK:
        hItem = InsertItem (strDrive, ILI_RAM_DRIVE,
            ILI_RAM_DRIVE);
        SetButtonState (hItem, strDrive);
        //CreateMonitoringThread (strDrive);
        break;

    default:
        return FALSE;
    }

    return TRUE;
}

/*----------------------AddDirectoryNodes
*/
UINT CDriveTreeCtrl::AddDirectoryNodes (HTREEITEM hItem, CString& strPathName)
{
    HANDLE hFind;
    WIN32_FIND_DATA fd;

    UINT nCount = 0;

    CString strFileSpec = strPathName;
    if (strFileSpec.Right (1) != "\\")
        strFileSpec += "\\";
    strFileSpec += "*.*";

    if ((hFind = ::FindFirstFile ((LPCTSTR) strFileSpec, &fd)) ==
        INVALID_HANDLE_VALUE) {
        if (IsDriveNode (hItem))
            AddDummyNode (hItem);
        return 0;
    }

    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
            CString strFileName = (LPCTSTR) &fd.cFileName;
            if((strFileName != ".") && (strFileName != ".."))
			{
                HTREEITEM hChild =
                    InsertItem ((LPCTSTR) &fd.cFileName,
                    ILI_CLOSED_FOLDER, ILI_OPEN_FOLDER, hItem, TVI_SORT);

                CString strNewPathName = strPathName;
                if (strNewPathName.Right (1) != "\\")
                    strNewPathName += "\\";

                strNewPathName += (LPCTSTR) &fd.cFileName;
                SetButtonState (hChild, strNewPathName);
                nCount++;
            }
        }
    }while(::FindNextFile (hFind, &fd));

    ::FindClose(hFind);
    return nCount;
}

/*----------------------SetButtonState
*/
void CDriveTreeCtrl::SetButtonState (HTREEITEM hItem, CString& strPathName)
{
    if (HasSubdirectory (strPathName))
        AddDummyNode (hItem);
}

/*----------------------UpdateButtonState
*/
void CDriveTreeCtrl::UpdateButtonState (HTREEITEM hItem, CString& strPathName)
{
    if (HasSubdirectory (strPathName)) {
        if (!ItemHasChildren (hItem)) {
            AddDummyNode (hItem);
            Invalidate ();
        }
    }
    else {
        if (ItemHasChildren (hItem))
            DeleteChildren (hItem);
    }
}

/*----------------------HasSubdirectory
*/
BOOL CDriveTreeCtrl::HasSubdirectory (CString& strPathName)
{
    HANDLE hFind;
    WIN32_FIND_DATA fd;
    BOOL bResult = FALSE;

    CString strFileSpec = strPathName;
    if (strFileSpec.Right (1) != "\\")
        strFileSpec += "\\";
    strFileSpec += "*.*";

    if ((hFind = ::FindFirstFile ((LPCTSTR) strFileSpec, &fd)) !=
        INVALID_HANDLE_VALUE) {
        do {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                CString strFileName = (LPCTSTR) &fd.cFileName;
                if ((strFileName != ".") && (strFileName != ".."))
                    bResult = TRUE;
            }
        } while (::FindNextFile (hFind, &fd) && !bResult);
        ::FindClose (hFind);
    }

    return bResult;
}

/*----------------------IsDriveNode
*/
BOOL CDriveTreeCtrl::IsDriveNode (HTREEITEM hItem)
{
    return (GetParentItem (hItem) == NULL) ? TRUE : FALSE;
}

/*----------------------AddDummyNode
*/
void CDriveTreeCtrl::AddDummyNode (HTREEITEM hItem)
{
    InsertItem ("", 0, 0, hItem);
}

/*----------------------FindItem
*/
HTREEITEM CDriveTreeCtrl::FindItem (HTREEITEM hItem, CString& strTarget)
{
    while (hItem != NULL) {
        if (GetItemText (hItem) == strTarget)
            break;
        hItem = GetNextSiblingItem (hItem);
    }
    return hItem;
}

/*----------------------DeleteChildren
*/
UINT CDriveTreeCtrl::DeleteChildren (HTREEITEM hItem)
{
    UINT nCount = 0;
    HTREEITEM hChild = GetChildItem (hItem);

    while (hChild != NULL) {
        HTREEITEM hNextItem = GetNextSiblingItem (hChild);
        DeleteItem (hChild);
        hChild = hNextItem;
        nCount++;
    }
    return nCount;
}

/*----------------------GetDriveNode
*/
HTREEITEM CDriveTreeCtrl::GetDriveNode (HTREEITEM hItem)
{
    HTREEITEM hParent;

    do
	{
        hParent = GetParentItem (hItem);
        if (hParent != NULL)
            hItem = hParent;

    }while (hParent != NULL);

    return hItem;
}

/*----------------------GetSerialNumber
*/
DWORD CDriveTreeCtrl::GetSerialNumber (CString& strDrive)
{
    DWORD dwSerialNumber;
    if (!::GetVolumeInformation((LPCTSTR)strDrive, NULL, 0, 
								&dwSerialNumber, NULL, NULL, NULL, 0))
        dwSerialNumber = 0xFFFFFFFF;
    return dwSerialNumber;
}

/*----------------------IsMediaValid
*/
BOOL CDriveTreeCtrl::IsMediaValid (CString& strPathName)
{
    // Return TRUE if the drive doesn't support removable media.
    UINT nDriveType = GetDriveType ((LPCTSTR) strPathName);
    if ((nDriveType != DRIVE_REMOVABLE) && (nDriveType != DRIVE_CDROM))
        return TRUE;

    // Return FALSE if the drive is empty (::GetVolumeInformation fails).
    DWORD dwSerialNumber;
    CString strDrive = strPathName.Left (3);
    UINT nDrive = (UINT) strDrive[0] - 0x41;

    if (!::GetVolumeInformation ((LPCTSTR) strDrive, NULL, 0,
        &dwSerialNumber, NULL, NULL, NULL, 0))
	{
        m_dwMediaID[nDrive] = 0xFFFFFFFF;
        return FALSE;
    }

    // Also return FALSE if the disk's serial number has changed.
    if (((UINT)m_dwMediaID[nDrive] != dwSerialNumber) &&
        ((UINT)m_dwMediaID[nDrive] != 0xFFFFFFFF)) {
        m_dwMediaID[nDrive] = dwSerialNumber;
        return FALSE;
    }

    // Update our record of the serial number and return TRUE.
    m_dwMediaID[nDrive] = dwSerialNumber;
    return TRUE;
}

/*----------------------IsPathValid
*/
BOOL CDriveTreeCtrl::IsPathValid (CString& strPathName)
{
    if (strPathName.GetLength () == 3)
        return TRUE;

    HANDLE hFind;
    WIN32_FIND_DATA fd;
    BOOL bResult = FALSE;

    if ((hFind = ::FindFirstFile ((LPCTSTR) strPathName, &fd)) !=
        INVALID_HANDLE_VALUE) {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                bResult = TRUE;
        ::CloseHandle (hFind);
    }
    return bResult;
}

/*----------------------RefreshDirectory
*/
void CDriveTreeCtrl::RefreshDirectory (HTREEITEM hItem)
{
    // If the item is not expanded, update its button state and return.
    if (!(GetItemState (hItem, TVIS_EXPANDED) &
        TVIS_EXPANDED) || !ItemHasChildren (hItem)) {
        if (!IsDriveNode (hItem)) {
            CString strPathName = GetPathFromItem (hItem);
            UpdateButtonState (hItem, strPathName);
            Expand (hItem, TVE_COLLAPSE);
        }
        return;
    }

    // Delete items corresponding to subdirectories that no longer exist
    // and build a CStringList containing the names of all the items that
    // are children of hItem.
    CStringList list;
    WIN32_FIND_DATA fd;
    HANDLE hFind;

    HTREEITEM hChild = GetChildItem (hItem);

    while (hChild != NULL) {
        HTREEITEM hNextItem = GetNextSiblingItem (hChild);
        CString strDirName = GetItemText (hChild);

        if ((hFind = ::FindFirstFile ((LPCTSTR) strDirName, &fd)) !=
            INVALID_HANDLE_VALUE) {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                list.AddTail (strDirName);
            else
                DeleteItem (hChild);
            ::FindClose (hFind);
        }
        else
            DeleteItem (hChild);

        hChild = hNextItem;
    }

    // Add items for newly created subdirectories.
    if ((hFind = ::FindFirstFile ("*.*", &fd)) != INVALID_HANDLE_VALUE) {
        do {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                CString strFileName = (LPCTSTR) &fd.cFileName;
                if ((strFileName != ".") && (strFileName != "..")) {
                    if (list.Find (strFileName) == NULL) {
                        hChild = InsertItem (strFileName,
                            ILI_CLOSED_FOLDER, ILI_OPEN_FOLDER,
                            hItem, TVI_SORT);
                        CString strPathName = GetPathFromItem (hChild);
                        SetButtonState (hChild, strPathName);
                        list.AddTail (strFileName);
                    }
                }
            }
        } while (::FindNextFile (hFind, &fd));
    ::FindClose (hFind);
    }

    // Remove all items from the CStringList.
    list.RemoveAll ();

    // Now repeat this procedure for hItem's children.
    hChild = GetChildItem (hItem);

    while (hChild != NULL) {
        CString string = GetItemText (hChild);
        ::SetCurrentDirectory ((LPCTSTR) string);
        RefreshDirectory (hChild); // Recursion!
        ::SetCurrentDirectory ("..");
        hChild = GetNextSiblingItem (hChild);
    }
}
