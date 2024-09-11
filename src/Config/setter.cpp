#include "../../includes/Config.hpp"

// basic setting
void Config::_errPageFn(const std::string &str)
{
	std::istringstream lineStream(str);
	std::string firstStr;
	std::string secondStr;
	lineStream >> firstStr;
	getline(lineStream, secondStr);
	_errPage[std::stoul(firstStr)] = secondStr;
}

// route setting
void Config::_allowMethFn(const std::string &str, routeSetting &rs)
{
	std::map<std::string, Method> methodMap = {
			{"POST", POST},
			{"DELETE", DELETE},
			{"GET", GET}
		};
		std::string line;
		std::string currentStr;
		std::istringstream contentStream(str);
		std::getline(contentStream, line);
		std::istringstream lineStream(line);
		while (lineStream >> currentStr)
		{
			for (auto const &pair : methodMap)
				if (pair.first == currentStr)
					rs.method.push_back(pair.second);
		}
}

void Config::_returnFn(const std::string &str, routeSetting &rs)
{
	std::regex pathRegex("^(/|(\\./)|(\\.\\./))?(([\\w.-]+/)*)([\\w.-]+)?(/)?(\\.[\\w]+)?$");
	std::string newString;
	newString = _trimSpace(str);
	if (!std::regex_match(newString, pathRegex))
	{
		std::cerr << newString << " is not valid path..." << std::endl;
		exit(1);
	}
	rs.redPath = newString;
}

void Config::_rootFn(const std::string &str, routeSetting &rs)
{
	std::regex pathRegex("(^(/|(\\./)|(\\.\\./))?(([\\w.-]+/)*)([\\w.-]+)?(/)?(\\.[\\w]+)?$)");
	std::string newString;
	newString = _trimSpace(str);
	if (!std::regex_match(newString, pathRegex))
	{
		std::cerr << newString << " is not valid path..." << std::endl;
		exit(1);
	}
	rs.rootPath = newString;
}

void Config::_autoIndexFn(const std::string &str, routeSetting &rs)
{
	std::string newString;
	newString = _trimSpace(str);
	if (newString == "on")
		rs.isListingOn = true;
}

void Config::_indexFn(const std::string &str, routeSetting &rs)
{
	std::string newString;
	newString = _trimSpace(str);
	rs.defaultFile = newString;
}

void Config::_cgiExtFn(const std::string &str, routeSetting &rs)
{
	std::string line;
	std::string currentStr;
	std::istringstream contentStream(str);
	std::getline(contentStream, line);
	std::istringstream lineStream(line);
	std::regex exRegex("^\\.(\\w+)?$");
	while (lineStream >> currentStr)
	{
		if (!std::regex_match(currentStr, exRegex))
		{
			std::cerr << currentStr << " is not valid extension..." << std::endl;
			exit(1);
		}
		rs.cgiExtensions.push_back(currentStr);
	}
}

void Config::_cgiPathFn(const std::string &str, routeSetting &rs)
{
	std::regex pathRegex("^(/|(\\./)|(\\.\\./))?(([\\w.-]+/)*)([\\w.-]+)?(/)?(\\.[\\w]+)?$");
	std::string line;
	std::string currentStr;
	std::istringstream contentStream(str);
	std::getline(contentStream, line);
	std::istringstream lineStream(line);
	while (lineStream >> currentStr)
	{
		currentStr = _trimSpace(currentStr);
		if (!std::regex_match(currentStr, pathRegex))
		{
			std::cerr << currentStr << " is not valid path..." << std::endl;
			exit(1);
		}
		rs.cgiPaths.push_back(currentStr);
	}
}
