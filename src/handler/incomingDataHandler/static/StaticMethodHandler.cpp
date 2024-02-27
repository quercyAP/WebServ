/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticMethodHandler.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/13 12:38:37 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StaticMethodHandler.hpp"

namespace StaticMethodHandler
{
	
	
	int MethodsHandler(ClientConnexion &clientConnexion, pollfd &pollfd, Server &server, int pollIndex)
	{
		if (clientConnexion.getRequest().method == ClientConnexion::GET)
			return (Get::handler(clientConnexion, pollfd, server, pollIndex));
		else if (clientConnexion.getRequest().method == ClientConnexion::POST)
			return (Post::handler(clientConnexion, pollfd, server, pollIndex));
		else if (clientConnexion.getRequest().method == ClientConnexion::DELETE)
			return (Delete::handler(clientConnexion, pollfd, server, pollIndex));
		return 0;
	}
}
