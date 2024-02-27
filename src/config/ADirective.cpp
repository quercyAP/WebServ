/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADirective.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillaume <guillaume@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/06/27 11:08:49 by guillaume        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ADirective.hpp"

#pragma region[constructor]
ADirective::ADirective() : _error(false) {}

ADirective::~ADirective() {}
#pragma endregion
#pragma region[getter]
bool ADirective::getError() const { return _error; }
const string ADirective::getName() const { return _name; }
#pragma endregion
#pragma region[method]
bool ADirective::hasEnded(const string line) const
{
    if (line.find(';') != string::npos)
        return true;
    cerr << RED << "Error line not terminated with ; ==>" << line << CLEAR << endl;
    return false;
}

bool ADirective::isNum(const string str) const
{
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] < '0' || str[i] > '9')
            return false;
    }
    return true;
}
void ADirective::trim(string &str)
{
    str.erase(0, str.find_first_not_of(" \t\n\r\f\v"));
    str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1);
}

bool ADirective::isFile(const std::string& path)
{
    struct stat buf;
    if (stat(path.c_str(), &buf) != 0)
        return false;
    return S_ISREG(buf.st_mode);
}

bool ADirective::isDirectory(const std::string& path)
{
    struct stat buf;
    if (stat(path.c_str(), &buf) != 0)
        return false;
    return S_ISDIR(buf.st_mode);
}
#pragma endregion