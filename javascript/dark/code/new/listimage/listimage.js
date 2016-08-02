var namespace = dark_namespace("dark.ui");

namespace.listimage	=	{
	create:function(_jquery,_args){
		//派生子類
		var instance	=	namespace.base.create(_jquery,_args);



		//獲取初始化參數
		instance.edit = _args.edit != undefined?_args.edit:true;
		instance.edit_text = _args.edit_text != undefined?_args.edit_text:"input new values";
		instance.select	=	_args.select != undefined?_args.select:true;
		instance.on_edit_change = _args.on_edit_change;
		instance.on_click_node = _args.on_click_node;
		instance.on_dbclick_node = _args.on_dbclick_node;
		instance.on_mouse_down = _args.on_mouse_down;
		instance.on_mouse_up = _args.on_mouse_up;


		//private 方法定義 為元素綁定事件
		var private_bind_node_event = function(jq){
			jq.children('a:first').click(function(event) {
				var tmp = $(this).parent(".dark_ui_listimage_node");
				var n = tmp.index();

				if(instance.select){
					if(instance.check_select(n)){
						instance.unselect_node(n);
					}else{
						instance.select_node(n);
					}
				}

				if(instance.on_click_node != undefined){
					instance.on_click_node(n);
				}
				return false;
			}).dblclick(function(event) {
				if(instance.on_dbclick_node != undefined){
					var tmp = $(this).parent(".dark_ui_listimage_node");
					var n = tmp.index();
					instance.on_dbclick_node(n);
				}
			}).mousedown(function(event) {
				if(instance.on_mouse_down != undefined){
					var tmp = $(this).parent(".dark_ui_listimage_node");
					var n = tmp.index();
					instance.on_mouse_down(n,event);
				}
				return true;
			}).mouseup(function(event) {
				if(instance.on_mouse_up != undefined){
					var tmp = $(this).parent(".dark_ui_listimage_node");
					var n = tmp.index();
					instance.on_mouse_up(n,event);
				}
				return true;
			});
			jq.children('.dark_ui_listimage_node_text').click(function(event) {
				if(instance.edit){
					var old = $(this).text();
					var input = prompt(instance.edit_text,old);
					if(input != null 
						&& input != old){
						$(this).text(input);

						if(instance.on_edit_change != undefined){
							var tmp = $(this).parent(".dark_ui_listimage_node");
							var n = tmp.index();
							instance.on_edit_change(n);
						}
					}
				}
				return false;
			});
		}


		//private 方法定義 生成 元素 html
		var private_cteate_node_html = function(obj){
			var html = "<li class='dark_ui_listimage_node'";
			if(obj.data != undefined){
				html += " data-dark='";
				html += obj.data;
				html += "'";
			}
			html += "><a href='#'><img src='";
			html += obj.url;
			html += "' class='dark_ui_listimage_node_image' alt='#'></a><a href='#' class='dark_ui_listimage_node_text'>";
			html += obj.text;
			html += "</a><span></span></li>";

			return html;
		}



		//方法定義 返回 元素數量
		instance.size	=	function(){
			return _jquery.children('.dark_ui_listimage_node').length;
		}

		//方法定義 插入 元素 到末尾
		instance.append	=	function(obj){
			//構造元素
			var html = private_cteate_node_html(obj);



			//綁定事件
			var jq = $(html);
			private_bind_node_event(jq);



			//插入元素
			_jquery.append(jq);
		}

		//方法定義 插入 元素
		instance.insert	=	function(_where,obj){
			/*{
				text:顯示文本
				url:圖片路徑
				data:附加數據
			}
			*/

			//構造元素
			var html = private_cteate_node_html(obj);



			//綁定事件
			var jq = $(html);
			private_bind_node_event(jq);



			//插入元素
			var size = instance.size();
			if( size == 0
				|| _where == -1){
				_jquery.append(jq);
			}else{
				if(_where >= size){
					_where = size - 1;
				}

				_jquery.children('.dark_ui_listimage_node').eq(_where).before(jq);
			}
			return instance;
		}

		//方法定義 刪除 元素
		instance.remove = function(_where){
			var jq = _jquery.children('.dark_ui_listimage_node').eq(_where);
			jq.remove();

			return instance;
		}
		//方法定義 清空 元素
		instance.clear = function(){
			_jquery.html("");
			return instance;
		}

		//方法定義 設置/獲取 文本
		instance.set_text = function(n,text){
			var jq = _jquery.children('.dark_ui_listimage_node').eq(n);
			jq.children('.dark_ui_listimage_node_text').text(text);
			return instance;
		}
		instance.get_text = function(n){
			var jq = _jquery.children('.dark_ui_listimage_node').eq(n);
			return jq.children('.dark_ui_listimage_node_text').text();
		}

		//方法定義 選中 元素
		instance.select_node	=	function(n){
			if(!instance.select){
				return instance;
			}

			var jq = _jquery.children('.dark_ui_listimage_node').eq(n);
			jq.children('span').addClass('dark_ui_listimage_node_select');
			return instance;
		}
		//方法定義 選中 元素
		instance.unselect_node	=	function(n){
			var jq = _jquery.children('.dark_ui_listimage_node').eq(n);
			jq.children('span').removeClass('dark_ui_listimage_node_select');
			return instance;
		}
		//方法定義 返回 元素是否被選中
		instance.check_select	=	function(n){
			var jq = _jquery.children('.dark_ui_listimage_node').eq(n);
			jq = jq.children('span');
			return jq.hasClass('dark_ui_listimage_node_select');
		}
		//方法定義 返回首先被選中元素 不存在返回 undefined
		instance.find_first_select = function(){
			var jq = _jquery.children('.dark_ui_listimage_node');
			var size = jq.size();
			if(size >0 ){
				for(var i=0;i<size;++i){
					var pos = jq.eq(i);
					if(pos.children('span').hasClass('dark_ui_listimage_node_select')){
						var obj = {};
						obj.pos = pos;
						obj.jq = jq;
						obj.n = i;
						obj.size = size;

						return obj;
					}
				}
			}
		}
		//方法定義 返回下個被選中元素 不存在返回 undefined
		instance.find_next_select = function(obj){
			for (var i = obj.n + 1; i < obj.size; ++i) {
				var pos = obj.jq.eq(i);
				if(pos.children('span').hasClass('dark_ui_listimage_node_select')){
					obj.pos = pos;
					obj.n = i;

					return true;
				}

			};
			return false;
		}

		//方法定義 設置/獲取 url
		instance.set_url = function(n,url){
			var jq = _jquery.children('.dark_ui_listimage_node').eq(n);
			jq.find('.dark_ui_listimage_node_image').attr("src",url);
			return instance;
		}
		instance.get_url = function(n){
			var jq = _jquery.children('.dark_ui_listimage_node').eq(n);
			return jq.find('.dark_ui_listimage_node_image').attr("src");
		}

		//方法定義 設置/獲取 附加數據
		instance.set_data = function(n,data){
			var jq = _jquery.children('.dark_ui_listimage_node').eq(n);
			jq.attr("data-dark",data);
			return instance;
		}
		instance.get_data = function(n){
			var jq = _jquery.children('.dark_ui_listimage_node').eq(n);
			return jq.attr("data-dark");
		}



		//事件綁定
		_jquery.children('.dark_ui_listimage_node')
			.each(function(index, el) {
				private_bind_node_event($(el));
			});
	

		return instance;
	}
}