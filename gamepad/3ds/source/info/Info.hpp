#ifndef INFO_HPP
#define INFO_HPP

#include <string>
#include <unordered_map>

enum STATUS
{
	NOT_CONNECTED,
	CONNECTING,
	CONNECTED
};

static std::unordered_map<STATUS, std::string> message({
	{NOT_CONNECTED, "\x1b[31mNot connected"},
	{CONNECTING, 	"\x1b[33mConnecting..."},
	{CONNECTED, 	"\x1b[32mConnected    "}
	});

class Info
{
	public:
		Info();

		void display();

		void setIp(std::string s);
		void setId(std::string s);
		void setStatus(STATUS s);

		std::string getIp();
		std::string getId();

	private:
		std::string _ip = "0.0.0.0";
		std::string _id = "00000";
		STATUS _status = NOT_CONNECTED;
};

#endif // INFO_HPP