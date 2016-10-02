
// WaveEditView.cpp : implementation of the CWaveEditView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WaveEdit.h"
#include "WaveFile.h"
#endif

#include "WaveEditDoc.h"
#include "WaveEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWaveEditView
IMPLEMENT_DYNCREATE(CWaveEditView, CScrollView)
BEGIN_MESSAGE_MAP(CWaveEditView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CWaveEditView::OnFilePrintPreview)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_VIEW_ZOOMIN, &CWaveEditView::OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMNORMAL, &CWaveEditView::OnViewZoomnormal)
	ON_COMMAND(ID_VIEW_ZOOMOUT, &CWaveEditView::OnViewZoomout)
	ON_COMMAND(ID_VIEW_ZOOMSELECTION, &CWaveEditView::OnViewZoomselection)
END_MESSAGE_MAP()

// CWaveEditView construction/destruction

CWaveEditView::CWaveEditView()
{
	// TODO: add construction code here
	mousePressed = false;
	selectionStart = 0;
	selectionEnd = 0;
	drawOffset = 0;
	zoom = 1;
}

CWaveEditView::~CWaveEditView()
{
}

BOOL CWaveEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CView::PreCreateWindow(cs);
}

// CWaveEditView set scroll extent
void CWaveEditView::OnInitialUpdate() {
	CScrollView::OnInitialUpdate();
	// Initial scroll sizes
	CRect rect;
	GetClientRect(rect);
	CSize sizeTotal;
	sizeTotal.cx = rect.Width();
	sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

// CWaveEditView drawing
void CWaveEditView::OnDraw(CDC* pDC)
{
	CWaveEditDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	WaveFile * wave = &pDoc->wave;
	if (wave->hdr == NULL)
		return;
	// Get dimensions of the window.
	CRect rect;
	GetClientRect(rect);

	CSize sizeTotal;
	sizeTotal.cx = zoom * rect.Width();
	sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal);

	// Set color in pen and brush for wave
	COLORREF color = RGB(0, 150, 230);
	CPen pen2(PS_SOLID, 0, color);
	pDC->SelectObject(&pen2);
	CBrush brush2(color);
	pDC->SelectObject(&brush2);

	// Draw selection if any
	if (selectionStart != selectionEnd) {
		CBrush newBrush(RGB(75, 75, 75));
		CBrush* pOldBrush = pDC->SelectObject(&newBrush);
		pDC->Rectangle(selectionStart, 0, selectionEnd, rect.Height());
		pDC->SelectObject(pOldBrush);
	}

	// Draw the wave
	pDC->MoveTo(0, 0);
	for (int x = 0; x < sizeTotal.cx; x++) {
		// Assuming the whole file will be fit in the window, for every x value in the window
		// we need to find the equivalent sample in the wave file.
		float val = wave->get_sample((int)(x*wave->lastSample / sizeTotal.cx));
		// We need to fit the sound also in the y axis. The y axis goes from 0 in the
		// top of the window to rect.Height at the bottom. The sound goes from -32768 to 32767
		// we scale it in that way.
		int y = (int)((val + 32768) * (rect.Height() - 1) / (32767 + 32768));
		pDC->LineTo(x, rect.Height() - y);
	}

}


// CWaveEditView printing


void CWaveEditView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CWaveEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWaveEditView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWaveEditView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CWaveEditView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CWaveEditView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWaveEditView diagnostics

#ifdef _DEBUG
void CWaveEditView::AssertValid() const
{
	CView::AssertValid();
}

void CWaveEditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWaveEditDoc* CWaveEditView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWaveEditDoc)));
	return (CWaveEditDoc*)m_pDocument;
}
#endif //_DEBUG


// CWaveEditView message handlers

void CWaveEditView::OnMouseMove(UINT nFlags, CPoint point)
{
	CScrollView::OnMouseMove(nFlags, point);
	if (mousePressed) {
		selectionEnd = point.x;
		RedrawWindow();
	}
}
void CWaveEditView::OnLButtonDown(UINT nFlags, CPoint point)
{
	mousePressed = true;
	selectionStart = point.x;
	selectionEnd = point.x;
	CScrollView::OnLButtonDown(nFlags, point);
}
void CWaveEditView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	mousePressed = false;
	this->selectionEnd = point.x;
	this->RedrawWindow();
	CScrollView::OnLButtonUp(nFlags, point);
}

void CWaveEditView::OnViewZoomin()
{
	zoom *= 2;
	RedrawWindow();
}

void CWaveEditView::OnViewZoomnormal()
{
	zoom = 1;
	RedrawWindow();
}


void CWaveEditView::OnViewZoomout()
{
	zoom /= 2;
	RedrawWindow();
}

void CWaveEditView::OnViewZoomselection()
{
	if (selectionStart != selectionEnd) {
		float selectionSize = selectionEnd - selectionStart;
		CRect rect;
		GetClientRect(rect);
		zoom = ((float)rect.Width() / selectionSize);
		drawOffset = selectionStart;
		RedrawWindow();
	}
	SetScrollPos(SB_HORZ, drawOffset, TRUE);
}
