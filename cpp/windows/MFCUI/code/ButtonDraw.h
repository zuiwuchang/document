#pragma once
#include"ui_base.h"

// CButtonDraw

class CButtonDraw : public CButton
	,public ui_base
{
	DECLARE_DYNAMIC(CButtonDraw)

public:
	CButtonDraw();
	virtual ~CButtonDraw();
	
protected:
	DECLARE_MESSAGE_MAP()
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
public:
	BOOL create(CWnd* pParentWnd,const RECT& rect,LPCTSTR lpszCaption,UINT nID=0xffff,DWORD dwStyle=0);

	virtual void init_default();
protected:
	bool is_tracking;
	bool is_hover;

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void PreSubclassWindow();
};


