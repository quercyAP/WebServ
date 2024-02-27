/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/19 11:01:59 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationBlock.hpp"

#pragma region[constructor]
LocationBlock::LocationBlock() : _error(false) {}
LocationBlock::~LocationBlock()
{
    vector<LocationBlock *>::iterator it2 = _locationBlocks.begin();
    for (; it2 != _locationBlocks.end(); ++it2)
    {
        if (*it2 != nullptr)
            delete *it2;
    }
    vector<ADirective *>::iterator it = _directives.begin();
    for (; it != _directives.end(); ++it)
    {
        if (*it != nullptr)
            delete *it;
    }
}
#pragma endregion
#pragma region[getter]
const vector<LocationBlock *> &LocationBlock::getLocationBlocks() const
{
    return _locationBlocks;
}
const string &LocationBlock::getLocationPath() const
{
    return _locationPath;
}
const bool &LocationBlock::getError() const
{
    return _error;
}
#pragma endregion
#pragma region[method]
ADirective *LocationBlock::directiveFactory(string line)
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
    else if (findName(line, "limit_exept"))
        return new LimitMethode(line);
    else if (findName(line, "return"))
        return new Redir(line);
    else if (findName(line, "root"))
        return new Root(line);
    else if (findName(line, "upload_store"))
        return new Upload(line);
    else if (!findName(line, "location") && !findName(line, "server") && line.find("{") == string::npos && line.find("}") == string::npos)
    {
        _error = true;
        cerr << "Unknown or unsupported directive: " << line << endl;
    }
    return nullptr;
}
void LocationBlock::processBlock(string &block, int lineNumber, const string &lastPath)
{
    // Extraire le chemin de location
    if (size_t pos = block.find("location") != string::npos)
    {
        size_t start = pos + 8;
        size_t end = block.find_first_of("{", start);
        if (end != string::npos)
        {
            string tmp = block.substr(start, end - start);
            trim(tmp);
            if (tmp.empty())
            {
                _error = true;
                cerr << "Error: location path is empty" << endl;
                return;
            }
            if (tmp == "/" && !lastPath.empty())
            {
                _error = true;
                cerr << "Error: location path '/' in locationBlock is invalid and useless" << endl;
                return;
            }
            if (tmp.back() == '/')
                tmp.pop_back();
            _locationPath = lastPath + tmp;
            block.erase(0, end);
        }
    }

    // Ajoutez des directives pour le bloc de location actuel (c'est-à-dire this)
    vector<string> directives = splitDirectives(block);
    vector<string>::iterator it = directives.begin();
    for (; it != directives.end(); ++it)
    {
        trim((*it));
        addDirective((*it));
        lineNumber++;
    }

    // Recherche des blocs 'location' imbriqués et créez un nouvel objet LocationBlock pour chacun
    size_t pos = 0;
    while ((pos = block.find("location", pos)) != string::npos)
    {
        // Trouver le début et la fin du bloc 'location' actuel
        size_t start = block.find("{", pos);
        size_t end = findBlockEnd(block, start);

        // Créer un nouvel objet LocationBlock pour traiter ce bloc
        LocationBlock *lb = new LocationBlock();

        // Extraire le bloc 'location' et le traiter
        string nestedBlock = block.substr(pos, end - pos + 1);
        lb->processBlock(nestedBlock, lineNumber, _locationPath);

        // Ajouter le nouveau bloc à la liste
        _locationBlocks.push_back(lb);

        // Supprimer le bloc 'location' traité
        block.erase(pos, end - pos + 1);
    }
}

vector<string> LocationBlock::splitDirectives(string &str)
{
    string locBlock;
    // Trouver et supprimer le premier {
    size_t start = str.find_first_of("{");
    if (start != string::npos)
        str.erase(start, 1);

    // Trouver et supprimer le dernier }
    size_t end = str.find_last_of("}");
    if (end != string::npos)
        str.erase(end, 1);
    // Trouver et conserver les bloc de location imbriquer
    size_t locPos = str.find("location");
    if (locPos != string::npos)
    {
        size_t start = str.find("{");
        size_t end = str.find_last_of("}");
        locBlock = str.substr(locPos, end - locPos + 1);
        str = str.substr(0, locPos) + str.substr(locPos + locBlock.size());
    }
    vector<string> directives;
    string directive;
    string line;
    stringstream ss(str);
    while (getline(ss, line, ';'))
    {
        line.erase(remove(line.begin(), line.end(), '{'), line.end());
        line.erase(remove(line.begin(), line.end(), '}'), line.end());

        directive = line;
        directives.push_back(directive);
    }
    str = locBlock.empty() ? str : locBlock;
    return directives;
}
size_t LocationBlock::findBlockEnd(const string &str, size_t start)
{
    stack<char> brackets;
    brackets.push('{'); // compte le '{' initial
    for (size_t i = start + 1; i < str.size(); ++i)
    {
        if (str[i] == '{')
        {
            brackets.push('{');
        }
        else if (str[i] == '}')
        {
            brackets.pop();
            if (brackets.empty())
            {
                return i;
            }
        }
    }
    throw runtime_error("Block does not end");
}

void LocationBlock::addDirective(const string &Line)
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
bool LocationBlock::findName(const string line, const string wanted)
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
void LocationBlock::printDirectives() const
{
    vector<ADirective *>::const_iterator it = _directives.begin();
    for (; it != _directives.end(); ++it)
    {
        if (*it != nullptr)
            (*it)->print();
    }
}
void LocationBlock::printLocationBlockDirective() const
{
    for (vector<LocationBlock *>::const_iterator it = _locationBlocks.begin(); it != _locationBlocks.end(); ++it)
    {
        cout << RED << "LOCATIONBLOCK locationPath: " << (*it)->getLocationPath() << CLEAR << endl;
        (*it)->printDirectives();
        (*it)->printLocationBlockDirective();
    }
}
string &LocationBlock::trim(string &s)
{
    size_t start = s.find_first_not_of(" \n\r\t\f\v");
    s = (start == std::string::npos) ? "" : s.substr(start);

    size_t end = s.find_last_not_of(" \n\r\t\f\v");
    s = (end == std::string::npos) ? "" : s.substr(0, end + 1);
    return s;
}
bool LocationBlock::isDirectory(const std::string &path)
{
    struct stat buf;
    if (stat(path.c_str(), &buf) != 0)
        return false;
    return S_ISDIR(buf.st_mode);
}
void LocationBlock::setConfig(Config &conf)
{
    if (_error)
        throw runtime_error("Directive Error in LocationBlock");
    vector<ADirective *>::const_iterator it = _directives.begin();
    for (; it != _directives.end(); ++it)
    {
        if (*it != nullptr)
        {
            if ((*it)->getError())
            {
                _error = true;
                return;
            }
            (*it)->execute(conf, _locationPath);
        }
    }
}
void LocationBlock::setLocationBlockConfig(Config &conf)
{
    if (_error)
        throw runtime_error("Directive Error in LocationBlock");
    for (vector<LocationBlock *>::const_iterator it = _locationBlocks.begin(); it != _locationBlocks.end(); ++it)
    {
        conf.routes[(*it)->getLocationPath()] = conf.routes[_locationPath];
        (*it)->setConfig(conf);
        if ((*it)->getError())
        {
            _error = true;
            return;
        }
        (*it)->setLocationBlockConfig(conf);
    }
}
#pragma endregion
