#include <iostream>
#include <nan.h>
#include <string>

#include "common/helpers/digest.h"
#include "common/helpers/pow.h"
#include "common/helpers/sign.h"

#include "utils/memset_safe.h"

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

  bundle_transactions_t *bundle = NULL;
  iota_transaction_t tx;
  iota_transaction_t *curTx = NULL;
  flex_trit_t serializedFlexTrits[FLEX_TRIT_SIZE_8019];
  char serializedTrytes[NUM_TRYTES_SERIALIZED_TRANSACTION + 1] = {0};
  flex_trit_t flexTrunk[FLEX_TRIT_SIZE_243];
  flex_trit_t flexBranch[FLEX_TRIT_SIZE_243];
  size_t i = 0;

  std::string trunk(*Nan::Utf8String(info[1]));
  flex_trits_from_trytes(flexTrunk, NUM_TRITS_TRUNK, (tryte_t *)trunk.c_str(),
                         NUM_TRYTES_TRUNK, NUM_TRYTES_TRUNK);
  std::string branch(*Nan::Utf8String(info[2]));
  flex_trits_from_trytes(flexBranch, NUM_TRITS_BRANCH,
                         (tryte_t *)branch.c_str(), NUM_TRYTES_BRANCH,
                         NUM_TRYTES_BRANCH);

  bundle_transactions_new(&bundle);

  v8::Local<v8::Array> txsTrytes = v8::Local<v8::Array>::Cast(info[0]);
  size_t txNum = txsTrytes->Length();
  for (size_t i = 0; i < txNum; i++) {
    flex_trits_from_trytes(
        serializedFlexTrits, NUM_TRITS_SERIALIZED_TRANSACTION,
        (tryte_t *)(*Nan::Utf8String(txsTrytes->Get(i).As<v8::String>())),
        NUM_TRYTES_SERIALIZED_TRANSACTION, NUM_TRYTES_SERIALIZED_TRANSACTION);
    transaction_deserialize_from_trits(&tx, serializedFlexTrits, false);
    bundle_transactions_add(bundle, &tx);
  }

  auto mwm = static_cast<uint8_t>(Nan::To<unsigned>(info[3]).FromJust());

  if (iota_pow_bundle(bundle, flexTrunk, flexBranch, mwm) != RC_OK) {
    bundle_transactions_free(&bundle);
    Nan::ThrowError("Binding iota_pow_bundle failed");
    return;
  }

  v8::Local<v8::Array> ret = Nan::New<v8::Array>(txNum);
  i = 0;
  BUNDLE_FOREACH(bundle, curTx) {
    transaction_serialize_on_flex_trits(curTx, serializedFlexTrits);
    flex_trits_to_trytes((tryte_t *)serializedTrytes,
                         NUM_TRYTES_SERIALIZED_TRANSACTION, serializedFlexTrits,
                         NUM_TRITS_SERIALIZED_TRANSACTION,
                         NUM_TRITS_SERIALIZED_TRANSACTION);
    ret->Set(i,
             Nan::New<v8::String>((char *)serializedTrytes).ToLocalChecked());
    i++;
  }
  bundle_transactions_free(&bundle);

  info.GetReturnValue().Set(ret);
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
    memset_safe((void*)cseed, 81, 0, 81);
    Nan::ThrowError("Binding iota_sign_address_gen_trytes failed");
    return;
  }

  memset_safe((void*)cseed, 81, 0, 81);

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
    seed[i] = trits->Get(i)->NumberValue(Nan::GetCurrentContext()).FromJust();
  }
  uint64_t index = static_cast<uint64_t>(Nan::To<unsigned>(info[1]).FromJust());

  trit_t *address = iota_sign_address_gen_trits(seed, index, 2);

  memset_safe((void*)seed, 243, 0, 243);

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
    memset_safe((void*)cseed, 81, 0, 81);
    Nan::ThrowError("Binding iota_sign_signature_gen_trytes failed");
    return;
  }

  memset_safe((void*)cseed, 81, 0, 81);

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
    seed[i] = seed_array->Get(i)->NumberValue(Nan::GetCurrentContext()).FromJust();
  }
  uint64_t index = static_cast<uint64_t>(Nan::To<unsigned>(info[1]).FromJust());
  uint64_t security =
      static_cast<uint64_t>(Nan::To<unsigned>(info[2]).FromJust());
  trit_t bundle[243];
  v8::Local<v8::Array> bundle_array = v8::Local<v8::Array>::Cast(info[3]);
  for (size_t i = 0; i < bundle_array->Length(); i++) {
    bundle[i] = bundle_array->Get(i)->NumberValue(Nan::GetCurrentContext()).FromJust();
  }

  trit_t *signature =
      iota_sign_signature_gen_trits(seed, index, security, bundle);

  memset_safe((void*)seed, 243, 0, 243);

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
