/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillaume <guillaume@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/06/22 15:29:02 by guillaume        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UPLOAD_HPP
# define UPLOAD_HPP

#include "ADirective.hpp"

using namespace std;

class Upload : public ADirective
{
    protected:
        void parseDirective(string line);
    public:
        Upload(string line);
        void execute(Config &conf, const string root);
        void print();
};

#endif
