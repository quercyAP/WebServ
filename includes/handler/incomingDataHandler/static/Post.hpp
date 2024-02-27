/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/13 12:40:30 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_HPP
# define POST_HPP
#include "ClientConnexion.hpp"
#include "Response.hpp"
#include "define.hpp"
#include <poll.h>
#include "Server.hpp"

namespace Post
{
	int handler(ClientConnexion &clientConnexion, pollfd &pollfd, Server &server, int pollIndex);
}

#endif
