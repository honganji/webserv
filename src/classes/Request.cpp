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

void Request::_checkCurlyBrace(const std::string &line)
{
	for (size_t i = line.length() - 1; i >= 0; i--)
	{
		if (line[i] == ' ' || line[i] == '\n')
			continue;
		if (line[i] != '{')
		{
			std::cerr << "The file is not surrounded by curly braces " << std::endl;
			exit(1);
		}
		else
			break;
	}
}

bool Request::_parseConfig(const std::string &content)
{
	std::istringstream contentStream(content);
	std::string line;
	std::getline(contentStream, line);
	// check if the content is surrounded by curly brace for the first line
	_checkCurlyBrace(line);
	// read each property and update values
	while (std::getline(contentStream, line))
	{
		std::istringstream lineStream(line);
		std::string propKey;
		std::string propValue;
		std::string str;
		lineStream >> propKey;
		lineStream >> propValue;
		for (const auto &pair: _setterMap)
		{
			if (pair.first == propKey)
			{
				pair.second(propValue);
				break ;
			}
		}
	}
	
	
	
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
_routes()
{
	// initialize setter map
	_setterMap["listen"] = [this](const std::string &value) {_port = std::stoi(value);};
	_setterMap["host"] = [this](const std::string &value) {_host = value;};
	_setterMap["server_name"] = [this](const std::string &value) {_serverName = value;};
	_setterMap["error_page"] = [this](const std::string &value) {_errPage = value;};
	_setterMap["client_max_body_size"] = [this](const std::string &value) {_bodySize = std::stoul(value);};
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

	// TODO Parse configuration file and update each variables
	if (!_parseConfig(content))
	{
		std::cerr << "There is something wrong in configuration file..." << std::endl;
		exit(1);
	}
	// Display content
	_routes.emplace("/", routeSetting({POST}));
	std::cout << "port: " << getPort() << std::endl;
	std::cout << "host: " << getHost() << std::endl;
	std::cout << "err page: " << getErrPage() << std::endl;
	std::cout << "body size: " << getBodySize() << std::endl;
	std::cout << "routes info:";
	for(const auto &pair : getRoutes())
		std::cout << (std::find(pair.second.method.begin(), pair.second.method.end(), GET) != pair.second.method.end() ? "Get" : "No") << std::endl;
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
