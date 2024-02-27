/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnexion.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/08 15:33:49 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientConnexion.hpp"

ClientConnexion::ClientConnexion()
{
#ifdef WEBDEBUG
	cout << BLUE << "New clientConnexion default NOOOOOOOOOOOOOOOOOONNNN" << CLEAR << endl;
#endif
};

ClientConnexion::ClientConnexion(int clientFd, int serverFd, string clientIp,
								 const Config *config) : _clientFd(clientFd),
														 _serverFd(serverFd),
														 _clientIp(clientIp),
														 _config(config)
{
#ifdef WEBDEBUG
	cout << GREEN << "New clientConnexion on fd " << _clientFd << " @ " << _clientIp << CLEAR << endl;
#endif
}

ClientConnexion::~ClientConnexion()
{
#ifdef WEBDEBUG
	cout << RED << "ClientConnexion on fd " << _clientFd << " @ " << _clientIp << " destroyed" << CLEAR << endl;
#endif
}
