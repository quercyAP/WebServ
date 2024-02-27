/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StaticUploadHandler.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/13 11:41:30 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATICUPLOADHANDLER_HPP
# define STATICUPLOADHANDLER_HPP

#include "ClientConnexion.hpp"
#include "Response.hpp"
#include "define.hpp"
#include <poll.h>
#include "Server.hpp"

namespace StaticUploadHandler
{
	int handler(ClientConnexion &clientConnexion, pollfd &pollfd, Server &server, int pollIndex);
}

#endif
