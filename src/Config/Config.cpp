#include "../../includes/Config.hpp"

Config::Config(const std::string &fileName) :
_port(1),
_host("default_host"),
_serverName("default_name"),
_errPage("default_path"),
_bodySize(0),
_routes({})
{
	// initialize setter map
	_setterMap["listen"] = [this](const std::string &value) {_port = std::stoi(_trimSpace(value));};
	_setterMap["host"] = [this](const std::string &value) {_host = _trimSpace(value);};
	_setterMap["server_name"] = [this](const std::string &value) {_serverName = _trimSpace(value);};
	_setterMap["error_page"] = [this](const std::string &value) {_errPage = _trimSpace(value);};
	_setterMap["client_max_body_size"] = [this](const std::string &value) {_bodySize = std::stoul(_trimSpace(value));};

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
