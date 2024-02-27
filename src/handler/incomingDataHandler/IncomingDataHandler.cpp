/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IncomingDataHandler.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/05 10:24:24 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IncomingDataHandler.hpp"
#include "ParseHeader.hpp"
#include "Router.hpp"
#include "StaticMethodHandler.hpp"
#include "StaticUploadHandler.hpp"
#include "PythonCgiHandler.hpp"
#include "PythonInCgiHandler.hpp"

namespace IncomingDataHandler
{

	void incomingData(ClientConnexion &clientConnexion, pollfd &pollfd, Server &server, int pollIndex)
	{
		if (STATIC_DEBUG)
			cout << "incoming data...\n####### Header is parsed? " << clientConnexion.isHeaderParsed() << endl;

		if (clientConnexion.isHeaderParsed() == false)
		{
			if (ParseHeader::parseHeader(clientConnexion, pollfd, server, pollIndex))
				return;
		}
		if (STATIC_DEBUG)
			cout << "###### Route is checked? " << clientConnexion.isRouteChecked() << endl;

		if (clientConnexion.isRouteChecked() == false)
		{
			if (Router::setAndCheckRoute(clientConnexion, pollfd, server, pollIndex))
				return;
		}
		if (STATIC_DEBUG)
			cout << "###### StaticMethodsHandled ? " << clientConnexion.isStaticMethodHandled() << endl;

		if (!clientConnexion.getRequest().route->hasPythonCgi() &&
			!clientConnexion.getRequest().route->hasPhpCgi() &&
			clientConnexion.isStaticMethodHandled() == false &&
			!clientConnexion.isReadyToUpload())
		{
			if (StaticMethodHandler::MethodsHandler(clientConnexion, pollfd, server, pollIndex))
				return;
		}
		if (STATIC_DEBUG)
			cout << "###### Route is python cgi ? " << clientConnexion.getRequest().route->hasPythonCgi() << endl;

		if (clientConnexion.getRequest().route->hasPythonCgi() && clientConnexion.getCgi().isInit == false)
		{
			if (PythonCgiHandler::handler(clientConnexion, pollfd, server, pollIndex))
				return;
		}
		if (STATIC_DEBUG)
			cout << "###### Python cgi Is init? " << clientConnexion.getCgi().isInit << endl;

		if (clientConnexion.getRequest().route->hasPythonCgi() && clientConnexion.getCgi().isInit == true && clientConnexion.getRequest().method == ClientConnexion::POST)
		{
			if (PythonInCgiHandler::handler(clientConnexion, pollfd, server, pollIndex))
				return;
		}
		if (STATIC_DEBUG)
			cout << "###### ReadyToUpload ? " << clientConnexion.isReadyToUpload() << endl;

		if (clientConnexion.isReadyToUpload())
		{
			if (StaticUploadHandler::handler(clientConnexion, pollfd, server, pollIndex))
				return;
		}
	}
}
