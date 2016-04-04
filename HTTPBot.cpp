#include <iostream>
#include <string>
#include <curl/curl.h>
#include <vector>
#include <sstream>
#include "Response.h"
#include "HTTPBot.h"
#include <stdlib.h>
#include <functional>
#include <time.h>
#include <queue>

std::string HTTPBot::content;

// Constructors & Destructors
HTTPBot::HTTPBot()
{
	HTTPBot("", 0, DISABLED_PROXY, "");
}		

HTTPBot::HTTPBot(std::string prxy, int port, proxy_type_t pType, std::string creds)
{
	ch = new cURLH(prxy, port, pType, creds);
	content = "";
	url = "";
	verbose = false;
	handle = ch->getCH();
	lastRequest = new Response();
	curl_easy_setopt(ch->getCH(), CURLOPT_WRITEFUNCTION, WriteCallback);
	std::cout << "This works...\n";
	curl_easy_setopt(ch->getCH(), CURLOPT_WRITEDATA, &readBuffer);
	url = "http://www.google.ca";
	curl_easy_setopt(ch->getCH(), CURLOPT_URL, url.c_str());
	std::cout << "hmmm...\n";
	CURLcode res = curl_easy_perform(ch->getCH());

	if(res != CURLE_OK)
	{
		//std::string strerr(curl_easy_strerror(res));
		std::cerr << "curl_easy_perform() failed: " << res << "\n";
	}
	else
		std::cout << content << std::endl;
	url = "http://www.msn.ca";
	ch->setCH();
	curl_easy_setopt(ch->getCH(), CURLOPT_URL, url.c_str());
	std::cout << "hmmm...\n";
	res = curl_easy_perform(ch->getCH());

	if(res != CURLE_OK)
	{
		//std::string strerr(curl_easy_strerror(res));
		std::cerr << "curl_easy_perform() failed: " << res << "\n";
	}
	else
		std::cout << content << std::endl;
}	

HTTPBot::~HTTPBot()
{
	if(ch != NULL)
	{
		ch->cleanup();
		delete ch;
		ch = NULL;
	}
	if(lastRequest != NULL)
	{
		delete lastRequest;
		lastRequest = NULL;
	}
}		

void HTTPBot::pushURL(std::string newURL)
{
	urls.push(newURL);
}

std::string HTTPBot::popURL()
{
	std::string newURL = "";
	if(urls.size() > 0)
	{
		newURL = urls.front();
		urls.pop();
	}
	else
	{
		std::cerr << "No URLs in Queue";
		throw EmptyURLQueueException();
	}
	return newURL;
}

void HTTPBot::requestGET()
{
	requestGET("", "");
}

void HTTPBot::requestGET(std::string surl, std::string ref)
{
	std::cout << "Entering requestGET()\n";
	if(surl == "")
	{
		if(urls.size() > 0)
		{
			url = popURL();
		}
		else 
		{
			if(verbose)
				std::cout << "No URLs currently in queue\n";
			return;
		}
	}
	if(ch->getCH() == NULL)
	{
		std::cout << "NULLZIES\n";
		return;
	}
	std::cout << "Loaded URL " << url.c_str() << std::endl;		
	if(ref == "")
		ref = url;
	std::cout << "Loaded referer " << ref.c_str() << std::endl;
	if(verbose)
		std::cout << "Attempting to scrape URL: " << url.c_str() << std::endl;
	ch->setCH();
	std::cout << sizeof(ch->getCH()) << std::endl;
	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
	std::cout << "URL Set\n";
	curl_easy_setopt(handle, CURLOPT_POST, 0);
	/*
	if(headers.size() > 0)
	{
		std::string* head = &headers[0];
		//curl_easy_setopt(ch, CURLOPT_HTTPHEADER, head);	
	}
	*/
	std::cout << "Headers set\n";
	
	CURLcode res = curl_easy_perform(handle);

	if(res != CURLE_OK)
	{
		//std::string strerr(curl_easy_strerror(res));
		std::cerr << "curl_easy_perform() failed: " << res << "\n";
	}
	//return new Response($x);
}

void HTTPBot::requestPOST(std::string params)
{
	requestPOST("", params);
}

void HTTPBot::requestPOST(std::string url, std::string params, std::string ref)
{
	if(url == "")
	{
		if(urls.size() > 0)
			url = popURL();
		else 
		{
			if(verbose)
				std::cerr << "No URLs currently in stack\n";
			return;
		}
	}
	if(ref == "")
		ref = url;
	std::string* head = &headers[0];
	curl_easy_setopt(ch->getCH(), CURLOPT_URL, url.c_str());
	curl_easy_setopt(ch->getCH(), CURLOPT_POST, 1);
	curl_easy_setopt(ch->getCH(), CURLOPT_POSTFIELDS, params.c_str());
	curl_easy_setopt(ch->getCH(), CURLOPT_HTTPHEADER, head);
	curl_easy_setopt(ch->getCH(), CURLOPT_POSTREDIR, 3);
	int res = curl_easy_perform(ch->getCH());

	if(res != CURLE_OK)
	{
		//std::string strerr(curl_easy_strerror(res));
		std::cerr << "curl_easy_perform() failed.\n";
	}
	curl_easy_setopt(ch->getCH(), CURLOPT_POST, 0);
}

void HTTPBot::setSSL(bool verify, int hostval, std::string certfile)
{
	ch->setSSL(verify, hostval, certfile);
}

void HTTPBot::setProxy(std::string prxy, int port, proxy_type_t ptype, std::string creds)
{
	ch->setProxy(prxy, port, ptype, creds);
}

void HTTPBot::rebuildHandle()
{
	ch->setCH();
}

size_t HTTPBot::WriteCallback(char *wd, size_t size, size_t nmemb, std::string *stream)
{
	size_t actualSize = size*nmemb;
	content.append(wd, actualSize);

	return actualSize;
}