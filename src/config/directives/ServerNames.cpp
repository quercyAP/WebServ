/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerNames.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/07/10 12:25:06 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerNames.hpp"

#pragma region[constructor]
ServerNames::ServerNames(string line) : ADirective()
{
    parseDirective(line);
}
#pragma endregion;
#pragma region[method]
void ServerNames::parseDirective(string line)
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

void ServerNames::execute(Config &conf, const string root)
{
    vector<string> serverNames;

    for (int i = 0; i < _values.size(); i++)
    {
        serverNames.push_back(_values[i]);
    }
    conf.serverNames = serverNames;
}
void ServerNames::print()
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