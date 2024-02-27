/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 10:52:52 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/09 08:28:09 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_HPP
#define DEFINE_HPP

#define CYAN "\033[0;36m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RED "\033[0;31m"
#define CLEAR "\033[0m"

#define WEBDEBUG 0
#define PARSE_HEADER_DEBUG 0
#define ENV_DEBUG 0
#define CGI_DEBUG 0
#define STATIC_DEBUG 0
#define ROUTER_DEBUG 0
#define OUT_DEBUG 0


#define READ 0
#define WRITE 1

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctime>
#include <sys/stat.h>
#include <sstream>
#include <limits.h>
#include <csignal>

using namespace std;


#endif
