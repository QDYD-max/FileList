// FilelistView.cpp : implementation of the CFilelistView class
//

#include "stdafx.h"
#include "Filelist.h"

#include "FilelistDoc.h"
#include "FilelistView.h"
#include "FilelistDlg.h"
#include "FileDlg.h"
#include "FolderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilelistView

IMPLEMENT_DYNCREATE(CFilelistView, CView)

BEGIN_MESSAGE_MAP(CFilelistView, CView)
	//{{AFX_MSG_MAP(CFilelistView)
	ON_COMMAND(IDM_DIALOG, OnDialog)
	ON_COMMAND(IDM_MYDIR, OnMydir)
	ON_COMMAND(ID_MYFILE, OnMyfile)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilelistView construction/destruction

CFilelistView::CFilelistView()
{
	// TODO: add construction code here

}

CFilelistView::~CFilelistView()
{
}

BOOL CFilelistView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFilelistView drawing

void CFilelistView::OnDraw(CDC* pDC)
{
	CFilelistDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CFilelistView printing

BOOL CFilelistView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFilelistView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFilelistView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFilelistView diagnostics

#ifdef _DEBUG
void CFilelistView::AssertValid() const
{
	CView::AssertValid();
}

void CFilelistView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFilelistDoc* CFilelistView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFilelistDoc)));
	return (CFilelistDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFilelistView message handlers

void CFilelistView::OnDialog() 
{
	// TODO: Add your command handler code here
	CFilelistDlg dlg;
	dlg.DoModal();
}

void CFilelistView::OnMydir() 
{
	CFolderDlg dlg;
	dlg.DoModal();
	// TODO: Add your command handler code here
	
}

void CFilelistView::OnMyfile() 
{
	CFileDlg dlg;
	dlg.DoModal();
	// TODO: Add your command handler code here
	
}
