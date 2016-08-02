// JavaScript Document
var namespace = dark_namespace("dark.ui");

namespace.base	=	{	
	create:function(_jquery,_args){	//構造工程 需要傳入 jquery(作為ui) 以及附加參數json(根據子類不同而不同)
		//實例化對象
		var instance	=	{};
		
		//private:
		//保存 jquery對象 以便於直接使用 jquery 查找元素
		var jquery_	=	_jquery;
		
		//ui 寬 高
		var width_	=	jquery_.width();
		var height_	=	jquery_.height();
		
		
		//public:
		//返回保存的jquery對象
		instance.jquery	=	function(){
			return jquery_;
		}
		
		//返回設置 寬 高
		instance.width	=	function(_width){
			if(undefined == _width){
				return width_;
			}
			width_	=	_width;			
			jquery_.width(_width);
			
			return instance;
		}
		instance.height	=	function(_height){
			if(undefined == _height){
				return height_;
			}
			height_	=	_height;			
			jquery_.height(_height);
			
			return instance;
		}
		
		
		//初始化操作
		if(undefined != _args){
			if(undefined != _args.width){
				instance.width(_args.width);	
			}
			if(undefined != _args.height){
				instance.height(_args.height_);
			}
		}
		
		//返回對象實例
		return instance;
	}
}