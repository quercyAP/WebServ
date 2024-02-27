/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/19 11:03:51 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerBlock.hpp"

#pragma region[constructor]
ServerBlock::ServerBlock() : _error(false) {}
ServerBlock::~ServerBlock()
{
    vector<LocationBlock *>::iterator it = _locationBlocks.begin();
    for (; it != _locationBlocks.end(); ++it)
    {
        if (*it != nullptr)
            delete *it;
    }
    vector<ADirective *>::iterator it2 = _directives.begin();
    for (; it2 != _directives.end(); ++it2)
    {
        if (*it2 != nullptr)
            delete *it2;
    }
}
#pragma endregion
#pragma region[method]
ADirective *ServerBlock::directiveFactory(string line)
{
    if (line.empty())
        return nullptr;
    if (findName(line, "autoindex"))
        return new AutoIndex(line);
    else if (findName(line, "cgi_php"))
        return new CgiPhp(line);
    else if (findName(line, "cgi_py"))
        return new CgiPy(line);
    else if (findName(line, "error_page"))
        return new Error(line);
    else if (findName(line, "index"))
        return new Index(line);
    else if (findName(line, "client_max_body_size"))
        return new MaxBodySize(line);
    else if (findName(line, "listen"))
        return new Listen(line);
    else if (findName(line, "limit_exept"))
        return new LimitMethode(line);
    else if (findName(line, "return"))
        return new Redir(line);
    else if (findName(line, "root"))
        return new Root(line);
    else if (findName(line, "server_name"))
        return new ServerNames(line);
    else if (findName(line, "upload_store"))
        return new Upload(line);
    else if (!findName(line, "location") && !findName(line, "server") && line.find("{") == string::npos && line.find("}") == string::npos)
    {
        cerr << "Unknown or unsupported directive: " << line << endl;
        _error = true;
    }
    return nullptr;
}
Config &ServerBlock::setConfig()
{
    if (_error)
        throw runtime_error("Directive Error in ServerBlock");
    vector<ADirective *>::const_iterator it = _directives.begin();
    for (; it != _directives.end(); ++it)
    {
        if (*it != nullptr)
        {
            (*it)->execute(_conf, "/");
            if ((*it)->getError())
                throw runtime_error("Directive Error in ServerBlock");
        }
    }
    vector<LocationBlock *>::iterator it2 = _locationBlocks.begin();
    for (; it2 != _locationBlocks.end(); ++it2)
    {
        (*it2)->setConfig(_conf);
        (*it2)->setLocationBlockConfig(_conf);
        if ((*it2)->getError())
                throw runtime_error("Directive Error in ServerBlock");
    }
    appendServerName();
    return _conf;
}
void ServerBlock::appendServerName(void)
{
    map<string, Route> routes = _conf.routes;
    map<string, Route> appendRoutes(routes);
    for (int i = 0; i < _conf.serverNames.size(); i++)
    {
        map<string, Route>::iterator it = routes.begin();
        for (; it != routes.end(); ++it)
        {
            appendRoutes[_conf.serverNames[i] + (*it).first] = (*it).second;
        }
    }
    _conf.routes = appendRoutes;
}
void ServerBlock::addDirective(const string &Line)
{
    ADirective *d = directiveFactory(Line);
    if (d != nullptr)
    {
        if (_existingDirectives.find(d->getName()) == _existingDirectives.end())
        {
            _directives.push_back(d);
            _existingDirectives.insert(d->getName());
        }
        else
        {
            cerr << "Duplicate directive: " << d->getName() << endl;
            _error = true;
        }
    }
}

bool ServerBlock::findName(const string line, const string wanted)
{
    size_t pos = line.find(wanted);
    if (pos != string::npos)
    {
        bool startCond = pos == 0 || isspace(line[pos - 1]);
        bool endCond = pos + wanted.length() == line.size() || isspace(line[pos + wanted.length()]);
        if (startCond && endCond)
        {
            return true;
        }
    }
    return false;
}

void ServerBlock::printDirectives() const
{
    vector<ADirective *>::const_iterator it = _directives.begin();
    for (; it != _directives.end(); ++it)
    {
        if (*it != nullptr)
            (*it)->print();
    }
    vector<LocationBlock *>::const_iterator it2 = _locationBlocks.begin();
    for (; it2 != _locationBlocks.end(); ++it2)
    {
        cout << YELLOW << "SERVERBLOCK locationPath: " << (*it2)->getLocationPath() << CLEAR << endl;
        (*it2)->printDirectives();
        (*it2)->printLocationBlockDirective();
    }
}
void ServerBlock::processBlock(const string &block, int lineNumber)
{
    string line = block;
    trim(line);
    addDirective(line);
    lineNumber++;
}

void ServerBlock::processLocationBlock(string &block, int lineNumber)
{
    LocationBlock *lb = new LocationBlock();
    lb->processBlock(block, lineNumber, "");
    _locationBlocks.push_back(lb);
}
string &ServerBlock::trim(string &s)
{
    size_t start = s.find_first_not_of(" \n\r\t\f\v");
    s = (start == std::string::npos) ? "" : s.substr(start);

    size_t end = s.find_last_not_of(" \n\r\t\f\v");
    s = (end == std::string::npos) ? "" : s.substr(0, end + 1);
    return s;
}
#pragma endregion