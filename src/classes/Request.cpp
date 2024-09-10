#include "../../includes/Request.hpp"

void Request::_setRoutes(std::map<std::string, routeSetting> routes)
{
	for(const auto &pair : routes)
		_routes.emplace(pair.first, pair.second);
}

std::string Request::_readStr(const std::string &line)
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

bool Request::_checkCurlyBrace(const std::string &line)
{
	for (size_t i = line.length() - 1; i >= 0; i--)
	{
		if (line[i] == ' ' || line[i] == '\n')
			continue;
		if (line[i] != '{')
			return (false);
		else
			break;
	}
	return (true);
}

void Request::_updateRouteMap(const std::string &path, std::istringstream &contentStream)
{
	routeSetting rs;
	std::string line;
	std::string firstStr;
	std::string secondStr;
	std::getline(contentStream, line);
	std::istringstream lineStream(line);
	lineStream >> firstStr;
	getline(lineStream, secondStr);
	// std::regex propRegex("^[\\s\n]*$");
	while (!_checkCurlyBrace(line))
	{
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
		while (!(lineStream >> firstStr))
		{
			std::getline(contentStream, line);
			std::istringstream lineStream(line);
		}
		getline(lineStream, secondStr);
		std::cout << "first string: " << firstStr << std::endl;
		std::cout << "secondStr: " << secondStr << std::endl;
	}
	_routes[path] = rs;
}

void Request::_updateValues(std::istringstream &contentStream)
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

	// Test for route settings
	std::cout << "\e[31mroute setting" << std::endl;
	std::cout << "===============================\e[0m" << std::endl;

	// root path
	std::cout << "\"/tours\" root path: " << _routes["/tours"].rootPath << std::endl;

	// redirection path
	std::cout << "\"/tours\" redirection path: " << _routes["/tours"].redPath << std::endl;

	// allowed paths
	if (_routes["/tours"].method.size() == 0)
		std::cout << "No method is allowed..." << std::endl;
	for (size_t i = 0; i < _routes["/tours"].method.size(); i++)
	{
		switch (_routes["/tours"].method[i])
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
		if (i != _routes["/tours"].method.size() - 1)
			std::cout << ", ";
	}
	std::cout << std::endl;

	// ListingOn
	std::cout << "listing: " << (_routes["/tours"].isListingOn ? "on" : "off") << std::endl;

	// Default file
	std::cout << "default file: " << _routes["/tours"].defaultFile << std::endl;

	// CGI extensions
	std::cout << "extensions: ";
	if (_routes["/tours"].cgiExtensions.size() == 0)
	std::cout << "none";
	for (size_t i = 0; i < _routes["/tours"].cgiExtensions.size(); i++)
	{
		std::cout << "\"" << _routes["/tours"].cgiExtensions[i] << "\"";
		if (i != _routes["/tours"].cgiExtensions.size() - 1)
			std::cout << ", ";
	}
	std::cout << std::endl;

	// CGI paths
	std::cout << "paths: ";
	if (_routes["/tours"].cgiPaths.size() == 0)
	std::cout << "none";
	for (size_t i = 0; i < _routes["/tours"].cgiPaths.size(); i++)
	{
		std::cout << "\"" << _routes["/tours"].cgiPaths[i] << "\"";
		if (i != _routes["/tours"].cgiPaths.size() - 1)
			std::cout << ", ";
	}
	std::cout << std::endl;
	std::cout << "size of paths: " << _routes["/tours"].cgiPaths.size() << std::endl;
	
	std::cout << "\e[31m===============================\e[0m" << std::endl;
}

std::string Request::_eraseChar(const std::string &str, char c)
{
	size_t start = str.find_first_not_of(c);
	std::string newString = str;
	if (start != std::string::npos)
		newString = str.substr(start);
	size_t end = str.find_last_not_of(c);
	if (end != std::string::npos)
		newString = newString.substr(0, end + 1);
	return (newString);
}

bool Request::_parseConfig(const std::string &content)
{
	std::istringstream contentStream(content);
	std::string line;
	std::getline(contentStream, line);
	// check if the content is surrounded by curly brace for the first line
	if (!_checkCurlyBrace(line))
	{
		std::cerr << "The file is not surrounded by curly braces " << std::endl;
		exit(1);
	}
	// read each property and update values
	_updateValues(contentStream);

	return (true);
}

unsigned int Request::getPort(void) const
{
	return (_port);
}

const std::string Request::getHost(void) const
{
	return (_host);
}

const std::string Request::getErrPage(void) const
{
	return (_errPage);
}

unsigned int Request::getBodySize(void) const
{
	return (_bodySize);
}

const std::map<std::string, Request::routeSetting> Request::getRoutes(void) const
{
	return (_routes);
}

Request::Request(const std::string &fileName) :
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
	_routeSetterMap["allow_methods"] = [](const std::string &str, routeSetting &rs)
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
	};

	_routeSetterMap["return"] = [this](const std::string &str, routeSetting &rs)
	{
		std::regex pathRegex("^(/|(\\./)|(\\.\\./))?(([\\w.-]+/)*)([\\w.-]+)?(/)?(\\.[\\w]+)?$");
		std::string newString;
		newString = _eraseChar(str, ' ');
		if (!std::regex_match(newString, pathRegex))
		{
			std::cerr << newString << " is not valid path..." << std::endl;
			exit(1);
		}
		rs.redPath = newString;
	};

	_routeSetterMap["root"] = [this](const std::string &str, routeSetting &rs)
	{
		std::regex pathRegex("^(/|(\\./)|(\\.\\./))?(([\\w.-]+/)*)([\\w.-]+)?(/)?(\\.[\\w]+)?$");
		std::string newString;
		newString = _eraseChar(str, ' ');
		if (!std::regex_match(newString, pathRegex))
		{
			std::cerr << newString << " is not valid path..." << std::endl;
			exit(1);
		}
		rs.rootPath = newString;
	};

	_routeSetterMap["autoindex"] = [this](const std::string &str, routeSetting &rs)
	{
		std::string newString;
		newString = _eraseChar(str, ' ');
		if (newString == "on")
			rs.isListingOn = true;
	};

	_routeSetterMap["index"] = [this](const std::string &str, routeSetting &rs)
	{
		std::string newString;
		newString = _eraseChar(str, ' ');
		rs.defaultFile = newString;
	};

	_routeSetterMap["cgi_ext"] = [](const std::string &str, routeSetting &rs)
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
	};

	_routeSetterMap["cgi_path"] = [this](const std::string &str, routeSetting &rs)
	{
		std::regex pathRegex("^(/|(\\./)|(\\.\\./))?(([\\w.-]+/)*)([\\w.-]+)?(/)?(\\.[\\w]+)?$");
		std::string line;
		std::string currentStr;
		std::istringstream contentStream(str);
		std::getline(contentStream, line);
		std::istringstream lineStream(line);
		while (lineStream >> currentStr)
		{
			currentStr = _eraseChar(currentStr, ' ');
			if (!std::regex_match(currentStr, pathRegex))
			{
				std::cerr << currentStr << " is not valid path..." << std::endl;
				exit(1);
			}
			rs.cgiPaths.push_back(currentStr);
		}
	};

	// read the content of file
	std::string content;
	std::string path;
  std::stringstream buffer;

	path = "configs/" + fileName;
	std::ifstream fileStream(path);
	if (!fileStream.is_open()) {
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
	// Display content
	// _routes.emplace("/", routeSetting({POST}));
	// Test for settings
	std::cout << std::endl << "\e[33msetting" << std::endl;
	std::cout << "===============================\e[0m" << std::endl;
	std::cout << "port: " << getPort() << std::endl;
	std::cout << "host: " << getHost() << std::endl;
	std::cout << "err page: " << getErrPage() << std::endl;
	std::cout << "body size: " << getBodySize() << std::endl;
	std::cout << "routes info:" << std::endl;
	std::cout << "\e[33m===============================\e[0m" << std::endl;
}

Request::Request(const Request &ref)
{
	_setRoutes(ref._routes);
}
 
Request &Request::operator=(const Request &ref)
{
	if (this != &ref)
		_setRoutes(ref._routes);
	return (*this);
}

Request::~Request(void)
{
	std::cout << "destroy Request instance" << std::endl;
}
