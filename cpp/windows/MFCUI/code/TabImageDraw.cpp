// TabImageDraw.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCUI.h"
#include "TabImageDraw.h"


// CTabImageDraw

IMPLEMENT_DYNAMIC(CTabImageDraw, CStatic)

CTabImageDraw::CTabImageDraw()
{
	is_tracking	=	false;
	track_left	=	false;

	scroll	=	0;
	drag_index	=	hover	=	select	=	MFCUI_INVALID_INDEX;
	
}

CTabImageDraw::~CTabImageDraw()
{
}


BEGIN_MESSAGE_MAP(CTabImageDraw, CStatic)
	
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CTabImageDraw 消息处理程序

BOOL CTabImageDraw::create(CWnd* pParentWnd,const RECT& rect,LPCTSTR lpszCaption,UINT nID,DWORD dwStyle)
{
	dwStyle	|=	WS_CHILD | WS_VISIBLE;
	return Create(lpszCaption,dwStyle,rect,pParentWnd,nID);
}

void CTabImageDraw::init_default()
{
	ui_base::init_default();

	item_padding	=	5;
	
	ModifyStyle(0,SS_OWNERDRAW | SS_NOTIFY | WS_TABSTOP);
}

void CTabImageDraw::PreSubclassWindow()
{
	init_default();
	__super::PreSubclassWindow();
}

void CTabImageDraw::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rect(lpDrawItemStruct->rcItem);

	Gdiplus::Bitmap		bitmap(rect.Width(),rect.Height());
	Gdiplus::Graphics	graphics_catche(&bitmap);

	//繪製背景
	bool is_draw_back	=	true;;
	if(MFCUI_STYLE_BACKGROUND & get_style())
	{
		is_draw_back	=	draw_what(graphics_catche,MFCUI_STYLE_BACKGROUND,rect);
		if(!is_draw_back)
		{
			graphics_catche.FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color(255,255,255)),0,0,rect.Width(),rect.Height());
		}
	}
	
	//繪製子項
	for(UINT i = scroll;i < scroll + get_need_show();++i)
	{
		draw_item(graphics_catche,i);
	}

	//繪製背景
	if(!is_draw_back)
	{
		graphics_catche.DrawRectangle(&Gdiplus::Pen(Gdiplus::Color(105,105,105)),0,0,rect.Width()-1,rect.Height()-1);
	}

	Gdiplus::Graphics graphics(m_hWnd);
	Gdiplus::CachedBitmap cache_bitmap(&bitmap,&graphics);
	graphics.DrawCachedBitmap(&cache_bitmap,0,0);    
}

void CTabImageDraw::draw_item(Gdiplus::Graphics& graphics,const UINT index)
{
	CRect rect;
	
	if(get_item_rect(index,rect))
	{
		if(index == hover)
		{
			//繪製鼠標經過 高光
			if(!draw_what(graphics,MFCUI_HOVER_INDEX,rect))
			{
				Gdiplus::Rect fill_rect(rect.left,rect.top,rect.Width(),rect.Height());

				Gdiplus::LinearGradientBrush brush(fill_rect,Gdiplus::Color(),Gdiplus::Color(),Gdiplus::LinearGradientModeVertical);
				Gdiplus::Color colors[]	=	{Gdiplus::Color(224,240,240)
					,Gdiplus::Color(192,224,232)
					,Gdiplus::Color(176,216,232)
					,Gdiplus::Color(190,214,224)
				};
				Gdiplus::REAL reals[]	=	{0.0f,0.5f,0.5f,1.0f};
				brush.SetInterpolationColors(colors,reals,4);
				graphics.FillRectangle(&brush,fill_rect);

				graphics.DrawRectangle(&Gdiplus::Pen(Gdiplus::Color(72,144,200)),fill_rect);
			}

		}
		else if(index == select)
		{
			if(!draw_what(graphics,MFCUI_FOCUS_INDEX,rect))
			{
				//繪製選中項
				Gdiplus::Pen pen(Gdiplus::Color(108,124,154));
				//垂直排列
				if(get_style() & MFCUI_STYLE_VERTICAL)
				{
					Gdiplus::Rect fill_rect(rect.left,rect.top,rect.Width()+1,rect.Height());
				
					Gdiplus::LinearGradientBrush brush(fill_rect,Gdiplus::Color(223,226,231),Gdiplus::Color(255,255,255),Gdiplus::LinearGradientModeHorizontal);
					graphics.FillRectangle(&brush,fill_rect);

					Gdiplus::Point points[]	=	{Gdiplus::Point(rect.right,rect.top)
						,Gdiplus::Point(rect.left,rect.top)
						,Gdiplus::Point(rect.left,rect.bottom)
						,Gdiplus::Point(rect.right,rect.bottom)
					};
					graphics.DrawLines(&pen,points,4);
				}
				else
				{
					Gdiplus::Rect fill_rect(rect.left,rect.top,rect.Width(),rect.Height()+1);
				
					Gdiplus::LinearGradientBrush brush(fill_rect,Gdiplus::Color(223,226,231),Gdiplus::Color(255,255,255),Gdiplus::LinearGradientModeVertical);
					graphics.FillRectangle(&brush,fill_rect);

					Gdiplus::Point points[]	=	{Gdiplus::Point(rect.right,rect.bottom)
						,Gdiplus::Point(rect.right,rect.top)
						,Gdiplus::Point(rect.left,rect.top)
						,Gdiplus::Point(rect.left,rect.bottom)
					};
					graphics.DrawLines(&pen,points,4);
				}
			}
		}
		else if(!draw_what(graphics,MFCUI_DEFAULT_INDEX,rect))
		{
			//繪製一般狀態
		
			Gdiplus::Pen pen(Gdiplus::Color(108,124,154));
			//垂直排列
			if(get_style() & MFCUI_STYLE_VERTICAL)
			{
				Gdiplus::Rect fill_rect(rect.left,rect.top,rect.Width()-3,rect.Height());
				
				Gdiplus::LinearGradientBrush brush(fill_rect,Gdiplus::Color(),Gdiplus::Color(),Gdiplus::LinearGradientModeHorizontal);
				Gdiplus::Color colors[]	=	{Gdiplus::Color(255,255,255)
					,Gdiplus::Color(252,252,252)
					,Gdiplus::Color(231,233,238)
					,Gdiplus::Color(205,210,219)
				};
				Gdiplus::REAL reals[]	=	{0.0f,0.5f,0.5f,1.0f};
				brush.SetInterpolationColors(colors,reals,4);
				graphics.FillRectangle(&brush,fill_rect);


				graphics.DrawRectangle(&pen,rect.left,rect.top,rect.Width()-3,rect.Height());
				graphics.DrawLine(&pen,rect.right,rect.top,rect.right,rect.bottom);
			}
			else
			{
				Gdiplus::Rect fill_rect(rect.left,rect.top,rect.Width(),rect.Height()-3);
				
				Gdiplus::LinearGradientBrush brush(fill_rect,Gdiplus::Color(),Gdiplus::Color(),Gdiplus::LinearGradientModeVertical);
				Gdiplus::Color colors[]	=	{Gdiplus::Color(255,255,255)
					,Gdiplus::Color(252,252,252)
					,Gdiplus::Color(231,233,238)
					,Gdiplus::Color(205,210,219)
				};
				Gdiplus::REAL reals[]	=	{0.0f,0.5f,0.5f,1.0f};
				brush.SetInterpolationColors(colors,reals,4);
				graphics.FillRectangle(&brush,fill_rect);

				graphics.DrawRectangle(&pen,rect.left,rect.top,rect.Width(),rect.Height()-3);
				graphics.DrawLine(&pen,rect.left,rect.bottom,rect.right,rect.bottom);
			}
		}

		graphics.DrawImage(items[index].image_ptr.get()
			,(INT)(rect.left + item_padding)
			,(INT)(rect.top + item_padding)
			,(INT)(rect.Width() - item_padding * 2)
			,(INT)(rect.Height() - item_padding * 2)
			);
	}
}

bool CTabImageDraw::insert_revision(const UINT _where,const UINT count,const bool is_draw)
{
	UINT max_show	=	get_max_show();

	//是否需要重繪
	bool is_need_draw	=	false;
	if(_where >= scroll
		&& _where < scroll + max_show
		)
	{
		is_need_draw	=	true;
	}

	//調整 滾動條
	if(scroll >= _where)
	{
		if(_where + count != items.size())
		{
			scroll_down(count,false);
		}
	}

	//調整 當前選中項
	if(select != MFCUI_INVALID_INDEX)
	{
		if(select >= _where)
		{		
			select	+=	count;
		}
	}

	//調整高光
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	UINT index	=	get_point_index(point);
	hover_revision(index);

	//重繪
	if(is_draw 
		&& is_need_draw
		)
	{
		RedrawWindow();
	}
	return is_need_draw;
}

bool CTabImageDraw::erase_revision(const UINT _where,const UINT count,const bool is_draw)
{
	UINT max_show	=	get_max_show();

	//是否需要重繪
	bool is_need_draw	=	false;
	if(_where >= scroll
		&& _where < scroll + max_show
		)
	{
		is_need_draw	=	true;
	}

	//調整 滾動條
	if(scroll >= _where)
	{
		if(scroll > _where + count)
		{
			scroll_up(count,false);
		}
	}

	//調整 當前選中項
	if(select != MFCUI_INVALID_INDEX)
	{
		if(select >= _where)
		{
			if(select < _where + count)
			{
				select = MFCUI_INVALID_INDEX;
			}
			else
			{
				select	-=	count;
			}
		}
	}
	//調整高光
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	UINT index	=	get_point_index(point);
	hover_revision(index);

	//重繪
	if(is_draw 
		&& is_need_draw
		)
	{
		RedrawWindow();
	}
	return is_need_draw;
}
bool CTabImageDraw::is_item_draw(const UINT index) const
{
	UINT need_show	=	get_need_show();

	if(index < scroll)
	{
		return false;
	}
	else if(scroll + need_show <= index)
	{
		return false;
	}

	return true;
}
bool CTabImageDraw::get_item_rect(const UINT index,CRect& rect)const
{
	if(!is_item_draw(index))
	{
		return false;
	}

	UINT size	=	get_item_size();
	UINT x,y;
	if(get_style() & MFCUI_STYLE_VERTICAL)
	{
		x	=	get_padding();
		y	=	get_padding() + (index - scroll) * (size + get_padding());
	}
	else
	{
		x	=	get_padding() + (index - scroll) * (size + get_padding());
		y	=	get_padding();
	}

	rect.left	=	x;
	rect.top	=	y;
	rect.right	=	x + size;
	rect.bottom	=	y + size;
	
	return true;
}

BOOL CTabImageDraw::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//return __super::OnEraseBkgnd(pDC);
}
UINT CTabImageDraw::get_item_size() const
{
	UINT size;
	CRect rect;
	GetClientRect(rect);
	
	if(get_style() & MFCUI_STYLE_VERTICAL)
	{
		size	=	rect.Width();
	}
	else
	{
		size	=	rect.Height();
	}

	size	-=	get_padding() * 2;

	return size;
}
UINT CTabImageDraw::get_max_show()const
{
	UINT tmp;
	UINT size	=	get_item_size() + get_padding();
	CRect rect;
	GetClientRect(rect);
	UINT length;
	if(get_style() & MFCUI_STYLE_VERTICAL)
	{
		length	=	rect.Height();
	}
	else
	{
		length	=	rect.Width();
	}

	tmp		=	length / size;	
	length -= (tmp * size) - get_padding() * 2;
	if(length > size /3)
	{
		++tmp;
	}

	return tmp;
}
UINT CTabImageDraw::get_need_show()const
{
	UINT tmp		=	items.size() - scroll;
	UINT max_show	=	get_max_show();
	if(tmp > max_show)
	{
		return max_show;
	}

	return tmp ;
}

UINT CTabImageDraw::get_max_scroll()const
{
	UINT max;
	UINT size		=	items.size();
	UINT max_show	=	get_max_show();
	if(size < max_show)
	{
		max	=	0;
	}
	else if(size == max_show)
	{
		if(size > 1)
		{
			max	=	1;
		}
		else
		{
			max	=	0;
		}
	}
	else
	{
		max	=	size - max_show ;
		if(size > 1)
		{
			++max;
		}
	}
	return max;
}
UINT CTabImageDraw::scroll_down(const UINT count,const bool is_draw)
{
	UINT old	=	scroll;
	scroll	+=	count;
	UINT max_scroll	=	get_max_scroll();
	if(scroll > max_scroll)
	{
		scroll = max_scroll;
	}

	//調整 hover
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	int index	=	get_point_index(point);
	hover_revision(index);

	if(is_draw
		&& old != scroll
		)
	{
		RedrawWindow();
	}
	return old;
}
UINT CTabImageDraw::scroll_up(const UINT count,const bool is_draw)
{
	UINT old	=	scroll;
	if(scroll > count)
	{
		scroll	-=	count;
	}
	else
	{
		scroll	=	0;
	}

	//調整 hover
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	int index	=	get_point_index(point);
	hover_revision(index);

	if(is_draw
		&& old != scroll
		)
	{
		RedrawWindow();
	}

	return old;
}
UINT CTabImageDraw::scroll_to(const UINT _where)
{
	if(_where == scroll)
	{
		return scroll;
	}
	UINT old	=	scroll;

	if(_where <= get_max_scroll())
	{
		scroll	=	_where;
	}

	//調整 hover
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	int index	=	get_point_index(point);
	hover_revision(index);

	if(old != scroll)
	{
		RedrawWindow();
	}

	return old;
}
BOOL CTabImageDraw::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

	if(zDelta < 0)
	{
		scroll_down(1);
	}
	else if(zDelta > 0)
	{
		scroll_up(1);
	}
	return __super::OnMouseWheel(nFlags, zDelta, pt);
}


void CTabImageDraw::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	drag_index	=	get_point_index(point);
	SetCapture();
	__super::OnLButtonDown(nFlags, point);
}

UINT CTabImageDraw::get_point_index(const POINT& point)const
{
	CRect rect;
	for(UINT i=scroll;i < scroll + get_need_show();++i)
	{
		if(!get_item_rect(i,rect))
		{
			break;
		}

		if(point.x < rect.right
			&& point.x > rect.left
			&& point.y < rect.bottom
			&& point.y > rect.top
			)
		{
			return i;
		}
	}
	return MFCUI_INVALID_INDEX;
}

void CTabImageDraw::hover_revision(const UINT index)
{
	if(hover != index)
	{
		UINT old	=	hover;
		hover		=	index;
		bool is_draw_old	=	false;
		bool is_draw_now	=	false;
		if(old != MFCUI_INVALID_INDEX)
		{
			is_draw_old	= is_item_draw(old);
		}
		if(hover != MFCUI_INVALID_INDEX)
		{
			is_draw_now	= is_item_draw(hover);
		}

		if(is_draw_old 
			|| is_draw_now
			)
		{
			CRect rect;
			GetClientRect(rect);
	
			Gdiplus::Bitmap		bitmap(rect.Width(),rect.Height());
			Gdiplus::Graphics	graphics_catche(&bitmap);

			if(is_draw_old)
			{
				draw_item(graphics_catche,old);
			}
			if(is_draw_now)
			{
				draw_item(graphics_catche,hover);
			}

			Gdiplus::Graphics graphics(m_hWnd);
			Gdiplus::CachedBitmap cache_bitmap(&bitmap,&graphics);
			graphics.DrawCachedBitmap(&cache_bitmap,0,0);    
		}
	}
}
void CTabImageDraw::OnMouseMove(UINT nFlags, CPoint point)
{
	UINT index	=	get_point_index(point);
	hover_revision(index);

	if (!is_tracking)
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = this->m_hWnd;
        
        if (::_TrackMouseEvent(&tme))
        {
            is_tracking = true;   
        }
    }

	if(!track_left
		&& drag_index	!=	MFCUI_INVALID_INDEX
		)
	{
		track_left	=	true;
	}
	
	__super::OnMouseMove(nFlags, point);
}


void CTabImageDraw::OnMouseLeave()
{
	is_tracking = false;
	hover_revision(MFCUI_INVALID_INDEX);

	__super::OnMouseLeave();
}


void CTabImageDraw::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	UINT index	=	get_point_index(point);
	if(track_left)
	{
		if(index != MFCUI_INVALID_INDEX)
		{
			//swap
		}
	}
	else if(drag_index	!=	MFCUI_INVALID_INDEX)
	{
		if(drag_index == index)
		{
			int old_select	=	select;
			if(select != index)
			{
				select	=	index;
				
				CRect rect;
				GetClientRect(rect);

				Gdiplus::Bitmap		bitmap(rect.Width(),rect.Height());
				Gdiplus::Graphics	graphics_catche(&bitmap);

				draw_item(graphics_catche,select);
				if(old_select != MFCUI_INVALID_INDEX)
				{
					draw_item(graphics_catche,old_select);
				}

				Gdiplus::Graphics graphics(m_hWnd);
				Gdiplus::CachedBitmap cache_bitmap(&bitmap,&graphics);
				graphics.DrawCachedBitmap(&cache_bitmap,0,0);    
			}
			//click item (index)
		}
	}
	else if(drag_index	==	MFCUI_INVALID_INDEX)
	{
		if(MFCUI_INVALID_INDEX == index)
		{
			//click unitem
		}
	}

	drag_index	=	MFCUI_INVALID_INDEX;
	track_left	=	false;	

	__super::OnLButtonUp(nFlags, point);
}
