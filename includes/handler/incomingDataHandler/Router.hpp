/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:41:06 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/12 11:36:25 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "ClientConnexion.hpp"
#include "Response.hpp"
#include "define.hpp"
#include <poll.h>
#include "Server.hpp"

namespace Router
{
	int setAndCheckRoute(ClientConnexion &clientConnexion, pollfd &pollfd, Server &server, int pollIndex);
}
#endif
