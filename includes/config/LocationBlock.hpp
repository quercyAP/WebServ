/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationBlock.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONBLOCK_HPP
#define LOCATIONBLOCK_HPP

#include "ADirective.hpp"
#include "AutoIndex.hpp"
#include "CgiPhp.hpp"
#include "CgiPy.hpp"
#include "Error.hpp"
#include "Index.hpp"
#include "LimitMethode.hpp"
#include "MaxBodySize.hpp"
#include "Root.hpp"
#include "Redir.hpp"
#include "Upload.hpp"
#include <set>

using namespace std;

class LocationBlock
{
private:
    vector<ADirective *> _directives;
    bool _error;
    string _locationPath;
    string _locationBlock;
    vector<LocationBlock*> _locationBlocks;
    set<string> _existingDirectives;
    bool findName(const string line, const string wanted);
    bool isDirectory(const std::string& path);
    vector<string> splitDirectives(string &str);
    size_t findBlockEnd(const string& str, size_t start);

public:
    LocationBlock();
    ~LocationBlock();
    void addDirective(const string &directive);
    ADirective *directiveFactory(string line);
    const string &getLocationPath() const;
    const bool &getError() const;
    const vector<LocationBlock*> &getLocationBlocks() const;
    void printDirectives() const;
    string &trim(string & s);
    void processBlock(string& block, int lineNumber, const string& lastPath);
    void printLocationBlockDirective() const;
    void setLocationBlockConfig(Config &conf);
    void setConfig(Config &conf);
};
#endif
