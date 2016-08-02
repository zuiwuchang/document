// JavaScript Document

//模擬 名字空間
var dark_namespace_object	=	{};
function dark_namespace(name){
	if(undefined	==	dark_namespace_object.name){
		dark_namespace_object.name	=	{};
	}
	return dark_namespace_object.name;
}
function dark_using_namespace(name){
	return dark_namespace_object.name
}
function dark_check_namespace(name){
	return undefined	==	dark_namespace_object.name;
}