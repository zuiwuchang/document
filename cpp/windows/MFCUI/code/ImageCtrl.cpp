// ImageCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "Gamble.h"
#include "ImageCtrl.h"


// CImageCtrl

IMPLEMENT_DYNAMIC(CImageCtrl, CStatic)

CImageCtrl::CImageCtrl()
{

}

CImageCtrl::~CImageCtrl()
{
}


BEGIN_MESSAGE_MAP(CImageCtrl, CStatic)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CImageCtrl 消息处理程序




BOOL CImageCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return __super::OnEraseBkgnd(pDC);
}


void CImageCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rect(lpDrawItemStruct->rcItem);

	Gdiplus::Bitmap		bitmap(rect.Width(),rect.Height());
	Gdiplus::Graphics	graphics_catche(&bitmap);

	//繪製背景
	if(MFCUI_STYLE_BACKGROUND & get_style())
	{
		bool is_draw_back	=	draw_what(graphics_catche,MFCUI_STYLE_BACKGROUND,rect);
		if(!is_draw_back)
		{
			graphics_catche.FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color(255,255,255)),0,0,rect.Width(),rect.Height());
		}
	}
	
	draw_what(graphics_catche,MFCUI_DEFAULT_INDEX,rect);

	Gdiplus::Graphics graphics(m_hWnd);
	Gdiplus::CachedBitmap cache_bitmap(&bitmap,&graphics);
	graphics.DrawCachedBitmap(&cache_bitmap,0,0);    
}

void CImageCtrl::init_default()
{
	ui_base::init_default();
	
	ModifyStyle(0,SS_OWNERDRAW | SS_NOTIFY | WS_TABSTOP);
}
	
BOOL CImageCtrl::create(CWnd* pParentWnd,const RECT& rect,LPCTSTR lpszCaption,UINT nID,DWORD dwStyle)
{
	dwStyle	|=	WS_CHILD | WS_VISIBLE;
	return Create(lpszCaption,dwStyle,rect,pParentWnd,nID);
}