/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Redir.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillaume <guillaume@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/06/22 15:28:18 by guillaume        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIR_HPP
# define REDIR_HPP

#include "ADirective.hpp"

using namespace std;

class Redir : public ADirective
{
    protected:
        void parseDirective(string line);
    private:
        vector<int> _params;
    public:
        Redir(string line);
        void execute(Config &conf, const string root);
        void print();
};

#endif
