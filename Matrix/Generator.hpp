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
	struct DSLDescription
	{
		using ElementType = ElementT;
		using Shape = rect<IndexT>;
		using OptFlag = rect<IndexT>;
		using BoundsChecking = BoundsChecker<Array<Generator>>;
		using Format = array<Generator>;
		using ArrayFormat = Array<Generator>;
		using IndexType = IndexT;
	};
	
	using ParsedDSL = MATRIX_GENERATOR<DSLDescription,1>::RET;
	
	struct Configuration
	{
		using IndexType = IndexT;
		using ElementType = ElementT;
		using Container = ContainerT;
		using Shape = rect<IndexT>;
		using OptFlag = rect<IndexT>;
		using Format = array<Generator>;
		using BoundsChecking = BoundsChecker<Array<Generator>>;
		using Row = RowT;
		using CommaInitializer = DenseCCommaInitializer<Generator>;
		using MatrixType =  Matrix<BoundsChecker<Array<Generator>>>;
                bool constexpr ScalarMultiplication =  false;
		using DSLFeatures = ParsedDSL;
	};
	
public:
	using Config = Configuration;
	using RET =  Config::MatrixType;
};

#endif
