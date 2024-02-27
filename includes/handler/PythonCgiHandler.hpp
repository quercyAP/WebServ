/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PythonCgiHandler.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/27 12:55:53 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PYTHONCGIHANDLER_HPP
# define PYTHONCGIHANDLER_HPP

#include "define.hpp"
#include "Server.hpp"
#include "ClientConnexion.hpp"
#include "Response.hpp"
 #include <sys/wait.h>
#include <poll.h>
#include "CgiEnv.hpp"


namespace PythonCgiHandler
{
    int handler(ClientConnexion &clientConnexion, pollfd &pollFd, Server &server, int i);
}

#endif
