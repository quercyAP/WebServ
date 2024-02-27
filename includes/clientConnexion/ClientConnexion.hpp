/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnexion.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:58:01 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/09 08:32:20 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTCONNEXION_HPP
#define CLIENTCONNEXION_HPP

#include "define.hpp"
#include "Config.hpp"
#include "Route.hpp"
class ClientConnexion
{
public:
	enum Method
	{
		GET = 0,
		HEAD,
		POST,
		PUT,
		DELETE,
		CONNECT,
		OPTIONS,
		TRACE,
		PATCH
	};
	typedef struct s_request
	{
		Method method;
		string target;	 // adrresse complette
		string url;		 // target sans query
		string routeUrl; // route choisi sans serverName
		string pathInfo; // tout ce qui a derierre
		string query;
		string version;
		time_t timeUploadBegan = 0;
		bool isUploaded = false;
		const Route *route;
		map<string, string> headers;

		s_request() : method((Method)0),
					  route(nullptr) {}
	} Request;
	typedef struct s_cgi
	{
		int pipeIn[2] = {-1, -1};
		int pipeOut[2] = {-1, -1};
		int pipeErr[2] = {-1, -1};
		vector<string> env;
		time_t timeAsForked = 0;
		bool continueToSend = false;
		pid_t pid;
		int cgiUploadByteRead = 0;
		bool isInit = false;
		bool isCgiError = false;
		bool isStdOut = false;
		bool isCgiFinish = false;
		bool cgiResponseHeaderParsed = false;
		int cgiResponseHeaderByteRead = 0;

		s_cgi() : pid(0) {}
	} Cgi;
	typedef struct s_headerParsing
	{
		int headerByteRead;
		bool methodIsParsed;
		bool targetIsParsed;
		bool versionIsParsed;
		bool keyValueIsParsed;
		bool routeIsChecked;
		bool staticMethodIsHandled;
		bool readyToUpload;
		long int bodyByteRead;
		s_headerParsing() : headerByteRead(0),
							methodIsParsed(false),
							targetIsParsed(false),
							versionIsParsed(false),
							keyValueIsParsed(false),
							routeIsChecked(false),
							staticMethodIsHandled(false),
							readyToUpload(false),
							bodyByteRead(0) {}

	} HeaderParsing;

	ClientConnexion(int clientFd, int serverFd, string clientIp, const Config *config);
	// void setResponseHeader(string header) { _responseHeader = header; }
	// string getResponseHeader() const { return _responseHeader; }

	void incHeaderByteRead() { _headerParsing.headerByteRead++; }
	int getHeaderByteRead() const { return _headerParsing.headerByteRead; }

	const Request &getRequest() const { return _request; }
	Cgi &getCgi() { return _cgi; }
	void setRequestMethod(Method method) { _request.method = method; }
	void setRequestTarget(string target) { _request.target = target; }
	void setRequestUrl(string url) { _request.url = url; }
	void setRequestRouteUrl(string routeUrl) { _request.routeUrl = routeUrl; }
	void setRequestPathInfo(string pathInfo) { _request.pathInfo = pathInfo; }
	void setRequestQuery(string query) { _request.query = query; }
	void setRequestVersion(string version) { _request.version = version; }
	void setRequestRoute(const Route *route) { _request.route = route; }
	void setRequestHeader(string key, string value) { _request.headers[key] = value; }
	void setCgiTimeAsForked(time_t timeAsForked) { _cgi.timeAsForked = timeAsForked; }
	time_t getCgiTimeAsForked() const { return _cgi.timeAsForked; }
	void setCgiPid(pid_t pid) { _cgi.pid = pid; }
	vector<string> &getCgiEnv() { return _cgi.env; }
	void setCgiEnv(vector<string> env)
	{
		for (vector<string>::const_iterator it = env.begin(); it != env.end(); ++it)
		{
			_cgi.env.push_back(*it);
		}
	}
	void setCgiUploadByteRead(int cgiUploadByteRead) { _cgi.cgiUploadByteRead = cgiUploadByteRead; }
	int getCgiUploadByteRead() const { return _cgi.cgiUploadByteRead; }
	void setTimeUploadBegan(time_t timeUploadBegan) { _request.timeUploadBegan = timeUploadBegan; }
	time_t getTimeUploadBegan() const { return _request.timeUploadBegan; }
	void setIsUploaded(bool isUploaded) { _request.isUploaded = isUploaded; }
	bool isUploaded() const { return _request.isUploaded; }
	void setCgiContinueToSend(bool continueToSend) { _cgi.continueToSend = continueToSend; }
	bool isCgiContinueToSend() const { return _cgi.continueToSend; }
	void setCgiPipeIn(int pipeIn[2])
	{
		_cgi.pipeIn[0] = pipeIn[0];
		_cgi.pipeIn[1] = pipeIn[1];
	}
	void setCgiPipeOut(int pipeOut[2])
	{
		_cgi.pipeOut[0] = pipeOut[0];
		_cgi.pipeOut[1] = pipeOut[1];
	}
	void setCgiPipeErr(int pipeErr[2])
	{
		_cgi.pipeErr[0] = pipeErr[0];
		_cgi.pipeErr[1] = pipeErr[1];
	}
	void setCgiIsInit(bool isInit) { _cgi.isInit = isInit; }
	void incCgiResponseHeaderByteRead() { _cgi.cgiResponseHeaderByteRead++; }
	void setCgiResponseHeaderParsed(bool cgiResponseHeaderParsed) { _cgi.cgiResponseHeaderParsed = cgiResponseHeaderParsed; }
	void setCgiError(bool isCgiError) { _cgi.isCgiError = isCgiError; }
	void setCgiStdOut(bool isStdOut) { _cgi.isStdOut = isStdOut; }
	void setCgiFinish(bool isCgiFinish) { _cgi.isCgiFinish = isCgiFinish; }
	bool isCgiError() const { return _cgi.isCgiError; }
	bool isStdOut() const { return _cgi.isStdOut; }
	bool isCgiFinish() const { return _cgi.isCgiFinish; }

	bool isCgiResponseHeaderParsed() const { return _cgi.cgiResponseHeaderParsed; }

	bool isHeaderParsed() const { return (_headerParsing.methodIsParsed && _headerParsing.targetIsParsed && _headerParsing.versionIsParsed && _headerParsing.keyValueIsParsed); }
	bool isMethodParsed() const { return _headerParsing.methodIsParsed; }
	bool isTargetParsed() const { return _headerParsing.targetIsParsed; }
	bool isVersionParsed() const { return _headerParsing.versionIsParsed; }
	bool isKeyValueParsed() const { return _headerParsing.keyValueIsParsed; }
	bool isRouteChecked() const { return _headerParsing.routeIsChecked; }
	bool isStaticMethodHandled() const { return _headerParsing.staticMethodIsHandled; }
	bool isReadyToUpload() const { return _headerParsing.readyToUpload; }
	long int getBodyByteRead() const { return _headerParsing.bodyByteRead; }

	void setMethodIsParsed(bool methodIsParsed) { _headerParsing.methodIsParsed = methodIsParsed; }
	void setTargetIsParsed(bool targetIsParsed) { _headerParsing.targetIsParsed = targetIsParsed; }
	void setVersionIsParsed(bool versionIsParsed) { _headerParsing.versionIsParsed = versionIsParsed; }
	void setKeyValueIsParsed(bool headersIsParsed) { _headerParsing.keyValueIsParsed = headersIsParsed; }
	void setRouteIsChecked(bool routeIsChecked) { _headerParsing.routeIsChecked = routeIsChecked; }
	void setStaticMethodIsHandled(bool staticMethodIsHandled) { _headerParsing.staticMethodIsHandled = staticMethodIsHandled; }
	void setReadyToUpload(bool readyToUpload) { _headerParsing.readyToUpload = readyToUpload; }
	void incBodyByteRead(long int size) { _headerParsing.bodyByteRead += size; }

	void setHeaderStream(istream *headerStream) { _headerStream = headerStream; }
	istream *getHeaderStream() const { return _headerStream; }
	void setTmpCgiOutStream(stringstream *tmpCgiOutStream) { _tmpCgiOutStream = tmpCgiOutStream; }
	stringstream *getTmpCgiOutStream() const { return _tmpCgiOutStream; }
	void setTmpCgiErrorStream(stringstream *tmpCgiErrorStream) { _tmpCgiErrorStream = tmpCgiErrorStream; }
	stringstream *getTmpCgiErrorStream() const { return _tmpCgiErrorStream; }
	void setDownloadStream(istream *inputStream) { _downloadStream = inputStream; }
	istream *getDownloadStream() const { return _downloadStream; }
	void setUploadStream(ostream *outputStream) { _uploadStream = outputStream; }
	ostream *getUploadStream() const { return _uploadStream; }
	inline static bool closeCon (){if(errno==EAGAIN)return false; return true;}
	const Config *getConfig() const { return _config; }
	void setKeepAlive(bool keepAlive) { _keepAlive = keepAlive; }
	bool isKeepAlive() const { return _keepAlive; }
	void setUploadName(string uploadName) { _uploadName = uploadName; }
	const string &getUploadName() const { return _uploadName; }
	const string &getClientIp() const { return _clientIp; }

	~ClientConnexion();

private:
	ClientConnexion();
	int _clientFd = -1;
	int _serverFd = -1;
	string _clientIp;
	const Config *_config;
	// string _responseHeader;
	bool _keepAlive = false;
	string _uploadName;
	stringstream *_tmpCgiOutStream = nullptr;
	stringstream *_tmpCgiErrorStream = nullptr;
	istream *_headerStream = nullptr;
	istream *_downloadStream = nullptr;
	ostream *_uploadStream = nullptr;
	Request _request;
	Cgi _cgi;
	HeaderParsing _headerParsing;
};

#endif
