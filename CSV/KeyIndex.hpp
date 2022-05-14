#include "CSV.hpp"
#include "../Logger/Logger.hpp"
#include <memory>

#ifndef KEYINDEX_H
#define KEYINDEX_H

//--------------------------------KeyIndex------------------------------------------------
namespace CSV
{
	template<typename T = std::string>
	class Key
	{
	};
	
	template<typename T = uint>
	class Index
	{
	};
	
	template<typename TKey = std::string, typename TIndex = uint>
	class KeyIndex
	{
		public:
			using KeyType = TKey;
			using IndexType = TIndex;
		
		private:
			Key<KeyType> key;
			Index<IndexType> index;
	};
	
	template<typename TKey = std::string, typename TIndex = uint>
	class KeyIndexContainer
	{
		public:
			using KeyType = TKey;
			using IndexType = TIndex;
			using KeyIndexType = KeyIndex<KeyType,IndexType>;
			using ContainerType  = std::vector<KeyIndexType>;
			using ContainerPtrType  = std::shared_ptr<std::vector<KeyIndexType>>;
		private:
			ContainerPtrType keyIndices = std::make_shared<ContainerType>();
	};	
}

#endif
