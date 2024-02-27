/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillaume <guillaume@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/06/22 15:26:52 by guillaume        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
# define ERROR_HPP

#include "ADirective.hpp"

using namespace std;

class Error : public ADirective
{
    protected:
        void parseDirective(string line);

    private:
        vector<int> _params;

    public:
        Error(string line);
        void execute(Config &conf, const string root);
        void print(void);
};

#endif
