// ButtonDraw.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCUI.h"
#include "ButtonDraw.h"


// CButtonDraw

IMPLEMENT_DYNAMIC(CButtonDraw, CButton)

CButtonDraw::CButtonDraw()
{
	is_tracking	=	is_hover	=	false;
}

CButtonDraw::~CButtonDraw()
{
}


BEGIN_MESSAGE_MAP(CButtonDraw, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CButtonDraw 消息处理程序


BOOL CButtonDraw::create(CWnd* pParentWnd,const RECT& rect,LPCTSTR lpszCaption,UINT nID,DWORD dwStyle)
{
	dwStyle	|=	WS_CHILD | WS_VISIBLE;
	return Create(lpszCaption,dwStyle,rect,pParentWnd,nID);
}
void CButtonDraw::init_default()
{
	ui_base::init_default();

	ModifyStyle(0,BS_OWNERDRAW);
}
void CButtonDraw::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rect(lpDrawItemStruct->rcItem);

	//內存畫布
	Gdiplus::Bitmap		bitmap(rect.Width(),rect.Height());
	Gdiplus::Graphics	graphics_catche(&bitmap);
	
	//繪製各狀態 外觀
	if(ODS_DISABLED & lpDrawItemStruct->itemState)
	{
		//禁用
		if(!draw_what(graphics_catche,MFCUI_DISABLED_INDEX,rect))
		{
			graphics_catche.FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color(244,244,244)),0,0,rect.Width(),rect.Height());
			graphics_catche.DrawRectangle(&Gdiplus::Pen(Gdiplus::Color(173,178,181)),0,0,rect.Width()-1,rect.Height()-1);
		}
	}
	else if(ODS_SELECTED & lpDrawItemStruct->itemState)
	{
		//按下
		if(!draw_what(graphics_catche,MFCUI_DOWN_INDEX,rect))
		{
			Gdiplus::LinearGradientBrush brush(Gdiplus::Rect(0,0,rect.Width(),rect.Height()),Gdiplus::Color(),Gdiplus::Color(),Gdiplus::LinearGradientModeVertical);
			Gdiplus::Color colors[]	=	{Gdiplus::Color(231,247,255)
				,Gdiplus::Color(198,231,247)
				,Gdiplus::Color(156,211,239)
				,Gdiplus::Color(123,190,231)
			};
			Gdiplus::REAL reals[]	=	{0.0f,0.5f,0.5f,1.0f};
			
			brush.SetInterpolationColors(colors,reals,4);

			graphics_catche.FillRectangle(&brush,0,0,rect.Width(),rect.Height());

			graphics_catche.DrawRectangle(&Gdiplus::Pen(Gdiplus::Color(41,97,140)),0,0,rect.Width()-1,rect.Height()-1);
		}
	}
	else if(ODS_FOCUS & lpDrawItemStruct->itemState)
	{
		if(is_hover)
		{
			//覆蓋
			if(!draw_what(graphics_catche,MFCUI_DISABLED_INDEX,rect))
			{
				Gdiplus::LinearGradientBrush brush(Gdiplus::Rect(0,0,rect.Width(),rect.Height()),Gdiplus::Color(),Gdiplus::Color(),Gdiplus::LinearGradientModeVertical);
				Gdiplus::Color colors[]	=	{Gdiplus::Color(232,246,253)
					,Gdiplus::Color(217,240,252)
					,Gdiplus::Color(190,230,253)
					,Gdiplus::Color(167,217,245)
				};
				Gdiplus::REAL reals[]	=	{0.0f,0.5f,0.5f,1.0f};
			
				brush.SetInterpolationColors(colors,reals,4);

				graphics_catche.FillRectangle(&brush,0,0,rect.Width(),rect.Height());

				graphics_catche.DrawRectangle(&Gdiplus::Pen(Gdiplus::Color(60,127,177)),0,0,rect.Width()-1,rect.Height()-1);
			}
		}
		else if(!draw_what(graphics_catche,MFCUI_FOCUS_INDEX,rect))
		{
			//焦點
			Gdiplus::LinearGradientBrush brush(Gdiplus::Rect(0,0,rect.Width(),rect.Height()),Gdiplus::Color(),Gdiplus::Color(),Gdiplus::LinearGradientModeVertical);
			Gdiplus::Color colors[]	=	{Gdiplus::Color(233,246,253)
				,Gdiplus::Color(228,244,253)
				,Gdiplus::Color(201,234,252)
				,Gdiplus::Color(180,223,247)
			};
			Gdiplus::REAL reals[]	=	{0.0f,0.5f,0.5f,1.0f};
			
			brush.SetInterpolationColors(colors,reals,4);

			graphics_catche.FillRectangle(&brush,0,0,rect.Width(),rect.Height());

			graphics_catche.DrawRectangle(&Gdiplus::Pen(Gdiplus::Color(60,127,177)),0,0,rect.Width()-1,rect.Height()-1);
			graphics_catche.DrawRectangle(&Gdiplus::Pen(Gdiplus::Color(114,219,252)),1,1,rect.Width()-3,rect.Height()-3);
		}
	}
	else
	{
		if(is_hover)
		{
			//覆蓋
			if(!draw_what(graphics_catche,MFCUI_DISABLED_INDEX,rect))
			{
				Gdiplus::LinearGradientBrush brush(Gdiplus::Rect(0,0,rect.Width(),rect.Height()),Gdiplus::Color(),Gdiplus::Color(),Gdiplus::LinearGradientModeVertical);
				Gdiplus::Color colors[]	=	{Gdiplus::Color(232,246,253)
					,Gdiplus::Color(217,240,252)
					,Gdiplus::Color(190,230,253)
					,Gdiplus::Color(167,217,245)
				};
				Gdiplus::REAL reals[]	=	{0.0f,0.5f,0.5f,1.0f};
			
				brush.SetInterpolationColors(colors,reals,4);

				graphics_catche.FillRectangle(&brush,0,0,rect.Width(),rect.Height());

				graphics_catche.DrawRectangle(&Gdiplus::Pen(Gdiplus::Color(60,127,177)),0,0,rect.Width()-1,rect.Height()-1);
			}
		}
		else
		{
			//默認
			if(!draw_what(graphics_catche,MFCUI_DEFAULT_INDEX,rect))
			{
				Gdiplus::LinearGradientBrush brush(Gdiplus::Rect(0,0,rect.Width(),rect.Height()),Gdiplus::Color(),Gdiplus::Color(),Gdiplus::LinearGradientModeVertical);
				Gdiplus::Color colors[]	=	{Gdiplus::Color(242,242,242)
					,Gdiplus::Color(235,235,235)
					,Gdiplus::Color(221,221,221)
					,Gdiplus::Color(207,207,207)
				};
				Gdiplus::REAL reals[]	=	{0.0f,0.5f,0.5f,1.0f};
			
				brush.SetInterpolationColors(colors,reals,4);

				graphics_catche.FillRectangle(&brush,0,0,rect.Width(),rect.Height());

				graphics_catche.DrawRectangle(&Gdiplus::Pen(Gdiplus::Color(112,112,112)),0,0,rect.Width()-1,rect.Height()-1);
			}
		}
	}

	//繪製焦點 虛線
	if(get_style() & MFCUI_STYLE_FOCUS_LINE
		&& ODS_FOCUS & lpDrawItemStruct->itemState
		)
	{
		Gdiplus::Pen pen(Gdiplus::Color(0,0,0));
		pen.SetDashStyle(Gdiplus::DashStyleDot);
		graphics_catche.DrawRectangle(&pen,2,2,rect.Width()-5,rect.Height()-5);
	}

	//繪製文本
	CString text;
	GetWindowText(text);
	if(!text.IsEmpty())
	{
		if(ODS_DISABLED & lpDrawItemStruct->itemState)
		{
			draw_text(graphics_catche,text.GetBuffer(),rect);
		}
		else
		{
			draw_disabled_text(graphics_catche,text.GetBuffer(),rect);
		}
	}

	//繪製到設備
	Gdiplus::Graphics graphics(m_hWnd);
	Gdiplus::CachedBitmap cache_bitmap(&bitmap,&graphics);
	graphics.DrawCachedBitmap(&cache_bitmap,0,0);
}


void CButtonDraw::OnMouseMove(UINT nFlags, CPoint point)
{
	 if (!is_tracking)
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE | TME_HOVER;//要触发的消息
        tme.hwndTrack = this->m_hWnd;
        tme.dwHoverTime = 10;// 若不设此参数，则无法触发mouseHover

        if (::_TrackMouseEvent(&tme)) //MOUSELEAVE|MOUSEHOVER消息由此函数触发
        {
            is_tracking = true;   
        }

    }

	__super::OnMouseMove(nFlags, point);
}


void CButtonDraw::OnMouseHover(UINT nFlags, CPoint point)
{
	is_tracking	=	false;
	if(!is_hover)
	{
		is_hover	=	true;
		RedrawWindow();
	}
	__super::OnMouseHover(nFlags, point);
}


void CButtonDraw::OnMouseLeave()
{
	is_tracking	=	false;
	if(is_hover)
	{
		is_hover	=	false;
		RedrawWindow();
	}
	__super::OnMouseLeave();
}


BOOL CButtonDraw::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//return __super::OnEraseBkgnd(pDC);
}


void CButtonDraw::PreSubclassWindow()
{
	init_default();
	__super::PreSubclassWindow();
}
