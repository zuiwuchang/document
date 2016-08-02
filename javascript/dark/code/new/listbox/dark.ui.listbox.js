// JavaScript Document
var namespace = dark_namespace("dark.ui");

namespace.listbox	=	{
	item_format_html:function(img,text,value){
		if(undefined	==	text){
			text = "";
		}
		var html
		if(undefined	==	value){
			html	=	"<div>";
		}
		else{
			html	=	"<div data-value='" + value + "'>";
		}
		html	+=	"<img src='" + img + "' /><br />\
            <strong>" + text + "</strong>\
        	</div>";
		return html;
	}
	,item_format:function(instance,obj,items){
		items.addClass("dark_ui_listbox_item");	
		
		var item_width	=	instance.item_width();
		var item_height	=	instance.item_height();
		
		var image_width	=	instance.image_width();
		var image_height	=	instance.image_height();
		
		//屬性
		items.width(item_width);
		items.height(item_height);
		
		//images
		var imgs	=	items.find("img");
		imgs.addClass("dark_ui_listbox_item_image");
			
		imgs.mouseenter(function(e) {
			$(this).removeClass("dark_ui_listbox_item_image")
				.addClass("dark_ui_listbox_item_image_hover")
					.width(image_width + 10)
					.height(image_height + 10)
					;
		});
		imgs.mouseleave(function(e) {
			$(this).removeClass("dark_ui_listbox_item_image_hover")
				.addClass("dark_ui_listbox_item_image")
					.width(image_width)
					.height(image_height)
					;
		});
			
		//down
		imgs.mousedown(function(e) {
			if(e.button == 0){
				$(this).addClass("dark_ui_listbox_item_image_down")
					.width(image_width + 12)
					.height(image_height + 12)
					;
			}
		});
		imgs.mouseup(function(e) {
			if(e.button == 0){
				$(this).removeClass("dark_ui_listbox_item_image_down")
					.width(image_width + 10)
					.height(image_height + 10)
					;
			}
		});
	
		//click
		imgs.click(function(e) {
			if(undefined	!=	instance.click_item_call_back_){ 
				
				instance.click_item_call_back_(instance,$(this).parent("div").index());
			}
		});
		
	}
	
	,create:function(_jquery,_args){
		//派生子類
		var instance	=	namespace.base.create(_jquery,_args);
		
		var image_width_	=	128;
		var image_height_	=	128;
		var item_width_		=	128 + 5 * 2 + 2;
		var item_height_	=	128 + 5 * 2 + 2 + 22;		
		
		//初始化參數
		if("object"	==	typeof(_args)){
			if("function"	==	typeof(_args.click_item)){
				instance.click_item_call_back_	=	_args.click_item;
			}
			if(undefined	!=	_args.image_width){
				image_width_	=	_args.image_width;
			}
			if(undefined	!=	_args.image_height){
				image_height_	=	_args.image_height;
			}
			if(undefined	!=	_args.item_width){
				item_width_	=	_args.item_width;
			}
			if(undefined	!=	_args.item_height){
				item_height_	=	_args.item_height;
			}
		}	
		
		//返回設置 item 寬高
		instance.item_width	=	function(_width){
			if(undefined	==	_width){
				return item_width_;
			}
			else{
				item_width_ = _width;
				instance.jquery().children(".dark_ui_listbox_item").width(_width);
			}
			return instance;				
		}
		instance.item_height	=	function(_height){
			if(undefined	==	_height){
				return item_height_;
			}
			else{
				item_height_ = _height;
				instance.jquery().children(".dark_ui_listbox_item").height(_height);
			}
			return instance;				
		}
		//public
			//返回設置 image 寬高
			instance.image_width	=	function(_width){
				if(undefined	==	_width){
					return image_width_;
				}
				else{
					image_width_ = _width;
					var jauery = instance.jquery();
					jauery.find(".dark_ui_listbox_item_image").width(_width);
					jauery.find(".dark_ui_listbox_item_image_hover").width(_width + 10);
					jauery.find(".dark_ui_listbox_item_image_down").width(_width + 12);
				}
				return instance;
			}
			instance.image_height	=	function(_height){
				if(undefined	==	_height){
					return image_height_;
				}
				else{
					image_height_ = _height;
					var jauery = instance.jquery();
					jauery.find(".dark_ui_listbox_item_image").height(_height);
					jauery.find(".dark_ui_listbox_item_image_hover").height(_height + 10);
					jauery.find(".dark_ui_listbox_item_image_down").height(_height + 12);
				}
				return instance;
			}
			
			//返回/設置 item 文本
			instance.item_text = function(index,text){
				if(undefined	==	text){
					return	_jquery.children("div:eq(" + index + ")").find("strong").text();
				}
				
				_jquery.children("div:eq(" + index + ")").find("strong").text(text);
				return instance;
			};
			//返回/設置 item 圖片地址
			instance.item_img = function(index,src){
				if(undefined	==	src){
					return	_jquery.children("div:eq(" + index + ")").find("img").attr("src");
				}
				_jquery.children("div:eq(" + index + ")").find("img").attr("src",src);
				return instance;
			};
			//返回/設置 item 附加信息
			instance.item_data = function(index,value){
				if(undefined	==	value){
					return	_jquery.children("div:eq(" + index + ")").attr("data-value"); 
				}
				_jquery.children("div:eq(" + index + ")").attr("data-value",value); 
				return instance;
			}
		
		
			//刪除所有item
			instance.clear	=	function(){
				_jquery.html("");
				return instance;
			}
			//刪除指定item
			instance.erase	=	function(index){
				_jquery.children("div:eq(" + index + ")").remove();
				return instance;
			}
			var my	=	this;
			//壓入item
			instance.push_bakck	=	function(img,text,value){
				var html	=	my.item_format_html(img,text,value);
				var item	=	$(html);
				var obj		=	_jquery;
				my.item_format(instance,obj,item);
				obj.append(item);
				return instance;
			}
			instance.push_front	=	function(img,text,value){
				var html	=	my.item_format_html(img,text,value);
				var item	=	$(html);
				var obj		=	_jquery;
				my.item_format(instance,obj,item);
				obj.prepend(item);
				return instance;
			}
			//插入item
			instance.insert	=	function(index,img,text,value){
				var html	=	my.item_format_html(img,text,value);
				var item	=	$(html);
				var obj		=	_jquery;
				my.item_format(instance,obj,item);
				
				var note	=	obj.children("div:eq(" + index + ")");
				
				if(note.size() == 0){
					obj.append(item);
				}
				else{
					note.before(item);
				}
				
				return instance;
			}
	
		//初始化操作
		_jquery.addClass("dark_ui_listbox");
			this.item_format(instance,_jquery,_jquery.children("div"));
			//instance.item_width(item_width_);
			//instance.item_height(item_height_);
			instance.image_width(image_width_);
			instance.image_height(image_height_);
		return instance;
	}
}