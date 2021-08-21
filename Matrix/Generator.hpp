#include "DSL.hpp"

#ifndef GENERATOR_H
#define GENERATOR_H

template<class CheckedMatrix>
class Matrix;

template<class CheckedMatrix>
class BoundsChecker;

template<class CheckedMatrix>
class Array;

template<class CheckedMatrix>
class DenseCCommaInitializer;

template<typename ElementT, class Shape, class OptFlag, class BoundsChecking, class Format, class IndexT> struct matrix;
template<class> struct Rect;
template<class> struct Array;

template<typename IndexT = int,
			typename ElementT = int,
			template<typename> class ContainerType = std::vector,
			typename RowT = ContainerType<ElementT>,
			typename ContainerT = ContainerType<RowT>>
struct Generator
{
private:
	struct Configuration
	{
		using IndexType = IndexT;
		using ElementType = ElementT;
		using Container = ContainerT;
		using Shape = Rect<IndexT>;
		using OptFlag = Rect<IndexT>;
		using Format = Array<Generator>;
		using BoundsChecking = BoundsChecker<Array<Generator>>;
		using Row = RowT;
		using CommaInitializer = DenseCCommaInitializer<Generator>;
		using MatrixType =  Matrix<BoundsChecker<Array<Generator>>>;
	};
	
	
public:
	using Config = Configuration;
	using RET =  Config::MatrixType;
};

#endif
