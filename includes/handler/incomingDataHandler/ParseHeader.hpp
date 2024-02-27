/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseHeader.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:35:36 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/12 11:38:53 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEHEADER_HPP
#define PARSEHEADER_HPP

#include "ClientConnexion.hpp"
#include "Response.hpp"
#include "define.hpp"
#include <poll.h>
#include "Server.hpp"

namespace ParseHeader
{
	
	int parseHeader(ClientConnexion &clientConnexion, pollfd &pollfd, Server &server, int pollIndex)
;
}

#endif
