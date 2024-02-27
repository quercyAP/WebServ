/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/08 14:16:31 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <exception>
#include <cstring>
#include <string>
#include "Config.hpp"
#include <vector>
#include <poll.h>
#include <map>
#include "Route.hpp"
#include "define.hpp"
#include "ClientConnexion.hpp"

using namespace std;

class Server
{
private:
	bool _isRunning;
	vector<pollfd> _pollFds;
	vector<int> _serverFds;
	map<int, ClientConnexion*> _connexions;
	runtime_error _serverError(string what);
	map<int, Config> _configs; 

	Server();
    int setSocketFd(const string &hostIp, unsigned int port);

public:
	Server(const vector<Config> &confs);
	~Server();
	void run();
	void stop();
	void closeClientConnexion(int pollIndex);
};

#endif
