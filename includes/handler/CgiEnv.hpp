/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiEnv.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 13:44:42 by glamazer          #+#    #+#             */
/*   Updated: 2023/10/05 09:32:53 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CGIENV_HPP
#define CGIENV_HPP

#include "define.hpp"
#include "ClientConnexion.hpp"
#include "CheckFile.hpp"

namespace CgiEnv {
    vector<string>getCgiEnv(ClientConnexion &client);

    void printEnv(vector<string> envp);

    const string getPathTranslate(ClientConnexion &client);

    string convertString(const string& input);

    string getMethod(ClientConnexion &client);
}

#endif