/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiPhp.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIPHP_HPP
# define CGIPHP_HPP

#include "ADirective.hpp"

using namespace std;

class CgiPhp : public ADirective
{
    protected:
        void parseDirective(string line);

    public:
        CgiPhp(string line);
        void execute(Config &conf, const string root);
        void print();
};


#endif
