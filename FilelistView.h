// FilelistView.h : interface of the CFilelistView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILELISTVIEW_H__296D3E4A_3298_404B_9DC5_89439468C575__INCLUDED_)
#define AFX_FILELISTVIEW_H__296D3E4A_3298_404B_9DC5_89439468C575__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFilelistView : public CView
{
protected: // create from serialization only
	CFilelistView();
	DECLARE_DYNCREATE(CFilelistView)

// Attributes
public:
	CFilelistDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilelistView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFilelistView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFilelistView)
	afx_msg void OnDialog();
	afx_msg void OnMydir();
	afx_msg void OnMyfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FilelistView.cpp
inline CFilelistDoc* CFilelistView::GetDocument()
   { return (CFilelistDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILELISTVIEW_H__296D3E4A_3298_404B_9DC5_89439468C575__INCLUDED_)
