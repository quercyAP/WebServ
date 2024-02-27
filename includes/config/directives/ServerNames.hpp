/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNames.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillaume <guillaume@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/06/22 15:28:29 by guillaume        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERNAMES_HPP
# define SERVERNAMES_HPP

#include "ADirective.hpp"

using namespace std;

class ServerNames : public ADirective
{
    protected:
        void parseDirective(string line);
    public:
        ServerNames(string line);
        void execute(Config &conf, const string root);
        void print();
};

#endif
