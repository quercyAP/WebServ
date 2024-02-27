/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillaume <guillaume@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/06/26 14:10:33 by guillaume        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndex.hpp"

#pragma region[constructor]
AutoIndex::AutoIndex(string line) : ADirective()
{
    parseDirective(line);
}
#pragma endregion
#pragma region[method]
void AutoIndex::parseDirective(string line)
{
    string name;
    string val;

    trim(line);
    istringstream iss(line);
    if (!getline(iss, name, ' ') || !getline(iss, val))
    {
        cerr << RED << "Error Autoindex bad input ==> " << line << CLEAR << endl;
        _error = true;
        return;
    }
    trim(val);
    if (val != "off" && val != "on" || getline(iss, val))
    {
        cerr << RED << "Error Autoindex bad value ==> " << val << CLEAR << endl;
        _error = true;
        return;
    }
    _name = name;
    _values.push_back(val);
}

void AutoIndex::execute(Config &conf, const string root)
{
    map<string, Route> &route = conf.routes;
    bool set = (_values[0] == "on");

    if (route.find(root) == route.end())
        route[root] = Route(route["/"]);

    Route &currentRoute = route[root];

    currentRoute.setAutoIndex(set);
}
void AutoIndex::print()
{
    if (!_values.empty())
    {
        cout << GREEN << _name << " : " << CLEAR;
        for (int i = 0; i < _values.size(); i++)
            cout << BLUE << _values[i] << " ";
        cout << CLEAR << endl;
    }
}
#pragma endregion
