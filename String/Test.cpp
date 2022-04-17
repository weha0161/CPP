#include <iostream>
#include "String_.hpp"
#include "AtomPassingParser.hpp"
#include "ParsedValues.hpp"
#include "../Traits/Traits.h"
// #include "../AbstractFactory/AbstractFactory.hpp"
using namespace String_;

int main()
{       

	using a = T::char_<'a'> ;
	using del = T::char_<'/'> ;
	
	T::Is_<a> isA;
	T::IsNot_<a> isNA;
	
	std::cout<<"a == a "<<isA('a')<<std::endl;
	std::cout<<"a == a "<<isNA('a')<<std::endl;
	std::cout<<"a == 64 "<<isNA(64)<<std::endl;
	
	//~ const std::string pathToShow{ "9456546541135457667864,,ZWEI\\/;;../3home/VIERmarkus/Dok_umente/cpp/Fi\nle" };
	const std::string pathToShow{ "05 Basislastschrift;931;Bausparkasse Schwaebisch Hall Aktiengesellschaft - Bau;0;DE96500604000000011404;50060400;GENODEFFXXX;Lastschrift 26219070T01 07.2021 EREF: BSH-2021-06-24LS-358260203 MREF: BSH0000062533540 CRED: DE87BSH00000024981 IBAN: DE96500604000000011404 BIC: GENODEFF ANAM: Bausparkasse Schwaebisch Hall Aktiengesellschaft - Bausparkasse der Vo;BSH-2021-06-24LS-358260203" };

    auto c = String_::Split<del>(pathToShow);
	auto p = String_::Parser();
	
	
	auto pi = std::make_shared<String_::ParsedInt>(std::make_shared<std::string>("3"));
	auto pw = std::make_shared<String_::ParsedWord>(std::make_shared<std::string>("TRE_84684"), pi);
	
	auto pwn = std::dynamic_pointer_cast<String_::ParsedInt>(pw->Next());
	
    std::cout<<pwn->Value()<<std::endl;
	
	p.Parse(pathToShow);
    
    return 0;
}
