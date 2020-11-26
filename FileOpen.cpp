// FileOpen.cpp : implementation file
//

#include "stdafx.h"
#include "Filelist.h"
#include "FileOpen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileOpen dialog


CFileOpen::CFileOpen(CWnd* pParent /*=NULL*/)
	: CDialog(CFileOpen::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileOpen)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFileOpen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileOpen)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileOpen, CDialog)
	//{{AFX_MSG_MAP(CFileOpen)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileOpen message handlers

void CFileOpen::OnButton1() 
{
	// TODO: Add your control notification handler code here
	CFilelistApp *app = (CFilelistApp *)AfxGetApp();
	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	strcpy(app->tmptext,str);
	GetParent()->SendMessage(NUM_SAVEFILE,0,0);
	CDialog::OnOK();
	MessageBox("±£´æ³É¹¦");
	strcpy(app->tmptext,"");
}

BOOL CFileOpen::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	CString ntitle;
	CFilelistApp *app = (CFilelistApp *)AfxGetApp(); 
	GetDlgItem(IDC_EDIT1)->SetWindowText(app->tmptext);
	ntitle.Format("%s",app->filenamestr);
	this->SetWindowText(ntitle);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CFileOpen::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	GetParent()->SendMessage(NUM_CLOSE,0,0);
	CDialog::OnClose();
}
