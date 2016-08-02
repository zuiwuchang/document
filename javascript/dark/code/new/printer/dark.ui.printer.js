// JavaScript Document
var namespace = dark_namespace("dark.ui");

namespace.printer	=	{
	create:function(_jquery,_args){
		//派生子類
		var instance	=	namespace.base.create(_jquery,_args);
		
		//private
		//被插入 原元素 副本
		var copy_;
		//輸入刷新時間
		var interval_ = 100;
		//輸入 定時器
		var timer_;
		//待刷新 棧
		var stack_	=	new Array();
		
		//插入符
		var caret_;
		//插入符刷新時間
		var caret_interval_ = 1000;
		//插入符 定時器
		var caret_timer_;
		//打印完成 是否隱藏插入符
		var end_hide_ = true;
		
		//private function
		function add_next(note){
			//添加兄弟元素
			var add	=	{father:note.father	
				,arrays:note.arrays
				,index_arrays:note.index_arrays + 1
				,index_text:0
			};
			stack_.push(add);
		}
		function print_loop(rs){
			var note	=	stack_.pop();
			//添加完成 返回
			if(note.arrays.size() == 0
				||	note.arrays.size() <= note.index_arrays
				){
				return rs;
			}
			
			//第一次遇到元素增加
			if(0 == note.index_text){
				add_next(note);
				
				child	=	$(note.arrays[note.index_arrays]).clone();
				var name = child.get(0).tagName;
				if(name == "IMG"
					|| name == "INPUT"
					){
					note.father.append(child);
					
					child.after(caret_);
					return true;
				}
				
				if(name != "BR"){
					note.text	=	child.text();
					child.text("");
				}
				note.father.append(child);
				note.child	=	child;
				
				child.append(caret_);
			}
						
			//存在子元素 添加之
			var childs	=	$(note.arrays[note.index_arrays]).children();
			if(childs.size() != 0){			
				if(childs.size() != 0){
					var add	=	{father:child
						,arrays:childs
						,index_arrays:0
						,index_text:0
					};
					stack_.push(add);
				}
				
				while(rs != true
					&& stack_.length != 0
					){
					rs	=	print_loop(rs);
				}	
				return rs;
			}
			
			//存在text 添加之
			if(undefined != note.text
				&& "" != note.text
				)
				{	
				//添加兄弟元素
				if(note.index_text >= note.text.length){
					return rs;
				}
				else{				
					//添加字符
					++note.index_text;
					
					note.child.text(note.text.substr(0,note.index_text));
					
					var add	=	{father:note.father
						,"child":note.child
						,arrays:note.arrays
						,index_arrays:note.index_arrays
						,text:note.text
						,index_text:note.index_text
					};
					stack_.push(add);
					
					child.append(caret_);
					return true;
				}
					
			}
			return rs;		
		}
		
		//初始化參數
		if(undefined	!=	_args){
			if(undefined != _args.interval){
				interval_ = _args.interval;
			}
			if(undefined != _args.caret_interval){
				caret_interval_	=	_args.caret_interval;
			}		
			if(undefined != _args.caret_interval){
				end_hide_	=	_args.end_hide
			}
		}
		
		var obj	=	_jquery;
		copy	=	obj.clone();
		obj.html("<div></div>");
		
		
		//print_loop(obj,0,copy.children(),0,"",0);
		var note	=	{father:obj.children("div:first")	//元素插入位置
			//,child:				//文本插入位置
			,arrays:copy.children()	//兄弟元素
			,index_arrays:0
			//,text					//文本
			,index_text:0
		};
		stack_.push(note);
		
		//創建插入符
		if(_args.caret == undefined){
			caret_	=	$("<span>|</span>");
		}
		else{	
			caret_	=	$(_args.caret);
		}
		obj.children("div:first").append(caret_);
		var show = true;
		caret_timer_	=	setInterval(function(){
				if(show){
					show = false;
					caret_.hide();
				}
				else{
					show = true;
					caret_.show();
				}
		},caret_interval_);
		
		instance.play	=	function(_interval){
			if(undefined != _interval){
				interval_ = _interval
			}
			else{
				interval_ = 100;
			}
			if(undefined != timer_){
				clearInterval(timer_);
			}
			if(stack_.length != 0){
				timer_	=	setInterval(function(){
					if(stack_.length == 0){
						clearInterval(timer_);
						timer_	=	undefined;
						
						if(end_hide_){
							clearInterval(caret_timer_);
							caret_timer_	=	undefined;
							caret_.hide();
						}
					}
					else{
						var rs = false;
						while(!rs
							&& stack_.length != 0){			
							rs	=	print_loop(false);
						}
					}
				},interval_);
			}
			return instance;
		}
		instance.stop	=	function(){
			if(undefined != timer_){
				clearInterval(timer_);
				timer_	=	undefined;
			}	
			return instance;
		}
		if(_args.start != false){
			instance.play(interval_);
			
			
		}
		
		return instance;
	}
}