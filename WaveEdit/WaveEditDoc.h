
// WaveEditDoc.h : interface of the CWaveEditDoc class
//


#pragma once
#include "WaveFile.h"

class CWaveEditDoc : public CDocument
{
	friend class CWaveEditView;
	WaveFile wave;
protected: // create from serialization only
	CWaveEditDoc();
	DECLARE_DYNCREATE(CWaveEditDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CWaveEditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnTransportPlay();
	afx_msg void OnTransformPlay();
	afx_msg void OnTransportStop();
	afx_msg void OnEffectEcho();
	afx_msg void OnEffectSpeedup();
	afx_msg void OnEffectSlowdown();
};