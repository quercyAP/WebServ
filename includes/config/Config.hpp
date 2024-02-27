/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <exception>
#include <fcntl.h>
#include <netdb.h>
#include <cstring>
#include "Route.hpp"

using namespace std;
class Route;
typedef struct Config
{
	string host_ip;
	unsigned short port;
	vector<string> serverNames;
	map<string, Route> routes;

	Config() : port(8081), host_ip("0.0.0.0") { routes["/"]; }
} Config;

#endif
