// FileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Filelist.h"

#include "FileDlg.h"
#include "FolderDlg.h"

#ifdef _DEBUG

#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDlg dialog


CFileDlg::CFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialog)
	//{{AFX_MSG_MAP(CFileDlg)
	ON_COMMAND(IDM_MYDIR, CreateDir)
	ON_COMMAND(ID_MYFILE, CreateFile)
	ON_BN_CLICKED(IDOK, OnCreateFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileDlg message handlers


void CFileDlg::CreateDir() 
{
	// TODO: Add your command handler code here
	CFolderDlg dlg;
	dlg.DoModal();
	
}

void CFileDlg::CreateFile() 
{
	// TODO: Add your command handler code here
	CFileDlg dlg;
	dlg.DoModal();
}

void CFileDlg::OnCreateFile() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CFilelistApp *app = (CFilelistApp *)AfxGetApp(); 
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);
	app->name_str=str.GetBuffer(0);
	GetParent()->SendMessage(NUM_CREATEFILE,0,0);
	CDialog::OnOK();
}
