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
```
[0 : a]
[1 : b]
[2 : c]
[3 : d]
```

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
```
[123]
[456]
Remainder points to: 7
```
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
```
[123]
[456]
[78]
```
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

auto TriangleHandler = [](auto First, auto Last) {
  for(; First != Last; ++First)
  {
    // In this example, Vertex can be written to std::cout
    std::cout << *First << "\n";
  }
  std::cout << "\n";
};

wgc::ForEachIndexedEntity(
  Indices.cbegin(),
  Indices.cend(),
  Vertices.cbegin(),
  3,
  TriangleHandler);
```
Output:
```
[0,0,0]
[2,0,0]
[1,1,0]

[2,0,0]
[1,-1,0]
[1,1,0]
```

### wgc::Sequence

Provides an increment sequence of values and rolls over when Max is reached.  Default version is not thread-safe. The Synchronized method returns a thread-safe version of the Sequence.

Rollover Example:
```
// This example uses a uint8_t Sequence with InitValue = 0 and MaxValue = 255
// We burn off the first 256 Sequence values (0 - 255). Then we print Seq.Next()
// which will give the rolled value of 0;

wgc::Sequence<uint8_t>Seq;

for(auto i = 0; i < 256; i++)
{
  Seq.Next();
}

std::cout << "Rolled Value: " << std::to_string(Seq.Next()) << std::endl;
```
Output:
```
Rolled Value: 0
```

Limits Example:
```
// This example uses an uint32_t Sequence with range 1 t0 5

wgc::Sequence<uint32_t>Seq(1, 5);

for(auto i = 0; i < 6; i++)
{
  std::cout << "Val: " << std::to_string(Seq.Next() << "\n";
}
```
Output:
```
Val: 1
Val: 2
Val: 3
Val: 4
Val: 5
Val: 1
```

Synchronized Example:
```
// This example uses a Synchronized Sequence of uint32_t with range 1 t0 5
auto SyncSeq = wgc::Sequence(1,5).Synchronized();

std::vector<std::future<void>> Futures;

for(auto i = 0; i < 6; i++)
{
  auto Future = std::async(std::launch::async, [&SyncSeq](){
    std::cout << "Val: " << std::to_string(SyncSeq->Next()) << "\n";
  });
  Futures.push_back(std::move(Future));
}
```
### wgc::ByteBuffer

A convenience class for creating a vector of bytes.

Example:
```
enum class Test
{
  One,
  Two,
};

wgc::ByteBuffer bytes;

bytes 
  << Test::Two
  << 10
  << "10"
  << std::to_string(11)
  << std::vector<int>{1,2};

uint16_t Array[3]{4,5,6};
bytes.AsStringStream().write(
  reinterpret_cast<const char*>(Array), 
  sizeof(Array)
);

auto t = bytes.GetBytes();
```