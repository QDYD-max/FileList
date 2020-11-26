// FilelistDoc.h : interface of the CFilelistDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILELISTDOC_H__766EC682_3D4E_4066_BD17_E1DF33ABA6F6__INCLUDED_)
#define AFX_FILELISTDOC_H__766EC682_3D4E_4066_BD17_E1DF33ABA6F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFilelistDoc : public CDocument
{
protected: // create from serialization only
	CFilelistDoc();
	DECLARE_DYNCREATE(CFilelistDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilelistDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFilelistDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFilelistDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILELISTDOC_H__766EC682_3D4E_4066_BD17_E1DF33ABA6F6__INCLUDED_)
