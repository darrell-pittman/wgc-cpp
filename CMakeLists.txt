cmake_minimum_required (VERSION 3.0.0)
project (wgc VERSION 0.1.0)

set(HEADER 
  include/wgc/wgc_algorithms.h
  include/wgc/wgc_bytebuffer.h
  include/wgc/wgc_job_thread.h
  include/wgc/wgc_memory.h
  include/wgc/wgc_sequence.h
)

set(SOURCE
  src/wgc_job_thread.cpp
  src/wgc_bytebuffer.cpp
)

add_library(wgc SHARED ${HEADER} ${SOURCE})

target_compile_features(wgc PUBLIC cxx_std_17)
target_include_directories(wgc PUBLIC include)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(wgc Threads::Threads)
