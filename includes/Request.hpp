#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <any>

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

		routeSetting(std::vector<Method> m) : method(m) {};
	};

	private:
		int _port;
		std::string _host;
		std::string _serverName;
		std::string _errPage;
		unsigned int _bodySize;
		std::map<std::string, routeSetting> _routes;
		std::map<std::string, std::function<void(const std::string &)>> _setterMap;
		void _setRoutes(std::map<std::string, routeSetting> routes);
		std::string _readStr(const std::string &line);
		void _checkCurlyBrace(const std::string &line);
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
