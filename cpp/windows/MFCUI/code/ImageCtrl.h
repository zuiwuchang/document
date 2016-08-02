#pragma once

#include "ui_base.h"

// CImageCtrl

class CImageCtrl : public CStatic
	,public ui_base
{
	DECLARE_DYNAMIC(CImageCtrl)

public:
	CImageCtrl();
	virtual ~CImageCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void init_default();
	BOOL create(CWnd* pParentWnd,const RECT& rect,LPCTSTR lpszCaption,UINT nID=0xffff,DWORD dwStyle=0);
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


