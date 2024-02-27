/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/09 08:54:40 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfParser.hpp"

#pragma region[constructor]
ConfParser::ConfParser(string fileName) : _fileName(fileName), _error(false)
{

	DIR *dir = opendir("dist");
	if (!dir)
	{
		mkdir("dist", 0755);
	}
	else
	{
		closedir(dir);
	}
	_keywords.push_back("server");
	_keywords.push_back("location");
	parse();
}
ConfParser::~ConfParser()
{
	vector<ServerBlock *>::iterator it = _serverBlocks.begin();
	for (; it != _serverBlocks.end(); ++it)
	{
		if (*it != nullptr)
			delete *it;
	}
}
#pragma endregion
#pragma region[getter]
#pragma endregion
#pragma region[method]
bool checkMultiPort(vector<Config> configs)
{
	for (int i = 0; i < configs.size(); i++)
	{
		for (int j = i + 1; j < configs.size(); j++)
		{
			if (configs[i].port == configs[j].port)
				return true;
		}
	}
	return false;
}
vector<Config> ConfParser::setServerConfig()
{
	vector<Config> serverConfigs;
	for (int i = 0; i < _serverBlocks.size(); i++)
	{
		serverConfigs.push_back(_serverBlocks[i]->setConfig());
	}
	
	mergeServerConfigs(serverConfigs);
	for (int i = 0; i < serverConfigs.size(); i++)
	{
		map<string, Route>::iterator it = serverConfigs[i].routes.begin();
		for (; it != serverConfigs[i].routes.end(); it++)
		{
			it->second.setName(it->first);
		}
	}
	if (checkMultiPort(serverConfigs))
	{
		throw runtime_error("Error: Multiple server blocks with the same port");
	}
	return serverConfigs;
}
void ConfParser::mergeServerConfigs(vector<Config> &Configs)
{
	for (size_t i = 0; i < Configs.size(); i++)
	{
		for (size_t j = i + 1; j < Configs.size();)
		{
			if (Configs[i].port == Configs[j].port && (Configs[i].host_ip == Configs[j].host_ip || Configs[j].host_ip == "0.0.0.0"))
			{
				mergeConfigs(Configs[j], Configs[i]);
				Configs.erase(Configs.begin() + j);
			}
			else
			{
				++j;
			}
		}
	}
}
void ConfParser::mergeConfigs(Config &conf, Config &conf2)
{
	map<string, Route> routes = conf.routes;
	map<string, Route>::iterator it = routes.begin();
	while (it != routes.end())
	{
		if (it->first == "/" || conf2.routes.find(it->first) != conf2.routes.end())
		{
			it++;
			continue;
		}
		conf2.routes[it->first] = it->second;
		it++;
	}
}
void ConfParser::parse()
{
	processBlocks();
}
void ConfParser::processBlocks()
{
	int lineNumber = 0;
	ifstream file(_fileName.c_str());
	if (!file)
		throw runtime_error("parseConfFile: File not found");
	string str((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	if (str.size() == 0)
		throw runtime_error("parseConfFile: File empty");
	lexConfigFile(_fileName);
	if (!checkSyntaxErrors())
	{
		throw runtime_error("Syntax Error: check your config file");
	}
	vector<vector<string>> blocksNested = splitBlocks(str);
	vector<vector<string>>::iterator it = blocksNested.begin();
	for (; it != blocksNested.end(); ++it)
	{
		ServerBlock *serverBlock = new ServerBlock;
		processBlock((*it)[0], lineNumber, serverBlock);
		for (size_t i = 1; i < (*it).size(); i++)
		{
			serverBlock->processLocationBlock((*it)[i], lineNumber);
		}
		if (WEBDEBUG)
			serverBlock->printDirectives();
		_serverBlocks.push_back(serverBlock);
	}
}

void ConfParser::processBlock(const string &block, int &lineNumber, ServerBlock *serverBlock)
{
	vector<string> directives = splitDirectives(block);

	vector<string>::const_iterator it = directives.begin();
	for (; it != directives.end(); ++it)
	{
		serverBlock->processBlock(*it, lineNumber);
	}
}
vector<string> ConfParser::splitDirectives(const string &str)
{
	vector<string> directives;
	string directive;
	stringstream ss(str);
	string line;

	while (getline(ss, line, ';'))
	{
		line.erase(remove(line.begin(), line.end(), '{'), line.end());
		line.erase(remove(line.begin(), line.end(), '}'), line.end());

		if (findWithlength(line, "server"))
		{
			line.erase(line.find("server"), string("server").length());
		}

		directive = line;
		directives.push_back(directive);
	}

	return directives;
}
bool ConfParser::checkSyntaxErrors()
{
	stack<Token> blockStack;
	bool semicolonExpected = false;

	vector<Token>::const_iterator it = _tokens.begin();
	for (; it != _tokens.end(); ++it)
	{
		switch ((*it).type)
		{
		case KEYWORD:
			if (semicolonExpected)
			{
				cerr << "Syntax Error: line " << (*it).lineNumber << ": Expected ';'" << endl;
				return false;
			}

			if (!blockStack.empty() && blockStack.top().value == "server" && (*it).value == "server")
			{
				cerr << "Syntax Error: line " << (*it).lineNumber << ": Nested server block" << endl;
				return false;
			}
			blockStack.push((*it));
			semicolonExpected = false;
			break;
		case DIRECTIVE:
			if (blockStack.empty())
			{
				cerr << "Syntax Error: line " << (*it).lineNumber << ": Directive \"" << (*it).value << "\" outside of any block" << endl;
				return false;
			}
			semicolonExpected = true;
			break;
		case OPEN_BRACKET:
			if (semicolonExpected)
			{
				cerr << "Syntax Error: line " << (*it).lineNumber << ": '{' without preceding server or location keyword or expected ';'" << endl;
				return false;
			}
			break;
		case CLOSE_BRACKET:
			if (semicolonExpected)
			{
				cerr << "Syntax Error: line " << (*it).lineNumber << ": Expected ';'" << endl;
				return false;
			}
			if (blockStack.empty())
			{
				cerr << "Syntax Error: line " << (*it).lineNumber << ": Unmatched '}'" << endl;
				return false;
			}
			blockStack.pop();
			semicolonExpected = false;
			break;
		case SEMICOLON:
			if (!semicolonExpected)
			{
				cerr << "Syntax Error: line " << (*it).lineNumber << ": Unexpected ';'" << endl;
				return false;
			}
			semicolonExpected = false;
			break;
		case END_OF_FILE:
			if (!blockStack.empty())
			{
				cerr << "Syntax Error: EOF: " << blockStack.top().value << " block is not closed" << endl;
				return false;
			}
			break;
		case UNKNOWN:
			cerr << "Syntax Error: line " << (*it).lineNumber << ": Unknown token" << endl;
			return false;
		case ARGUMENT:
			continue;
		}
	}

	return true;
}

vector<Token> ConfParser::lexConfigFile(const string &filename)
{
	ifstream file(filename);
	int i = 0;
	string word;
	char c;
	int lineNumber = 1;
	bool isDirective = false;
	bool isLocPath = false;
	bool isComment = false;

	while (file.get(c))
	{
		if (c == '\n')
		{
			lineNumber++;
			isComment = false;
		}
		else if (isComment)
		{
			continue;
		}
		else if (isspace(c))
		{
			if (!word.empty())
			{
				if (isLocPath)
				{
					_tokens.push_back(Token(ARGUMENT, word, lineNumber));
					isLocPath = false;
				}
				else
				{
					bool what = find(_keywords.begin(), _keywords.end(), word) != _keywords.end();
					if (what)
					{
						if (word == "location")
							isLocPath = true;
						_tokens.push_back(Token(KEYWORD, word, lineNumber));
					}
					else
					{
						_tokens.push_back(Token(DIRECTIVE, word, lineNumber));
					}
				}
				word.clear();
			}
		}
		else if (c == ';')
		{
			if (word.length() >= 1)
			{
				word.pop_back();
				_tokens.push_back(Token(DIRECTIVE, word, lineNumber));
				word.clear();
			}
			_tokens.push_back(Token(SEMICOLON, ";", lineNumber));
		}
		else if (c == '{')
		{
			_tokens.push_back(Token(OPEN_BRACKET, "{", lineNumber));
		}
		else if (c == '}')
		{
			_tokens.push_back(Token(CLOSE_BRACKET, "}", lineNumber));
		}
		else if (c == '#')
		{
			isComment = true;
		}
		else
		{
			word.push_back(c);
		}
	}

	if (!word.empty())
	{
		if (find(_keywords.begin(), _keywords.end(), word) != _keywords.end())
			_tokens.push_back(Token(KEYWORD, word, lineNumber));
		else
			_tokens.push_back(Token(DIRECTIVE, word, lineNumber));
	}

	_tokens.push_back(Token(END_OF_FILE, "", lineNumber));

	return _tokens;
}

void ConfParser::trim(string &s)
{
	size_t start = s.find_first_not_of(" \n\r\t\f\v");
	s = (start == string::npos) ? "" : s.substr(start);

	size_t end = s.find_last_not_of(" \n\r\t\f\v");
	s = (end == string::npos) ? "" : s.substr(0, end + 1);
}
string ConfParser::trimEnd(const string &str)
{
	size_t endpos = str.find_last_not_of(" \t\n\r");
	return (endpos == string::npos) ? "" : str.substr(0, endpos + 1);
}
bool ConfParser::findWithlength(const string line, const string wanted)
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
vector<string> ConfParser::splitIntoLines(const string &str)
{
	vector<string> lines;
	string line;
	stringstream ss(str);

	while (getline(ss, line, '\n'))
	{
		lines.push_back(line);
	}
	return lines;
}
vector<vector<string>> ConfParser::splitBlocks(string &str)
{
	vector<vector<string>> blocks;
	string block, locationBlock;
	vector<string> locationBlocks;
	int depth = 0;
	bool isLocationBlock = false;
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] == '#')
		{
			while (i < str.length() && str[i] != '\n')
				++i;
		}
		if (str[i] == '{')
			++depth;
		if (str[i] == '}')
			--depth;
		if (isLocationBlock)
		{
			locationBlock += str[i];
			if (depth == 1 && str[i] == '}')
			{
				locationBlocks.push_back(locationBlock);
				locationBlock.clear();
				isLocationBlock = false;
			}
		}
		else
		{
			block += str[i];
			if (depth == 2 && block.find("location") != string::npos && str[i] == '{')
			{
				isLocationBlock = true;
				string locationPath = block.substr(block.find("location") + 9);
				locationBlock += "location " + locationPath;
				block.erase(block.find("location"));
			}
			else if (depth == 0 && str[i] == '}')
			{
				vector<string> serverBlocks(splitBlock(block));
				vector<string>::iterator it = locationBlocks.begin();
				for (; it != locationBlocks.end(); ++it)
				{
					serverBlocks.push_back(*it);
				}
				blocks.push_back(serverBlocks);
				block.clear();
			}
		}
	}
	return blocks;
}

vector<string> ConfParser::splitBlock(string &block)
{
	vector<string> directives;
	string directive;
	int depth = 0;
	for (size_t i = 0; i < block.length(); ++i)
	{
		if (block[i] == '{')
			++depth;
		if (block[i] == '}')
			--depth;
		directive += block[i];
		if (depth == 0 && (block[i] == '}' || block[i] == ';'))
		{
			directives.push_back(directive);
			directive.clear();
		}
	}
	return directives;
}
void ConfParser::printConf(const vector<Config> &Configs)
{
	if (WEBDEBUG)
	{
		for (int i = 0; i < Configs.size(); i++)
		{
			map<string, Route> routes = Configs[i].routes;
			map<string, Route>::iterator it = routes.begin();
			for (; it != routes.end(); it++)
			{
				cout << it->second;
			}
		}
	}
}

#pragma endregion
