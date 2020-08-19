#include <napi.h>
#include <uv.h>
#include <iostream>
#include <string>

#include "common/helpers/digest.h"
#include "common/helpers/pow.h"
#include "common/helpers/sign.h"
#include "utils/bundle_miner.h"
#include "utils/memset_safe.h"

static Napi::Value powTrytes(const Napi::CallbackInfo& info) {
  if (info.Length() < 2) {
    Napi::Error::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString() || !info[1].IsNumber()) {
    Napi::Error::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  char *nonce = NULL;
  std::string trytes(info[0].As<Napi::String>().Utf8Value().c_str());
  auto ctrytes = trytes.c_str();
  auto mwm = static_cast<uint8_t>(Napi::To<unsigned>(info[1]));

  if ((nonce = iota_pow_trytes(ctrytes, mwm)) == NULL) {
    Napi::Error::New(env, "Binding iota_pow_trytes failed").ThrowAsJavaScriptException();
    return env.Null();
  }

  auto ret = Napi::New(env, nonce);
  free(nonce);

  return ret;
}

static Napi::Value powBundle(const Napi::CallbackInfo& info) {
  if (info.Length() < 4) {
    Napi::Error::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsArray() || !info[1].IsString() || !info[2].IsString() || !info[3].IsNumber()) {
    Napi::Error::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  bundle_transactions_t *bundle = NULL;
  iota_transaction_t tx;
  iota_transaction_t *curTx = NULL;
  flex_trit_t serializedFlexTrits[FLEX_TRIT_SIZE_8019];
  char serializedTrytes[NUM_TRYTES_SERIALIZED_TRANSACTION + 1] = {0};
  flex_trit_t flexTrunk[FLEX_TRIT_SIZE_243];
  flex_trit_t flexBranch[FLEX_TRIT_SIZE_243];
  size_t i = 0;

  std::string trunk(info[1].As<Napi::String>().Utf8Value().c_str());
  flex_trits_from_trytes(flexTrunk, NUM_TRITS_TRUNK, (tryte_t *)trunk.c_str(), NUM_TRYTES_TRUNK, NUM_TRYTES_TRUNK);
  std::string branch(info[2].As<Napi::String>().Utf8Value().c_str());
  flex_trits_from_trytes(flexBranch, NUM_TRITS_BRANCH, (tryte_t *)branch.c_str(), NUM_TRYTES_BRANCH, NUM_TRYTES_BRANCH);

  bundle_transactions_new(&bundle);

  Napi::Array txsTrytes = info[0].As<Napi::Array>();
  size_t txNum = txsTrytes->Length();
  for (size_t i = 0; i < txNum; i++) {
    flex_trits_from_trytes(
        serializedFlexTrits, NUM_TRITS_SERIALIZED_TRANSACTION,
        (tryte_t *)(txsTrytes->Get(Napi::GetCurrentContext(->As<Napi::String>().Utf8Value().c_str(), i).As<Napi::String>())),
        NUM_TRYTES_SERIALIZED_TRANSACTION, NUM_TRYTES_SERIALIZED_TRANSACTION);
    transaction_deserialize_from_trits(&tx, serializedFlexTrits, false);
    bundle_transactions_add(bundle, &tx);
  }

  auto mwm = static_cast<uint8_t>(Napi::To<unsigned>(info[3]));

  if (iota_pow_bundle(bundle, flexTrunk, flexBranch, mwm) != RC_OK) {
    bundle_transactions_free(&bundle);
    Napi::Error::New(env, "Binding iota_pow_bundle failed").ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Array ret = Napi::Array::New(env, txNum);
  i = 0;
  BUNDLE_FOREACH(bundle, curTx) {
    transaction_serialize_on_flex_trits(curTx, serializedFlexTrits);
    flex_trits_to_trytes((tryte_t *)serializedTrytes, NUM_TRYTES_SERIALIZED_TRANSACTION, serializedFlexTrits,
                         NUM_TRITS_SERIALIZED_TRANSACTION, NUM_TRITS_SERIALIZED_TRANSACTION);
    ret.Set(Napi::GetCurrentContext(), i, Napi::String::New(env, (char *)serializedTrytes));
    i++;
  }
  bundle_transactions_free(&bundle);

  return ret;
}

static Napi::Value genAddressTrytes(const Napi::CallbackInfo& info) {
  if (info.Length() < 3) {
    Napi::Error::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber()) {
    Napi::Error::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  char *address = NULL;
  std::string seed(info[0].As<Napi::String>().Utf8Value().c_str());
  auto cseed = seed.c_str();
  auto index = static_cast<uint64_t>(Napi::To<unsigned>(info[1]));
  auto security = static_cast<uint64_t>(Napi::To<unsigned>(info[2]));

  if ((address = iota_sign_address_gen_trytes(cseed, index, security)) == NULL) {
    memset_safe((void *)cseed, 81, 0, 81);
    Napi::Error::New(env, "Binding iota_sign_address_gen_trytes failed").ThrowAsJavaScriptException();
    return env.Null();
  }

  memset_safe((void *)cseed, 81, 0, 81);

  auto ret = Napi::New(env, address);
  free(address);

  return ret;
}

static Napi::Value genAddressTrits(const Napi::CallbackInfo& info) {
  if (info.Length() < 3) {
    Napi::Error::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsArray() || !info[1].IsNumber() || !info[2].IsNumber()) {
    Napi::Error::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  trit_t seed[243];
  Napi::Array trits = info[0].As<Napi::Array>();
  for (size_t i = 0; i < trits->Length(); i++) {
    seed[i] =
        trits->Get(Napi::GetCurrentContext(), i)->NumberValue(Napi::GetCurrentContext());
  }
  uint64_t index = static_cast<uint64_t>(Napi::To<unsigned>(info[1]));

  trit_t *address = iota_sign_address_gen_trits(seed, index, 2);

  memset_safe((void *)seed, 243, 0, 243);

  Napi::Array ret = Napi::Array::New(env, 243);
  for (size_t i = 0; i < 243; i++) {
    ret.Set(Napi::GetCurrentContext(), i, Napi::New(env, address[i]));
  };

  return ret;
}

static Napi::Value genSignatureTrytes(const Napi::CallbackInfo& info) {
  if (info.Length() < 4) {
    Napi::Error::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber() || !info[3].IsString()) {
    Napi::Error::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  char *signature = NULL;
  std::string seed(info[0].As<Napi::String>().Utf8Value().c_str());
  auto cseed = seed.c_str();
  auto index = static_cast<uint64_t>(Napi::To<unsigned>(info[1]));
  auto security = static_cast<uint64_t>(Napi::To<unsigned>(info[2]));
  std::string bundle(info[3].As<Napi::String>().Utf8Value().c_str());
  auto cbundle = bundle.c_str();

  if ((signature = iota_sign_signature_gen_trytes(cseed, index, security, cbundle)) == NULL) {
    memset_safe((void *)cseed, 81, 0, 81);
    Napi::Error::New(env, "Binding iota_sign_signature_gen_trytes failed").ThrowAsJavaScriptException();
    return env.Null();
  }

  memset_safe((void *)cseed, 81, 0, 81);

  auto ret = Napi::New(env, signature);
  free(signature);

  return ret;
}

static Napi::Value genSignatureTrits(const Napi::CallbackInfo& info) {
  if (info.Length() < 4) {
    Napi::Error::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsArray() || !info[1].IsNumber() || !info[2].IsNumber() || !info[3].IsArray()) {
    Napi::Error::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  trit_t seed[243];
  Napi::Array seed_array = info[0].As<Napi::Array>();
  for (size_t i = 0; i < seed_array->Length(); i++) {
    seed[i] =
        seed_array->Get(Napi::GetCurrentContext(), i)->NumberValue(Napi::GetCurrentContext());
  }
  uint64_t index = static_cast<uint64_t>(Napi::To<unsigned>(info[1]));
  uint64_t security = static_cast<uint64_t>(Napi::To<unsigned>(info[2]));
  trit_t bundle[243];
  Napi::Array bundle_array = info[3].As<Napi::Array>();
  for (size_t i = 0; i < bundle_array->Length(); i++) {
    bundle[i] = bundle_array->Get(Napi::GetCurrentContext(), i)
                    
                    ->NumberValue(Napi::GetCurrentContext())
                    ;
  }

  trit_t *signature = iota_sign_signature_gen_trits(seed, index, security, bundle);

  memset_safe((void *)seed, 243, 0, 243);

  Napi::Array ret = Napi::Array::New(env, 6561 * security);
  for (size_t i = 0; i < 6561 * security; i++) {
    ret.Set(Napi::GetCurrentContext(), i, Napi::New(env, signature[i]));
  };

  return ret;
}

static Napi::Value transactionHash(const Napi::CallbackInfo& info) {
  if (info.Length() < 1) {
    Napi::Error::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsString()) {
    Napi::Error::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  char *hash = NULL;
  std::string trytes(info[0].As<Napi::String>().Utf8Value().c_str());
  auto ctrytes = trytes.c_str();

  if ((hash = iota_digest(ctrytes)) == NULL) {
    Napi::Error::New(env, "Binding iota_digest failed").ThrowAsJavaScriptException();
    return env.Null();
  }

  auto ret = Napi::New(env, hash);
  free(hash);

  return ret;
}

static Napi::Value bundleMiner(const Napi::CallbackInfo& info) {
  uint64_t index = 0;

  if (info.Length() != 8) {
    Napi::Error::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsArray() || !info[1].IsNumber() || !info[2].IsArray() || !info[3].IsNumber() ||
      !info[4].IsNumber() || !info[5].IsNumber() || !info[6].IsNumber() || !info[7].IsNumber()) {
    Napi::Error::New(env, "Wrong arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  byte_t bundleNormalizedMax[81];
  Napi::Array bundle_array = info[0].As<Napi::Array>();
  for (size_t i = 0; i < bundle_array->Length(); i++) {
    bundleNormalizedMax[i] = bundle_array->Get(Napi::GetCurrentContext(), i)
                                 
                                 ->NumberValue(Napi::GetCurrentContext())
                                 ;
  }

  uint8_t security = static_cast<uint8_t>(Napi::To<unsigned>(info[1]));
  size_t essenceLength = static_cast<size_t>(Napi::To<unsigned>(info[3]));
  trit_t *essence = (trit_t *)malloc(sizeof(trit_t) * essenceLength);
  Napi::Array essence_array = info[2].As<Napi::Array>();
  for (size_t i = 0; i < essence_array->Length(); i++) {
    essence[i] = essence_array->Get(Napi::GetCurrentContext(), i)
                     
                     ->NumberValue(Napi::GetCurrentContext())
                     ;
  }
  uint32_t count = static_cast<uint32_t>(Napi::To<unsigned>(info[4]));
  uint8_t nprocs = static_cast<uint8_t>(Napi::To<unsigned>(info[5]));

  uint32_t miningThreshold = static_cast<uint32_t>(Napi::To<unsigned>(info[6]));

  uint8_t fullySecure = static_cast<uint8_t>(Napi::To<unsigned>(info[7]));

  bundle_miner_ctx_t *ctxs = NULL;
  size_t num_ctxs = 0;
  bool found_optimal_index = false;

  bundle_miner_allocate_ctxs(nprocs, &ctxs, &num_ctxs);

  if (bundle_miner_mine(bundleNormalizedMax, security, essence, essenceLength, count, miningThreshold,
                        fullySecure == 1 ? true : false, &index, ctxs, num_ctxs, &found_optimal_index) != RC_OK) {
    bundle_miner_deallocate_ctxs(&ctxs);

    return -1;
    free(essence);
    Napi::Error::New(env, "Bundle mining failed").ThrowAsJavaScriptException();
    return env.Null();
  }

  bundle_miner_deallocate_ctxs(&ctxs);

  return static_cast<uint32_t>(index);
  free(essence);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  NAN_EXPORT(target, powTrytes);
  NAN_EXPORT(target, powBundle);
  NAN_EXPORT(target, genAddressTrytes);
  NAN_EXPORT(target, genAddressTrits);
  NAN_EXPORT(target, genSignatureTrytes);
  NAN_EXPORT(target, genSignatureTrits);
  NAN_EXPORT(target, transactionHash);
  NAN_EXPORT(target, bundleMiner);
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
