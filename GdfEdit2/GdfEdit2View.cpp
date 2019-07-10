// GdfEdit2View.cpp : implementation of the CGdfEdit2View class
//

#include "stdafx.h"
#include "GdfEdit2.h"

#include "GdfEdit2Doc.h"
#include "CntrItem.h"
#include "GdfEdit2View.h"

#include "ChildFrm.h"
#include "ConsoleView.h"
#include "Parse.h"
#include "ReportDialog.h"
#include "EntityDialog.h"
#include "KeyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MATCH_LIST 34
CString g_pList[MATCH_LIST] =
{
	"def_key",
	"def_att",
	"def_flg",
	"def_param",
	"def_type",
	"def_path",
	"def_pathtype",
	"def_solid",
	"def_point",
	"def_author",
	"def_version",
	"$DefBuild",
	"$DefEnt",
	"$DefFlags",
	"$DefGlobal",
	"$DefModule",
	"$DefOptions",
	"$DefParams",
	"$ExportMap",
	"$SpawnNoWait",
	"$SpawnWait",
	"@ABORT",
	"@ALWAYS",
	"@CONSOLE",
	"@DIR",
	"@FILENAME",
	"@NOT_RUNNABLE",
	"@NULL",
	"@Q2_WAL",
	"@RUNNABLE",
	"@TGA",
	"@THIS",
	"@USER",
	"@VIRTUAL"	
};

/////////////////////////////////////////////////////////////////////////////
// CGdfEdit2View

IMPLEMENT_DYNCREATE(CGdfEdit2View, CRichEditView)

BEGIN_MESSAGE_MAP(CGdfEdit2View, CRichEditView)
	//{{AFX_MSG_MAP(CGdfEdit2View)
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	ON_COMMAND(IDT_INSERT_ENTITY, OnInsertEntity)
	ON_COMMAND(IDT_INSERT_CLASS, OnInsertClass)
	ON_COMMAND(IDT_INSERT_GLOBAL, OnInsertGlobal)
	ON_COMMAND(IDT_INSERT_ATTRIBUTE, OnInsertAttribute)
	ON_COMMAND(IDT_INSERT_KEY, OnInsertKey)
	ON_COMMAND(IDT_INSERT_FLAG, OnInsertFlag)
	ON_COMMAND(IDT_INSERT_OPTION, OnInsertOption)
	ON_COMMAND(IDT_INSERT_FLAGKEY, OnInsertFlagkey)
	ON_COMMAND(IDT_WIZARD_ENTITY, OnWizardEntity)
	ON_COMMAND(IDT_WIZARD_KEY, OnWizardKey)
	ON_COMMAND(IDT_VERIFY_SCRIPT, OnVerifyScript)
	ON_COMMAND(IDT_INSERT_CLASS_DERIVED, OnInsertClassDerived)
	ON_COMMAND(IDM_FONT, OnFont)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	//ON_WM_KEYUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRichEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGdfEdit2View construction/destruction

CGdfEdit2View::CGdfEdit2View()
{
	m_nStart = m_nEnd = -1;
	m_nTab = 0;
	m_pSmartDlg = NULL;
	m_bSmart = false;
}

CGdfEdit2View::~CGdfEdit2View()
{
	m_pSmartDlg->DestroyWindow();
	delete m_pSmartDlg;
}

BOOL CGdfEdit2View::PreCreateWindow(CREATESTRUCT& cs)
{
	m_nWordWrap = WrapNone;
	return CRichEditView::PreCreateWindow(cs);
}

void CGdfEdit2View::OnInitialUpdate()
{
	m_nWordWrap = WrapNone;
	CRichEditView::OnInitialUpdate();

	CWinApp* pApp = AfxGetApp();
	m_sFont = pApp->GetProfileString("Settings", "Font", "Courier New");
	m_nSize = pApp->GetProfileInt("Settings", "Size", 200);

	SCROLLINFO scroll;
	GetScrollInfo(SB_HORZ, &scroll);
	scroll.nMax = 1000;
	SetScrollInfo(SB_HORZ, &scroll, true);
		
	m_RichEdit->GetSelectionCharFormat(m_chFormat);
	m_RichEdit->GetParaFormat(m_prFormat);

	m_chFormat.dwMask  |= CFM_COLOR;
	m_chFormat.dwEffects &= ~CFE_AUTOCOLOR;
	m_chFormat.dwEffects &= ~CFE_BOLD;
	strcpy(m_chFormat.szFaceName, m_sFont);
	m_chFormat.yHeight = m_nSize;
	m_chFormat.crTextColor = RGB(0, 0, 0);

	m_RichEdit->SetSel(0, -1);
	m_RichEdit->SetSelectionCharFormat(m_chFormat);
	m_RichEdit->SetParaFormat(m_prFormat);
	m_RichEdit->SetSel(0, 0);

	GetDocument()->SetModifiedFlag(false);
	
	//Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(400, 400, 400, 400));

	CString sPath;
	CString sExt;
	int nIndex;

	sPath = GetDocument()->GetPathName();
	nIndex = sPath.Find('.');
	sExt = sPath.Right(sPath.GetLength() - (nIndex + 1));

	if(GetDocument()->IsNewDoc())
		return;
}

/////////////////////////////////////////////////////////////////////////////
// CGdfEdit2View printing

BOOL CGdfEdit2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGdfEdit2View::OnDestroy()
{
   CRichEditView::OnDestroy();
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
}

/////////////////////////////////////////////////////////////////////////////
// CGdfEdit2View diagnostics

#ifdef _DEBUG
void CGdfEdit2View::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CGdfEdit2View::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CGdfEdit2Doc* CGdfEdit2View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGdfEdit2Doc)));
	return (CGdfEdit2Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGdfEdit2View message handlers

void CGdfEdit2View::SetOwnerFrame(CChildFrame* pOwnerFrame)
{
	m_pOwnerFrame = pOwnerFrame;
}

void CGdfEdit2View::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CMenu commandMenu;
	ClientToScreen(&point);

	commandMenu.LoadMenu(IDR_POPUP_TEXTVIEW);
	CMenu *popup = commandMenu.GetSubMenu(0);
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
							point.x, point.y, this);

	m_nTab = 0;

	CRichEditView::OnRButtonDown(nFlags, point);
}

void CGdfEdit2View::InsertText( const CString& sInput)
{
	m_RichEdit->ReplaceSel(sInput, true);
	ColorCode();
}

void CGdfEdit2View::InsertTextAndMove(const CString & sInput)
{
	m_RichEdit->ReplaceSel(sInput, true);

	long lStart, lEnd;
	CPoint ptCursor;
	CRect rcWindow;

	m_RichEdit->GetSel(lStart, lEnd);
	ptCursor = m_RichEdit->GetCharPos(lStart);
	m_RichEdit->GetClientRect(&rcWindow);
	ClientToScreen(&rcWindow);
	ptCursor.x += rcWindow.left;
	ptCursor.y += rcWindow.top;
	m_pSmartDlg->SetWindowPosition(ptCursor);
}

void CGdfEdit2View::RemoveTextAndMove(int nCount)
{
	long lStart, lEnd;
	CPoint ptCursor;
	CRect rcWindow;

	m_RichEdit->GetSel(lStart, lEnd);
	m_RichEdit->SetSel(lStart - nCount, lStart);
	m_RichEdit->Clear();

	m_RichEdit->GetSel(lStart, lEnd);
	ptCursor = m_RichEdit->GetCharPos(lStart);
	m_RichEdit->GetClientRect(&rcWindow);
	ClientToScreen(&rcWindow);
	ptCursor.x += rcWindow.left;
	ptCursor.y += rcWindow.top;
	m_pSmartDlg->SetWindowPosition(ptCursor);
}

void CGdfEdit2View::RemoveText(int nCount)
{
	long lStart, lEnd;

	m_RichEdit->GetSel(lStart, lEnd);
	m_RichEdit->SetSel(lStart - nCount, lStart);
	m_RichEdit->Clear();
}

void CGdfEdit2View::AppendText(CString sInput, ...)
{
	int nLength  = GetTextLength();

	va_list argptr;
	CString  sBuffer;

	va_start(argptr, sInput);
	vsprintf((LPSTR)((LPCSTR)sBuffer), sInput, argptr);
	va_end(argptr);
	
	m_RichEdit->SetSel( -1, -1 );		// move insertion point to end of text
	m_RichEdit->ReplaceSel(sInput, false);
	m_RichEdit->SetSel( nLength, -1 );	// select the inserted text
	SetCharFormat( m_chFormat );		//change the new text to the current format
	m_RichEdit->SetSel( -1, -1 );		// again, move insertion point to end of text
}

int CGdfEdit2View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_RichEdit = &GetRichEditCtrl();

	CString pList[23] =
	{"$DefFlags",	"$DefEnt",		"$DefGlobal",	"$DefOptions",
	"$DefBuild",	"$DefModule",	"$DefParams",	"$ExportMap",
	"$SpawnWait",	"$SpawnNoWait", "@VIRTUAL",		"@THIS",
	"@CONSOLE",		"@DIR",			"@NOT_RUNNABLE", "@ALWAYS",
	"@FILENAME",	"@ABORT",		"@Q2_WAL",		"@TGA",
	"@RUNNABLE",	"@NULL",		"@USER"};

	m_pSmartDlg = new CPopupDialog();
	m_pSmartDlg->Create(IDD_DIALOG_POPUP);
	m_pSmartDlg->AddStringList(pList, 23);
	m_pSmartDlg->SetParentWindow(this);

	return 0;
}

void CGdfEdit2View::OnInsertEntity() 
{
	InsertText("$DefEnt [ \"\" : \"\" ] [ \"base_\" ]\n{}\n");
}

void CGdfEdit2View::OnInsertClass() 
{
	InsertText("$DefEnt [ \"\" : @VIRTUAL : @NULL ] []\n{}\n");
}

void CGdfEdit2View::OnInsertClassDerived() 
{
	InsertText("$DefEnt [ \"\" : @VIRTUAL : \"\" ] [ \"base_\" ]\n{}\n");
}

void CGdfEdit2View::OnInsertGlobal() 
{
	InsertText("$DefGlobal [ \"\" ]\n{\n\tdef_author\t\t\t: \"\"\n\tdef_version\t\t: \"\"\n\tdef_solid\t\t\t: \"\"\n\tdef_point\t\t\t: \"\"\n\tdef_palette\t\t: \"\"\n\tdef_gamemaptype\t\t: \"\"\n\tdef_pakdir\t\t\t: \"\"\n\tdef_targetkey\t\t: \"\"\n\tdef_namekey\t\t: \"\"\n\tdef_textype\t\t: \"\"\n}\n");
}

void CGdfEdit2View::OnInsertAttribute() 
{
	InsertText("\tdef_att : \"\" : \"\"");
}

void CGdfEdit2View::OnInsertKey() 
{
	InsertText("\tdef_key :  : \"\" : \"\" : \"\"");
}

void CGdfEdit2View::OnInsertFlag() 
{
	InsertText("\t$DefFlags [ \"\" : \"\" ]\n\t{\n\t\tdef_flg : \"\" : 1 : false\n\t}");
}

void CGdfEdit2View::OnInsertOption() 
{
	InsertText("\t$DefOptions [ \"\" : \"\" ]\n\t{\n\t\tdef_flg : \"\" : 0 : false\n\t}");
}

void CGdfEdit2View::OnInsertFlagkey() 
{
	InsertText("\t\tdef_flg : \"\" :  : false");
}

void CGdfEdit2View::OnWizardEntity() 
{
	CEntityDialog dlgTemp;
	CString sBuffer;

	if(dlgTemp.DoModal() != IDOK)
		return;
	sBuffer = dlgTemp.GetString();
	if(!sBuffer.IsEmpty())
		InsertText(sBuffer);
}

void CGdfEdit2View::OnWizardKey() 
{
	CKeyDialog dlgTemp;
	CString sBuffer;

	if(dlgTemp.DoModal() != IDOK)
		return;
	sBuffer = dlgTemp.GetString();
	if(!sBuffer.IsEmpty())
		InsertText(sBuffer);
}

void CGdfEdit2View::OnVerifyScript() 
{
	int nLastLine = GetLine();

	bool bSuccess = false;
	CString sBuffer;
	CParseGdf parse;
	CReportDialog infoDlg;

	//UpdateLastLine();

	parse.AttachConsole(m_pOwnerFrame->GetConsole());
	parse.AttachTreeBar(m_pOwnerFrame->GetTreeBar());
	
	GetRichEditCtrl().SetSel(0, -1);
	sBuffer = GetRichEditCtrl().GetSelText();
	GetRichEditCtrl().SetSel(0, 0);
	if(sBuffer.IsEmpty())
	{
		MessageBox("The script is empty.", "Script Verification", MB_OK);
		return;
	}

	BeginWaitCursor();
	SetCapture();

	CTreeBar* pTreeBar = m_pOwnerFrame->GetTreeBar();
	pTreeBar->ResetContent();
	bSuccess = parse.ParseGdf(sBuffer);
	pTreeBar->FillTreeCtrl();
	
	ReleaseCapture();
	EndWaitCursor();

	if(bSuccess)
	{
		sBuffer = "Verification of the ";
		sBuffer += m_pOwnerFrame->GetTreeBar()->GetGameName();
		sBuffer +=  " script was successful!";
		infoDlg.SetMessage(sBuffer);
		infoDlg.SetCount(parse.GetCount());
		infoDlg.SetClasses(pTreeBar->GetClasses());
		infoDlg.SetEntity(pTreeBar->GetEntities());
		infoDlg.SetSeconds(parse.GetTimeElapsed());
		infoDlg.SetUnique(parse.GetUniqueCount());
		infoDlg.DoModal();
		ScrollToLine(nLastLine);
	}
	else
	{
		ScrollToLine(1);
	}
}

void CGdfEdit2View::ScrollToLine(int nLine, bool bHighlite)
{
	SetCapture();
	BeginWaitCursor();

	long nLength = m_RichEdit->GetTextLength();
	CString sBuffer;
	int nStart = 0;
	int nEnd;
	int nVisible;

	//UpdateLastLine();
	
	nStart = m_RichEdit->LineIndex(nLine - 1); //go to the start of the line
	/*if(bHighlite)
	{
		nEnd = m_RichEdit->LineIndex(nLine) - 1;
		m_RichEdit->SetSel(nStart, nEnd);
		m_chFormat.crTextColor = RGB(255, 0, 0);
		SetCharFormat( m_chFormat );		//change the new text to the current format
		//ColorCodeLine(nStart);
		m_nStart = nStart;
		m_nEnd = nEnd;
	}*/
	nEnd = m_RichEdit->LineIndex(nLine) - 1;
	if(bHighlite)
		m_RichEdit->SetSel(nStart, nEnd);
						
	nVisible = m_RichEdit->GetFirstVisibleLine() + 2;
	nVisible = nLine - nVisible;
	m_RichEdit->LineScroll(nVisible, 0);

	sBuffer.Format("Line : %d", nLine);
	CGdfEdit2App* pApp = (CGdfEdit2App*)AfxGetApp();
	pApp->SetPaneText(sBuffer);
	
	SetFocus();

	ReleaseCapture();
	EndWaitCursor();
}

bool CGdfEdit2View::FindLine(int nLine, int &nPos, int nGap)
{
	if(nLine < 0)
		return false;

	for(nPos; nPos < (m_RichEdit->GetTextLength() + nGap); nPos += nGap)
	{
		if((m_RichEdit->LineFromChar(nPos) + 1) == nLine)
			return true;
		else if((m_RichEdit->LineFromChar(nPos) + 1) > nLine)
		{
			if(FindLine(nLine, (nPos -= nGap), nGap / 2))
				return true;
			break;
		}
	}

	return false;
}

void CGdfEdit2View::OnFont() 
{
	LOGFONT logFont;
	//CDC* pDC = GetDC();
	memset(&logFont, 0, sizeof(LOGFONT));
	//logFont.lfHeight = -12;
	//logFont.lfHeight = -MulDiv(m_nSize, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), 72);
	//ReleaseDC(pDC);
	strcpy(logFont.lfFaceName, m_sFont);

	CFontDialog dlgFont(&logFont);
	
	if(dlgFont.DoModal() == IDCANCEL)
		return;

	m_chFormat.dwEffects &= ~CFE_AUTOCOLOR;
	m_chFormat.dwEffects &= ~CFE_BOLD;
	strcpy(m_chFormat.szFaceName, dlgFont.GetFaceName());
	m_sFont = m_chFormat.szFaceName;
	m_chFormat.yHeight = 2 * dlgFont.GetSize();
	m_nSize = m_chFormat.yHeight;

	m_RichEdit->SetSel(0, -1);
	m_RichEdit->SetSelectionCharFormat(m_chFormat);
	m_RichEdit->SetParaFormat(m_prFormat);
	m_RichEdit->SetSel(0, 0);

	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileString("Settings", "Font", m_sFont);
	pApp->WriteProfileInt("Settings", "Size", m_nSize);
}

void CGdfEdit2View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRichEditView::OnLButtonDown(nFlags, point);

	CString sBuffer;
	sBuffer.Format("Line : %d", GetLine());
	CGdfEdit2App* pApp = (CGdfEdit2App*)AfxGetApp();
	pApp->SetPaneText(sBuffer);

	m_nTab = 0;
	//UpdateLastLine();
}

int CGdfEdit2View::GetLine()
{
	long nPos = 0;
	GetRichEditCtrl().GetSel(nPos, nPos);
	nPos = GetRichEditCtrl().LineFromChar(nPos) + 1;
	return nPos;
}

void CGdfEdit2View::OnSetFocus(CWnd* pOldWnd) 
{
	CRichEditView::OnSetFocus(pOldWnd);
	
	CString sBuffer;
	sBuffer.Format("Line : %d", GetLine());
	CGdfEdit2App* pApp = (CGdfEdit2App*)AfxGetApp();
	pApp->SetPaneText(sBuffer);
}

void CGdfEdit2View::OnKillFocus(CWnd* pNewWnd) 
{
	CRichEditView::OnKillFocus(pNewWnd);
	
	CGdfEdit2App* pApp = (CGdfEdit2App*)AfxGetApp();
	pApp->SetPaneText("");
}

void CGdfEdit2View::UpdateLastLine()
{
	if(m_nStart == -1)
		return;

	long nStart;
	long nEnd;

	m_RichEdit->GetSel(nStart, nEnd);	//save current position

	m_RichEdit->SetSel(m_nStart, m_nEnd);	//select the highlited line
	m_chFormat.crTextColor = RGB(0, 0, 0);	//change the color to black
	SetCharFormat( m_chFormat );			//set the format
	//ColorCodeLine(m_nStart);
	m_nStart = -1;							//mark that the operation is done

	m_RichEdit->SetSel(nStart, nEnd);	//return to the old position
	
}

void CGdfEdit2View::ColorCodeLine(long nPos)
{
	//CString sBuffer;
	
	int nCur;
	int nLast = nPos;
	int nTemp;
	int nLength;
	int nLine = m_RichEdit->LineFromChar(nPos);
	short* pLength = (short*)m_pBuffer;
	
	*pLength = 200;
	nLength = m_RichEdit->GetLine(nLine, m_pBuffer, 200);		//get the string
	m_pBuffer[nLength] = '\0';		//append a NULL
	//sBuffer = m_pBuffer;				//assign the array to a CString so we can use Find()
	nLength += nPos;				//change the length to a RichEditCtrl position

	for(nCur = Find(' ', 0); nCur < nLength; nCur = Find(' ', nLast - nPos))
	{
		if(nCur == -1)	//did not find a space (' ')
		{
			nCur = Find('\n', nLast - nPos);	//look for the end of line
			if(nCur == -1)
				break;
		}

		nCur += nPos;

		m_RichEdit->SetSel(nLast - 1, nCur);
		switch(m_pBuffer[nLast - nPos])
		{
		case '$':
			m_chFormat.crTextColor = RGB(0, 50, 200);
			break;
		case '\"':
			m_chFormat.crTextColor = RGB(0, 100, 100);
			nTemp = Find('\"', nLast - nPos + 1);
			if(nTemp != -1)
			{
				nTemp += nPos + 1;
				m_RichEdit->SetSel(nLast-1, nTemp);
				nCur = nTemp;
			}
			else
			{
				m_RichEdit->SetSel(nLast-1, nLength);
				nCur = nLength;
			}
			break;
		case '@':
			m_chFormat.crTextColor = RGB(0, 200, 0);
			break;
		default:
			m_chFormat.crTextColor = RGB(0, 0, 0);	//change the color to black
		}
		
		SetCharFormat( m_chFormat );			//set the format
		nLast = nCur + 1;
	}
}

int CGdfEdit2View::Find(char nChar, int nStart)
{
	for(; m_pBuffer[nStart] != '\0'; nStart++)
		if(nChar == m_pBuffer[nStart])
			return nStart;

	return -1;
}

void CGdfEdit2View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CString sBuffer;
	long lStart, lEnd;
	CPoint ptCursor;
	CRect rcWindow;
	
	switch(nChar)
	{
	case VK_TAB:
		break;
	case 13:
		{
			if(m_nTab > 0)
			{
				sBuffer = "\n";
				for(int n = 0; n < m_nTab; n++)
					sBuffer += '\t';
				InsertText(sBuffer);
				
				sBuffer.Format("Line : %d", GetLine());
				CGdfEdit2App* pApp = (CGdfEdit2App*)AfxGetApp();
				pApp->SetPaneText(sBuffer);
				return;
			}

			ColorCode();
		}
		break;
	case '{':
		m_nTab++;
		break;
	case '}':
		if(m_nTab > 0)
		{
			m_RichEdit->GetSel(lStart, lEnd);
			m_RichEdit->SetSel(lStart-1, lEnd);
			sBuffer = m_RichEdit->GetSelText();
			if(sBuffer == "\t")
				m_RichEdit->Clear();
			m_RichEdit->SetSel(lStart, lStart);

			m_nTab--;
		}
		break;
	case '@':
	case '$':
		CRichEditView::OnChar(nChar, nRepCnt, nFlags);
		m_RichEdit->GetSel(lStart, lEnd);
		ptCursor = m_RichEdit->GetCharPos(lStart);

		m_RichEdit->GetClientRect(&rcWindow);
		ClientToScreen(&rcWindow);
		ptCursor.x += rcWindow.left;
		ptCursor.y += rcWindow.top;
		m_pSmartDlg->SetWindowPosition(ptCursor);
		m_pSmartDlg->SetFindString(nChar);

		sBuffer.Format("Line : %d", GetLine());
		CGdfEdit2App* pApp = (CGdfEdit2App*)AfxGetApp();
		pApp->SetPaneText(sBuffer);
		return;
	}

	CRichEditView::OnChar(nChar, nRepCnt, nFlags);
	ColorCode();
	if(m_bSmart)
	{
		m_RichEdit->GetSel(lStart, lEnd);
		ptCursor = m_RichEdit->GetCharPos(lStart);
		m_RichEdit->GetClientRect(&rcWindow);
		ClientToScreen(&rcWindow);
		ptCursor.x += rcWindow.left;
		ptCursor.y += rcWindow.top;
		m_pSmartDlg->SetWindowPosition(ptCursor);
		m_pSmartDlg->AddToFindString(nChar);
	}

	sBuffer.Format("Line : %d", GetLine());
	CGdfEdit2App* pApp = (CGdfEdit2App*)AfxGetApp();
	pApp->SetPaneText(sBuffer);
}

void CGdfEdit2View::ColorCode(bool bSpecial)
{
	long lStart, lEnd;
	int nLine;
	int nIndex;
	int nEnd;
	int nReturn;
	int nLength;
	char pArray[500];
	CString sMessage;
	m_RichEdit->GetSel(lStart, lEnd); //Get the current selection

	if(lStart == lEnd) //color code the word preceeding this location
	{
		//get the current line that we are on and find the first character index
		nLine = m_RichEdit->LineFromChar(lStart);
		nIndex = m_RichEdit->LineIndex(nLine);
		nLength = m_RichEdit->LineLength(nIndex);	//get the length of the line
		pArray[nLength] = '\0';
		m_RichEdit->GetLine(nLine, pArray, nLength);	//get the text from the line
		nReturn = FindCharReverse(pArray, lStart - nIndex, ' ');	// look for the last space
		nEnd = FindCharForward(pArray, lStart - nIndex, ' ');		//look for the first space

		if(nReturn == -1 && nEnd == -1)
		{
			m_RichEdit->SetSel(nIndex, nIndex + nLength);
			m_RichEdit->GetSelText(pArray);	//select the text

			//DEBUG message
			//sMessage.Format("1 (\"%s\") [ %d, %d ]", pArray, nReturn, nEnd);
			//m_pOwnerFrame->GetConsole()->AddMessage(sMessage);

			if(FindMatch(pArray))
			{
				m_chFormat.crTextColor = RGB(0, 50, 200);
				SetCharFormat( m_chFormat );
			}
			else
			{
				m_chFormat.crTextColor = RGB(0, 0, 0);
				SetCharFormat( m_chFormat );
			}
			m_RichEdit->SetSel(lEnd, lEnd);
		}
		else if(nReturn == -1)
		{
			m_RichEdit->SetSel(nIndex, nIndex + nEnd);	//not at the beginning
			m_RichEdit->GetSelText(pArray);	//select the text

			//DEBUG message
			//sMessage.Format("2 (\"%s\") [ %d, %d ]", pArray, nReturn, nEnd);
			//m_pOwnerFrame->GetConsole()->AddMessage(sMessage);

			if(FindMatch(pArray))
			{
				m_chFormat.crTextColor = RGB(0, 50, 200);
				SetCharFormat( m_chFormat );
			}
			else
			{
				m_chFormat.crTextColor = RGB(0, 0, 0);
				SetCharFormat( m_chFormat );
			}
			m_RichEdit->SetSel(lEnd, lEnd);
		}
		else if(nEnd == -1)
		{
			m_RichEdit->SetSel(nIndex + nReturn, nIndex + nLength);
			m_RichEdit->GetSelText(pArray);	//select the text

			//DEBUG message
			//sMessage.Format("3 (\"%s\") [ %d, %d ]", pArray, nReturn, nEnd);
			//m_pOwnerFrame->GetConsole()->AddMessage(sMessage);

			if(FindMatch(pArray))
			{
				m_chFormat.crTextColor = RGB(0, 50, 200);
				SetCharFormat( m_chFormat );
			}
			else
			{
				m_chFormat.crTextColor = RGB(0, 0, 0);
				SetCharFormat( m_chFormat );
			}
			m_RichEdit->SetSel(lEnd, lEnd);
		}
		else
		{
			if(nReturn > nEnd)
				nReturn = FindCharReverse(pArray, lStart - 1 - nIndex, ' ');

			if(nReturn == -1)
				m_RichEdit->SetSel(nIndex, nIndex + nEnd);
			else
				m_RichEdit->SetSel(nIndex + nReturn, nIndex + nEnd);

			m_RichEdit->GetSelText(pArray);	//select the text

			//DEBUG message
			//sMessage.Format("4 (\"%s\") [ %d, %d ]", pArray, nReturn, nEnd);
			//m_pOwnerFrame->GetConsole()->AddMessage(sMessage);

			if(FindMatch(pArray))
			{
				m_chFormat.crTextColor = RGB(0, 50, 200);
				SetCharFormat( m_chFormat );
			}
			else
			{
				m_chFormat.crTextColor = RGB(0, 0, 0);
				SetCharFormat( m_chFormat );
			}
			m_RichEdit->SetSel(lEnd, lEnd);
		}
	}
	else				//color code the whole selection
	{
	}
	
	m_chFormat.crTextColor = RGB(0, 0, 0);
	SetCharFormat( m_chFormat );
}

int CGdfEdit2View::FindCharReverse(char* pString, int nStart, char cFind)
{
	for(int n = nStart; n >= 0; n--)
	{
		if(pString[n] == cFind)
			return n+1;
		
		if(pString[n] == VK_TAB)
			return n+1;
	}

	return -1;
}

int CGdfEdit2View::FindCharForward(char* pString, int nStart, char cFind)
{
	int nEnd = strlen(pString);

	for(int n = nStart; n < nEnd; n++)
	{
		if(pString[n] == cFind)
			return n;
		
		if(pString[n] == VK_TAB)
			return n;
	}

	return -1;
}

bool CGdfEdit2View::FindMatch(char* pString)
{
	for(int n = 0; n < MATCH_LIST; n++)
	{
		if(g_pList[n] == pString)
			return true;
	}

	return false;
}

void CGdfEdit2View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == 13)	//this happens before the keyup
		ColorCode();

	CRichEditView::OnKeyUp(nChar, nRepCnt, nFlags);

	if(nChar == VK_DELETE)	//this happens after the keyup
		ColorCode();
}

BOOL CGdfEdit2View::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST && pMsg->message == WM_KEYUP)
	{
		bool bRet = CRichEditView::PreTranslateMessage(pMsg);
		ColorCode((pMsg->wParam == VK_TAB) ? true : false);
		return bRet;
	}
	
	return CRichEditView::PreTranslateMessage(pMsg);
}
