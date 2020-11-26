// FilelistDoc.cpp : implementation of the CFilelistDoc class
//

#include "stdafx.h"
#include "Filelist.h"

#include "FilelistDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilelistDoc

IMPLEMENT_DYNCREATE(CFilelistDoc, CDocument)

BEGIN_MESSAGE_MAP(CFilelistDoc, CDocument)
	//{{AFX_MSG_MAP(CFilelistDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilelistDoc construction/destruction

CFilelistDoc::CFilelistDoc()
{
	// TODO: add one-time construction code here

}

CFilelistDoc::~CFilelistDoc()
{
}

BOOL CFilelistDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFilelistDoc serialization

void CFilelistDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFilelistDoc diagnostics

#ifdef _DEBUG
void CFilelistDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFilelistDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFilelistDoc commands
