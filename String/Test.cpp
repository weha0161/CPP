#include <iostream>
#include "String_.hpp"
#include "AtomPassingParser.hpp"
#include "ParsedValues.hpp"
#include "../Traits/Traits.h"
#include "../Quantity/Quantity.h"
#include <typeinfo>
#include <typeindex>
using namespace String_;

int main()
{       

	using a = T::char_<'a'> ;
	using del = T::char_<'/'> ;
	
	T::Is_<a> isA;
	T::IsNot_<a> isNA;
	
	std::cout<<"a == a "<<isA('a')<<std::endl;
	std::cout<<"a == a "<<isNA('a')<<std::endl;
	
	const std::string pathToShow{ "9456546541135457667864,,ZWEI\\/;;../3home/VIERmarkus/Dok_umente/cpp/Fi\nle" };
	//~ const std::string pathToShow{ "05 Basislastschrift;931;Bausparkasse Schwaebisch Hall Aktiengesellschaft - Bau;0;DE96500604000000011404;50060400;GENODEFFXXX;Lastschrift 26219070T01 07.2021 EREF: BSH-2021-06-24LS-358260203 MREF: BSH0000062533540 CRED: DE87BSH00000024981 IBAN: DE96500604000000011404 BIC: GENODEFF ANAM: Bausparkasse Schwaebisch Hall Aktiengesellschaft - Bausparkasse der Vo;BSH-2021-06-24LS-358260203" };

	auto p = String_::Parser();
	    
	const std::string dc{ "920_08" };
	p.Parse(dc);
	p.Parse(pathToShow);
	auto v = p.Values();
	auto vi = std::dynamic_pointer_cast<String_::ParsedNumber>(*(v->cbegin()));

	const std::string dc1{ "9,8" };
	p.Parse(dc1);
	auto v1 = p.Values();
	auto vi1 = std::dynamic_pointer_cast<String_::ParsedNumber>(*(v->cbegin()));
	std::cout<<vi1->Is<double>()<<std::endl;
	std::cout<<vi1->Is<uint>()<<std::endl;
	std::cout<<vi1->Is<char>()<<std::endl;
	std::cout<<vi1->Cast<uint>()<<std::endl;
	
	const std::string din{ "1123A" };
	p.Parse(din);
	v1 = p.Values();
	vi1 = std::dynamic_pointer_cast<String_::ParsedNumber>(*(v->cbegin()));
	
	std::cout<<vi1->Cast<uint>()<<std::endl;
	vi1->AsQuantity();
	
	const std::string din2{ "1123A" };
	p.Parse(din2);
	auto v12 = p.Values();
	auto vi12 = std::dynamic_pointer_cast<String_::ParsedNumber>(*(v->cbegin()));
	
	std::cout<<vi1->Cast<uint>()<<std::endl;
	vi12->AsQuantity();
	std::cout<<"UNIT"<<vi1->GetQuantityUnit()<<std::endl;
	
	
	auto pc = ParsedNumber::QuantityContainerType::Instance().Size<Quantity<Current>>();
	auto qs = ParsedNumber::QuantityContainerType::Instance().Size<Quantity<Sum>>();
	std::cout<<"Size: "<<pc<<std::endl;
	std::cout<<"Size: "<<qs<<std::endl;
	std::cout<<"Display"<<std::endl;
	ParsedNumber::Display(std::cout);

	ParsedNumber::Get<Quantity<Current>>()->size();
	
	std::cout<<"Size2: "<<ParsedNumber::Get<Quantity<Current>>()->size()<<std::endl;
	std::cout<<"Size2: "<<qs<<std::endl;
	
	//~ auto ps = ParsedNumber::QuantityContainerType::Instance().Size<Quantity<int>>();
	//~ std::cout<<q.Value()<<q.UnitSign()<<"  "<<q<<std::endl;
	//~ std::cout<<q.Value()<<q.UnitSign()<<"  "<<q<<std::type_index(q)<<std::endl;
	
	//~ p.Parse(pathToShow);
    
    return 0;
}
