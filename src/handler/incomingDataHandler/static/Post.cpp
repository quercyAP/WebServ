/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 09:20:54 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/05 11:39:30 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Post.hpp"

namespace Post
{
	int handler(ClientConnexion &clientConnexion, pollfd &pollfd, Server &server, int pollIndex)
	{
		clientConnexion.setTimeUploadBegan(time(nullptr));
		if (clientConnexion.getRequest().headers.find("Content-Length") != clientConnexion.getRequest().headers.end())
		{
			const long int uploadSize = strtol(clientConnexion.getRequest().headers.at("Content-Length").c_str(), nullptr, 10);
			const long int maxSize = clientConnexion.getRequest().route->getMaxBody().empty() ? LONG_MAX : strtol(clientConnexion.getRequest().route->getMaxBody().c_str(), nullptr, 10);
			if (uploadSize > 0L && uploadSize < LONG_MAX && uploadSize > LONG_MIN)
			{
				if (uploadSize > maxSize)
				{
					clientConnexion.setHeaderStream(Response::header413(clientConnexion));
					pollfd.events = POLLOUT;
					return 1;
				}
				else
				{
					const string &uploadDir = clientConnexion.getRequest().route->getUploadStore();
					string uploadName(to_string(time(0)));
					try
					{
						uploadName.append("-" + clientConnexion.getRequest().headers.at("Filename"));
					}
					catch (const std::out_of_range &oor)
					{
						if (STATIC_DEBUG)
							cout << "POST 'Filename' not found in header" << endl;
					}
					clientConnexion.setUploadName(uploadName);
					ofstream *file = new ofstream(uploadDir + "/" + uploadName, ios::out | ios::binary | ios::trunc);
					if (!file->is_open())
					{
						clientConnexion.setHeaderStream(Response::header500(clientConnexion, "Error creating file"));
						pollfd.events = POLLOUT;
						return 1;
					}
					clientConnexion.setUploadStream(file);
					clientConnexion.setReadyToUpload(true);
					clientConnexion.setKeepAlive(true);
					clientConnexion.setTimeUploadBegan(time(nullptr));
					clientConnexion.setHeaderStream(Response::header100());
					pollfd.events |= POLLOUT;
					return 0;
				}
			}
			else
			{
				clientConnexion.setHeaderStream(Response::header400(clientConnexion, "Content-Length is invalid"));
				pollfd.events = POLLOUT;
				return 1;
			}
		}
		else
		{
			clientConnexion.setHeaderStream(Response::header411(clientConnexion));
			pollfd.events = POLLOUT;
			return 1;
		}
		return 0;
	}
}
