/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 09:52:39 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/05 12:08:37 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"
#include "Config.hpp"
#include <iostream>
#include <exception>
#include <string>
#include "define.hpp"
#include <csignal>
#include "Server.hpp"

using namespace std;

Server* g_server = nullptr;

void signalHandler(int signum) {
    if (g_server != nullptr) {
        delete g_server;
        g_server = nullptr;
    }
    exit(signum);
}

int main(int argc, char **argv)
{
	cout << CYAN << "Welcome to " << GREEN << "webserv" << CYAN << " !" << CLEAR << endl;
	cout << CYAN << "Version: " << GREEN << "1.0" << CLEAR << endl;
	cout << CYAN << "Debug Mode: " << GREEN << (WEBDEBUG ? "Yes" : "No") << CLEAR << endl;
	cout << CYAN << "Parsing config file..." << CLEAR << endl;

    signal(SIGINT, signalHandler);
	signal(SIGPIPE, SIG_IGN);
	try
	{
		if (argc > 2)
			throw std::invalid_argument("Too many arguments");
		else if (argc == 1)
		{
			ConfParser confParser("./asset/webserv.conf");
			vector<Config> serverConfigs = confParser.setServerConfig();
			confParser.printConf(serverConfigs);
			Server *server = new Server(serverConfigs);
			g_server = server;
			server->run();
		}
		else if (argc == 2)
		{
			ConfParser confParser(argv[1]);
			vector<Config> serverConfigs = confParser.setServerConfig();
			confParser.printConf(serverConfigs);
			Server *server = new Server(serverConfigs);
			g_server = server;
			server->run();
		}
	}
	catch (exception &e)
	{
		cout << e.what() << endl;
	}

	return (0);
}
