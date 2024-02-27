/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP

#include "Config.hpp"
#include "ADirective.hpp"
#include "LocationBlock.hpp"
#include "AutoIndex.hpp"
#include "CgiPhp.hpp"
#include "CgiPy.hpp"
#include "Error.hpp"
#include "Index.hpp"
#include "LimitMethode.hpp"
#include "MaxBodySize.hpp"
#include "Redir.hpp"
#include "Listen.hpp"
#include "Root.hpp"
#include "Upload.hpp"
#include "ServerNames.hpp"
#include <set>

using namespace std;

class ServerBlock {
    private:
        Config _conf;
        vector<ADirective*>  _directives;
        vector<LocationBlock*>  _locationBlocks;
        set<string> _existingDirectives;
        bool _error;
        ADirective* directiveFactory(string line);
        bool findName(const string line, const string wanted);
        string& trim(string &s);
        void appendServerName(void);
    
    public:
        ServerBlock();
        ~ServerBlock();
        void addDirective(const string &Line);
        void printDirectives() const;
        void processBlock(const string& block, int lineNumber);
        void processLocationBlock(string &block, int lineNumber);
        Config &setConfig();
};

#endif
