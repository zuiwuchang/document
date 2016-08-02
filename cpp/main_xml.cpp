#include "StdAfx.h"
#include "main_xml.h"


main_xml::main_xml(void)
{
}


main_xml::~main_xml(void)
{
}
std::wstring main_xml::url()const
{
	std::wstring rs	=	DARK_DEFAULT_URL;

	boost::property_tree::wptree tree;
	if(load_utf8_xml(tree,DARK_CONFIG_PATH))
	{
		try
		{
			rs	=	tree.get(L"cnf.url",DARK_DEFAULT_URL);
		}
		catch(const boost::property_tree::ptree_bad_path&)
		{
		}
	}
	return rs;
}

void main_xml::url(const std::wstring& url)const
{
	boost::property_tree::wptree tree;
	load_utf8_xml(tree,DARK_CONFIG_PATH);
	
	tree.put(L"cnf.url",url);
	
	save_uft8_xml(tree,DARK_CONFIG_PATH);

}
std::string main_xml::aurl()const
{
	std::string rs	=	"kk";

	boost::property_tree::ptree tree;
	if(load_utf8_xml(tree,DARK_CONFIG_PATH))
	{
		try
		{
			rs	=	tree.get("cnf.url","kk");
		}
		catch(const boost::property_tree::ptree_bad_path&)
		{
		}
	}
	return rs;
}
void main_xml::aurl(const std::string& url)const
{
	boost::property_tree::ptree tree;
	load_utf8_xml(tree,DARK_CONFIG_PATH);
	
	tree.put("cnf.url",url);
	
	save_uft8_xml(tree,DARK_CONFIG_PATH);

}