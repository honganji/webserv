#include "../../includes/Config.hpp"

// -------------------private----------------------------------

// Setters
void Config::_setRoutes(std::map<std::string, routeSetting> routes)
{
	for(const auto &pair : routes)
		_routes.emplace(pair.first, pair.second);
}

// Internal functions for parsing
std::string Config::_readStr(const std::string &line)
{
	std::string str;
	for (size_t i = 0; i < line.size(); i++)
	{
		if (line[i] == ';')
			return (str);
		str += line[i];
	}
	return (str);
}

bool Config::_checkLastChar(const std::string &line, char c)
{
	std::string str = _trimSpace(line);
	if (*(line.end() - 1) == c)
		return (true);
	else
		return (false);
}

void Config::_updateRouteMap(const std::string &path, std::istringstream &contentStream)
{
	routeSetting rs;
	std::string line;
	std::string checkLine;
	std::string firstStr;
	std::string secondStr;
	std::getline(contentStream, line);
	std::istringstream lineStream(line);
	lineStream >> firstStr;
	getline(lineStream, secondStr);
	while (!_checkLastChar(line, '}'))
	{
		checkLine = _trimSpace(line);
		if (!checkLine.empty())
			for (const auto &pair : _routeSetterMap)
			{
				if (pair.first == firstStr)
				{
					pair.second(_readStr(secondStr), rs);
					break;
				}
			}
		std::getline(contentStream, line);
		std::istringstream lineStream(line);
		lineStream >> firstStr;
		getline(lineStream, secondStr);
	}
	_routes[path] = rs;
}

void Config::_updateValues(std::istringstream &contentStream)
{
	std::string line;

	while (std::getline(contentStream, line))
	{
		std::istringstream lineStream(line);
		std::string propKey;
		std::string propValue;
		std::string str;
		lineStream >> propKey;
		std::getline(lineStream, propValue);
		for (const auto &pair: _setterMap)
		{
			if (pair.first == propKey)
			{
				pair.second(_readStr(propValue));
				break ;
			}
		}
		// check for route setting
		if (propKey == "location")
		{
			std::istringstream lineStream(propValue);
			lineStream >> propValue;
			_updateRouteMap(propValue, contentStream);
		}
	}
}

std::string Config::_trimSpace(const std::string &str)
{
	std::string::const_iterator start = str.begin();
	while (start != str.end() && std::isspace(*start))
		start++;

	std::string::const_iterator end = str.end();
	while (std::distance(start, end) > 0 && std::isspace(*start))
		end--;
	
	return (std::string(start, end));
}

bool Config::_parseConfig(const std::string &content)
{
	std::istringstream contentStream(content);
	std::string line;
	std::getline(contentStream, line);
	// check if the content is surrounded by curly brace for the first line
	if (!_checkLastChar(line, '{'))
	{
		std::cerr << "The file is not surrounded by curly braces" << std::endl;
		exit(1);
	}
	// read each property and update values
	_updateValues(contentStream);

	return (true);
}

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

// -------------------public----------------------------------
Config::Config(const std::string &fileName) :
_port(1),
_host("default_host"),
_serverName("default_name"),
_errPage("default_path"),
_bodySize(0),
_routes({})
{
	// initialize setter map
	_setterMap["listen"] = [this](const std::string &value) {_port = std::stoi(value);};
	_setterMap["host"] = [this](const std::string &value) {_host = value;};
	_setterMap["server_name"] = [this](const std::string &value) {_serverName = value;};
	_setterMap["error_page"] = [this](const std::string &value) {_errPage = value;};
	_setterMap["client_max_body_size"] = [this](const std::string &value) {_bodySize = std::stoul(value);};

	// initialize route setter map
	_routeSetterMap["allow_methods"] = [this](const std::string &str, routeSetting &rs) { _allowMethFn(str, rs); };
	_routeSetterMap["return"] = [this](const std::string &str, routeSetting &rs) { _returnFn(str, rs); };
	_routeSetterMap["root"] = [this](const std::string &str, routeSetting &rs) { _rootFn(str, rs); };
	_routeSetterMap["autoindex"] = [this](const std::string &str, routeSetting &rs) { _autoIndexFn(str, rs); };
	_routeSetterMap["index"] = [this](const std::string &str, routeSetting &rs) { _indexFn(str, rs); };
	_routeSetterMap["cgi_ext"] = [this](const std::string &str, routeSetting &rs) { _cgiExtFn(str, rs); };
	_routeSetterMap["cgi_path"] = [this](const std::string &str, routeSetting &rs) { _cgiPathFn(str, rs); };

	// read the content of file
	std::string content;
	std::string path;
  std::stringstream buffer;

	path = "configs/" + fileName;
	std::ifstream fileStream(path);
	if (!fileStream.is_open())
	{
			std::cerr << "Failed to open file: " << path << std::endl;
			exit (1);
	}
	buffer << fileStream.rdbuf();
	content = buffer.str();

	// parse configuration file and update each variables
	if (!_parseConfig(content))
	{
		std::cerr << "There is something wrong in configuration file..." << std::endl;
		exit(1);
	}
}

Config::Config(const Config &ref)
{
	_setRoutes(ref._routes);
}
 
Config &Config::operator=(const Config &ref)
{
	if (this != &ref)
		_setRoutes(ref._routes);
	return (*this);
}

Config::~Config(void) {}

// Getters
unsigned int Config::getPort(void) const
{
	return (_port);
}

const std::string Config::getHost(void) const
{
	return (_host);
}

const std::string Config::getErrPage(void) const
{
	return (_errPage);
}

unsigned int Config::getBodySize(void) const
{
	return (_bodySize);
}

const std::map<std::string, Config::routeSetting> Config::getRoutes(void) const
{
	return (_routes);
}

// test to check if the content is stored correctly
void Config::test(void)
{
	// Test for settings
	std::cout << "\e[33mSetting test" << std::endl;
	std::cout << "===============================\e[0m" << std::endl;
	std::cout << "port: " << getPort() << std::endl;
	std::cout << "host: " << getHost() << std::endl;
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
