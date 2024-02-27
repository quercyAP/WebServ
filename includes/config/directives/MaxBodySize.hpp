/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MaxBodySize.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAXBODYSIZE_HPP
# define MAXBODYSIZE_HPP

#include "ADirective.hpp"
#include <regex>

using namespace std;

class MaxBodySize : public ADirective
{
    protected:
        void parseDirective(string line);
    private:
        long long _size;       
    public:
        MaxBodySize(string line);
        void execute(Config &conf, const string root);
        void print();
};

#endif
