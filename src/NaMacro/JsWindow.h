#pragma once

#include "JsObjectBase.h"

class NaWindow;
class JsWindow : public JsObjectBase
{
public:
	JsWindow();
	virtual ~JsWindow();

	// member
	NaWindow *m_pNativeWindow;

	// static
	static NaWindow* UnwrapNativeWindow(Local<Object> obj);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void FindWindows(Isolate *isolate, const wchar_t *name, Local<Array> &results);

	// wrap object
	virtual Local<ObjectTemplate> MakeObjectTemplate(Isolate *isolate);
	virtual Global<ObjectTemplate>& GetObjectTemplate() { return s_JsWindowTemplate; };
	static Global<ObjectTemplate> s_JsWindowTemplate;

	// accessors
	DEFINE_CLASS_ACCESSOR(x);
	DEFINE_CLASS_ACCESSOR(y);
	DEFINE_CLASS_ACCESSOR(width);
	DEFINE_CLASS_ACCESSOR(height);
	DEFINE_CLASS_ACCESSOR_RO(clientWidth);
	DEFINE_CLASS_ACCESSOR_RO(clientHeight);
	DEFINE_CLASS_ACCESSOR_RO(class);
	DEFINE_CLASS_ACCESSOR(text);
	DEFINE_CLASS_ACCESSOR(visible);
	DEFINE_CLASS_ACCESSOR(topmost);
	DEFINE_CLASS_ACCESSOR(handle);
	DEFINE_CLASS_ACCESSOR(state);

	// methods
	DEFINE_CLASS_METHOD(constructor);
	DEFINE_CLASS_METHOD(create);
	DEFINE_CLASS_METHOD(move);
	DEFINE_CLASS_METHOD(activate);
	DEFINE_CLASS_METHOD(close);
	DEFINE_CLASS_METHOD(alert);
	DEFINE_CLASS_METHOD(addControl);
};
