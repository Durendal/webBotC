#include <curl/curl.h>
#include <vector>
#include <string>
#include <sstream>

#ifndef __CURLH_H__
#define __CURLH_H__

/**
 *		proxy_type_t
 *
 *			The type of proxy being used
 *			Values act as aliases for the cURL values
 *		
 */
enum proxy_type_t
{
	DISABLED_PROXY			= 0x00,
	SOCKS_PROXY 			= 0x07,
	HTTP_PROXY				= CURLPROXY_HTTP
};

/**
 *		cookie_mode_t
 *
 *			The mode for the cookie
 *			COOKIE_MODE_FILE - Store the cookie in a cookiefile
 *			COOKIE_MODE_ARRAY - Store the cookie in a vector
 *			COOKIE_MODE_STRING - Store the cookie in memory
 *		
 */
enum cookie_mode_t
{
	COOKIE_MODE_FILE		= 0x00,
	COOKIE_MODE_ARRAY		= 0x01,
	COOKIE_MODE_STRING		= 0x02
};

/**
 *		proxy_t
 *
 *			basic struct for proxy attributes
 *		
 */
struct proxy_t
{
	std::string address;
	int port;
	proxy_type_t proxyType;
	std::string credentials;
	proxy_t(std::string adr = "", int prt = 0, proxy_type_t ptype = HTTP_PROXY, std::string creds = "") : address(adr), port(prt), proxyType(ptype), credentials(creds) 
	{
			std::ostringstream temp;
			temp << port;
			address += ":" + temp.str();
	}
};

/**
 *		cookie_t
 *
 *			basic struct for cookie attributes
 *		
 */
struct cookie_t
{
	cookie_mode_t mode;
	std::string file;
	std::string contents;

	cookie_t(cookie_mode_t md = COOKIE_MODE_FILE, std::string fname = "cookies.txt", std::string cont = "") : mode(md), file(fname), contents(cont) {}
};

/**
 *		cURLH
 *
 *			Basic cURL Handle object, includes ability to set and modify proxy SSL and cookie settings.
 *
 */
class cURLH
{
	public:

		cURLH( std::string pxy = "", int port = 0, proxy_type_t ptype = HTTP_PROXY, std::string creds = "");				// Proxy Constructor
		~cURLH();																											// Destructor

		CURL* getCH() { return ch; };																						// Return pointer to cURL Handle
		void setCH();																										// Set a cURL Handle
		void setup();																										// Initialize default values to cURL Handle
		void setProxy(std::string pxy = "", int prt = 0, proxy_type_t ptype = DISABLED_PROXY, std::string creds = "");		// Set or unset a proxy
		void setSSL(bool verify = false, int hostval = 0, std::string certfile = "");										// Set SSL Settings on the cURL Handle
		void cleanup();																										// Cleanup the cURL handle and cURL global settings

		void setCookie(std::string filename = "cookies.txt");																// Set the name of the cookie file to use
		void setCookieFile(std::string filename = "");																		// Add the cookie to the cURL handle
		cookie_t getCookie() { return cookies; };

		// Proxy Setters
		void setProxyType(proxy_type_t type = HTTP_PROXY);																	// Set the type of proxy to use
		void setProxyAddress(std::string adr = "", int prt = 0);															// Set the address of the proxy
		void setProxyCredentials(std::string creds = "");																	// Set any credentials for the proxy if necessary
		proxy_t getProxy() { return proxy; };
		CURL* ch;

	private:

		bool verbose;																										// Verbosity level
																															// Pointer to cURL handle
		proxy_t proxy; 																										// Proxy struct if proxy in use
		cookie_t cookies;																									// File to store cookies in
		static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);								// Callback for requests
};

#endif