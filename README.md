#WGC-CPP
Some c++ utilities.

###wgc::JobThread

A functor used to create a long running thread to which Tasks can be submitted.

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


