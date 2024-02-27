/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiCrashHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/26 12:49:23 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGICRASHHANDLER_HPP
# define CGICRASHHANDLER_HPP

#include "define.hpp"
#include "Server.hpp"
#include "ClientConnexion.hpp"
#include "PythonOutCgiHandler.hpp"
#include "Response.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace CgiCrashHandler
{
	int handler(ClientConnexion &clientConnexion, pollfd &pollFd, Server &server, int i);
}

#endif
