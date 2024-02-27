/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/07/12 08:11:01 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listen.hpp"

#pragma region[constructor]
Listen::Listen(string line) : ADirective()
{
    parseDirective(line);
}
#pragma endregion
#pragma region[method]
void Listen::parseDirective(string line)
{
    string name;
    string val;

    trim(line);
    istringstream iss(line);
    if (!getline(iss, name, ' ') || !getline(iss, val))
    {   
        cerr << RED << "Error bad Listen input ==>" << line << CLEAR << endl;
        _error = true;
        return;
    }
    trim(val);

    string host, port;
    istringstream valStream(val);
    if (getline(valStream, host, ':') && getline(valStream, port))
    {
		try
		{
			
        if (!isNum(port) || stoi(port) > 65535 || !isValidIP(host))
        {
            cerr << RED << "Error bad Listen value ==> " << port << CLEAR << endl;
            _error = true;
            return;
        }
		}
		catch(const std::exception& e)
		{;}
        _values.push_back(host);
        _values.push_back(port);
    }
    else if (isNum(host))
    {
		try
		{
        if (stoi(host) > 65535)
        {
            cerr << RED << "Error bad Listen value ==> " << host << CLEAR << endl;
            _error = true;
            return;
        }
		}
		catch(const std::exception& e)
		{;}
        _values.push_back(host);
    }
    else
    {
        cerr << RED << "Error bad Listen value ==> " << val << CLEAR << endl;
        _error = true;
        return;
    }
    _name = name;
}

bool Listen::isValidIP(string ip)
{
    stringstream ss(ip);
    string num;
    int count = 0;
    while (getline(ss, num, '.')) 
    {
        count++;
		try
		{
        if (count > 4 || !isNum(num) || stoi(num) > 255 || stoi(num) < 0)
            return false;
		}
		catch(const std::exception& e)
		{;}
    }
    return count == 4;
}

void Listen::execute(Config &conf, const string root)
{
    if (!_values.empty())
    {
		try
		{
        if (_values.size() == 1)
        {
            conf.port = stoi(_values[0]);
            conf.host_ip = "0.0.0.0";
        }
        else
        {
            conf.host_ip = _values[0];
            conf.port = stoi(_values[1]);
        }       
		}
		catch(const std::exception& e)
		{
            cerr << "Listen directive: " << e.what() << endl;
        }
    }
}

void Listen::print()
{
    if (!_values.empty())
    {
        cout << GREEN << _name << " : " << CLEAR;
        cout << BLUE << _values[0];
        if (_values.size() > 1)
            cout << ":" << _values[1];        
        cout << CLEAR << endl;
    }
}
#pragma endregion
