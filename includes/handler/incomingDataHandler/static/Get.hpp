/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/26 14:03:23 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_HPP
# define GET_HPP
#include "ClientConnexion.hpp"
#include "Response.hpp"
#include "define.hpp"
#include <poll.h>
#include "Server.hpp"
#include "CheckFile.hpp"
#include "MimesTypes.hpp"
#include "DirectoryListing.hpp"
#include "CgiEnv.hpp"

namespace Get
{
	int handler(ClientConnexion &clientConnexion, pollfd &pollfd, Server &server, int pollIndex);
}

#endif
