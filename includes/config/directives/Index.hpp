/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Index.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillaume <guillaume@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/06/22 15:27:11 by guillaume        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INDEX_HPP
# define INDEX_HPP

#include "ADirective.hpp"

using namespace std;

class Index : public ADirective
{
    protected:
        void parseDirective(string line);
    
    public:
        Index(string line);
        void execute(Config &conf, const string root);
        void print(void);

};

#endif
