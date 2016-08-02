// JavaScript Document
var dark_listbox_wrap	=	{
	create:function(_element){
		var instance	=	{};
		
		//保存被包裹元素
		var wrap_	=	_element;
		
		//返回設置 高度
		instance.height	=	function(){
			var parent	=	wrap_.next(".lbjs:first");
			if(arguments.length == 0){
				return parent.children(".lbjs-list:first").height();
			}
			else{
				parent.height(arguments[0]);
				parent.children(".lbjs-list:first").height(arguments[0]);
			}
		}
		//返回設置 寬度
		instance.width	=	function(){
			var parent	=	wrap_.next(".lbjs:first");
			if(arguments.length == 0){
				return parent.children(".lbjs-list:first").width();
			}
			else{
				parent.width(arguments[0]);
				parent.children(".lbjs-list:first").width(arguments[0]);
			}
		}		
		
		//返回被包裹元素
		instance.wrap	=	function(){
				return wrap_;
			};
			
		//返回/設置 被選中 item 值 (多個值 返回數組)	
		instance.val	=	function(){
			var parent	=	wrap_.next(".lbjs:first");
			
			//返回選中項
			if(arguments.length == 0){
				var item	=	parent.find(".lbjs-item[selected='selected']");
				var size = item.size();
				var rs	=	null;
				if(size == 1){
					var index = item.index();
					var items	=	wrap_.children("option");
					size = 	items.size();
					if(index < size){
						rs	=	$(items[index]).val();
					}
				}
				else if(size > 1){
					rs	=	new Array();
					var items	=	wrap_.children("option");
					item.each(function(i, element) {
                    	var index = $(element).index();
						rs[i]	=	 $(items[index]).val();
                    });
				}
				return rs;
			}
			
			//設置選中項 傳入索引
			var size	=	instance.size();
			if(wrap_.attr("multiple") == "multiple"){
				//多選
				for(var i=0;i<arguments.length;++i){
					var index	=	arguments[i];
					if(index >= 0
						&& index < size){
						var item	=	parent.find(".lbjs-item:eq(" + index + ")");
						item.attr("selected","selected");
					}
				}
			}
			else{
				//單選
				var index	=	arguments[0];
				if(index >= 0
					&& index < size){
					parent.find(".lbjs-item").removeAttr("selected");
					var item	=	parent.find(".lbjs-item:eq(" + index + ")");
					item.attr("selected","selected");
				}
			}
			wrap_.val(instance.val());
		}	
		
		//清空被選項
		instance.clear_select	=	function(){
			var parent	=	wrap_.next(".lbjs:first");
			parent.find(".lbjs-item").removeAttr("selected");
		}
		
		//返回 item 數量
		instance.size	=	function(){
			return wrap_.children("option").size();
		}
		
		//增加item
		instance.insert	=	function(where,value,text,selected){
			var size = instance.size();
			if(where >= size
				|| where == -1
				){
				where = size - 1;
			}
			if(where < 0){
				where = 0;
			}
			
			var html;
			if(selected){
				html	=	"<div class='lbjs-item'>" + text + "</div>";
			}
			else{
				html	=	"<div class='lbjs-item'>" + text + "</div>";
			}	
			
			var option;
			if(value == text){
				option	=	"<option>" + value + "</option>"
			}
			else{
				option	=	"<option value='" + value + "'>" + text + "</option>"
			}
			
			var parent	=	wrap_.next(".lbjs:first");
			parent	=	parent.children(".lbjs-list");
			if(size == 0){
				parent.append($(html));
				wrap_.append($(option));
			}
			else{
				var children	=	parent.children(".lbjs-item");
				$(children[where]).before($(html));
				$(wrap_.children("option")[where]).before($(option));
			}
			
			
		}
		
		return instance;
	}
};