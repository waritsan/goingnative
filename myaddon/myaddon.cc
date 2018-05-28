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

NAN_METHOD(Delay) {
  Nan::Maybe<int> maybeInt = Nan::To<int>(info[0]);
  if (maybeInt.IsNothing()) {
    Nan::ThrowError("Error converting first argument to int");
  }
  int delay = maybeInt.FromJust();
  #ifdef _WIN32
    Sleep(delay);
  #else
    usleep(delay * 1000);
  #endif
  v8::Local<Function> callback = info[1].As<Function>();
  Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callback, 0, NULL);
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, Nan::New("delay").ToLocalChecked(),
      Nan::GetFunction(Nan::New<FunctionTemplate>(Delay)).ToLocalChecked());
}

NODE_MODULE(myaddon, Init);
