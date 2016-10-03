
// WaveEditDoc.cpp : implementation of the CWaveEditDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WaveEdit.h"
#endif

#include "WaveEditDoc.h"
#include "WaveFile.h"
#include <propkey.h>
#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWaveEditDoc

IMPLEMENT_DYNCREATE(CWaveEditDoc, CDocument)

BEGIN_MESSAGE_MAP(CWaveEditDoc, CDocument)
	ON_COMMAND(ID_TRANSPORT_PLAY, &CWaveEditDoc::OnTransportPlay)
	ON_COMMAND(ID_TRANSPORT_STOP, &CWaveEditDoc::OnTransportStop)
	ON_COMMAND(ID_EFFECT_ECHO, &CWaveEditDoc::OnEffectEcho)
	ON_COMMAND(ID_EFFECT_SPEEDUP, &CWaveEditDoc::OnEffectSpeedup)
	ON_COMMAND(ID_EFFECT_SLOWDOWN, &CWaveEditDoc::OnEffectSlowdown)
	ON_COMMAND(ID_EFFECT_FADEIN, &CWaveEditDoc::OnEffectFadein)
	ON_COMMAND(ID_EFFECT_FADEOUT, &CWaveEditDoc::OnEffectFadeout)
	ON_COMMAND(ID_EFFECT_REVERSE, &CWaveEditDoc::OnEffectReverse)
	ON_COMMAND(ID_EDIT_UNDO, &CWaveEditDoc::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CWaveEditDoc::OnEditRedo)
END_MESSAGE_MAP()


// CWaveEditDoc construction/destruction
CWaveEditDoc::CWaveEditDoc()
{
	std::vector<Modifier*> modList = std::vector<Modifier*>();
	listIndex = 0;
}

CWaveEditDoc::~CWaveEditDoc()
{
}

BOOL CWaveEditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

// CWaveEditDoc serialization
void CWaveEditDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
		// TODO: add storing code here
	} else {
		wave.read(ar.GetFile());
		copy = WaveFile(wave);
		wave.play();
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CWaveEditDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CWaveEditDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CWaveEditDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWaveEditDoc diagnostics

#ifdef _DEBUG
void CWaveEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWaveEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWaveEditDoc commands


void CWaveEditDoc::OnTransportPlay()
{
	wave.play();
}

void CWaveEditDoc::OnTransportStop()
{
	PlaySoundW(NULL, NULL, 0);
}

void CWaveEditDoc::OnEffectEcho()
{
	Modifier *m = new FilterSpeed();
	WaveFile * echoFile = m->TransformSelect(&wave, sStart, sEnd, 2);
	wave = *echoFile;
	UpdateList(m);
}

void CWaveEditDoc::OnEffectSpeedup()
{
	Modifier *m = new FilterSpeed();
	WaveFile * fastFile = m->TransformSelect(&wave, sStart, sEnd, 2);
	wave = *fastFile;
	UpdateList(m);
}

void CWaveEditDoc::OnEffectSlowdown()
{
	Modifier *m = new FilterSpeed();
	WaveFile * slowFile = m->TransformSelect(&wave, sStart, sEnd, 0.5);
	wave = *slowFile;
	UpdateList(m);
}

void CWaveEditDoc::OnEffectFadein()
{
	Modifier *m = new FilterFade(true);
	WaveFile * fadeFile = m->TransformSelect(&wave, sStart, sEnd);
	wave = *fadeFile;
	UpdateList(m);
}

void CWaveEditDoc::OnEffectFadeout()
{
	Modifier *m = new FilterFade(false);
	WaveFile * fadeFile = m->TransformSelect(&wave, sStart, sEnd);
	wave = *fadeFile;
	UpdateList(m);
}

void CWaveEditDoc::OnEffectReverse()
{
	Modifier *m = new FilterFade(false);
}

void CWaveEditDoc::UpdateList(Modifier *m)
{
	modList.resize(listIndex);
	this->modList.push_back(m);
	listIndex++;
	UpdateAllViews(NULL);
}

void CWaveEditDoc::OnEditUndo()
{
	wave = WaveFile(copy);

	if (listIndex == 0)
		return;
	
	Modifier * m;
	listIndex--;
	for (int i = 0; i < listIndex; i++) {
		m = modList.at(i);
		wave = *(m->TransformSelect(&wave, m->start, m->end, m->pnum));
	}
	wave.updateHeader();
	UpdateAllViews(NULL);
}


void CWaveEditDoc::OnEditRedo()
{
	if ((listIndex) == modList.size())
		return;

	Modifier * m = modList.at(listIndex);
	wave = *(m->TransformSelect(&wave, m->start, m->end, m->pnum));
	listIndex++;
	wave.updateHeader();
	UpdateAllViews(NULL);
}
