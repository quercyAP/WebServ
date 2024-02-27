/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/05 10:16:29 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Get.hpp"

namespace Get
{
	void setResponse(istream *file, vector<string> headers, ClientConnexion &clientConnexion)
	{
		clientConnexion.setHeaderStream(Response::header200(headers));
		clientConnexion.setDownloadStream(file);
	}

	int handler(ClientConnexion &clientConnexion, pollfd &pollfd, Server &server, int pollIndex)
	{
		string path = clientConnexion.getRequest().route->getRoot() + clientConnexion.getRequest().pathInfo;
		vector<string> headers;

		// si le path est un fichier on l'envoie
		if (CheckFile::isFile(path.c_str()))
		{
			headers.push_back("Content-Type: " + MimesTypes::getContentType(path));
			headers.push_back("Content-Length: " + to_string(CheckFile::getFileSize(path.c_str())));
			setResponse(new ifstream(path), headers, clientConnexion);
			if (STATIC_DEBUG)
				cout << YELLOW << path << " is file" << CLEAR << endl;
		}
		// sinon si c'est un directory
		else if (CheckFile::isDir(path.c_str()))
		{
			string index = CheckFile::appendIndex(path, clientConnexion.getRequest().route->getIndex());

			// si on a trouver un index dans le directory on l'envoie
			if (index.empty() == false)
			{
				headers.push_back("Content-Type: " + MimesTypes::getContentType(index));
				headers.push_back("Content-Length: " + to_string(CheckFile::getFileSize(index.c_str())));
				setResponse(new ifstream(index), headers, clientConnexion);
				if (STATIC_DEBUG)
					cout << YELLOW << index << " is dir index" << CLEAR << endl;
			}
			else if (clientConnexion.getRequest().route->isAutoIndex())
			{
				// sinon on envoie le listing du directory
				string file = DirectoryListing::getHtml(path, clientConnexion.getRequest().target);
				headers.push_back("Content-Type: text/html");
				headers.push_back("Content-Length: " + to_string(file.size()));
				setResponse(new stringstream(file), headers, clientConnexion);

				if (STATIC_DEBUG)
					cout << YELLOW << path << " is dir" << CLEAR << endl;
			}
			else
				clientConnexion.setHeaderStream(Response::header404(clientConnexion));
		}
		else
			clientConnexion.setHeaderStream(Response::header404(clientConnexion));
		pollfd.events = POLLOUT;
		clientConnexion.setStaticMethodIsHandled(true);
		return 0;
	}
}
