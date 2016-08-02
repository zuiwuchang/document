 // JavaScript Document
var namespace = dark_namespace("dark.ui");
namespace.rotation	=	{
	 create:function(_jquery,_args){
		//實例化對象
		var instance	=	namespace.base.create(_jquery,_args);		
		//private:
		//item 信息
		var items_propertys_	=	new Array;
		var item_width_ = 120;
		var item_height_ = 120;
		var play = true;
		
		//是否開始播放
	 	var play_	=	false;
	 	//保存定時器
	 	var timer_;
	 
	 	//fps
	 	var fps_	=	60;
	 
	 	//是否循環播放
		var loop_	=	true;
	 
	 	//循環一次使用時間
	 	var times_	=	10000;
	 
	 	//旋轉方向
	 	var clockwise_	=	true;
		
		//初始化參數
		if(undefined != _args){
			if(undefined != _args.play){
				play	=	_args.play;
			}
			if(undefined != _args.fps){
				fps_	=	_args.fps;
			}
			if(undefined != _args.times){
				times_	=	_args.times;
			}
			if(undefined != _args.loop){
				loop_	=	_args.loop;
			}
			if(undefined != _args.clockwise){
				clockwise_	=	_args.clockwise;
			}
			
			if(undefined != _args.item_width){
				item_width_	=	_args.item_width;
			}
			if(undefined != _args.item_height){
				item_height_	=	_args.item_height;
			}
			
		}		
		

		//private function
		function calculate_x(rx,ry,k){
			return Math.sqrt(rx * rx * ry * ry /(ry * ry + k * k * rx * rx));
		}
		function calculate_y(rx,ry,x){
			return Math.sqrt(Math.abs((rx *rx * ry * ry - x * x * ry * ry) / rx / rx));
		}
		function calculate(angle){
			var width = instance.width();
			var height = instance.height();
			var item_width = item_width_;
			var item_height = item_height_;
			
			//定義返回值 point.x point.y
			var point	=	{};
			 
			//標準化弧度
			if(angle > 360){
				angle	=	angle % 360;
			}
			else if(angle < 0){
				angle	=	Math.abs(angle);
				if(angle > 360){
					angle	=	angle % 360;
				}
				angle = 360 - angle;
			}
			var radian	=	Math.PI / 180 * angle;
			 
			//如果是橢圓
			if(width	!=	height){
				// x^2 / rx^2 + y^2 / ry^2 = 1
				// y = k * x
				// x ^2 = (rx ^2 * ry ^ 2) / (ry^2 + k^2 * rx^2)
				//x y 為坐標 rx ry 為 橢圓兩個半徑
				 
				var rx	=	width / 2;
				var ry	=	height / 2;
				 
				var x = 0;
				var y = 0;
				var k;
				if(radian < Math.PI / 2){
					//y = x * cot@
					k	=	Math.tan(Math.PI / 2 - radian);
					 
					x	=	calculate_x(rx,ry,k);
					y	=	calculate_y(rx,ry,x);
					
					if(clockwise_){
						x	+=	rx;
					}
					else{
						x	=	rx - x;
					}
					y	=	ry - y;
				}
				else if(radian < Math.PI){
					//y = x * tan(pi/2-@) 
					k	=	Math.tan(Math.PI / 2 - radian);
					 
					x	=	calculate_x(rx,ry,k);
					y	=	calculate_y(rx,ry,x);
					
					if(clockwise_){
						x	+=	rx;
					}
					else{
						x	=	rx - x;
					}
					y	+=	ry;
				}
				else if(radian < Math.PI + Math.PI / 2 ){
					//y = x * tan(Math.PI + Math.PI / 2 -@) 
					k	=	Math.tan(Math.PI + Math.PI / 2 - radian);
					 
					x	=	calculate_x(rx,ry,k);
					y	=	calculate_y(rx,ry,x);
					
					if(clockwise_){
						x	=	rx - x;
					}
					else{
						x	+=	rx
					}
					y	+=	ry;
				}
				else{
					//y = x * tan(@ - (Math.PI + Math.PI / 2)) 
					k	=	Math.tan(radian - (Math.PI + Math.PI / 2));
					 
					x	=	calculate_x(rx,ry,k);
					y	=	calculate_y(rx,ry,x);
					
					if(clockwise_){
						x	=	rx - x;
					}
					else{
						x	+=	rx;
					}
					y	=	ry - y;	 
				}
				 
				point.x	=	x - item_height / 2;
				point.y	=	y - item_width / 2;
				 
			}
			//如果是圓 (亦可將圓當作 橢圓 不過效率會比直接以圓算慢)
			else{
				
				var radius = width / 2			 
				var x = 0;
				var y = 0;
				if(radian < Math.PI / 2){
					if(clockwise_){
						x	=	radius + radius * Math.sin(radian);
					}
					else{
						x	=	radius - radius * Math.sin(radian);
					}
					y	=	radius - radius * Math.cos(radian);
				}
				else if(radian < Math.PI){
					if(clockwise_){
						x	=	radius + radius * Math.sin(Math.PI - radian);
					}
					else{
						x	=	radius - radius * Math.sin(Math.PI - radian);
					}
					y	=	radius + radius * Math.cos(Math.PI - radian);
				}
				else if(radian < Math.PI + Math.PI / 2 ){
					if(clockwise_){
						x	=	radius - radius * Math.sin(radian - Math.PI);
					}
					else{
						x	=	radius + radius * Math.sin(radian - Math.PI);
					}
					y	=	radius + radius * Math.cos(radian - Math.PI);
				}
				else{
					if(clockwise_){
						x	=	radius - radius * Math.sin(Math.PI * 2 - radian);
					}
					else{
						x	=	radius + radius * Math.sin(Math.PI * 2 - radian);
					}
					y	=	radius - radius * Math.cos(Math.PI * 2 - radian);
				}
				point.x	=	x - item_height / 2;
				point.y	=	y - item_width / 2;	
				
			}
			return point;
		}
			 
		//初始化
		_jquery.addClass("dark_ui_rotation");
		
		//設置子類
		var items	=	_jquery.children("div");
			var size = items.size();
			if(size == 0){
				return instance;
			}
			
			items.addClass("dark_ui_rotation_item");
			items.width(item_width_);
			items.height(item_height_);
			items.css("line-height",item_height_ + "px");
			
			
			var step	=	360 / size;
			var radian;
			var pos;
			$.each(items,function(index,obj){
					radian	=	-index * step;
					items_propertys_[index]			=	{};
					
					items_propertys_[index].radian	=	radian;
					
					pos								=	calculate(radian);
					items_propertys_[index].x		=	pos.x;
					items_propertys_[index].y		=	pos.y;
					
					$(obj).css("left",pos.x);
					$(obj).css("top",pos.y);
				});			
			
		//開始播放
		instance.play	=	function(){
			if(!play_){
				play_ = true;
				var interval	=	1000 / fps_;
				
				var step =  360 * (interval) / times_; 
				var sum	=	0;
				//啟動定時器
				timer_ = setInterval(function(){
					sum	+=	step;
					if(!loop_
						&& sum >= 360
						)
						{
						sum = 0;
						instance.stop();
						return;	
					}
					var pos;
					var radian;
					$.each(items,function(index,obj){
						radian	=	items_propertys_[index].radian + step;
						if(radian >= 360){
							radian	-=	radian;
						}
						items_propertys_[index].radian	=	radian;
						
						pos								=	calculate(radian);
						items_propertys_[index].x		=	pos.x;
						items_propertys_[index].y		=	pos.y;
						
						$(obj).css("left",pos.x);
						$(obj).css("top",pos.y);
					});
				},interval);
			}
			
			return instance;
		 }
		 //停止播放
		instance.stop	=	function(){
			if(play_){
				play_ = false;
				clearInterval(timer_);
			}
			return instance;
		}
			//綁定事件
			items.mouseenter(function(e) {
				instance.stop();
			});
			items.mouseleave(function(e) {
			   instance.play();
			});
		if(play){
			instance.play();
		}
		return instance;
	 }
};