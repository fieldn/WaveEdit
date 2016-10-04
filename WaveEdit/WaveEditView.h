
// WaveEditView.h : interface of the CWaveEditView class
//

#pragma once
#include "Modifier.h"
#include "ModifierCut.h"
#include "ModifierPaste.h"
#include "WaveEditDoc.h"
static short* clipboard = NULL;
static int clipboardSize = 0;

class CWaveEditView : public CScrollView
{
	bool mousePressed;
	int selectionStart; // Selected sample start
	int selectionEnd;	// Selected sample end
	int pointer;
	float zoom;
	int drawOffset;
	int index;

protected: // create from serialization only
	CWaveEditView();
	DECLARE_DYNCREATE(CWaveEditView)

// Attributes
public:
	CWaveEditDoc* GetDocument() const;

// Operations
public:
	int sampleStart(double startms, double endms);
	void UpdatePDoc();

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CWaveEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomnormal();
	afx_msg void OnViewZoomout();
	afx_msg void OnViewZoomselection();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditDeselectall();
	afx_msg void OnEditDeselectall32792();
	afx_msg void OnEditLeftArrow();
	afx_msg void OnEditRightArrow();
	afx_msg void OnSelectSelectall();
	afx_msg void OnSelectPointertostart();
	afx_msg void OnSelectPointertoend();
};

#ifndef _DEBUG  // debug version in WaveEditView.cpp
inline CWaveEditDoc* CWaveEditView::GetDocument() const
   { return reinterpret_cast<CWaveEditDoc*>(m_pDocument); }
#endif

