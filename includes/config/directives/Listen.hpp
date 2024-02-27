/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillaume <guillaume@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/06/26 13:43:27 by guillaume        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTEN_HPP
# define LISTEN_HPP

#include "ADirective.hpp"

using namespace std;

class Listen : public ADirective
{   
    protected:
        void parseDirective(string line);
    private:
        bool isValidIP(string ip);    
    public:
        Listen(string line);
        void execute(Config &conf, const string root);
        void print();
};

#endif
