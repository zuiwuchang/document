// JavaScript Document
var listctrl	=	dark_namespace("dark.ui.listctrl");

listctrl.create	=	function(_param){
	var instance	=	{};
	instance.param	=	_param;
	var type	=	typeof(_param.element);
	
	//構造html
	var html	=	"<div class='dark_ui_listctrl'>"
		html	+=	"<div class='dark_ui_listctrl_list'>";
	
		if(type	==	"object"){
			var i = 0;
			_param.element.children("option").each(function(index, element) {
				if($(element).attr("selected") == "selected"
					&& i == 0
					){
					html	+=	"<div selected = 'selected' class='dark_ui_listctrl_list_item' data-value='"+ $(element).val() +"'>" + $(element).text() + "</div>";
					if(_param.multiple != "multiple"){
						++i;
					}
				}
				else{
					html	+=	"<div class='dark_ui_listctrl_list_item' data-value='"+ $(element).val() +"'>" + $(element).text() + "</div>";
				}
            });
			
		}
		
		html	+=	"</div>";
	html	+=	"</div>";
	
	//生成 jquery 元素
	var jquery	=	$(html);
	instance.jQuery	=	jquery;
	
	//綁定事件
	jquery.find(".dark_ui_listctrl_list_item").click(function(e) {
        if(instance.param.multiple	==	"multiple"){
			if($(this).attr("selected")	==	"selected"){
				$(this).removeAttr("selected");
			}
			else{
				$(this).attr("selected","selected");
			}
		}
		else{
			jquery.find("div[selected='selected']").removeAttr("selected");
			$(this).attr("selected","selected");
		}
		
		if(instance.param.click_item != undefined){
			instance.param.click_item(e,$(this));
		}	
		if(instance.param.change_item != undefined){
			instance.param.change_item();
		}
    });
	
	//方法定義
	{
		//返回 item 數量
		instance.size	=	function(){
			return jquery.find(".dark_ui_listctrl_list_item").size();
		}
		
		//清空列表
		instance.clear	=	function(){
			jquery.children(".dark_ui_listctrl_list").html("");
		}
		
		//刪除元素
		instance.erase	=	function(){
			var change = false;
			for(var i=0;i<arguments.length;++i){
				var item	=	jquery.find(".dark_ui_listctrl_list_item:eq(" + arguments[i] + ")");
				if(instance.param.change_item != undefined
					&& !change
					&& item.attr('selected','selected')
					){
					change	=	true;
				}
				item.remove();
			}
			if(change){
				instance.param.change_item();
			}
		}
		
		//插入元素
		instance.insert	=	function(_where/*-1 最後*/,_value/*值*/,_text/*顯示文本 默認=_value*/){
			var size = instance.size();
			if(_where > size
				|| _where == -1
				){
				_where = size;
			}
			if(_where < 0){
				_where = 0;
			}
			if(undefined == _text){
				_text = _value;
			}
			
			var html	=	"<div class='dark_ui_listctrl_list_item' data-value='" + _value +"'>" + _text + "</div>";
			if(_where == size){
				
				jquery.children(".dark_ui_listctrl_list").append($(html));
			}
			else{
				jquery.find(".dark_ui_listctrl_list_item:eq(" + _where + ")").before($(html));
			}
		}
		//返回/設置 高度
		instance.height	=	function(_height){
			if(undefined	==	_height){
				return jquery.height();
			}
			else{
				jquery.height(_height);
			}
		}
		//返回/設置 寬度
		instance.width	=	function(_width){
			if(undefined	==	_width){
				return jquery.width();
			}
			else{
				jquery.width(_width);
			}
		}
		
		//返回/選中 指定項
		instance.select	=	function(){
			//返回
			if(arguments.length == 0){
				var rs	=	null;
				var notes	=	jquery.find(".dark_ui_listctrl_list_item[selected='selected']");
				if(notes.size() > 0){
					rs	=	notes;
				}
				return rs;
			}
			//選中
			else{
				//多選
				if(_param.multiple == 'multiple'){
					for(var i=0;i<arguments.length;++i){
						jquery.find(".dark_ui_listctrl_list_item:eq(" + arguments[i] + ")").attr('selected','selected');
					}
				}
				//單選
				else{
					jquery.find(".dark_ui_listctrl_list_item[selected='selected']").removeAttr('selected');
					jquery.find(".dark_ui_listctrl_list_item:eq(" + arguments[0] + ")").attr('selected','selected');
				}
				if(instance.param.change_item != undefined){
					instance.param.change_item();
				}	
			}
		}
	}
	//替換元素
	if(type	==	"object"){
		jquery.replaceAll(_param.element);
	}	
	return instance;
}
