#include <iostream>
#include <string>
#include <curl/curl.h>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <functional>
#include <time.h>
#include <queue>
#include "Response.h"
#include "cURLH.h"

#ifndef __HTTPBOT_H__
#define __HTTPBOT_H__

class EmptyURLQueueException {};

class HTTPBot
{
	public:

		// Constructors & Destructors
		HTTPBot();
		HTTPBot(std::string prxy, int port, proxy_type_t pType = DISABLED_PROXY, std::string creds = "");
		~HTTPBot();

		void pushURL(std::string newURL);																					// Push a URL onto the Queue
		std::string popURL();																								// Pop a URL from the Queue
		
		void requestGET(); 																									// Call using the Queue
		void requestGET(std::string url, std::string ref = ""); 															// Call with a specified URL
		void requestPOST(std::string params);																				// Call using the Queue
		void requestPOST(std::string url, std::string params, std::string ref = "");										// Call with a specified URL
		
		void setSSL(bool verify = false, int hostval = 0, std::string certfile = "");										// Set SSL Settings for connection		
		void setProxy(std::string prxy = "", int port = 0, proxy_type_t ptype = DISABLED_PROXY, std::string creds = "");	// Set Proxy settings for connection
		
		void setVerbose(bool val) { verbose = val; };																		// Set verbosity explicitly
		void setVerbose() { verbose = !verbose; };																			// Toggle verbosity
		
		void rebuildHandle();																								// Rebuild the cURL handle
		std::string getData() { return content; };
		cURLH* ch;	
		CURL* handle;																										// cURL Handle
	
	private:
		static std::string content;																							// Content from last request
		std::vector<std::string> headers;
		std::string url;																									// Current URL to process
		std::queue<std::string> urls;																						// Queue of URLs to process
		bool verbose;																										// Enable debugging output
		std::string readBuffer;																								// Stream to write to in the callback
		Response* lastRequest;
		static size_t WriteCallback(char *wd, size_t size, size_t nmemb, std::string *stream);								// Callback for requests

		
};


#endif