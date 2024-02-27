/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiCrashHandler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/26 12:53:33 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiCrashHandler.hpp"

namespace CgiCrashHandler
{
	int handler(ClientConnexion &clientConnexion, pollfd &pollFd, Server &server, int i)
	{

		int status = 0;
		waitpid(clientConnexion.getCgi().pid, &status, WNOHANG);
		if (WIFEXITED(status))
		{
			int exit_status = WEXITSTATUS(status);
			cout << "Exit status of the child was " << exit_status << endl;
			clientConnexion.setHeaderStream(Response::header500(clientConnexion, "CGI crashed"));
			pollFd.events = POLLOUT;
			return 1;
		}
		return 0;
	}
}
