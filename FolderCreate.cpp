// FolderCreate.cpp : implementation file
//

#include "stdafx.h"
#include "Filelist.h"
#include "FolderCreate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFolderCreate

IMPLEMENT_DYNAMIC(CFolderCreate, CFileDialog)

CFolderCreate::CFolderCreate(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}


BEGIN_MESSAGE_MAP(CFolderCreate, CFileDialog)
	//{{AFX_MSG_MAP(CFolderCreate)
	ON_COMMAND(IDM_MYDIR, OnMydir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CFolderCreate::OnMydir() 
{
	// TODO: Add your command handler code here
	
}
