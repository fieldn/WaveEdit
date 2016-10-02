
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
	ON_COMMAND(ID_EDIT_CUT, &CWaveEditView::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CWaveEditView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CWaveEditView::OnEditPaste)
	ON_COMMAND(ID_EDIT_DESELECTALL, &CWaveEditView::OnEditDeselectall)
	ON_COMMAND(ID_EDIT_LEFT_ARROW, &CWaveEditView::OnEditLeftArrow)
	ON_COMMAND(ID_SELECT_SELECTALL, &CWaveEditView::OnSelectSelectall)
END_MESSAGE_MAP()

// CWaveEditView construction/destruction

CWaveEditView::CWaveEditView()
{
	// TODO: add construction code here
	mousePressed = false;
	selectionStart = 0;
	selectionEnd = 0;
	pointer = 0;
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
	} else if (pointer != 0) {
		CBrush newBrush(RGB(75, 75, 75));
		CBrush* pOldBrush = pDC->SelectObject(&newBrush);
		pDC->Rectangle(pointer, 0, pointer + 1, rect.Height());
		pDC->SelectObject(pOldBrush);
	} else {

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
	mousePressed = false;
	this->selectionEnd = point.x;
	this->pointer = point.x;
	this->RedrawWindow();
	CScrollView::OnLButtonUp(nFlags, point);
}

void CWaveEditView::OnViewZoomin()
{
	zoom *= 2;
	selectionStart *= zoom;
	selectionEnd *= zoom;
	RedrawWindow();
}

void CWaveEditView::OnViewZoomnormal()
{
	zoom = 1;
	selectionStart *= zoom;
	selectionEnd *= zoom;
	RedrawWindow();
}

void CWaveEditView::OnViewZoomout()
{
	zoom /= 2;
	selectionStart *= zoom;
	selectionEnd *= zoom;
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
		selectionStart *= zoom;
		selectionEnd *= zoom;
		RedrawWindow();
	}

	SetScrollPos(SB_HORZ, drawOffset, TRUE);
}

void CWaveEditView::OnEditCut()
{
	CWaveEditDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	WaveFile wave = pDoc->wave;
	if (wave.hdr == NULL)
		return;
	// Get dimensions of the window.
	CRect rect;
	GetClientRect(rect);
	// Scale the start and end of the selection.
	double startms = (1000.0 * wave.lastSample / wave.sampleRate) * this->selectionStart / rect.Width();
	// Scale the start and end of the selection.
	double endms = (1000.0 * wave.lastSample / wave.sampleRate) * this->selectionEnd / rect.Width();

	// Copy first the fragment
	clipboard = wave.get_fragment(startms, endms);
	// Copy the clipboard
	WaveFile w2 = *wave.remove_fragment(startms, endms);
	// Remove old wave
	// delete wave;
	// Substitute old wave with new one
	pDoc->wave = w2;
	// Update window
	this->RedrawWindow();
}

void CWaveEditView::OnEditCopy()
{
	CWaveEditDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	WaveFile wave = pDoc->wave;
	if (wave.hdr == NULL)
		return;
	// Get dimensions of the window.
	CRect rect;
	GetClientRect(rect);
	// Scale the start and end of the selection.
	double startms = (1000.0 * wave.lastSample / wave.sampleRate) * this->selectionStart / rect.Width();
	// Scale the start and end of the selection.
	double endms = (1000.0 * wave.lastSample / wave.sampleRate) * this->selectionEnd / rect.Width();

	// Copy first the fragment
	clipboard = wave.get_fragment(startms, endms);
}


void CWaveEditView::OnEditPaste()
{
	CWaveEditDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	WaveFile wave = pDoc->wave;
	if (wave.hdr == NULL)
		return;
	// Get dimensions of the window.
	CRect rect;
	GetClientRect(rect);

	for (int i = 0; i < clipboardSize; i++) {
		wave.add_sample(clipboard[i]);
	}
}

void CWaveEditView::OnEditDeselectall()
{
	selectionStart = selectionEnd = pointer = 0;
	RedrawWindow();
}


void CWaveEditView::OnEditLeftArrow()
{
	if (selectionEnd != selectionStart) {
		pointer = selectionStart - 1;
		selectionEnd == selectionStart;
	} else if (pointer != 0) {
		pointer--;
	} else {
		return;
	}
		RedrawWindow();
	// TODO: Add your command handler code here
}


void CWaveEditView::OnSelectSelectall()
{
	OnViewZoomnormal();
	CRect rect;
	GetClientRect(rect);

	selectionStart = 0;
	selectionEnd = rect.Width();
	RedrawWindow();
	// TODO: Add your command handler code here
}
