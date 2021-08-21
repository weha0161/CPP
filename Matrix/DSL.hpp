#ifndef DSL_H
#define DSL_H

struct whatever
{
	enum
	{
		whatever_id = -1,
		rect_id,symm_id,
		speed_id,space_id,
		check_bounds_id,no_check_bounds_id,
		array_id, vector_id,
		c_like_id, fortran_like_id,
		id = whatever_id
	}
}

template<class T = whatever> struct rect : whatever { enum {id = rect_id}; };

template<class T = whatever> struct array: whatever { enum {id = array_id}; using ArrayOrder = T};
template<class T = whatever> struct vector: whatever { enum {id = vector_id}; };

template<class T = whatever>struct symm: whatever { enum {id = symm_id}; };

template<class T = whatever>struct speed: whatever { enum {id = speed_id}; };
template<class T = whatever>struct space: whatever { enum {id = space_id}; };

template<class T = whatever>struct check_bounds: whatever { enum {id = check_bounds_id}; };
template<class T = whatever>struct no_check_bounds: whatever { enum {id = no_check_bounds_id}; };

template<class T = whatever>struct c_like: whatever { enum {id = c_like_id}; };
template<class T = whatever>struct fortran_like: whatever { enum {id = fortran_like_id}; };


template<
	class ElementT = whatever, 
	class Shape  = whatever,
	class OptFlag  = whatever,
	class BoundsChecking  = whatever, 
	class Format  = whatever, 
	class Index  = whatever> 
struct matrix
{
	using elementType = ElementType;
	using shape = Shape;
	using optFlag = OptFlag;
	using boundsChecking = BoundsChecking;
	using format = Format;
	using indexType = IndexType;
};

template<class CheckedMatrix>
class DenseCCommaInitializer;


template<typename DSLDescription>
struct MATRIX_DSL_PARSER
{
private:
	using ElementType_ = typename DSLDescription::ElementType;
	using Shape_ = typename DSLDescription::Shape;
	using OptFlag_ = typename DSLDescription::OptFlag;
	using BoundsChecking_ = typename DSLDescription::BoundsChecking;
	using Format_ = typename DSLDescription::Format;
	using ArrayFormat_ = typename DSLDescription::Format;
	using IndexType_ = typename DSLDescription::IndexType;

public:
	struct DSLFeatures
	{
		using ElementType = ElementType_;
		using Shape = Shape_;
		using OptFlag = OptFlag_;
		using BoundsChecking = BoundsChecking_;
		using Format = Format_;
		using ArrayOrder = ArrayFormat_;
		using IndexType = IndexType_;
	};

	using RET =  DSLFeatures;
};

struct DSLFeaturesDefaults
{
	using ElementType = double;
	using Shape = rect<>;
	using OptFlag = space<>;
	using BoundsChecking = check_bounds<>;
	using ArrayOrder = c_like<>;
	using IndexType = unsigned int;
};

template<typename ParsedDSLDescription>
struct MATRIX_ASSIGN_DEFAULTS
{
	using ParsedDSL = ParsedDSLDescription;
	
	using ElementType_ = typename ParsedDSLDescription::ElementType;
	using Shape_ = typename ParsedDSLDescription::Shape;
	using OptFlag_ = typename ParsedDSLDescription::OptFlag;
	using BoundsChecking_ = typename ParsedDSLDescription::BoundsChecking;
	using Format_ = typename ParsedDSLDescription::Format;
	using ArrayFormat_ = typename ParsedDSLDescription::Format;
	using IndexType_ = typename ParsedDSLDescription::IndexType;
	
public:
	struct DSLFeatures
	{
		using ElementType = ElementType_;
		using Shape = Shape_;
		using OptFlag = OptFlag_;
		using BoundsChecking = BoundsChecking_;
		using Format = Format_;
		using ArrayOrder = ArrayFormat_;
		using IndexType = IndexType_;
	};

	using RET =  DSLFeatures;
}
template<typename CompletedDSLDescription>
struct MATRIX_ASSEMBLE_COMPONENTS
{
private:
	using Generator = MATRIX_ASSEMBLE_COMPONENTS<CompletedDSLDescription>;
	using DSLFeatures_ = CompletedDSLDescription;

// 	using Format_ = typename CompletedDSLDescription::Format;
	using Array = typename DSLFeatures::Format;
	using IndexType_ = typename DSLFeatures::IndexType;
	using ElementType_ = typename DSLFeatures::IndexType;
	using BoundsCheckedMatrix = typename DSLFeatures::BoundsChecking;
	using CommaInitializer_ = DenseCCommaInitializer<Generator>;
	using MatrixType_ = Matrix<BoundsCheckedMatrix>;
	
	enum
	{
		shapeID = DSLFeatures::Shape::id,
		symmetric = DSLFeatures::Shape::symm_id,
		formatID = DSLFeatures::Format::id,
		vector = DSLFeatures::Format::vector_id
	}

public:
	struct Config
	{
		using DSLFeatures = CompletedDSLDescription;
		using ElementType = ElementType_;
		using CommaInitializer = CommaInitializer_;
		using MatrixType = MatrixType_;
	};

	using RET =  MatrixType_;
};

template<class InputDSL = matrix<>, int whatToDo = do_all>
struct MATRIX_GENERATOR
{
private:
	using ParsedDSL__ = MATRIX_DSL_PARSER<InputDSL>::RET;
	using CompletedDSL__ = MATRIX_ASSIGN_DEFAULTS<ParsedDSL__>::RET;
	using Result = MATRIX_ASSEMBLE_COMPONENTS<ParsedDSL>;
public:
	using RET =  Result::RET;
};

#endif
