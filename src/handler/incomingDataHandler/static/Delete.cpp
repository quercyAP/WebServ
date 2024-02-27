/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 09:20:48 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/19 14:15:46 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Delete.hpp"
#include "CheckFile.hpp"

namespace Delete
{
	int handler(ClientConnexion &clientConnexion, pollfd &pollfd, Server &server, int pollIndex) 
	{
		string path = clientConnexion.getRequest().route->getRoot() + clientConnexion.getRequest().pathInfo;
		cout << YELLOW << "Delete " << path << CLEAR << endl;
		if (CheckFile::isFile(path.c_str()))
		{
			if (remove(path.c_str()) != 0)
				clientConnexion.setHeaderStream(Response::header500(clientConnexion, "Delete Failed"));
			else
				clientConnexion.setHeaderStream(Response::header204());
		}
		else if (CheckFile::isDir(path.c_str()))
		{
			if (CheckFile::removeDirectory(path.c_str()) == false)
				clientConnexion.setHeaderStream(Response::header500(clientConnexion, "Delete Failed"));
			else
				clientConnexion.setHeaderStream(Response::header204());
		}
		else
		{
			clientConnexion.setHeaderStream(Response::header404(clientConnexion));
		}
		pollfd.events = POLLOUT;
		clientConnexion.setStaticMethodIsHandled(true);
		return 0;
	}
}
