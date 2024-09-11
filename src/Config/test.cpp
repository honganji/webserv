#include "../../includes/Config.hpp"

void Config::test(void)
{
	// Test for settings
	std::cout << "\e[33mSetting test" << std::endl;
	std::cout << "===============================\e[0m" << std::endl;
	std::cout << "port: " << getPort() << std::endl;
	std::cout << "host: " << getHost() << std::endl;
	std::cout << "server name: " << getServerName() << std::endl;
	std::cout << "err page: " << getErrPage() << std::endl;
	std::cout << "body size: " << getBodySize() << std::endl;
	std::cout << "routes info:" << std::endl;
	std::cout << "\e[33m===============================\e[0m" << std::endl;
	
	std::cout << std::endl;

	// Test for route settings
	std::cout << "\e[31mRoute setting" << std::endl;
	std::cout << "===============================\e[0m" << std::endl;

	for (auto const &pairs: _routes)
	{
		// key path
		std::cout << "key path : " << pairs.first << std::endl;

		// redirection path
		std::cout << "redirection path: " << pairs.second.redPath << std::endl;

		// root path
		std::cout << "root path: " << pairs.second.rootPath << std::endl;

		// allowed paths
		if (pairs.second.method.size() == 0)
			std::cout << "No method is allowed...";
		for (size_t i = 0; i < pairs.second.method.size(); i++)
		{
			switch (pairs.second.method[i])
			{
				case GET:
					std::cout << "GET";
					break;

				case POST:
					std::cout << "POST";
					break;

				case DELETE:
					std::cout << "DELETE";
					break;
				
				default:
					break;
			}
			if (i != pairs.second.method.size() - 1)
				std::cout << ", ";
		}
		std::cout << std::endl;

		// ListingOn
		std::cout << "listing: " << (pairs.second.isListingOn ? "on" : "off") << std::endl;

		// Default file
		std::cout << "default file: " << pairs.second.defaultFile << std::endl;

		// CGI extensions
		std::cout << "extensions: ";
		if (pairs.second.cgiExtensions.size() == 0)
		std::cout << "none";
		for (size_t i = 0; i < pairs.second.cgiExtensions.size(); i++)
		{
			std::cout << "\"" << pairs.second.cgiExtensions[i] << "\"";
			if (i != pairs.second.cgiExtensions.size() - 1)
				std::cout << ", ";
		}
		std::cout << std::endl;

		// CGI paths
		std::cout << "paths: ";
		if (pairs.second.cgiPaths.size() == 0)
		std::cout << "none";
		for (size_t i = 0; i < pairs.second.cgiPaths.size(); i++)
		{
			std::cout << "\"" << pairs.second.cgiPaths[i] << "\"";
			if (i != pairs.second.cgiPaths.size() - 1)
				std::cout << ", ";
		}
		std::cout << std::endl;
		if (&pairs != &(*(std::prev(_routes.end()))))
			std::cout << std::endl;
	}
	
	std::cout << "\e[31m===============================\e[0m" << std::endl;
}
