/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NewClientConnexionHandler.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/05 10:42:38 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NewClientConnexionHandler.hpp"

namespace NewClientConnexionHandler
{
	static int getClientFd(int serverFd, struct sockaddr_in *clientAddr)
	{
		socklen_t clientAddrLen = sizeof(clientAddr);
		int clientFd = accept(serverFd, (struct sockaddr *)clientAddr, &clientAddrLen);
		if (clientFd == -1)
			throw runtime_error("accept: " + string(strerror(errno)));
		return clientFd;
	}

	static string getClientIpAsString(struct sockaddr_in *clientAddr)
	{
		int ip = ntohl(clientAddr->sin_addr.s_addr);
		return to_string(ip >> 24 & 0xFF) + "." + to_string(ip >> 16 & 0xFF) + "." + to_string(ip >> 8 & 0xFF) + "." + to_string(ip & 0xFF);
	}

	void newClientConnexion(int serverFd, map<int, ClientConnexion *> &clientConnexions, vector<pollfd> &pollfds, const map<int, Config> &configs)
	{
		struct sockaddr_in clientAddr;
		int clientFd = getClientFd(serverFd, &clientAddr);
		string clientIp = getClientIpAsString(&clientAddr);
		ClientConnexion *clientConnexion = new ClientConnexion(clientFd, serverFd, clientIp, &configs.at(serverFd));
		pollfds.push_back({clientFd, POLLIN, 0});
		clientConnexions[clientFd] = clientConnexion;
	}

};
