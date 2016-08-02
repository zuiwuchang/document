#include "StdAfx.h"
#include "ui_base.h"


ui_base::ui_base(void)
{
	style	=	MFCUI_STYLE_BACKGROUND;
	padding	=	5;
}


ui_base::~ui_base(void)
{
}

void ui_base::init_default()
{
	font_ptr			=	boost::make_shared<Gdiplus::Font>(L"ËÎÌו",10.0f);
	
	string_format_ptr	=	boost::make_shared<Gdiplus::StringFormat>();
	string_format_ptr->SetAlignment(Gdiplus::StringAlignmentCenter);
	string_format_ptr->SetLineAlignment(Gdiplus::StringAlignmentCenter);

	text_brush_ptr		=	boost::make_shared<Gdiplus::SolidBrush>(Gdiplus::Color(173,178,181));

	text_disabled_brush_ptr	=	boost::make_shared<Gdiplus::SolidBrush>(Gdiplus::Color(0,0,0));
}
UINT ui_base::modify_style(const UINT add,const UINT dec)
{
	UINT result	=	style;
	style	|=	add;
	style	&=	(~dec);
	return result;
}

bool ui_base::draw_what(Gdiplus::Graphics& graphics,const int what,const CRect& rect)
{
	if(bitmaps[what])
	{
		return graphics.DrawImage(bitmaps[what].get()
			,rect.left
			,rect.top
			,rect.Width()
			,rect.Height()
			)	== Gdiplus::Ok;
	}

	return false;
}
bool ui_base::draw_what(Gdiplus::Graphics& graphics,const int what,const int x,const int y,const int width,const int height)
{
	if(bitmaps[what])
	{
		return graphics.DrawImage(bitmaps[what].get()
		,x
		,y
		,width
		,height
		)	== Gdiplus::Ok;
	}

	return false;
}

void ui_base::draw_text(Gdiplus::Graphics& graphics,const std::wstring& text,const CRect& rect)
{
	graphics.DrawString(text.data()
		,text.size()
		,font_ptr.get()
		,Gdiplus::RectF((Gdiplus::REAL)rect.left,(Gdiplus::REAL)rect.top,(Gdiplus::REAL)rect.Width(),(Gdiplus::REAL)rect.Height())
		,string_format_ptr.get()
		,text_brush_ptr.get()
		);
}
void ui_base::draw_text(Gdiplus::Graphics& graphics,const std::wstring& text,const int x,const int y,const int width,const int height)
{
	graphics.DrawString(text.data()
		,text.size()
		,font_ptr.get()
		,Gdiplus::RectF((Gdiplus::REAL)x,(Gdiplus::REAL)y,(Gdiplus::REAL)width,(Gdiplus::REAL)height)
		,string_format_ptr.get()
		,text_brush_ptr.get()
		);
}

void ui_base::draw_disabled_text(Gdiplus::Graphics& graphics,const std::wstring& text,const CRect& rect)
{
	graphics.DrawString(text.data()
		,text.size()
		,font_ptr.get()
		,Gdiplus::RectF((Gdiplus::REAL)rect.left,(Gdiplus::REAL)rect.top,(Gdiplus::REAL)rect.Width(),(Gdiplus::REAL)rect.Height())
		,string_format_ptr.get()
		,text_disabled_brush_ptr.get()
		);
}
void ui_base::draw_disabled_text(Gdiplus::Graphics& graphics,const std::wstring& text,const int x,const int y,const int width,const int height)
{
	graphics.DrawString(text.data()
		,text.size()
		,font_ptr.get()
		,Gdiplus::RectF((Gdiplus::REAL)x,(Gdiplus::REAL)y,(Gdiplus::REAL)width,(Gdiplus::REAL)height)
		,string_format_ptr.get()
		,text_disabled_brush_ptr.get()
		);
}