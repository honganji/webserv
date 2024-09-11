#include "../../includes/Config.hpp"

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

	while (std::getline(contentStream, line) && !_checkLastChar(line, '}'))
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
	std::cout << line << std::endl;
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
