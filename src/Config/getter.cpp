#include "../../includes/Config.hpp"

unsigned int Config::getPort(void) const
{
	return (_port);
}

const std::string Config::getHost(void) const
{
	return (_host);
}

const std::string Config::getServerName(void) const
{
	return (_serverName);
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
