/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticUploadHandler.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/09 08:31:50 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StaticUploadHandler.hpp"

namespace StaticUploadHandler
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
			if (STATIC_DEBUG)
				cout << "error code staticUploadHandler: READ_ERROR: " << endl;
			return READ_ERROR;
		}
		else if (ret == 0)
		{
			if (STATIC_DEBUG)
				cout << "staticUploadHandler: FINISHED: " << endl;
			return FINISHED;
		}
		else
		{
			clientConnexion.incBodyByteRead(ret);
			cout << "byte received: " << clientConnexion.getBodyByteRead() << endl;
			// *clientConnexion.getUploadStream() << buffer;
			if (clientConnexion.getUploadStream()->write(buffer, ret).bad())
			{
				if (STATIC_DEBUG)
					cout << "error code staticUploadHandler: READ_ERROR: " << endl;
				return UPLOAD_STREAM_ERROR;
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

	int handler(ClientConnexion &clientConnexion, pollfd &pollfd, Server &server, int pollIndex)
	{
		switch (read(pollfd.fd, clientConnexion))
		{
		case CONTINUE:
			clientConnexion.setTimeUploadBegan(time(nullptr));
			return 1;
		case READ_ERROR:
			if (ClientConnexion::closeCon())
				server.closeClientConnexion(pollIndex);
			return 1;
		case UPLOAD_STREAM_ERROR:
			clientConnexion.setHeaderStream(Response::header500(clientConnexion, "Error uploading file: filestream"));
			clientConnexion.setKeepAlive(false);
			pollfd.events = POLLOUT;
			return 1;
		case FINISHED:
			cout << "total byte received: " << clientConnexion.getBodyByteRead() << endl;
			clientConnexion.setReadyToUpload(false);
			((ofstream *)(clientConnexion.getUploadStream()))->close();
			clientConnexion.setHeaderStream(Response::header201(clientConnexion.getUploadName()));
			clientConnexion.setKeepAlive(false);
			clientConnexion.setIsUploaded(true);
			pollfd.events = POLLOUT;
			return 1;
		default:
			break;
		}
		return 0;
	}
}
