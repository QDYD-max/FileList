// FolderDlg.cpp : implementation file
//
#include "stdafx.h"
#include "Filelist.h"
#include "FolderDlg.h"
#include "FilelistDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFolderDlg dialog


CFolderDlg::CFolderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFolderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFolderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFolderDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFolderDlg, CDialog)
	//{{AFX_MSG_MAP(CFolderDlg)
	ON_COMMAND(IDM_MYDIR, OnMydir)
	ON_BN_CLICKED(IDOK, OnCreateFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFolderDlg message handlers

void CFolderDlg::OnMydir() 
{
	// TODO: Add your command handler code here
	
}

void CFolderDlg::OnCreateFolder() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CFilelistApp *app = (CFilelistApp *)AfxGetApp(); 
	GetDlgItem(IDC_EDIT2)->GetWindowText(str);
	app->name_str=str.GetBuffer(0);
	GetParent()->SendMessage(NUM_CREATEFOLDER,0,0);
	CDialog::OnOK();
}
