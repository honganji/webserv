#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <any>
#include <regex>

class Request
{
	enum Method
	{
		GET,
		POST,
		DELETE
	};

	struct routeSetting
	{
		std::vector<Method> method;
		std::string redPath;
		std::string rootPath;
		bool isListingOn;
		std::string defaultFile;
		std::vector<std::string> cgiExtensions;
		std::vector<std::string> cgiPaths;

		routeSetting() :
		method({}),
		redPath(""),
		rootPath(""),
		isListingOn(false),
		defaultFile(""),
		cgiExtensions({}),
		cgiPaths({})
		{};
	};

	private:
		int _port;
		std::string _host;
		std::string _serverName;
		std::string _errPage;
		unsigned int _bodySize;
		std::map<std::string, routeSetting> _routes;
		std::map<std::string, std::function<void(const std::string &)>> _setterMap;
		std::map<std::string, std::function<void(const std::string &, routeSetting &rs)>> _routeSetterMap;
		void _setRoutes(std::map<std::string, routeSetting> routes);
		std::string _readStr(const std::string &line);
		bool _checkCurlyBrace(const std::string &line);
		void _updateValues(std::istringstream &contentStream);
		void _updateRouteMap(const std::string &path, std::istringstream &contentStream);
		std::string _eraseChar(const std::string &str, char c);
		bool _parseConfig(const std::string &content);
		Request(void);

	public:
		Request(const std::string &fileName);
		Request(const Request &ref);
		Request &operator=(const Request &ref);
		~Request(void);
		unsigned int getPort(void) const;
		const std::string getHost(void) const;
		const std::string getErrPage(void) const;
		unsigned int getBodySize(void) const;
		const std::map<std::string, routeSetting> getRoutes(void) const;
};
