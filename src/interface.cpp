#include "common/helpers/digest.h"
#include "common/helpers/pow.h"
#include "common/helpers/sign.h"
#include <iostream>
#include <nan.h>
#include <string>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

const char *ToCString(Local<String> str) {
  String::Utf8Value value(str);
  return *value ? *value : "<string conversion failed>";
}

void powTrytes(const Nan::FunctionCallbackInfo<v8::Value> &info) {

  if (info.Length() < 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsString() || !info[1]->IsNumber()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  Nan::Utf8String nan_string(info[0]);
  std::string name(*nan_string);
  const char *trytes = name.c_str();

  uint8_t mwm = (uint8_t)info[1]->NumberValue();

  char *foundNonce = iota_pow_trytes(trytes, mwm);

  info.GetReturnValue().Set(Nan::New(foundNonce).ToLocalChecked());
}

void powBundle(const Nan::FunctionCallbackInfo<v8::Value> &info) {}

void genAddressTrytes(const Nan::FunctionCallbackInfo<v8::Value> &info) {

  if (info.Length() < 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsString() || !info[1]->IsNumber()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  Nan::Utf8String nan_string(info[0]);
  std::string name(*nan_string);
  const char *seed = name.c_str();

  uint64_t index = (uint64_t)info[1]->NumberValue();

  char *address = iota_sign_address_gen_trytes(seed, index, 2);

  info.GetReturnValue().Set(Nan::New(address).ToLocalChecked());
}

void genAddressTrits(const Nan::FunctionCallbackInfo<v8::Value> &info) {

  if (info.Length() < 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsArray() || !info[1]->IsNumber()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  trit_t seed[243];
  Local<v8::Array> trits = Local<v8::Array>::Cast(info[0]);
  for (int i = 0; i < trits->Length(); i++) {
    seed[i] = trits->Get(i)->NumberValue();
  }

  uint64_t index = (uint64_t)info[1]->NumberValue();

  trit_t *address = iota_sign_address_gen_trits(seed, index, 2);

  v8::Local<v8::Array> ret = Nan::New<v8::Array>(243);
  for (size_t i = 0; i < 243; i++) {
    ret->Set(i, Nan::New(address[i]));
  };

  info.GetReturnValue().Set(ret);
}

void genSignatureTrytes(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.Length() < 4) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsString() || !info[1]->IsNumber() || !info[2]->IsNumber() ||
      !info[3]->IsString()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  Nan::Utf8String nan_string_seed(info[0]);
  std::string seed_str(*nan_string_seed);
  const char *seed = seed_str.c_str();

  uint64_t index = (uint64_t)info[1]->NumberValue();
  uint64_t security = (uint64_t)info[2]->NumberValue();

  Nan::Utf8String nan_string_bundle(info[3]);
  std::string bundle_str(*nan_string_bundle);
  const char *bundle = bundle_str.c_str();

  char *signature =
      iota_sign_signature_gen_trytes(seed, index, security, bundle);

  info.GetReturnValue().Set(Nan::New(signature).ToLocalChecked());
}

void genSignatureTrits(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.Length() < 4) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsArray() || !info[1]->IsNumber() || !info[2]->IsNumber() ||
      !info[3]->IsArray()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  trit_t seed[243];
  Local<v8::Array> seed_array = Local<v8::Array>::Cast(info[0]);
  for (int i = 0; i < seed_array->Length(); i++) {
    seed[i] = seed_array->Get(i)->NumberValue();
  }

  uint64_t index = (uint64_t)info[1]->NumberValue();
  uint64_t security = (uint64_t)info[2]->NumberValue();

  trit_t bundle[243];
  Local<v8::Array> bundle_array = Local<v8::Array>::Cast(info[3]);
  for (int i = 0; i < bundle_array->Length(); i++) {
    bundle[i] = bundle_array->Get(i)->NumberValue();
  }

  trit_t *signature =
      iota_sign_signature_gen_trits(seed, index, security, bundle);

  v8::Local<v8::Array> ret = Nan::New<v8::Array>(6561 * security);
  for (size_t i = 0; i < 6561 * security; i++) {
    ret->Set(i, Nan::New(signature[i]));
  };

  info.GetReturnValue().Set(ret);
}

void transactionHash(const Nan::FunctionCallbackInfo<v8::Value> &info) {
  if (info.Length() < 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsString()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  Nan::Utf8String nan_string(info[0]);
  std::string name(*nan_string);
  const char *trytes = name.c_str();

  char *hash = iota_digest(trytes);

  info.GetReturnValue().Set(Nan::New(hash).ToLocalChecked());
}

void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("powTrytes").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(powTrytes)->GetFunction());
  exports->Set(Nan::New("powBundle").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(powBundle)->GetFunction());
  exports->Set(Nan::New("genAddressTrytes").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(genAddressTrytes)->GetFunction());
  exports->Set(Nan::New("genAddressTrits").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(genAddressTrits)->GetFunction());
  exports->Set(
      Nan::New("genSignatureTrytes").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(genSignatureTrytes)->GetFunction());
  exports->Set(
      Nan::New("genSignatureTrits").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(genSignatureTrits)->GetFunction());
  exports->Set(Nan::New("transactionHash").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(transactionHash)->GetFunction());
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)
