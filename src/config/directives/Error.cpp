/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/19 11:05:52 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Error.hpp"

#pragma region[constructor]
Error::Error(string line) : ADirective()
{
    parseDirective(line);
}
#pragma endregion;
#pragma region[method]
void Error::parseDirective(string line)
{
    string name;
    string val;
    string param;
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
    while (iss >> param)
    {
        tmp.push_back(param);
    }
    if (!tmp.empty())
    {
        vector<string>::iterator it = tmp.begin();
        while (it != tmp.end() && isNum(*it))
        {
            int nb = stoi(*it);
            if ((nb >= 400 && nb <= 451) || (nb >= 500 && nb <= 511))
                _params.push_back(nb);
            else
            {
                cerr << RED << "Error bad input not a valid error code ==> " << nb << CLEAR << endl;
                _error = true;
                return;
            }
            it++;
        }
        if (distance(it, tmp.end()) > 1)
        {
            cerr << RED << "Error bad number of values ==> " << line << CLEAR << endl;
            _error = true;
            return;
        }
        _values.push_back(*it);
    }
    else
    {
        cerr << RED << "Error bad input ==> " << line << CLEAR << endl;
        _error = true;
        return;
    }
    if (_params.empty())
    {
        cerr << RED << "Error no params ==> " << line << CLEAR << endl;
        _error = true;
        return;
    }
}

void Error::execute(Config &conf, const string root)
{
    map<string, Route> &route = conf.routes;

    if (route.find(root) == route.end())
        route[root] = Route(route["/"]);

    Route &currentRoute = route[root];

    if (!isFile(currentRoute.getRoot() + _values.at(0)))
    {
        cerr << RED << "Error bad value ==> " << currentRoute.getRoot() + _values.at(0) << CLEAR << endl;
        _error = true;
        throw runtime_error("Parsing Error");
    }
    for (int i = 0; i < _params.size(); i++)
    {
        ostringstream oss;
        oss << _params[i];
        currentRoute.setErrorPage(oss.str(), currentRoute.getRoot() + _values[0]);
    }
}
void Error::print()
{
    if (!_values.empty() && !_params.empty())
    {
        cout << GREEN << _name << " : " << CLEAR;
        for (int i = 0; i < _params.size(); i++)
            cout << YELLOW << _params[i] << " ";
        for (int i = 0; i < _values.size(); i++)
            cout << BLUE << _values[i] << " ";
        cout << CLEAR << endl;
    }
}
#pragma endregion