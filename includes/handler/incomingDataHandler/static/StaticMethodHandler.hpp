/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticMethodHandler.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/13 12:38:57 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATICMETHODHANDLER_HPP
# define STATICMETHODHANDLER_HPP

#include "ClientConnexion.hpp"
#include "Response.hpp"
#include "define.hpp"
#include <poll.h>
#include "Server.hpp"

#include "Get.hpp"
#include "Post.hpp"
#include "Delete.hpp"

namespace StaticMethodHandler
{
	int MethodsHandler(ClientConnexion &clientConnexion, pollfd &pollfd, Server &server, int pollIndex);
}

#endif
