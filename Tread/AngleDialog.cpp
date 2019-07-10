// AngleDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tread3d.h"
#include "AngleDialog.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RADIUS 45.0
#define PI 3.141592654
/////////////////////////////////////////////////////////////////////////////
// CAngleDialog dialog


CAngleDialog::CAngleDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAngleDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAngleDialog)
	m_sDegree = _T("");
	//}}AFX_DATA_INIT

	m_bDrag = false;
	m_nDegree = 0;
	m_sDegree = "0";
}


void CAngleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAngleDialog)
	DDX_Control(pDX, IDC_EDIT_ANGLE, m_edAngle);
	DDX_Control(pDX, IDC_COMBO_ANGLE, m_cbDegree);
	DDX_Text(pDX, IDC_EDIT_ANGLE, m_sDegree);
	DDV_MaxChars(pDX, m_sDegree, 3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAngleDialog, CDialog)
	//{{AFX_MSG_MAP(CAngleDialog)
	ON_BN_CLICKED(IDC_BUTTON_DONE, OnButtonDone)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_ANGLE, OnSelchangeComboAngle)
	ON_EN_CHANGE(IDC_EDIT_ANGLE, OnChangeEditAngle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAngleDialog message handlers

BOOL CAngleDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_edAngle.SetFilter(FILTER_CHAR | FILTER_PUNCTUATION | FILTER_SPACE);

	GetClientRect(m_rcClient);
	m_ptCenter = m_rcClient.CenterPoint();
	m_ptCenter.y -= 10;
	m_ptAngle = m_ptCenter;

	m_rcBound.left = (long)(m_ptCenter.x - RADIUS);
	m_rcBound.right = (long)(m_ptCenter.x + RADIUS);
	m_rcBound.bottom = (long)(m_ptCenter.y + RADIUS);
	m_rcBound.top = (long)(m_ptCenter.y - RADIUS);
	m_rcBound.NormalizeRect();

	int n;
	CString sBuffer;

	n = m_cbDegree.AddString("User");
	m_cbDegree.SetItemData(n, -3);
	n = m_cbDegree.AddString("Up");
	m_cbDegree.SetItemData(n, -1);
	n = m_cbDegree.AddString("Down");
	m_cbDegree.SetItemData(n, -2);

	for(int k = 0; k < 360; k += 45)
	{
		sBuffer.Format("%d", k);
		n = m_cbDegree.AddString(sBuffer);
		m_cbDegree.SetItemData(n, k);
	}

	m_nDegree = atoi(m_sDegree);
	int nSelect = m_cbDegree.FindStringExact(-1, m_sDegree);
	if(nSelect == -1)
		m_cbDegree.SetCurSel(0);
	else
		m_cbDegree.SetCurSel(nSelect);
	UpdateDialog();
	CalcAngle();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAngleDialog::OnButtonDone() 
{
	OnOK();
}

void CAngleDialog::CalcAngle()
{
	m_ptAngle.x = m_ptCenter.x + (int)(RADIUS * cos(m_nDegree * PI / 180) + .5);
	m_ptAngle.y = m_ptCenter.y - (int)(RADIUS * sin(m_nDegree * PI / 180) + .5);
	InvalidateRect(m_rcBound, true);
}

void CAngleDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bDrag = true;
	m_cbDegree.SetCurSel(0);
	SetCapture();
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CAngleDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CPoint ptTemp;
	ptTemp.x = point.x - m_ptCenter.x;
	ptTemp.y = m_ptCenter.y - point.y;

	m_nDegree = (int)((atan2(ptTemp.y, ptTemp.x) * 180) / PI + .5);

	if(m_nDegree < 0)
		m_nDegree += 360;

	UpdateDialog();
	ReleaseCapture();
	m_bDrag = false;

	CDialog::OnLButtonUp(nFlags, point);
}

void CAngleDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bDrag)
	{
		CPoint ptTemp;
		ptTemp.x = point.x - m_ptCenter.x;
		ptTemp.y = m_ptCenter.y - point.y;

		m_nDegree = (int)((atan2(ptTemp.y, ptTemp.x) * 180) / PI + .5);

		if(m_nDegree < 0)
			m_nDegree += 360;

		UpdateDialog();
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CAngleDialog::UpdateDialog()
{
	m_sDegree.Format("%d", m_nDegree);

	int nSelect = m_cbDegree.FindStringExact(-1, m_sDegree);
	if(nSelect == -1)
	{
		m_cbDegree.SetCurSel(0);
		nSelect = 0;
	}
	else
		m_cbDegree.SetCurSel(nSelect);
	
	m_ptAngle.x = m_ptCenter.x + (int)(RADIUS * cos(m_nDegree * PI / 180) + .5);
	m_ptAngle.y = m_ptCenter.y - (int)(RADIUS * sin(m_nDegree * PI / 180) + .5);
	InvalidateRect(m_rcBound, true);
	
	UpdateData(false);
}

void CAngleDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CFont* pOldFont;
	CPen* pOldPen;
	int nOption = m_cbDegree.GetCurSel();
		
	pOldFont = dc.SelectObject(&m_ftText);
		
	dc.SetTextColor(RGB(50, 50, 200));
	dc.SetBkMode(TRANSPARENT);

	if(m_nDegree == -1)
	{
		pOldPen = dc.SelectObject(&m_pnBlue);
		dc.TextOut(m_rcBound.left, m_ptCenter.y - 6, "Vertical / Up");
	}
	else if(m_nDegree == -2)
	{
		pOldPen = dc.SelectObject(&m_pnBlue);
		dc.TextOut(m_rcBound.left, m_ptCenter.y - 6, "Vertical / Down");
	}
	else
	{
		pOldPen = dc.SelectObject(&m_pnBlue);
		dc.TextOut(m_rcBound.right + 5, m_ptCenter.y - 6, "0");

		dc.SelectObject(&m_pnBlack);
		dc.Ellipse(m_rcBound);

		dc.SelectObject(m_pnBlue);
		dc.MoveTo(m_ptCenter);
		dc.LineTo(m_ptAngle);
	}

	dc.SelectObject(pOldFont);
	dc.SelectObject(pOldPen);
}

void CAngleDialog::SetAngle(int nAngle)
{
	m_sDegree.Format("%d", nAngle);
}

CString CAngleDialog::GetAngle(void)
{
	return m_sDegree;
}

void CAngleDialog::SetAngle(CString sAngle)
{
	m_sDegree = sAngle;
}

void CAngleDialog::OnSelchangeComboAngle() 
{
	int nSelect = m_cbDegree.GetCurSel();
	if(nSelect == -1)
		return;

	int nValue = m_cbDegree.GetItemData(nSelect);

	m_nDegree = nValue;
	if(nValue < 0)
		m_sDegree = "";
	else
		m_sDegree.Format("%d", nValue);
	UpdateDialog();

	m_cbDegree.SetFocus();
}

void CAngleDialog::OnChangeEditAngle() 
{
	UpdateData(true);

	if(m_sDegree.IsEmpty())
		return;

	m_nDegree = atoi(m_sDegree);
	int nSelect = m_cbDegree.FindStringExact(-1, m_sDegree);
	if(nSelect == -1)
		m_cbDegree.SetCurSel(0);
	else
		m_cbDegree.SetCurSel(nSelect);
	UpdateDialog();
}
