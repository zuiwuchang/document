#pragma once
#include<dark\base_xml.hpp>
#include<boost\serialization\singleton.hpp>


#define DARK_CONFIG_PATH	L"cnf.xml"
#define DARK_DEFAULT_URL	L"http://www.google.com"
class main_xml
	:public dark::base_xml
	,public boost::serialization::singleton<main_xml>
{
public:
	main_xml(void);
	~main_xml(void);

public:
	std::wstring url()const;
	void url(const std::wstring& url)const;

	std::string aurl()const;

	void aurl(const std::string& url)const;
};

