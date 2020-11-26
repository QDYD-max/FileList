#if !defined(AFX_FOLDERCREATE_H__DD6658EB_2774_46F7_A5C4_C305B4E3C6E7__INCLUDED_)
#define AFX_FOLDERCREATE_H__DD6658EB_2774_46F7_A5C4_C305B4E3C6E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FolderCreate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFolderCreate dialog

class CFolderCreate : public CFileDialog
{
	DECLARE_DYNAMIC(CFolderCreate)

public:
	CFolderCreate(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

protected:
	//{{AFX_MSG(CFolderCreate)
	afx_msg void OnMydir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOLDERCREATE_H__DD6658EB_2774_46F7_A5C4_C305B4E3C6E7__INCLUDED_)
