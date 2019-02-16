#include <nan.h>
#include <string>

#include "common/helpers/digest.h"
#include "common/helpers/pow.h"
#include "common/helpers/sign.h"

static NAN_METHOD(powTrytes) {
  if (info.Length() < 2) {
    Nan::ThrowError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsString() || !info[1]->IsNumber()) {
    Nan::ThrowError("Wrong arguments");
    return;
  }

  char *nonce = NULL;
  std::string trytes(*Nan::Utf8String(info[0]));
  auto ctrytes = trytes.c_str();
  auto mwm = static_cast<uint8_t>(Nan::To<unsigned>(info[1]).FromJust());

  if ((nonce = iota_pow_trytes(ctrytes, mwm)) == NULL) {
    Nan::ThrowError("Binding iota_pow_trytes failed");
    return;
  }

  auto ret = Nan::New(nonce).ToLocalChecked();
  free(nonce);

  info.GetReturnValue().Set(ret);
}

static NAN_METHOD(powBundle) {
  if (info.Length() < 4) {
    Nan::ThrowError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsArray() || !info[1]->IsString() || !info[2]->IsString() ||
      !info[3]->IsNumber()) {
    Nan::ThrowError("Wrong arguments");
    return;
  }
}

static NAN_METHOD(genAddressTrytes) {
  if (info.Length() < 3) {
    Nan::ThrowError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsString() || !info[1]->IsNumber() || !info[2]->IsNumber()) {
    Nan::ThrowError("Wrong arguments");
    return;
  }

  char *address = NULL;
  std::string seed(*Nan::Utf8String(info[0]));
  auto cseed = seed.c_str();
  auto index = static_cast<uint64_t>(Nan::To<unsigned>(info[1]).FromJust());
  auto security = static_cast<uint64_t>(Nan::To<unsigned>(info[2]).FromJust());

  if ((address = iota_sign_address_gen_trytes(cseed, index, security)) ==
      NULL) {
    Nan::ThrowError("Binding iota_sign_address_gen_trytes failed");
    return;
  }

  auto ret = Nan::New(address).ToLocalChecked();
  free(address);

  info.GetReturnValue().Set(ret);
}

static NAN_METHOD(genAddressTrits) {
  if (info.Length() < 3) {
    Nan::ThrowError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsArray() || !info[1]->IsNumber() || !info[2]->IsNumber()) {
    Nan::ThrowError("Wrong arguments");
    return;
  }

  trit_t seed[243];
  v8::Local<v8::Array> trits = v8::Local<v8::Array>::Cast(info[0]);
  for (size_t i = 0; i < trits->Length(); i++) {
    seed[i] = trits->Get(i)->NumberValue();
  }
  uint64_t index = static_cast<uint64_t>(Nan::To<unsigned>(info[1]).FromJust());

  trit_t *address = iota_sign_address_gen_trits(seed, index, 2);

  v8::Local<v8::Array> ret = Nan::New<v8::Array>(243);
  for (size_t i = 0; i < 243; i++) {
    ret->Set(i, Nan::New(address[i]));
  };

  info.GetReturnValue().Set(ret);
}

static NAN_METHOD(genSignatureTrytes) {
  if (info.Length() < 4) {
    Nan::ThrowError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsString() || !info[1]->IsNumber() || !info[2]->IsNumber() ||
      !info[3]->IsString()) {
    Nan::ThrowError("Wrong arguments");
    return;
  }

  char *signature = NULL;
  std::string seed(*Nan::Utf8String(info[0]));
  auto cseed = seed.c_str();
  auto index = static_cast<uint64_t>(Nan::To<unsigned>(info[1]).FromJust());
  auto security = static_cast<uint64_t>(Nan::To<unsigned>(info[2]).FromJust());
  std::string bundle(*Nan::Utf8String(info[3]));
  auto cbundle = bundle.c_str();

  if ((signature = iota_sign_signature_gen_trytes(cseed, index, security,
                                                  cbundle)) == NULL) {
    Nan::ThrowError("Binding iota_sign_signature_gen_trytes failed");
    return;
  }

  auto ret = Nan::New(signature).ToLocalChecked();
  free(signature);

  info.GetReturnValue().Set(ret);
}

static NAN_METHOD(genSignatureTrits) {
  if (info.Length() < 4) {
    Nan::ThrowError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsArray() || !info[1]->IsNumber() || !info[2]->IsNumber() ||
      !info[3]->IsArray()) {
    Nan::ThrowError("Wrong arguments");
    return;
  }

  trit_t seed[243];
  v8::Local<v8::Array> seed_array = v8::Local<v8::Array>::Cast(info[0]);
  for (size_t i = 0; i < seed_array->Length(); i++) {
    seed[i] = seed_array->Get(i)->NumberValue();
  }
  uint64_t index = static_cast<uint64_t>(Nan::To<unsigned>(info[1]).FromJust());
  uint64_t security =
      static_cast<uint64_t>(Nan::To<unsigned>(info[2]).FromJust());
  trit_t bundle[243];
  v8::Local<v8::Array> bundle_array = v8::Local<v8::Array>::Cast(info[3]);
  for (size_t i = 0; i < bundle_array->Length(); i++) {
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

static NAN_METHOD(transactionHash) {
  if (info.Length() < 1) {
    Nan::ThrowError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsString()) {
    Nan::ThrowError("Wrong arguments");
    return;
  }

  char *hash = NULL;
  std::string trytes(*Nan::Utf8String(info[0]));
  auto ctrytes = trytes.c_str();

  if ((hash = iota_digest(ctrytes)) == NULL) {
    Nan::ThrowError("Binding iota_digest failed");
    return;
  }

  auto ret = Nan::New(hash).ToLocalChecked();
  free(hash);

  info.GetReturnValue().Set(ret);
}

NAN_MODULE_INIT(Init) {
  NAN_EXPORT(target, powTrytes);
  NAN_EXPORT(target, powBundle);
  NAN_EXPORT(target, genAddressTrytes);
  NAN_EXPORT(target, genAddressTrits);
  NAN_EXPORT(target, genSignatureTrytes);
  NAN_EXPORT(target, genSignatureTrits);
  NAN_EXPORT(target, transactionHash);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)
