# WGC-CPP
Some c++ utilities.

### wgc::JobThread

A functor used to create a long running thread to which Tasks can be submitted. Note: RunJob method will block if the thread is already executing a job. So the pattern of use should be:

> RunJob > Wait > RunJob > Wait > ... > Stop.

Thread is stopped in Destructor.

Example usage:
```
auto Thread = std::unique_ptr<wgc::JobThread>();

//Submit job
Thread->RunJob([]() {
  ...Some job executed.
});

//Wait for job to complete
Thread->Wait();

//Submit another job
Thread->RunJob([]() {
  ...Some job executed.
});

//Stop thread
Thread->Stop();
```

### wgc::ForEachWithIndex

A for-each algorithm that calls handler with item and index.

Example:

```
std::vector<char> Chars{'a', 'b', 'c', 'd'};
wgc::ForEachWithIndex(Chars.cbegin(), Chars.cend(), [](const char Item, int64_t Index) {
  std::cout << "[" << Index << " : " << Item << "]\n";
});
```
Output of above:

> [0 : a]
> [1 : b]
> [2 : c]
> [3 : d]


### wgc::Chunk

Algorithm to breah collection into chunks. Only complete chunks are handed to the handler. Returns an iterator that points to the first item of the remaining incomplete chunk.

Example:

```
std::vector<int> Numbers{1,2,3,4,5,6,7,8};
auto Remainder = wgc::Chunk(Numbers.cbegin(), Numbers.cend(), 3, [](auto First, auto Last) {
  std::cout << "[";
  for(; First != Last; ++First)
  {
    std::cout << *First;
  }
  std::cout << "]\n";
});
std::cout << "Remainder points to: " << *Remainder << "\n";
```
Output:
> [123]
> [456]
> Remainder points to: 7

### wgc::ChunkAll

Same as wgc::Chunk except handler is called with all chunks even if they are < ChunkSize.

Example:

```
std::vector<int> Numbers{1,2,3,4,5,6,7,8};
wgc::ChunkAll(Numbers.cbegin(), Numbers.cend(), 3, [](auto First, auto Last) {
  std::cout << "[";
  for(; First != Last; ++First)
  {
    std::cout << *First;
  }
  std::cout << "]\n";
});
```
Output:
> [123]
> [456]
> [78]

### wgc::ForEachIndexedEntity

An algorithm that chunks through a collection of indices, finds corresponding Entities in an entity collection and calls handler with Entities.  For example, in graphics programming you often have a vector of indices that hold the indices to Vertices in a Vertex vector. This algorithm will iterate the triangle vertices and call the handler with each triangle:

Example:

```
std::vector<int16_t> Indices{
  0,1,2,
  1,3,2
};

std::vector<Vertex> Vertices {
  {0,0,0},
  {2,0,0},
  {1,1,0},
  {1,-1,0}
};

wgc::ForEachIndexedEntity(Indices.cbegin(), Indices.cend(), Vertices.cbegin(), 3, [](auto First, auto Last) {
  for(; First != Last; ++First)
  {
    std::cout << *First << "\n";
  }
  std::cout << "\n";
});
```
Output:

> [0,0,0]
> [2,0,0]
> [1,1,0]
> 
> [2,0,0]
> [1,-1,0]
> [1,1,0]
