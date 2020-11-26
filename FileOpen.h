#if !defined(AFX_FILEOPEN_H__31B57C1C_9404_44B1_B469_2550AEE74B46__INCLUDED_)
#define AFX_FILEOPEN_H__31B57C1C_9404_44B1_B469_2550AEE74B46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileOpen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileOpen dialog

class CFileOpen : public CDialog
{
// Construction
public:
	CFileOpen(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CFileOpen)
	enum { IDD = IDD_DIALOG4 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileOpen)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileOpen)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEOPEN_H__31B57C1C_9404_44B1_B469_2550AEE74B46__INCLUDED_)
