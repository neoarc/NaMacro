#pragma once

#include <v8.h>
#include "Common.h"
#include "ModuleBase.h"

class NaExtModule : public ModuleBase
{
public:
	virtual void Init(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> &global_template);
	virtual void Release();
};

void ConvGMacroToNaMacro(V8_FUNCTION_ARGS);
void TTSSpeak(V8_FUNCTION_ARGS);