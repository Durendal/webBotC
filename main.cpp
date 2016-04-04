#include <iostream>
#include "HTTPBot.h"
#include <string>
#include <vector>

int main(int argc, char* argv[])
{

	std::cout << "Hello\n";
	HTTPBot* Bot = NULL;
	Bot = new HTTPBot();
	Bot->pushURL("http://www.google.ca");
	std::cout << sizeof(Bot) << std::endl;
	Bot->requestGET();
	return 0;
	std::cout << "Data: " << Bot->getData() << std::endl;	
	std::cout << "Goodbye\n";
	if(Bot != NULL)
	{
		delete Bot;
		Bot = NULL;
	}

	return 0;
}