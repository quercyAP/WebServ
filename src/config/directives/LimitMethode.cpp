/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LimitMethode.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillaume <guillaume@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/06/26 14:13:15 by guillaume        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LimitMethode.hpp"

#pragma region[constructor]
LimitMethode::LimitMethode(string line) : ADirective()
{
    parseDirective(line);
}
#pragma endregion
#pragma region[method]
void LimitMethode::parseDirective(string line)
{
    string name;
    string val;

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
        trim(val);
        if (val.empty())
        {
            cerr << RED << "Error: empty LimitMethode in directive " << line << CLEAR << endl;
            _error = true;
            return;
        }
        if (!isMethod(val))
        {
            cerr << RED << "Error: Unknown Method in directive " << val << CLEAR << endl;
            _error = true;
            return;
        }
        _values.push_back(val);
    }

    if (_values.empty())
    {
        cerr << RED << "Error: no LimitMethode provided in directive " << line << CLEAR << endl;
        _error = true;
    }
}
void LimitMethode::execute(Config &conf, const string root)
{
    map<string, Route> &route = conf.routes;

    if (route.find(root) == route.end())
        route[root] = Route(route["/"]);

    Route &currentRoute = route[root];

    currentRoute.setPOST_allowed(false);
    currentRoute.setGET_allowed(false);
    currentRoute.setDELETE_allowed(false);

    for (int i = 0; i < _values.size(); i++)
    {
        if (_values[i] == "POST")
            currentRoute.setPOST_allowed(true);
        else if (_values[i] == "GET")
            currentRoute.setGET_allowed(true);
        else if (_values[i] == "DELETE")
            currentRoute.setDELETE_allowed(true);
    }
}
bool LimitMethode::isMethod(const string method)
{
    if (method == "GET" || method == "POST" || method == "DELETE")
        return true;
    return false;
}
void LimitMethode::print()
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