
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Redir.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Redir.hpp"

#pragma region[constructor]
Redir::Redir(string line) : ADirective()
{
    parseDirective(line);
}
#pragma endregion;
#pragma region[method]
void Redir::parseDirective(string line)
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
    iss >> param;
	try
	{
  		int nb = stoi(param);
   		if (nb != 306 && nb != 304 && nb != 305 && (nb >= 300 && nb <= 310))
       		_params.push_back(nb);
	}
	catch(const std::exception& e)
	{
		;
	}
    while (iss >> val)
    {
        tmp.push_back(val);
    }
    if (!tmp.empty() && !_params.empty())
    {
        if (tmp.size() > 1 && _params[0] != 300)
        {
            cerr << RED << "Error bad number of values ==> " << line << CLEAR << endl;
            _error = true;
            return;
        }
        vector<string>::iterator it = tmp.begin();
        for (; it != tmp.end(); ++it)
        {
            _values.push_back(*it);
        }
    }
    else
    {
        cerr << RED << "Error bad input ==> " << line << CLEAR << endl;
        _error = true;
        return;
    }
}

void Redir::execute(Config &conf, const string root)
{
    map<string, Route> &route = conf.routes;

    if (route.find(root) == route.end())
        route[root] = Route(route["/"]);

    Route &currentRoute = route[root];

    for (int i = 0; i < _values.size(); i++)
    {
        ostringstream oss;
        oss << _params[0];
        currentRoute.setRedirection(oss.str(), _values[i]);
    }
}
void Redir::print()
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
