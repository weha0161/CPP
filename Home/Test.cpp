#include <iostream>
#include <boost/mpl/vector.hpp>
#include "UnitRatio.hpp"
#include "Counter.hpp"
#include "../Tuple/Tuple.hpp"
#include "CounterContainer.hpp"
#include "../Traits/Traits.h"
#include "../Unit/Unit.h"

int main()
{       
    std::cout<<"START"<<std::endl;
	
// 	using W1 = CounterConfiguration<Water,1>;
// 	using G1 = CounterConfiguration<Gas,1>;
// 	using E1 = CounterConfiguration<Energy,1>;
// 	using W2 = CounterConfiguration<Water,2>;
// 	using W3 = CounterConfiguration<Water,3>;

	using CG1 = Counter<GasConfiguration>;
	using CE1 = Counter<EnBWEnergyConfiguration>;
	using CWA = Counter<AllWaterConfiguration>;
	using CWO = Counter<OWaterConfiguration>;
	using CWOut = Counter<OutWaterConfiguration>;
	
	using CBHW = Counter<Bottom_HWaterConfiguration>;
	using CBCW = Counter<Bottom_CWaterConfiguration>;
	using CMCW = Counter<Middle_CWaterConfiguration>;
	using CMHW = Counter<Middle_HWaterConfiguration>;
	using CTCW = Counter<Top_CWaterConfiguration>;
	using CTHW = Counter<Top_HWaterConfiguration>;
	
	using TypeContainer = Tuple<CE1,CG1,CWA,CWO,CWOut, CBCW,CBHW, CMCW,CMHW,CTCW,CTCW>;
	auto tc = TypeContainer();
	
    std::cout<<"Single Calls to Get Tuple "<<std::endl;
	auto cw1 = GetType<CWO>(tc);
	cw1.Display(std::cout);
	auto cw2 = GetType<CWA>(tc);
	cw2.Display(std::cout);
	auto cw3 = GetType<CWOut>(tc);
	cw3.Display(std::cout);
	auto ce1 = GetType<CE1>(tc);
	ce1.Display(std::cout);
	auto cg1 = GetType<CG1>(tc);
	cg1.Display(std::cout);
	
    std::cout<<"Ostream "<<std::endl;
	CounterContainer<CE1,CG1,CWA,CWO,CWOut, CBCW,CBHW, CMCW,CMHW,CTCW,CTCW> cc;
    std::cout<<cc<<std::endl;
	
	cc.Write();

	
    std::cout<<"UNIT"<<std::endl;
	using s = Sum;
	using work = Work;
	auto cw = CWA(); 
		
    std::cout<<"Sum "<<Sum::SiUnit()<<std::endl;
    std::cout<<"Work "<<Work::SiUnit()<<std::endl;
	
    std::cout<<"Type "<<CWA::Type::Name<<std::endl;
    std::cout<<"Unit "<<CWA::Unit::SiUnit()<<std::endl;
    std::cout<<"Name "<<cw.GetName()<<std::endl;
    std::cout<<"F "<<Kilo::Type::Den<<std::endl;
    std::cout<<"F "<<Kilo::Type::Num<<std::endl;
    std::cout<<"F "<<Kilo::Sign<<std::endl;
    std::cout<<"END"<<std::endl;

    return 0;
}
