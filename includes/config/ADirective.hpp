/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillaume <guillaume@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 12:31:33 by guillaume         #+#    #+#             */
/*   Updated: 2023/06/27 11:08:40 by guillaume        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADIRECTIVE_HPP
# define ADIRECTIVE_HPP

#define CYAN "\033[0;36m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RED "\033[0;31m"
#define CLEAR "\033[0m"

#include "Config.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <sys/stat.h>

using namespace std;

class ADirective {
    protected:
        string _name;
        vector<string> _values;
        bool _error;
        virtual void parseDirective(string line) = 0;
    public:
        ADirective();
        virtual ~ADirective();
        virtual void execute(Config &conf, const string root) = 0;
        virtual void print(void) = 0;
        void trim(string &str);
        bool hasEnded(const string line) const;
        bool isNum(const string str) const;
        bool isFile(const string &path);
        bool getError() const;
        const string getName() const;
        bool isDirectory(const string &path);
};

#endif
