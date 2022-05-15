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
		public:
			using ValueType = T;
			using Type = Key<T>;
			using ContainerType  = std::vector<T>;
			using ContainerPtrType  = std::shared_ptr<ContainerType>;

			//~ bool Matches(ValueType k){ return std::find(this->patterns->cbegin(), this->patterns->cend(), k) != this->patterns->cend(); }
			bool Matches(std::string k){ return std::find(this->patterns->cbegin(), this->patterns->cend(), k) != this->patterns->cend(); }
			//~ bool Matches(std::string k){ return "123" == k; }
		private:
			ContainerPtrType patterns = std::make_shared<ContainerType>();
	};
	
	template<typename T = uint>
	class Index
	{
		public:
			using ValueType = T;
			void setValue(ValueType v){ this->value = v; }
		private:
			ValueType value;
	};
	
	template<typename TKey = std::string, typename TIndex = uint>
	class KeyIndex
	{
		public:
			using KeyType = Key<TKey>;
			using IndexType = Index<TIndex>;
			bool Is(std::string k){ return this->key.Matches(k);}
			void setIndex(TIndex i) { this->index.setValue(i); }
		
		private:
			Key<TKey> key;
			Index<TIndex> index;
	};
	
	template<typename TKey = std::string, typename TIndex = uint>
	class KeyIndexContainer
	{
		public:
			using KeyType = TKey;
			using IndexType = TIndex;
			using KeyIndexType = KeyIndex<TKey,TIndex>;
			using ContainerType  = std::vector<KeyIndexType>;
			using ContainerPtrType  = std::shared_ptr<std::vector<KeyIndexType>>;
			
			void UpdateKeys(const std::vector<std::string> keys)
			{
				for(uint i = 0; i < keys.size(); ++i)
					for(auto it = this->keyIndices->begin(); it != this->keyIndices->end(); ++it)
						if(it->Is(keys.at(i)))
							it->setIndex(i);
			}
		private:
			ContainerPtrType keyIndices = std::make_shared<ContainerType>();
	};	
}

#endif
