/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutgoingDataHandler.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 14:29:12 by glamazer          #+#    #+#             */
/*   Updated: 2023/09/26 12:41:26 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTGOINGDATAHANDLER_HPP
# define OUTGOINGDATAHANDLER_HPP

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


namespace OutgoingDataHandler
{
    void outgoingData(ClientConnexion &connexion, pollfd &pollFd, Server &server, int i);
}

#endif
