/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MaxBodySize.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillaume <guillaume@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/06/27 10:04:30 by guillaume        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MaxBodySize.hpp"

#pragma region[constructor]
MaxBodySize::MaxBodySize(string line) : ADirective()
{
    parseDirective(line);
}
#pragma endregion
#pragma region[method]
void MaxBodySize::parseDirective(string line)
{
    trim(line);
    istringstream iss(line);
    string name;
    string val;

    if (!getline(iss, name, ' '))
    {
        cerr << RED << "Error bad input ==> " << line << CLEAR << endl;
        _error = true;
        return;
    }
    _name = name;

    if (iss >> val)
    {
        std::regex rgx("([0-9]+)([KMG]?)");
        std::smatch matches;

        if (std::regex_search(val, matches, rgx))
        {
            string sizeStr = matches[1];
            string unit = matches[2];

            long long size = std::stoll(sizeStr);
            if (unit == "K")
            {
                size *= 1024;
            }
            else if (unit == "M")
            {
                size *= 1024 * 1024;
            }
            else if (unit == "G")
            {
                size *= 1024 * 1024 * 1024;
            }
            else if (unit != "")
            {
                cerr << RED << "Error: Invalid unit in value ==> " << val << CLEAR << endl;
                _error = true;
                return;
            }
            _size = size;
            string extra;
            if (iss >> extra)
            {
                cerr << RED << "Error extra input ==> " << extra << CLEAR << endl;
                _error = true;
            }
        }
        else
        {
            cerr << RED << "Error bad value ==> " << val << CLEAR << endl;
            _error = true;
            return;
        }
    }
    else
    {
        cerr << RED << "Error missing value for " << name << CLEAR << endl;
        _error = true;
    }
}

void MaxBodySize::execute(Config &conf, const string root)
{
    map<string, Route> &route = conf.routes;

    if (route.find(root) == route.end())
        route[root] = Route(route["/"]);

    Route &currentRoute = route[root];

    ostringstream oss;
    oss << _size;
    currentRoute.setMaxBody(oss.str());
}

void MaxBodySize::print()
{
    cout << GREEN << _name << " : " << CLEAR;
    cout << BLUE << _size << " bytes";
    cout << CLEAR << endl;
}

#pragma endregion