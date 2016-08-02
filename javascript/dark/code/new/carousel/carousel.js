var namespace = dark_namespace("dark.ui");

namespace.carousel	=	{
	create:function(_jquery,_args){
		//派生子類
		var instance	=	namespace.base.create(_jquery,_args);

		instance.interval = 0;
		instance.begin	=	0;
		if (_args != null) {

			//自動播放時間間隔 0 不自動播放
			if(_args.interval != null){
				instance.interval = _args.interval;
			}

			//第一個播放的圖片
			if(_args.begin != null){
				instance.begin = _args.begin;
			}

		};

		
		//獲取圖片數組
		var image_arrays = _jquery.children('.dark_ui_carousel_node');
		//獲取圖片數量
		instance.size = image_arrays.length;
		//設置當前播放位置
		instance.current = instance.begin;

		//顯示第一個 圖片
		$(image_arrays[instance.begin]).show();



		//創建分頁按鈕
		var pagination	=	"<ul class='dark_ui_carousel_slidesjs_pagination'>";
		for(var i=0;i<instance.size;++i){
			pagination	+=	"<li class='dark_ui_carousel_slidesjs_pagination_item'><a href='";
			pagination	+=	i;
			if(i == instance.current){
				pagination	+=	"' class='active";
			}
			pagination	+=	"'>";
			pagination	+=	i;
			pagination	+=	"</a></li>";
		}
		pagination	+=	"</ul>";
		instance.pagination	=	$(pagination);
		_jquery.append(instance.pagination);




		//調整按鈕位置
		var height = _jquery.height();
		var width = _jquery.width();
		var jq = _jquery.children('.dark_ui_carousel_previous');
		jq.css({
			top: (height - jq.height()) / 2,
			left:5
		});

		jq = _jquery.children('.dark_ui_carousel_next');
		jq.css({
			top: (height - jq.height()) / 2,
			left: width - jq.width() - 5
		});



		//調整分頁標籤位置
		jq	=	instance.pagination;
		jq.css({
			left: width - jq.width() - instance.size * (13 + 3),
			top: jq.height() / 3
		});



		//private 輔助方法定義 更改 分頁標準狀態
		var change_page_state = function(n){
			instance.pagination.find('.active').removeClass('active');
			var jq = $(instance.pagination.find('a')[n]);
			jq.addClass('active');
		}

		//private 輔助方法定義 更改 顯示文本
		var change_page_text = function(n){
			var jq = $(image_arrays[n]);
			var text = jq.children('span').text();
			_jquery.children('.dark_ui_carousel_text_show').html("<h3>" + text + "</h3>");

		}
		change_page_text(instance.begin);

		var set_play_timer = function(){
			if(instance.timer != undefined){
				clearTimeout(instance.timer);
				instance.timer = undefined;
			}
			if(instance.interval > 0){
				instance.timer = setTimeout(function(){
					instance.next();
				},instance.interval);
			}
		}

		//方法定義 顯示 下一個圖片
		instance.next	=	function(){
			var n = instance.current + 1;
			if(n >= instance.size){
				n = 0;
			}

			$(image_arrays[instance.current]).slideUp("slow");
			$(image_arrays[n]).slideDown("slow");
			instance.current = n;

			change_page_state(n);
			change_page_text(n);
			set_play_timer();
		}
		//方法定義顯示上一個 圖片
		instance.previous	=	function(){
			var n = instance.current - 1;
			if(n < 0 ){
				n = instance.size - 1;
			}

			$(image_arrays[instance.current]).slideUp("slow");
			$(image_arrays[n]).slideDown("slow");
			instance.current = n;

			change_page_state(n);
			change_page_text(n);
			set_play_timer();
		}
		

		//方法定義顯示指定位置的 圖片
		instance.show_n	=	function(n){
			if(n == instance.current){
				return;
			}
			if(n < 0){
				n = 0;
			}else if(n >= instance.size){
				n = instance.size - 1;
			}

			$(image_arrays[instance.current]).slideUp("slow");
			$(image_arrays[n]).slideDown("slow");
			instance.current = parseInt(n);

			change_page_state(n);
			change_page_text(n);
			set_play_timer();
		}

		//方法定義 自動播放
		instance.play	=	function(interval){
			instance.interval = interval;
		}

		//方法定義 停止 自動播放
		instance.stop	=	function(){
			instance.interval = 0;
			if(instance.timer != undefined){
				clearTimeout(instance.timer);
				instance.timer = undefined;
			}
		}



		//綁定事件
		_jquery.children('.dark_ui_carousel_next')
			.click(function(event) {
			instance.next();
			return false;
		});
		_jquery.children('.dark_ui_carousel_previous')
			.click(function(event) {
			instance.previous();
			return false;
		});
		instance.pagination.find('a')
			.click(function(event) {
			var n = $(this).attr("href");
			instance.show_n(n);
			return false;
		});




		//設置自動 播放
		if(instance.interval > 0 ){
			set_play_timer();
		}

		return instance;
	}
}