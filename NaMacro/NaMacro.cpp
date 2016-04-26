// NaMacro.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#include <v8.h>
#include "include/libplatform/libplatform.h"
#include "include/v8.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Common.h"

#include "BasicModule.h"
#include "MouseModule.h"
#include "KeyboardModule.h"
#include "ExtModule.h"
#include "ScreenModule.h"

#include "NaMacro.h"

using namespace v8;

// Main
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd)
{
	// Initialize V8
	V8::InitializeICU();
	V8::InitializeExternalStartupData(__argv[0]);
	Platform* platform = platform::CreateDefaultPlatform();
	V8::InitializePlatform(platform);
	V8::Initialize();

	// 16.03.31 disabled
	//SetFlagsFromCommandLine(&argc, argv, true);

	// Create a new Isolate and make it the current one.
	Isolate::CreateParams create_params;
	ArrayBufferAllocator allocator;
	create_params.array_buffer_allocator = &allocator;
	Isolate* isolate = Isolate::New(create_params);

	// Creates a new execution environment containing the built-in functions.
	{
		Isolate::Scope isolate_scope(isolate);
		HandleScope handle_scope(isolate);
	
		// Create a template for the global object.
		Local<ObjectTemplate> global_template = ObjectTemplate::New(isolate);

		// Load Default js
		Local<String> script_source;
		MaybeLocal<String> script_name;

		char *str = "NaMacro.js";
		if (__argc > 1)
			str = __argv[1];

		script_source = ReadFile(isolate, str);
		script_name = String::NewFromUtf8(isolate, str, NewStringType::kNormal);
		if (script_source.IsEmpty()) {
			printf("Error reading '%s'\n", str);
			return 1;
		}

		// Create Default Modules (Register FunctionTemplate)
		CreateDefaultModules(isolate, global_template);

		// Enter the newly created execution environment.
		Local<Context> context = Context::New(isolate, NULL, global_template);
		Context::Scope context_scope(context);

		// Initialize Modules (Access global context)
		InitModules(isolate, global_template);

		Local<Script> script;
		{
			// Compile script in try/catch context.
			TryCatch try_catch(isolate);

			ScriptOrigin origin(script_name.ToLocalChecked());
			Script::Compile(context, script_source, &origin).ToLocal(&script);
			if (script.IsEmpty())
			{
				// Print errors that happened during compilation.
				//if (report_exceptions)
				//	ReportException(isolate, &try_catch);
				return 1;
			}
		}

		{
			TryCatch try_catch(isolate);

			script->Run(context);
			if (try_catch.HasCaught())
			{
				if (report_exceptions)
					ReportException(isolate, &try_catch);
				return 1;
			}
		}

		// Find main()
		Local<Object> global = isolate->GetCurrentContext()->Global();
		Local<String> main_name = String::NewFromUtf8(isolate, "main", NewStringType::kNormal).ToLocalChecked();
		Local<Value> main_value = global->Get(main_name);
		if (main_value.IsEmpty() || main_value->IsUndefined())
		{
			// error
			printf("Cannot find main function\n");
			return 0;
		}

		if (!main_value->IsFunction())
		{
			// error
			printf("main is not function!\n");
			return 0;
		}

		Local<Function> main_fn = Local<Function>::Cast(global->Get(main_name));

		// Run main function
		{
			TryCatch try_catch(isolate);

			// main arguments
			const int js_argc = 1;
			Local<Value> js_argv[js_argc] = { String::NewFromUtf8(isolate, "hello :)", String::kNormalString) };
			main_fn->Call(isolate->GetCurrentContext()->Global(), js_argc, js_argv);
			
			if (try_catch.HasCaught())
			{
				if (report_exceptions)
					ReportException(isolate, &try_catch);
				return 1;
			}
		}

		// Collect Garbage
		isolate->IdleNotification(1000);

		// Release Modules
		ReleaseModules();
	}

	// Dispose the isolate and tear down V8.

	// 16.04.26 check; GlobalHandles broken
	//isolate->Dispose();

	V8::Dispose();
	V8::ShutdownPlatform();
	delete platform;

	return 0;
}

void ReportException(Isolate *isolate, TryCatch* try_catch) 
{
	HandleScope handle_scope(isolate);
	String::Utf8Value exception(try_catch->Exception());
	const char* exception_string = *exception;
	Local<Message> message = try_catch->Message();
	if (message.IsEmpty()) 
	{
		// V8 didn't provide any extra information about this error; just
		// print the exception.
		printf("%s\n", exception_string);
	}
	else 
	{
		// Print (filename):(line number): (message).
		String::Utf8Value filename(message->GetScriptResourceName());
		const char* filename_string = *filename;
		int linenum = message->GetLineNumber();
		printf("%s:%i: %s\n", filename_string, linenum, exception_string);

		// Print line of source code.
		String::Utf8Value sourceline(message->GetSourceLine());
		const char* sourceline_string = *sourceline;
		printf("%s\n", sourceline_string);

		// Print wavy underline (GetUnderline is deprecated).
		int start = message->GetStartColumn();
		for (int i = 0; i < start; i++) 
		{
			printf(" ");
		}

		int end = message->GetEndColumn();
		for (int i = start; i < end; i++) 
		{
			printf("^");
		}
		printf("\n");
	}
}

void CreateDefaultModules(Isolate * isolate, Local<ObjectTemplate>& global_template)
{
	ModuleBase *pModule;
#define CREATE_MODULE(_class) \
	pModule = new _class; \
	pModule->Create(isolate, global_template); \
	g_ModuleList.push_back(pModule);

	CREATE_MODULE(NaBasicModule);
	CREATE_MODULE(NaExtModule);
	CREATE_MODULE(NaMouseModule);
	CREATE_MODULE(NaKeyboardModule);
	CREATE_MODULE(NaScreenModule);
}

void InitModules(Isolate *isolate, Local<ObjectTemplate> &global_template)
{
	vector<ModuleBase*>::iterator it = g_ModuleList.begin();
	for (; it != g_ModuleList.end(); ++it)
	{
		ModuleBase *pModule = *it;
		pModule->Init(isolate, global_template);
	}
}

void ReleaseModules()
{
	vector<ModuleBase*>::iterator it = g_ModuleList.begin();
	for (; it != g_ModuleList.end(); ++it)
	{
		ModuleBase *pModule = *it;
		pModule->Release();
		delete pModule;
	}

	g_ModuleList.clear();
}
