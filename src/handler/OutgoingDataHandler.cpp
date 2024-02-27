/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutgoingDataHandler.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:57:01 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/05 10:43:42 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OutgoingDataHandler.hpp"
#include "CgiCrashHandler.hpp"

void OutgoingDataHandler::outgoingData(ClientConnexion &clientConnexion, pollfd &pollFd, Server &server, int pollIndex)
{
	// #ifdef WEBDEBUG
	// 	cout << YELLOW << "*****RESPONSE*****" << CLEAR << endl;
	// #endif
	const int buffSize = 16384;
	char buffer[buffSize];
	if (clientConnexion.getCgi().isInit == false && clientConnexion.getRequest().method == ClientConnexion::POST &&
		clientConnexion.getRequest().route->POST_allowed() && !clientConnexion.isUploaded() &&
		difftime(time(nullptr), clientConnexion.getTimeUploadBegan()) > 10)
	{
		cout << "upload timeout" << endl;
		clientConnexion.setHeaderStream(Response::header408(clientConnexion));
		clientConnexion.setKeepAlive(false);
	}
	if (!clientConnexion.isCgiFinish() && clientConnexion.getCgi().isInit == true && !clientConnexion.isCgiContinueToSend())
	{

		// cout << CYAN << "parsing cgi response" << CLEAR << endl;

		if (PythonOutCgiHandler::handler(clientConnexion, pollFd, server, pollIndex))
			return;
	}
	clientConnexion.setCgiContinueToSend(false);
	if (clientConnexion.getHeaderStream() != nullptr)
	{
		clientConnexion.getHeaderStream()->read(buffer, buffSize);
		int ret_read_header = clientConnexion.getHeaderStream()->gcount();
		if (ret_read_header > 0)
		{
			if (send(pollFd.fd, buffer, ret_read_header, 0) < 0)
			{
				if (OUT_DEBUG)
					cout << "send error " << strerror(errno) << endl;
				server.closeClientConnexion(pollIndex);
			}
		}
		else
		{
			if (clientConnexion.getDownloadStream() != nullptr)
			{
				if (OUT_DEBUG)
					cout << YELLOW << "sending body" << CLEAR << endl;
				char buffer[buffSize];
				int ret_read_body;

				clientConnexion.getDownloadStream()->read(buffer, buffSize);
				ret_read_body = clientConnexion.getDownloadStream()->gcount();

				if (ret_read_body > 0)
				{
					if (send(pollFd.fd, buffer, ret_read_body, 0) < 0)
					{
						if (OUT_DEBUG)
							cout << "send error " << strerror(errno) << endl;
						server.closeClientConnexion(pollIndex);
					}
				}
				else
				{
					server.closeClientConnexion(pollIndex);
				}
			}
			else
			{
				if (!clientConnexion.isKeepAlive())
					server.closeClientConnexion(pollIndex);
			}
		}
	}
	else
	{
		server.closeClientConnexion(pollIndex);
	}
}
