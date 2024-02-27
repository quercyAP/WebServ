/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/18 13:55:36 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <string>
#include <vector>
#include "Config.hpp"
#include <map>
#include <iostream>
#include "define.hpp"


class Config;

using namespace std;

class Route
{
private:
	string _name;
	bool _allowPost;
	bool _allowGet;
	bool _allowDelete;
	bool _isPythonCgi;
	string _pythonPath;
	bool _isPhpCgi;
	string _phpPath;
	bool _isAutoIndex;
	bool _isRedirected;

	string _maxBody;
	string _maxHeader;
	string _redirCode;
	vector<string> _redirUrls;
	string _root;
	string _locationPath;
	vector<string> _index;
	map<string, string> _errorPages;
	string _uploadStore;

	runtime_error _keyNotFoundError() const;

public:
	Route();
	Route(const Route &other);
	Route &operator=(const Route &other);
	~Route();

	Route &setPOST_allowed(bool allow);
	Route &setGET_allowed(bool allow);
	Route &setDELETE_allowed(bool allow);
	Route &setCgiPython(bool allow);
	Route &setPythonPath(const string path);
	Route &setCgiPhp(bool allow);
	Route &setPhpPath(const string path);
	Route &setAutoIndex(bool allow);

	Route &setMaxBody(const string maxBody);
	Route &setMaxHeader(const string maxHeader);
	Route &setRedirection(const string code, const string url);
	Route &setRedirections(const string code, const vector<string> urls);
	Route &setRoot(const string root);
	Route &setIndex(const vector<string> index);
	Route &setErrorPage(const string error, const string index);
	Route &setUploadStore(const string uploadStore);
	Route &setLocationPath(const string location);
	Route &setName(const string name);

	const bool POST_allowed() const;
	const bool GET_allowed() const;
	const bool DELETE_allowed() const;
	const bool hasPythonCgi() const;
	const string &getPythonPath() const;
	const bool hasPhpCgi() const;
	const string &getPhpPath() const;
	const bool isAutoIndex() const;
	const bool isRedirected() const;

	const string &getMaxBody() const;
	const string &getMaxHeader() const;
	const string &getRedirectionCode() const;
	const string &getRedirectionUrl() const;
	const vector<string> &getRedirectionUrls() const;
	const string &getRoot() const;
	const vector<string> &getIndex() const;
	const map<string, string> &getErrorPages() const;
	const string getErrorIndex(const string error) const;
	const string &getUploadStore() const;
	const string &getLocationPath() const;
	const string &getName() const;
};
ostream &operator<<(ostream &os, const Route &route);
#endif