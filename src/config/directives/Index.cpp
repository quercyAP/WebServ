/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Index.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillaume <guillaume@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/06/26 15:56:59 by guillaume        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Index.hpp"

#pragma region[constructor]
Index::Index(string line) : ADirective()
{
    parseDirective(line);
}
#pragma endregion
#pragma region[method]
void Index::parseDirective(string line)
{
    string name;
    string val;

    trim(line);
    istringstream iss(line);
    if (!getline(iss, name, ' '))
    {
        cerr << RED << "Error bad Index input ==> " << line << CLEAR << endl;
        _error = true;
        return;
    }
    _name = name;
    while (iss >> val)
    {
        trim(val);
        if (val.empty())
        {
            cerr << RED << "Error: empty index name in directive " << line << CLEAR << endl;
            _error = true;
            return;
        }
        _values.push_back(val);
    }

    if (_values.empty())
    {
        cerr << RED << "Error: no index name provided in directive " << line << CLEAR << endl;
        _error = true;
    }
}

void Index::execute(Config &conf, const string root)
{
    map<string, Route> &route = conf.routes;

    if (route.find(root) == route.end())
        route[root] = Route(route["/"]);

    Route &currentRoute = route[root];

        currentRoute.setIndex(_values);
}

void Index::print()
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