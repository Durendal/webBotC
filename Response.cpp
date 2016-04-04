#include <iostream>
#include <string>
#include <curl/curl.h>
#include <vector>
#include <sstream>
#include "Response.h"
#include <stdlib.h>
#include <functional>
#include <time.h>

Response::Response(std::string curlData)
{
	std::istringstream data(curlData);
	std::string iLine;
	std::string statusStr;
	headers.clear();
	status = 0;
	
	// Grab Headers First
	while(getline(data, iLine))
	{
		if(iLine == "\r\n\r\n")
			break;
		headers.push_back(iLine);
	}

	// parse content
	while(getline(data, iLine))
		content += iLine;
	for (std::vector<std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
    	if((*it).find("HTTP/1.1") != std::string::npos)
    	{
    		statusStr = (*it).substr(9, 3);
    		status = atoi(statusStr.c_str());
    		break;
    	}
    if(!status)
    {
    	std::cout << "Error parsing status code!\n";
    	exit(1);
    }
	setUID();	
}

void Response::setUID()
{
	std::istringstream convert;
	std::hash<std::string> str_hash;
	std::time_t timeNow = std::time(NULL);
    convert >> timeNow;
    content += convert.str();
    uid = str_hash(content);
}

