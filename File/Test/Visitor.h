// #include <algorithm>
// #include <functional>
// #include <iostream>
// #include <fstream>
// #include <chrono>
// #include <ctime>
// #include <iterator>
// #include <vector>
// #include <cstdlib>
// #include <unordered_map>
// #include <boost/mpl/vector.hpp>
// #include <boost/filesystem.hpp>
// #include <boost/mpl/for_each.hpp>
// #include <boost/mpl/placeholders.hpp>
// #include <filesystem>
// #include "../Logger/Logger.hpp"


#ifndef VISITOR_H
#define VISITOR_H

	class BaseVisitor
	{
	public:
		virtual ~BaseVisitor(){}
	};
	
	template<class T, typename R = void>
	class Visitor
	{
	public:
		using ReturnType = R;
		virtual ReturnType Visit(T&) = 0;
	};
	
	template<typename R = void>
	class BaseVisitable
	{
	public:
		using ReturnType = R;
		virtual ~BaseVisitable(){}
		virtual ReturnType Accept(BaseVisitor&) = 0;
	protected:
		template<class T>
		static ReturnType AcceptImpl(T& visited, BaseVisitor& visitor)
		{
			if(Visitor<T,R>* p = dynamic_cast<Visitor<T,R>*>(&visitor))
				return p->Visit(visited);
			
			return ReturnType();
		}		
	};
	
// #define DEFINE_VISITABLE() virtual ReturnType Accept(BaseVisitor& visitor) { return AcceptImpl(*this, visitor); }

#endif
