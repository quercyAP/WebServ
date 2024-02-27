/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/09/19 12:11:00 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"
#pragma region[constructor]

Route::Route() : _allowPost(true),
				 _allowGet(true),
				 _allowDelete(true),
				 _isPythonCgi(false),
				 _isPhpCgi(false),
				 _isAutoIndex(true),
				 _isRedirected(false),
				 _maxHeader("8192"),
				 _root("./dist"),
				 _uploadStore("./dist/uploads")
{
	vector<string> index;
	index.push_back("index.html");
	_index = index;
}
Route::Route(const Route &other) : _allowPost(other._allowPost),
								   _allowGet(other._allowGet),
								   _allowDelete(other._allowDelete),
								   _isPythonCgi(other._isPythonCgi),
								   _pythonPath(other._pythonPath),
								   _isPhpCgi(other._isPhpCgi),
								   _phpPath(other._phpPath),
								   _isAutoIndex(other._isAutoIndex),
								   _isRedirected(other._isRedirected),
								   _maxBody(other._maxBody),
								   _maxHeader(other._maxHeader),
								   _redirCode(other._redirCode),
								   _redirUrls(other._redirUrls),
								   _root(other._root),
								   _index(other._index),
								   _errorPages(other._errorPages),
								   _uploadStore(other._uploadStore),
								   _locationPath(other._locationPath),
								   _name(other._name)
{
}

Route &Route::operator=(const Route &other)
{
	_allowPost = other._allowPost;
	_allowGet = other._allowGet;
	_allowDelete = other._allowDelete;
	_isPythonCgi = other._isPythonCgi;
	_pythonPath = other._pythonPath;
	_isPhpCgi = other._isPhpCgi;
	_phpPath = other._phpPath;
	_isAutoIndex = other._isAutoIndex;
	_isRedirected = other._isRedirected;
	_maxBody = other._maxBody;
	_maxHeader = other._maxHeader;
	_redirCode = other._redirCode;
	_redirUrls = other._redirUrls;
	_root = other._root;
	_index = other._index;
	_errorPages = other._errorPages;
	_uploadStore = other._uploadStore;
	_locationPath = other._locationPath;
	_name = other._name;
	return *this;
}

Route::~Route() {}

#pragma endregion
#pragma region[setter]

Route &Route::setPOST_allowed(bool allow)
{
	_allowPost = allow;
	return *this;
}
Route &Route::setGET_allowed(bool allow)
{
	_allowGet = allow;
	return *this;
}
Route &Route::setDELETE_allowed(bool allow)
{
	_allowDelete = allow;
	return *this;
}
Route &Route::setCgiPython(bool allow)
{
	_isPythonCgi = allow;
	return *this;
}
Route &Route::setPythonPath(const string path)
{
	_pythonPath = path;
	return *this;
}
Route &Route::setCgiPhp(bool allow)
{
	_isPhpCgi = allow;
	return *this;
}
Route &Route::setPhpPath(const string path)
{
	_phpPath = path;
	return *this;
}
Route &Route::setAutoIndex(bool allow)
{
	_isAutoIndex = allow;
	return *this;
}

Route &Route::setMaxBody(const string maxBody)
{
	_maxBody = maxBody;
	return *this;
}

Route &Route::setMaxHeader(const string maxHeader)
{
	_maxHeader = maxHeader;
	return *this;
}

Route &Route::setRedirection(const string code, const string url)
{
	_isRedirected = true;
	_redirCode = code;
	_redirUrls.push_back(url);
	return *this;
}
Route &Route::setRedirections(const string code, const vector<string> urls)
{
	_isRedirected = true;
	_redirCode = code;
	_redirUrls = urls;
	return *this;
}

Route &Route::setRoot(const string root)
{
	_root = root;
	return *this;
}
Route &Route::setIndex(const vector<string> index)
{
	_index = index;
	return *this;
}
Route &Route::setErrorPage(const string error, const string index)
{
	_errorPages[error] = index;
	return *this;
}
Route &Route::setUploadStore(const string uploadStore)
{
	_uploadStore = uploadStore;
	return *this;
}
Route &Route::setLocationPath(const string locationPath)
{
	_locationPath = locationPath;
	return *this;
}
Route &Route::setName(const string name)
{
	_name = name;
	return *this;
}
#pragma endregion
#pragma region[getter]
const bool Route::POST_allowed() const
{
	return _allowPost;
}
const bool Route::GET_allowed() const { return _allowGet; }
const bool Route::DELETE_allowed() const { return _allowDelete; }
const bool Route::hasPythonCgi() const { return _isPythonCgi; }
const string &Route::getPythonPath() const { return _pythonPath; }
const bool Route::hasPhpCgi() const { return _isPhpCgi; }
const string &Route::getPhpPath() const { return _phpPath; }
const bool Route::isAutoIndex() const { return _isAutoIndex; }
const bool Route::isRedirected() const { return _isRedirected; }

const string &Route::getMaxBody() const { return _maxBody; }
const string &Route::getMaxHeader() const { return _maxHeader; }
const string &Route::getRedirectionCode() const { return _redirCode; }
const string &Route::getRedirectionUrl() const { return _redirUrls.at(0); }
const vector<string> &Route::getRedirectionUrls() const { return _redirUrls; }
const string &Route::getRoot() const { return _root; }
const vector<string> &Route::getIndex() const { return _index; }
const map<string, string> &Route::getErrorPages() const { return _errorPages; }
const string &Route::getLocationPath() const { return _locationPath; }

const string Route::getErrorIndex(const string error) const
{
	if (_errorPages.find(error) == _errorPages.end())
		return string();
	return (_errorPages.at(error));
}

const string &Route::getUploadStore() const { return _uploadStore; }
const string &Route::getName() const { return _name; }
#pragma endregion
#pragma region[overload]

ostream &operator<<(ostream &os, const Route &route)
{
	os << RED << "***** Route *****" << endl
	   << "name: " << route.getName() << endl
	   << BLUE << "GET: " << (route.GET_allowed() ? "allowed" : "disallowed") << endl
	   << "POST: " << (route.POST_allowed() ? "allowed" : "disallowed") << endl
	   << "DELETE: " << (route.DELETE_allowed() ? "allowed" : "disallowed") << endl
	   << "python CGI: " << boolalpha << route.hasPythonCgi() << endl
	   << "php CGI: " << route.hasPhpCgi() << endl
	   << "auto index: " << route.isAutoIndex() << endl
	   << "max body: " << (route.getMaxBody().empty() ? "unlimited" : route.getMaxBody()) << endl
	   << "root: " << route.getRoot() << endl
	   << "index: ";
	for (int i = 0; i < route.getIndex().size(); i++)
		os << route.getIndex()[i] + " ";
	os << endl;
	const map<string, string> errorPages = route.getErrorPages();
	for (map<string, string>::const_iterator it = errorPages.begin(); it != errorPages.end(); it++)
	{
		os << "error Pages " << it->first << " : index : " << it->second << endl;
	}
	if (route.isRedirected())
	{
		const vector<string> urls = route.getRedirectionUrls();
		os << "redirection code: " << route.getRedirectionCode() << endl;
		for (vector<string>::const_iterator it = urls.begin(); it != urls.end(); it++)
		{
			os << "redirection url: " << *it << endl;
		}
	}
	os << CLEAR << endl;
	return os;
}
#pragma endregion
#pragma region[exception]

runtime_error Route::_keyNotFoundError() const
{
	return (runtime_error("Key not found"));
}
#pragma endregion
