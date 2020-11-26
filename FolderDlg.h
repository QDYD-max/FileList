#if !defined(AFX_FOLDERDLG_H__769B93DC_2C40_4434_BFDE_A9EB6EE2A4DE__INCLUDED_)
#define AFX_FOLDERDLG_H__769B93DC_2C40_4434_BFDE_A9EB6EE2A4DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FolderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFolderDlg dialog

class CFolderDlg : public CDialog
{
// Construction
public:
	CFolderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFolderDlg)
	enum { IDD = IDD_DIALOG3 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFolderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFolderDlg)
	afx_msg void OnMydir();
	afx_msg void OnCreateFolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOLDERDLG_H__769B93DC_2C40_4434_BFDE_A9EB6EE2A4DE__INCLUDED_)
