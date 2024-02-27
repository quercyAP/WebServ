/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PythonInCgiHandler.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/26 11:36:28 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PYTHONINCGIHANDLER_HPP
# define PYTHONINCGIHANDLER_HPP

#include "define.hpp"
#include "Server.hpp"
#include "ClientConnexion.hpp"
#include "Response.hpp"
 #include <sys/wait.h>
#include <poll.h>


namespace PythonInCgiHandler
{
    int handler(ClientConnexion &clientConnexion, pollfd &pollFd, Server &server, int i);
}


#endif
