#include <nan.h>
#include <string>
#include "common/helpers/pow.h"
#include "common/helpers/sign.h"

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

const char* ToCString(Local<String> str) {
    String::Utf8Value value(str);
    return *value ? *value : "<string conversion failed>";
}

void PoW(const Nan::FunctionCallbackInfo<v8::Value>& info) {

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
    const char* trytes = name.c_str();

    uint8_t mwm = (uint8_t) info[1]->NumberValue();
    char* nonce = (char*)calloc(27 + 1, sizeof(char));

    char* foundNonce = iota_pow(trytes, mwm);

    info.GetReturnValue().Set(Nan::New(foundNonce).ToLocalChecked());
}

void GeN(const Nan::FunctionCallbackInfo<v8::Value>& info) {

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
    const char* seed = name.c_str();

    uint8_t index = (uint8_t) info[1]->NumberValue();

    char* foundNonce = iota_sign_address_gen(seed, index, 2);

    info.GetReturnValue().Set(Nan::New(foundNonce).ToLocalChecked());
}

void Init(v8::Local<v8::Object> exports) {
    exports->Set(Nan::New("pow").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(PoW)->GetFunction());
    exports->Set(Nan::New("gen").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(GeN)->GetFunction());
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)