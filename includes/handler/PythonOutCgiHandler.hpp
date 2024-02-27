/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PythonOutCgiHandler.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/26 11:03:04 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PYTHONOUTCGIHANDLER_HPP
# define PYTHONOUTCGIHANDLER_HPP

#include "define.hpp"
#include "Server.hpp"
#include "ClientConnexion.hpp"
#include "Response.hpp"
 #include <sys/wait.h>
#include <poll.h>


namespace PythonOutCgiHandler
{
    int handler(ClientConnexion &clientConnexion, pollfd &pollFd, Server &server, int i);
}


#endif
