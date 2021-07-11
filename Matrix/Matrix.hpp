#include "../String/String_.h"
#include "../Logger/Logger.h"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <map>

#ifndef CSV_H
#define CSV_H


//--------------------------------CreateElementNewPolicy------------------------------------------------

template<class CheckedMatrix>
class Matrix: public CheckedMatrix
{
public:
	using Config = CheckedMatrix::Config;
};


#endif
