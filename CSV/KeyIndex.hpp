#include "CSV.hpp"
#include "../Logger/Logger.hpp"
#include <memory>

#ifndef KEYINDEX_H
#define KEYINDEX_H

//--------------------------------KeyIndex------------------------------------------------
namespace CSV
{
	
	
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
			KeyIndex(TKey k): key{k}{};
			using KeyType = Key<TKey>;
			using IndexType = Index<TIndex>;
			bool Is(std::string k){ return this->key.Matches(k);}
			void setIndex(TIndex i) { this->index.setValue(i); }
			
			std::ostream& Display(std::ostream& os) const
			{
				return os<<this->key<<": "<<std::endl;		
			}
		
		private:
			Key<TKey> key;
			Index<TIndex> index;
	};
	
	template<typename TKey = std::string, typename TIndex = uint>
	std::ostream& operator<<(std::ostream& strm, const KeyIndex<TKey,TIndex> c)
	{
		return c.Display(strm);
	}
	
	template<typename AccountType,typename TKey = std::string, typename TIndex = uint>
	class KeyIndexContainer
	{
		public:
			using KeyType = TKey;
			using IndexType = TIndex;
			using KeyIndexType = KeyIndex<TKey,TIndex>;
			using ContainerType  = std::vector<KeyIndexType>;
			using ContainerPtrType  = std::unique_ptr<ContainerType>;
			
			void UpdateKeys(const std::vector<std::string> keys)
			{
				for(uint i = 0; i < keys.size(); ++i)
					for(auto it = this->keyIndices->begin(); it != this->keyIndices->end(); ++it)
						if(it->Is(keys.at(i)))
							it->setIndex(i);
			}
		private:
			ContainerPtrType keyIndices = std::make_unique<ContainerType>();
	};	
}

#endif
