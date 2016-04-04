#include "cURLH.h"
#include <iostream>
#include <curl/curl.h>
#include <sstream>

/**
 *		cURLH()
 *			Constructor class, initializes Cookie file, cURL handle, 
 *
 *
 *
 *
 *
 */
cURLH::cURLH(std::string pxy, int port, proxy_type_t ptype, std::string creds)
{
	verbose		= false;
	ch 			= NULL;
	setCH();
	setCookieFile("cookies.txt");
	//setProxy(pxy, port, ptype, creds);
	//setSSL();
}

/**
 *		~cURLH()
 *
 *			Destructor
 *			Handles cleanup of cURL handle
 *
 */
cURLH::~cURLH()
{
	cleanup();
}
/**
 *
 *
 *
 *
 *
 *
 *
 */
void cURLH::setup()
{
	curl_easy_setopt(getCH(), CURLOPT_SSL_VERIFYPEER, false);
	curl_easy_setopt(getCH(), CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(getCH(), CURLOPT_HEADER, 1);
}

/**
 *
 *
 *
 *
 *
 *
 *
 */
void cURLH::setCH()
{
	cleanup();
	ch = curl_easy_init();
	std::cout << sizeof(getCH()) << " " << sizeof(getCH()) << " " << (ch == getCH() ? "Equal" : "Not Equal") << std::endl;
	setup();
	std::cout << sizeof(getCH()) << " " << sizeof(getCH()) << " " << (ch == getCH() ? "Equal" : "Not Equal") << std::endl;
}

/**
 *
 *
 *
 *
 *
 *
 *
 */
void cURLH::cleanup()
{
	if(ch != NULL)
	{
		curl_easy_cleanup(getCH());
		ch = NULL;
		curl_global_cleanup();
	}
}

/**
 *
 *
 *
 *
 *
 *
 *
 */
void cURLH::setCookie(std::string filename)
{
	cookies.file = filename;
}

/**
 *
 *
 *
 *
 *
 *
 *
 */
void cURLH::setCookieFile(std::string filename)
{
	setCookie(filename);
	curl_easy_setopt(getCH(), CURLOPT_COOKIEJAR, filename.c_str());
	curl_easy_setopt(getCH(), CURLOPT_COOKIEFILE, filename.c_str());
}

/**
 *
 *
 *
 *
 *
 *
 *
 */
void cURLH::setProxyType(proxy_type_t type)
{
	proxy.proxyType = type;
}

/**
 *
 *
 *
 *
 *
 *
 *
 */
void cURLH::setProxyAddress(std::string adr, int prt)
{
	proxy.address = adr;
	proxy.port = prt;
	std::ostringstream temp;
	temp << prt;
	proxy.address += temp.str();
}

/**
 *
 *
 *
 *
 *
 *
 *
 */
void cURLH::setProxyCredentials(std::string creds)
{
	proxy.credentials = creds;
}

/**
 *
 *
 *
 *
 *
 *
 *
 */
void cURLH::setProxy(std::string pxy, int prt, proxy_type_t ptype, std::string creds)
{
	setProxyAddress(pxy, prt);
	setProxyType(ptype);
	setProxyCredentials(creds);

	if(proxy.address != "")
	{
		if(proxy.proxyType == SOCKS_PROXY)
			curl_easy_setopt(getCH(), CURLOPT_PROXYTYPE, 7);
		else
			curl_easy_setopt(getCH(), CURLOPT_PROXYTYPE, CURLPROXY_HTTP);

		curl_easy_setopt(getCH(), CURLOPT_HTTPPROXYTUNNEL, 1);
		curl_easy_setopt(getCH(), CURLOPT_PROXY, proxy.address.c_str());
		if(verbose)
			std::cout << "Using " << ((proxy.proxyType == HTTP_PROXY) ? "HTTP" : "SOCKS") << "Proxy: " << proxy.address;
		
		if(proxy.credentials != "")
		{
			if(verbose)
				std::cout << "Credentials: " << proxy.credentials;
			curl_easy_setopt(getCH(), CURLOPT_PROXYUSERPWD, proxy.credentials.c_str());
		}
		if(verbose)
			std::cout << "\n";
	// Disable Proxy Support if called with no parameters
	} 
	else 
	{
		if(verbose)
			std::cout << "Disabling Proxy.\n";
		curl_easy_setopt(getCH(), CURLOPT_PROXYTYPE, 0);
		curl_easy_setopt(getCH(), CURLOPT_HTTPPROXYTUNNEL, 0);
		curl_easy_setopt(getCH(), CURLOPT_PROXY, 0);
		curl_easy_setopt(getCH(), CURLOPT_PROXYUSERPWD, 0);
		setProxyAddress("", 0);
		setProxyType(DISABLED_PROXY);
		setProxyCredentials("");
	}
}

void cURLH::setSSL(bool verify, int hostval, std::string certfile)
{
	if(verify)
	{
		curl_easy_setopt(getCH(), CURLOPT_SSL_VERIFYPEER, true);
		if(hostval >= 0 && hostval < 3 && certfile != "")
		{
			curl_easy_setopt(getCH(), CURLOPT_SSL_VERIFYHOST, hostval);
			curl_easy_setopt(getCH(), CURLOPT_CAINFO, certfile.c_str());
		}
	} 
	else 
	{
		curl_easy_setopt(getCH(), CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(getCH(), CURLOPT_SSL_VERIFYHOST, false);
	}
}