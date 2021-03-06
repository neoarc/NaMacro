#pragma once

#include <iostream>
#include <ostream>
#include <map>

#include "curl_easy.h"
#include "curl_exception.h"
#include "curl_ios.h"
#include "curl_exception.h"

#include "NaString.h"

using std::cout;
using std::endl;
using std::ostringstream;

using curl::curl_easy;
using curl::curl_ios;
using curl::curl_easy_exception;
using curl::curlcpp_traceback;

class NaCurl
{
public:
	NaCurl();
	~NaCurl();

	// Methods
	NaString Post(NaString strUrl, NaString strBody = L"");
	NaString Put(NaString strUrl, NaString strBody = L"");
	bool Get(NaString strUrl, char **outBuf, long &lSize);

	// Stream
	ostringstream m_ostrOutput;

	// Error handling
	unsigned long GetLastError();
	NaString GetLastErrorMessage();

	unsigned long m_lLastError;
	NaString m_strLastError;

	// Callback
	void SetCallback(std::function<void(size_t)> fnCallback);
	void OnCallback(size_t added);
	static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp);
	std::function<void(size_t)> m_UserCallback;
	static std::map<ostringstream *, void*> s_mapInstance;

	unsigned long m_lDownloaded;

protected:
	virtual void ClearOutputStream();
	void ClearLastError();

	curl_easy *m_pCurlEasy;
};
