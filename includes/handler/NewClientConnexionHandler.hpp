/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NewClientConnexionHandler.hpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/08 14:12:07 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NEWCLIENTCONNEXIONHANDLER_HPP
#define NEWCLIENTCONNEXIONHANDLER_HPP

#include "ClientConnexion.hpp"
#include "Response.hpp"
#include "define.hpp"
#include "Config.hpp"
#include <poll.h>
namespace NewClientConnexionHandler
{
	void newClientConnexion(int fd, map<int, ClientConnexion *> &clientConnexions, vector<pollfd> &pollfds,const map<int, Config> &configs);

};

#endif
