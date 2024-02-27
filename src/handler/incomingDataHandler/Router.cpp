/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 14:13:03 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/05 10:51:44 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"

namespace Router
{
	string getHostIfDefined(ClientConnexion &clientConnexion)
	{
		for (map<string, string>::const_iterator it = clientConnexion.getRequest().headers.begin(); it != clientConnexion.getRequest().headers.end(); it++)
		{
			if (it->first == "Host")
			{
				return it->second;
			}
		}
		return "";
	}

	int selectRoute(ClientConnexion &clientConnexion)
	{
		string host = getHostIfDefined(clientConnexion);
		host = host.substr(0, host.find(':'));
		string routeTofindHost = host + clientConnexion.getRequest().url;
		string routeTofindNoHost = clientConnexion.getRequest().url;

		string tmp_route;
		if (ROUTER_DEBUG)
			cout << "routeTofindNoHost: " << routeTofindNoHost << endl;
		for (map<string, Route>::const_iterator it = clientConnexion.getConfig()->routes.begin(); it != clientConnexion.getConfig()->routes.end(); it++)
		{
			if (ROUTER_DEBUG)
				cout << "search route: " << it->first << endl;
			if (routeTofindNoHost.find(it->first) != string::npos)
			{
				if (ROUTER_DEBUG)
					cout << "??? match route: " << it->first << endl;
				if (it->first.back() != '/')
				{
					if (routeTofindNoHost.c_str()[it->first.length()] != '/' && routeTofindNoHost.c_str()[it->first.length()] != '\0')
					{
						if (ROUTER_DEBUG)
							cout << "--- route rejected" << endl;
						continue;
					}
				}
				tmp_route = it->first;
				if (ROUTER_DEBUG)
					cout << "--- route temp no host found: " << tmp_route << endl;
				clientConnexion.setRequestRoute(&(it->second));
			}
			if (routeTofindHost.find(it->first) != string::npos)
			{
				if (ROUTER_DEBUG)
					cout << "??? match route: " << it->first << endl;
				if (it->first.back() != '/')
				{
					if (routeTofindHost.c_str()[it->first.length()] != '/' && routeTofindHost.c_str()[it->first.length()] != '\0')
					{
						if (ROUTER_DEBUG)
							cout << "--- route rejected" << endl;
						continue;
					}
				}
				tmp_route = it->first;
				if (ROUTER_DEBUG)
					cout << "--- route temp host found: " << tmp_route << endl;
				clientConnexion.setRequestRoute(&(it->second));
			}
		}

		if (ROUTER_DEBUG)
			cout << "!!! route found: " << tmp_route << endl;

		if (clientConnexion.getRequest().route == nullptr)
			return 1;

		cout << "selected route: " <<tmp_route<< endl;
		return 0;

	}

	int checkMethodAllowed(ClientConnexion &clientConnexion)
	{
		Route route = *clientConnexion.getRequest().route;
		ClientConnexion::Method method = clientConnexion.getRequest().method;
		if (ROUTER_DEBUG)
			cout << "method: " << method << "<" << clientConnexion.getRequest().target << ">" << endl;
		if (method != ClientConnexion::GET && method != ClientConnexion::POST && method != ClientConnexion::DELETE)
			return 2;
		if (method == ClientConnexion::GET && !route.GET_allowed())
			return 1;
		if (method == ClientConnexion::POST && !route.POST_allowed())
			return 1;
		if (method == ClientConnexion::DELETE && !route.DELETE_allowed())
			return 1;
		return 0;
	}
	void setRouteUrlPathInfo(ClientConnexion &clientConnexion)
	{
		string url = clientConnexion.getRequest().url;
		// cout << "url: " << url << endl;
		string route = clientConnexion.getRequest().route->getName();
		string routeUrl = route.replace(0, route.find('/'), "");
		// cout << "routeUrl: " << routeUrl << endl;
		string pathInfo = url.replace(0, (routeUrl.length() == 1 ? 0 : routeUrl.length()), "");
		// cout << "pathinfo: " << pathInfo << endl;
		clientConnexion.setRequestRouteUrl(routeUrl);
		clientConnexion.setRequestPathInfo(pathInfo);
	}

	int setAndCheckRoute(ClientConnexion &clientConnexion, pollfd &pollfd, Server &server, int pollIndex)
	{
		if (int ret = selectRoute(clientConnexion))
		{
			if (ret == 1)
			{
				clientConnexion.setHeaderStream(Response::header500(clientConnexion, "no route found"));
				pollfd.events = POLLOUT;
				return 1;
			}
			if (ret == 2)
			{
				clientConnexion.setHeaderStream(Response::header404(clientConnexion));
				pollfd.events = POLLOUT;
				return 1;
			}
		}
		setRouteUrlPathInfo(clientConnexion);
		if (int error = checkMethodAllowed(clientConnexion))
		{
			if (error == 1)
				clientConnexion.setHeaderStream(Response::header405(clientConnexion));
			else if (error == 2)
				clientConnexion.setHeaderStream(Response::header501(clientConnexion));
			pollfd.events = POLLOUT;
			return 1;
		}
		if (clientConnexion.getRequest().route->isRedirected())
		{
			const string &redirectionUrl = clientConnexion.getRequest().route->getRedirectionUrl();
			const vector<string> &redirectionUrls = clientConnexion.getRequest().route->getRedirectionUrls();
			int redirectionCode = atoi(clientConnexion.getRequest().route->getRedirectionCode().c_str());
			switch (redirectionCode)
			{
			case 300:
				clientConnexion.setHeaderStream(Response::header300(redirectionUrls));
				break;
			case 301:
				clientConnexion.setHeaderStream(Response::header301(redirectionUrl));
				break;
			case 302:
				clientConnexion.setHeaderStream(Response::header302(redirectionUrl));
				break;
			case 303:
				clientConnexion.setHeaderStream(Response::header303(redirectionUrl));
				break;
			case 304:
				clientConnexion.setHeaderStream(Response::header304());
				break;
			case 307:
				clientConnexion.setHeaderStream(Response::header307(redirectionUrl));
				break;
			case 308:
				clientConnexion.setHeaderStream(Response::header308(redirectionUrl));
				break;

			default:
				break;
			}
			pollfd.events = POLLOUT;
			return 1;
		}
		clientConnexion.setRouteIsChecked(true);
		return 0;
	}
}
