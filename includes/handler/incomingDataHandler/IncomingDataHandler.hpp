/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IncomingDataHandler.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/08 09:13:16 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCOMINGDATAHANDLER_HPP
# define INCOMINGDATAHANDLER_HPP

#include "ClientConnexion.hpp"
#include "Response.hpp"
#include "define.hpp"
#include <poll.h>
#include "Server.hpp"

namespace IncomingDataHandler
{
	void incomingData(ClientConnexion &clientConnexion, pollfd &pollfd, Server &server, int pollIndex);
};
#endif
