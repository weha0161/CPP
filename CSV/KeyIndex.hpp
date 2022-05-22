#include "CSV.hpp"
#include "../Logger/Logger.hpp"
#include <memory>
#include <exception>

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
	
	template<typename TKeyValue = std::string, typename TIndexValue = uint>
	class KeyIndex
	{
		public:
			KeyIndex(TKeyValue k): key{k}{};
			using KeyType = Key<TKeyValue>;
			using IndexType = Index<TIndexValue>;
			using KeyIndexType = KeyIndex<TKeyValue,TIndexValue>;
			using Iterator = typename KeyType::Iterator;
			bool Is(std::string k){ return this->key.Matches(k);}
			void seTIndexValue(TIndexValue i) { this->index.setValue(i); }
			void seTKeyValuePatterns(Iterator begin, Iterator end) { this->key.UpdatePatterns(begin,end); }
			
			std::ostream& Display(std::ostream& os) const
			{
				return os<<this->key<<": "<<std::endl;		
			}
			
			bool operator ==(const KeyType& k){ return this->key == k;  }
			const KeyType& GetKey() { return this->key; }
			const IndexType& GetIndex() { return this->index; }
		
		private:
			KeyType key;
			IndexType index;
	};
	template<typename TKeyValue = std::string, typename TIndexValue = uint>
	bool operator ==(const Key<TKeyValue>& k, const KeyIndex<TKeyValue,TIndexValue>& ki){ return ki == k; }
	
	template<typename TKeyValue = std::string, typename TIndexValue = uint>
	bool operator ==(const KeyIndex<TKeyValue,TIndexValue>& ki, const Key<TKeyValue>& k){ return ki == k; }
	
	template<typename TKeyValue = std::string, typename TIndexValue = uint>
	std::ostream& operator<<(std::ostream& strm, const KeyIndex<TKeyValue,TIndexValue> c)
	{
		return c.Display(strm);
	}
	
	template<typename AccountType,typename TKeyValue = std::string, typename TIndexValue = uint>
	class KeyIndexContainer
	{
		public:
			using KeyType = Key<TKeyValue>;
			using IndexType = TIndexValue;
			using KeyIndexType = KeyIndex<TKeyValue,TIndexValue>;
			using ContainerType  = std::vector<KeyIndexType>;
			using ContainerPtrType  = std::unique_ptr<ContainerType>;
			KeyIndexContainer(ContainerPtrType k): keyIndices{std::move(k)} { Logger::Log()<<AccountType::Name<<": Key index container created "<<this->keyIndices->size()<<std::endl;};
			
			void UpdateKeys(const std::vector<std::string> keys)
			{
				for(uint i = 0; i < keys.size(); ++i)
					for(auto it = this->keyIndices->begin(); it != this->keyIndices->end(); ++it)
						if(it->Is(keys.at(i)))
							it->seTIndexValue(i);
			}
			
			void UpdateKeyPatterns(const KeyType&  k, const std::vector<std::string>& patterns)
			{
				this->UpdateKeyPatterns(k.Value, patterns);					
			}
			
			void UpdateKeyPatterns(const TKeyValue& k, const std::vector<std::string>& patterns)
			{
				try
				{
					auto i = std::find(this->keyIndices->begin(),this->keyIndices->end(),k);
					if(i != this->keyIndices->cend())
						i->seTKeyValuePatterns(patterns.cbegin(),patterns.cend());
					else
						Logger::Log<Error>()<<"UpdateKeyPatterns Key "<<k<<" not found!"<<std::endl;					
				}
				catch(std::exception e)
				{
					Logger::Log<Error>()<<"UpdateKeyPatterns Key "<<k<<"\t"<<e.what()<<std::endl;					
				}
					
			}
			
			bool Empty() { return this->keyIndices->size() == 0;}
		private:
			ContainerPtrType keyIndices;
	};	
}

#endif
