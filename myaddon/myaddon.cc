#include <nan.h>

#ifndef _WIN32
#include <unistd.h>
#endif

using namespace v8;

// NAN_METHOD(Print) {
//   Nan::MaybeLocal<String> maybeStr = Nan::To<String>(info[0]);
//   v8::Local<String> str;
//   if (maybeStr.ToLocal(&str) == false) {
//     Nan::ThrowError("Error converting first argument to string");
//   }
//   printf("%s\n", *String::Utf8Value(str));
// }

// NAN_MODULE_INIT(Init) {
//   Nan::Set(target, Nan::New("print").ToLocalChecked(),
//       Nan::GetFunction(Nan::New<FunctionTemplate>(Print)).ToLocalChecked());
// }

// NODE_MODULE(myaddon, Init);

// NAN_METHOD(Length) {
//   Nan::MaybeLocal<String> maybeStr = Nan::To<String>(info[0]);
//   v8::Local<String> str;
//   if (maybeStr.ToLocal(&str) == false) {
//     Nan::ThrowError("Error converting first argument to string");
//   }
//   int len = strlen(*String::Utf8Value(str));
//   info.GetReturnValue().Set(len);
// }
//
// NAN_MODULE_INIT(Init) {
//   Nan::Set(target, Nan::New("length").ToLocalChecked(),
//       Nan::GetFunction(Nan::New<FunctionTemplate>(Length)).ToLocalChecked());
// }
//
// NODE_MODULE(myaddon, Init);

// A worker class extending the NanAsyncWorker helper
// class, a simple encapsulation of worker-thread
// logic to make simple tasks easier

class MyWorker : public Nan::AsyncWorker {
 public:
  // Constructor
  MyWorker(Nan::Callback *callback, int delay)
    : Nan::AsyncWorker(callback), delay(delay) {}
  // Destructor
  ~MyWorker() {}

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on `this`.
  void Execute () {
    // Asynchronous, non-V8 work goes here
    #ifdef _WIN32
      Sleep(delay);
    #else
      usleep(delay * 1000);
    #endif
  }

  // Executed when the async work is complete
  // this function will be run inside the main event loop
  // so it is safe to use V8 again
  void HandleOKCallback () {
    Nan::HandleScope scope;

    // Nan::Callback#Call() does a Nan::MakeCallback() for us
    callback->Call(0, NULL);
  }

 private:
  int delay;
};

NAN_METHOD(Delay) {
  // get delay and callback
  // create NanCallback instance wrapping the callback
  // create a MyWorker instance, passing the callback and delay
  // queue the worker instance onto the thread-pool
  Nan::Maybe<int> maybeInt = Nan::To<int>(info[0]);
  if (maybeInt.IsNothing()) {
    Nan::ThrowError("Error converting first argument to int");
  }
  if (info[1]->IsFunction() == false) {
    Nan::ThrowError("Error converting second argument to function");
  }
  int delay = maybeInt.FromJust();
  v8::Local<Function> callback = info[1].As<Function>();
  Nan::Callback* nanCallback = new Nan::Callback(callback);
  MyWorker* worker = new MyWorker(nanCallback, delay);
  Nan::AsyncQueueWorker(worker);
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, Nan::New("delay").ToLocalChecked(),
      Nan::GetFunction(Nan::New<FunctionTemplate>(Delay)).ToLocalChecked());
}

NODE_MODULE(myaddon, Init);
