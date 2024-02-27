/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitMethode.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillaume <guillaume@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/06/22 15:27:35 by guillaume        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIMITMETHODE_HPP
#define LIMITMETHODE_HPP

#include "ADirective.hpp"

using namespace std;

class LimitMethode : public ADirective
{
protected:
    void parseDirective(string line);

private:
    bool isMethod(const string str);

public:
    LimitMethode(string line);
        void execute(Config &conf, const string root);
    void print(void);
};

#endif
