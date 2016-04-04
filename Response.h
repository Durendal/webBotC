#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <vector>

class Response
{
	public:
		Response(std::string curlData);
		Response() {};

		int getStatus() { return status; };
		std::string getContent() { return content; };
		std::vector<std::string> getHeaders() { return headers; };
		int getUID() { return uid; };

		void setStatus(int stat) { status = stat; };
		void setContent(std::string cont) { content = cont; };
		void setHeaders(std::vector<std::string> head) { headers = head; };
		void setUID();
	private:	
		int status;
		std::string content;
		std::vector<std::string> headers;
		int uid;
};

#endif