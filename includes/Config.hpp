#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <any>
#include <regex>

class Config
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
		bool _checkLastChar(const std::string &line, char c);
		void _updateRouteMap(const std::string &path, std::istringstream &contentStream);
		void _updateValues(std::istringstream &contentStream);
		std::string _trimSpace(const std::string &str);
		bool _parseConfig(const std::string &content);
		void _allowMethFn(const std::string &str, routeSetting &rs);
		void _returnFn(const std::string &str, routeSetting &rs);
		void _rootFn(const std::string &str, routeSetting &rs);
		void _autoIndexFn(const std::string &str, routeSetting &rs);
		void _indexFn(const std::string &str, routeSetting &rs);
		void _cgiExtFn(const std::string &str, routeSetting &rs);
		void _cgiPathFn(const std::string &str, routeSetting &rs);
		Config(void);

	public:
		Config(const std::string &fileName);
		Config(const Config &ref);
		Config &operator=(const Config &ref);
		~Config(void);
		unsigned int getPort(void) const;
		const std::string getHost(void) const;
		const std::string getErrPage(void) const;
		unsigned int getBodySize(void) const;
		const std::map<std::string, routeSetting> getRoutes(void) const;
		void test(void);
};
