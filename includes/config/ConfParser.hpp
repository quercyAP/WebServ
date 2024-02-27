/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/05 11:54:09 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFPARSER_HPP
#define CONFPARSER_HPP

#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <dirent.h>
#include "ADirective.hpp"
#include "ServerBlock.hpp"
#include "LocationBlock.hpp"
#include "Config.hpp"
#include <list>

using namespace std;

enum TokenType {
    KEYWORD,
    DIRECTIVE,
    ARGUMENT,
    SEMICOLON,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    END_OF_FILE,
    UNKNOWN
};

struct Token {
    TokenType type;
    string value;
    int lineNumber;
    Token(TokenType type, string value, int lineNumber) : type(type), value(value), lineNumber(lineNumber) {}
};

class ConfParser {
private:
    string _fileName;
    vector<ServerBlock*> _serverBlocks;
    vector<string> _keywords;
    vector<Token> _tokens;
    bool _error;
    bool findWithlength(const string line, const string wanted);
    void trim(string &s);
    vector<vector<string> >splitBlocks(string& str);
    vector<string> splitBlock(string &block);
    void processBlocks();
    void processBlock(const string &block, int &lineNumber, ServerBlock *serverBlock);
    vector<string> splitDirectives(const string& str);
    bool checkSyntaxErrors();
    vector<string> splitIntoLines(const string& str);
    string trimEnd(const string &str);
    vector<Token> lexConfigFile(const string& filename);
    void mergeServerConfigs(vector<Config> &Configs);
    void mergeConfigs(Config &conf, Config &conf2);
    
public:
    vector<Config> setServerConfig();
    void printConf(const vector<Config> &Configs);
    ConfParser(string fileName);
    ~ConfParser();
    void parse();
};

#endif
