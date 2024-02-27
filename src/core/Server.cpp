/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/05 12:02:28 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "NewClientConnexionHandler.hpp"
#include "IncomingDataHandler.hpp"
#include "OutgoingDataHandler.hpp"

#pragma region[constructor]

Server::Server(const vector<Config> &confs)
{
	for (int i = 0; i < confs.size(); i++)
	{
		_serverFds.push_back(setSocketFd(confs[i].host_ip, confs[i].port));
		struct pollfd serverPollFd;
		serverPollFd.fd = _serverFds.back();
		serverPollFd.events = POLLIN;
		_configs[_serverFds.back()] = confs[i];
		_pollFds.push_back(serverPollFd);
	}
}
Server::~Server()
{
	for (int i = 0; i < _serverFds.size(); i++)
	{
		close(_serverFds[i]);
	}
	for(map<int, ClientConnexion*>::iterator it = _connexions.begin(); it != _connexions.end(); ++it)
	{
		delete it->second;
	}
}
#pragma endregion
#pragma region[method]
void Server::run()
{
	_isRunning = true;
	while (_isRunning)
	{
		if (WEBDEBUG)
			cout << "waiting on poll.." << endl;

		if (poll(&_pollFds[0], _pollFds.size(), -1) == -1)
			throw _serverError("poll");
		if (WEBDEBUG)
			cout << "poll passed with: " << endl;
		for (size_t i = 0; i < _pollFds.size(); i++)
		{
			int fd = _pollFds[i].fd;
			if (_pollFds[i].revents & POLLIN) // si il y a des donnees a lire
			{
				// on check si fd est un fd de server? yes -> newClientConnexion
				if (find(_serverFds.begin(), _serverFds.end(), fd) != _serverFds.end())
					NewClientConnexionHandler::newClientConnexion(fd, _connexions, _pollFds, _configs);
				// sinon ca veut dire que le fd est un fd de client? yes -> handleIncoming
				else
					IncomingDataHandler::incomingData(*_connexions[fd], _pollFds[i], *this, i);
			}
			// sinon si on a des donnees a ecrire
			else if (_pollFds[i].revents & POLLOUT)
			{
				if (WEBDEBUG)
					cout << YELLOW << "outgoing data..." << CLEAR << endl;
				OutgoingDataHandler::outgoingData(*_connexions[fd], _pollFds[i], *this, i);
			}
			else if (_pollFds[i].revents & POLLHUP || _pollFds[i].revents & POLLERR || _pollFds[i].revents & POLLNVAL)
			{
				closeClientConnexion(i);
			}
		}
	}
}
void Server::stop()
{
	_isRunning = false;
}
int Server::setSocketFd(const string &hostIp, unsigned int port)
{
	int socketFd;
	int option = 1;
	if ((socketFd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		throw _serverError("socket");
	if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)))
		throw _serverError("setsockopt");

	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(hostIp.c_str(), NULL, &hints, &res) != 0)
		throw _serverError("getaddrinfo");

	struct sockaddr_in *addr_in = (struct sockaddr_in *)res->ai_addr;

	struct sockaddr_in socketAddrIn;
	memset(&socketAddrIn, 0, sizeof(socketAddrIn));
	socketAddrIn.sin_family = AF_INET;
	socketAddrIn.sin_port = htons(port);
	socketAddrIn.sin_addr = addr_in->sin_addr;
	freeaddrinfo(res);

	if (bind(socketFd, (struct sockaddr *)&socketAddrIn, sizeof(socketAddrIn)))
		throw _serverError("bind");

	if (listen(socketFd, 5))
		throw _serverError("listen");
	cout << CYAN << "Server listenning on Host: " << GREEN << hostIp << CYAN << " Port: " << GREEN << port << CLEAR << endl;

	if (fcntl(socketFd, F_SETFL, O_NONBLOCK))
		throw _serverError("fcntl");
	return socketFd;
}

void Server::closeClientConnexion(int pollIndex)
{
	ClientConnexion *clientConnexion = _connexions[_pollFds[pollIndex].fd];
	close(_pollFds[pollIndex].fd);
	close(clientConnexion->getCgi().pipeIn[READ]);
	close(clientConnexion->getCgi().pipeIn[WRITE]);
	close(clientConnexion->getCgi().pipeOut[READ]);
	close(clientConnexion->getCgi().pipeOut[WRITE]);
	close(clientConnexion->getCgi().pipeErr[WRITE]);
	close(clientConnexion->getCgi().pipeErr[READ]);

	if (clientConnexion->getHeaderStream() != nullptr)
		delete clientConnexion->getHeaderStream();
	if (clientConnexion->getUploadStream() != nullptr)
		delete clientConnexion->getUploadStream();
	if (clientConnexion->getDownloadStream() != nullptr)
		delete clientConnexion->getDownloadStream();
	delete clientConnexion;
	_connexions.erase(_pollFds[pollIndex].fd);
	_pollFds.erase(_pollFds.begin() + pollIndex);
}

runtime_error Server::_serverError(string what)
{
	return (runtime_error("Server error : " + what + " : " + static_cast<string>(strerror(errno))));
}
#pragma endregion
