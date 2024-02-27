/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Root.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillaume <guillaume@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/06/27 10:38:24 by guillaume        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Root.hpp"

#pragma region[constructor]
Root::Root(string line) : ADirective()
{
    parseDirective(line);
}
#pragma endregion;
#pragma region[method]
void Root::parseDirective(string line)
{
    string name;
    string val;
    vector<string> tmp;

    trim(line);
    istringstream iss(line);
    if (!getline(iss, name, ' '))
    {
        cerr << RED << "Error bad input ==> " << line << CLEAR << endl;
        _error = true;
        return;
    }
    _name = name;
    while (iss >> val)
    {
        tmp.push_back(val);
    }
    if (tmp.size() == 1 && isDirectory(tmp[0]))
    {
        _values.push_back(tmp[0]);
    }
    else
    {
        cerr << RED << "Error bad value input ==> " << line << CLEAR << endl;
        _error = true;
        return;
    }
}

void Root::execute(Config &conf, const string root)
{
    map<string, Route> &route = conf.routes;

    if (route.find(root) == route.end())
        route[root] = Route(route["/"]);

    Route &currentRoute = route[root];

    if (root == "/")
    {
        currentRoute.setRoot(_values[0]);
        currentRoute.setLocationPath("");
    }
    else
    {
        currentRoute.setRoot(_values[0]);
        currentRoute.setLocationPath(root);
    }
}
void Root::print()
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