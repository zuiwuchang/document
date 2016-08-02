#pragma once

#include<boost\array.hpp>
#include<boost\smart_ptr.hpp>

//背景 數量 索引
#define MFCUI_IMAGE_COUNT		6

#define MFCUI_BACKGROUND_INDEX	0
#define	MFCUI_DEFAULT_INDEX		1
#define MFCUI_DOWN_INDEX		2
#define MFCUI_HOVER_INDEX		3
#define MFCUI_FOCUS_INDEX		4
#define MFCUI_DISABLED_INDEX	5

//UI相關 風格
#define MFCUI_STYLE_FOCUS_LINE	0x1	//是否為焦點項目 繪製虛線
#define MFCUI_STYLE_BACKGROUND	0x2	//是否绘制背景
#define MFCUI_STYLE_VERTICAL	0x4	//垂直排列子項


#define MFCUI_INVALID_INDEX ((DWORD)-1)

class ui_base
{
protected:
	ui_base(void);
	~ui_base(void);

public:
	//初始化 默認設置 (在使用控件前 或需要重置默認設置時 調用)
	virtual void init_default();
private:
	UINT style;
	UINT padding;
public:
	inline void set_style(const UINT style)
	{
		this->style	=	style;
	}
	inline UINT get_style()const
	{
		return style;
	}
	UINT modify_style(const UINT add,const UINT dec = 0);

	inline void set_padding(const UINT padding)
	{
		this->padding	=	padding;
	}
	inline UINT get_padding()const
	{
		return padding;
	}
private:
	//繪製 各種背景
	boost::array<boost::shared_ptr<Gdiplus::Bitmap>,MFCUI_IMAGE_COUNT>	bitmaps;

	boost::shared_ptr<Gdiplus::Font>								font_ptr;
	boost::shared_ptr<Gdiplus::StringFormat>						string_format_ptr;
	boost::shared_ptr<Gdiplus::Brush>								text_brush_ptr;
	boost::shared_ptr<Gdiplus::Brush>								text_disabled_brush_ptr;

protected:
	bool draw_what(Gdiplus::Graphics& graphics,const int what,const CRect& rect);
	bool draw_what(Gdiplus::Graphics& graphics,const int what,const int x,const int y,const int width,const int height);


	void draw_text(Gdiplus::Graphics& graphics,const std::wstring& text,const CRect& rect);
	void draw_text(Gdiplus::Graphics& graphics,const std::wstring& text,const int x,const int y,const int width,const int height);

	void draw_disabled_text(Gdiplus::Graphics& graphics,const std::wstring& text,const CRect& rect);
	void draw_disabled_text(Gdiplus::Graphics& graphics,const std::wstring& text,const int x,const int y,const int width,const int height);
public:
	inline void set_what(const int what,boost::shared_ptr<Gdiplus::Bitmap> bitmap_ptr)
	{
		bitmaps[what]	=	bitmap_ptr;
	}
	inline boost::shared_ptr<Gdiplus::Bitmap> get_what(const int what)const
	{
		return bitmaps[what];
	}

	inline boost::shared_ptr<Gdiplus::Font> get_font()const
	{
		return font_ptr;
	}
	inline boost::shared_ptr<Gdiplus::StringFormat> get_string_format()const
	{
		return string_format_ptr;
	}
	inline boost::shared_ptr<Gdiplus::Brush> get_text_brush()const
	{
		return text_brush_ptr;
	}
	inline boost::shared_ptr<Gdiplus::Brush> get_disabled_text_brush()const
	{
		return text_disabled_brush_ptr;
	}
	inline void set_font(boost::shared_ptr<Gdiplus::Font> font_ptr)
	{
		this->font_ptr	=	font_ptr;
	}
	inline void set_string_format(boost::shared_ptr<Gdiplus::StringFormat> string_format_ptr)
	{
		this->string_format_ptr	=	string_format_ptr;
	}
	inline void set_text_brush(boost::shared_ptr<Gdiplus::Brush> text_brush_ptr)
	{
		this->text_brush_ptr	=	text_brush_ptr;
	}
	inline void set_disabled_text_brush(boost::shared_ptr<Gdiplus::Brush> text_disabled_brush_ptr)
	{
		this->text_disabled_brush_ptr	=	text_disabled_brush_ptr;
	}
};

