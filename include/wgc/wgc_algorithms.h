#ifndef WGC_ALGORITHMS_H
#define WGC_ALGORITHMS_H

#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <numeric>

namespace wgc
{
    template <typename Iter, typename F>
    void ForEachWithIndex(Iter First, Iter Last, F Callback)
    {
      for(int64_t Index = 0; First != Last; ++First, ++Index)
      {
        Callback(*First, Index);
      }
    }

    template<typename InputIterator, typename F>
    InputIterator Chunk(InputIterator First, InputIterator Last, size_t ChunkSize, F Callback)
    {
        if(First != Last && ChunkSize > 0)
        {
            size_t NumChunks = std::distance(First, Last) / ChunkSize;
            for(size_t i = 0; i < NumChunks; i++, First += ChunkSize)
            {
                Callback(First, First + ChunkSize);
            }
        }
        return First;
    }

    template<typename InputIterator, typename F>
    void ChunkAll(InputIterator First, InputIterator Last, size_t ChunkSize, F Callback)
    {
        InputIterator LastChunkStart = Chunk(First, Last, ChunkSize, Callback);
        if(LastChunkStart != Last)
        {
            Callback(LastChunkStart, Last);
        }
    }

    template <typename EntityIter, typename IndexIter>
    class IndexedEntityIterator
    {
        IndexIter Index;
        EntityIter EntityFirst;

        typedef std::iterator_traits<EntityIter> entity_traits_type;
        typedef std::iterator_traits<IndexIter> index_traits_type;
 
    public:
        typedef typename entity_traits_type::iterator_category iterator_category;
        typedef typename entity_traits_type::value_type value_type;
        typedef typename index_traits_type::difference_type difference_type;
        typedef typename entity_traits_type::reference reference;
        typedef typename entity_traits_type::pointer pointer;

        
        IndexedEntityIterator(IndexIter InIndex, EntityIter InEntityFirst)
            : Index { InIndex }
            , EntityFirst { InEntityFirst }
        {
        }
        
        reference operator*() const
        { 
            return EntityFirst[*Index];
        }

        pointer operator->() const
        { 
            return &EntityFirst[*Index];
        }

        IndexedEntityIterator& operator++()
        {
            ++Index;
            return *this;
        }

        IndexedEntityIterator& operator++(int)
        {
            Index++;
            return *this;
        }        

        bool operator==(const IndexedEntityIterator& Other) const
        {
            return Index == Other.Index && EntityFirst == Other.EntityFirst;
        }

        bool operator!=(const IndexedEntityIterator& Other) const
        {
            return Index != Other.Index || EntityFirst != Other.EntityFirst;
        }

        template<typename E, typename I>
        friend typename IndexedEntityIterator<E, I>::difference_type
        operator-(
            const IndexedEntityIterator<E, I>& LHS,
	        const IndexedEntityIterator<E, I>& RHS);
    };

    template<typename EntityIter, typename IndexIter>
    inline typename IndexedEntityIterator<EntityIter, IndexIter>::difference_type
    operator-(
        const IndexedEntityIterator<EntityIter, IndexIter>& LHS,
	    const IndexedEntityIterator<EntityIter, IndexIter>& RHS)
    { 
        return LHS.Index - RHS.Index; 
    }
    
    template <typename EntityIter, typename IndexIter, typename F>
    void ForEachIndexedEntity(
        IndexIter IndexStart,
        IndexIter IndexLast,
        EntityIter EntityFirst, 
        const size_t IndicesPerEntity, 
        F Callback)
    {
        Chunk(
            IndexStart, 
            IndexLast, 
            IndicesPerEntity,
            [&EntityFirst, &Callback](IndexIter First, IndexIter Last){
                Callback(
                    IndexedEntityIterator(First, EntityFirst),
                    IndexedEntityIterator(Last, EntityFirst)
                );
            }
        );
    }
}

#endif