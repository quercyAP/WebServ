/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PythonInCgiHandler.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/09 08:33:07 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PythonInCgiHandler.hpp"
#include <sys/ioctl.h>
namespace PythonInCgiHandler
{

	enum ErrorCode
	{
		READ_OK,
		READ_ERROR,
		UPLOAD_STREAM_ERROR,
		FINISHED,
		CONTINUE
	};
	ErrorCode read(int fd, ClientConnexion &clientConnexion)
	{

		const int bufSize = 131072;
		char buffer[bufSize];

		int ret = recv(fd, buffer, bufSize, MSG_DONTWAIT);
		if (ret == -1)
		{
			if (CGI_DEBUG)
				cout << "error code cgi in Handler: READ_ERROR: " << endl;
			return READ_ERROR;
		}
		else if (ret == 0)
		{
			if (CGI_DEBUG)
				cout << " cgi in Handler: FINISHED: " << endl;

			return FINISHED;
		}
		else
		{
			clientConnexion.incBodyByteRead(ret);
			cout << "cgi In byte Received: " << clientConnexion.getBodyByteRead() << endl;

		
			signal(SIGPIPE, SIG_IGN);
			int w_ret = write(clientConnexion.getCgi().pipeIn[WRITE], buffer, ret);
			if (w_ret != -1)
			{
				clientConnexion.setCgiUploadByteRead(clientConnexion.getCgiUploadByteRead() + w_ret);
				cout << "cgi upload byte wrote to python stdin buffer: " << clientConnexion.getCgiUploadByteRead() << endl;
			}

		

			if (clientConnexion.getBodyByteRead() >= strtol(clientConnexion.getRequest().headers.at("Content-Length").c_str(), nullptr, 10))
			{
				return FINISHED;
			}
			else
			{
				return CONTINUE;
			}
		}
	}

	int handler(ClientConnexion &clientConnexion, pollfd &pollFd, Server &server, int pollIndex)
	{
		switch (read(pollFd.fd, clientConnexion))
		{
		case CONTINUE:
			clientConnexion.setCgiTimeAsForked(time(nullptr));
			return 1;
		case READ_ERROR:
			if(ClientConnexion::closeCon())
				server.closeClientConnexion(pollIndex);
			return 1;
		case UPLOAD_STREAM_ERROR:
			clientConnexion.setHeaderStream(Response::header500(clientConnexion, "Error uploading file: filestream"));
			clientConnexion.setKeepAlive(false);
			pollFd.events = POLLOUT;
			return 1;
		case FINISHED:
			cout << "finished byteRead: " << clientConnexion.getBodyByteRead() << endl;
			clientConnexion.setReadyToUpload(false);
			close(clientConnexion.getCgi().pipeIn[WRITE]);
			clientConnexion.setKeepAlive(false);
			clientConnexion.setCgiTimeAsForked(time(nullptr));
			pollFd.events = POLLOUT;
			return 1;
		default:
			break;
		}
		return 0;
	}

}
