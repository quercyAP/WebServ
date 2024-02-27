/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiEnv.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 14:09:22 by glamazer          #+#    #+#             */
/*   Updated: 2023/10/05 10:42:22 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiEnv.hpp"

vector<string> CgiEnv::getCgiEnv(ClientConnexion &client)
{
	vector<string> envp;
	map<string, string> headers = client.getRequest().headers;
	string method = getMethod(client);
	string scriptPath = CheckFile::appendIndex(client.getRequest().route->getRoot(), client.getRequest().route->getIndex());
	string scriptName = scriptPath.substr(scriptPath.find_last_of("/"), scriptPath.size());
	string serverName = headers["Host"].substr(0, headers["Host"].find(":"));
	string port = !headers["Host"].substr(headers["Host"].find(":") + 1).empty() ? headers["Host"].substr(headers["Host"].find(":") + 1) : "80";

	envp.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envp.push_back(("PATH_INFO=" + client.getRequest().pathInfo).c_str());
	envp.push_back(("PATH_TRANSLATED=" + getPathTranslate(client)).c_str());
	envp.push_back(("QUERY_STRING=" + client.getRequest().query).c_str());
	envp.push_back(("REMOTE_ADDR=" + client.getClientIp()).c_str());
	envp.push_back(("REQUEST_METHOD=" + method).c_str());
	envp.push_back(("SCRIPT_NAME=" + scriptName).c_str());
	envp.push_back(("SERVER_NAME=" + serverName).c_str());
	envp.push_back(("SERVER_PORT=" + port).c_str());
	envp.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envp.push_back("SERVER_SOFTWARE=webserv/1.0");

	for (map<string, string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
	{
		string str = it->first;
		transform(str.begin(), str.end(), str.begin(), ::toupper);
		envp.push_back(("HTTP_" + str + "=" + it->second));
	}

	return envp;
}

void CgiEnv::printEnv(vector<string> envp)
{
	for (size_t i = 0; i < envp.size(); i++)
	{
		if (ENV_DEBUG)
			cout << envp[i] << endl;
	}
	if (ENV_DEBUG)
		cout << CLEAR << endl;
}

string CgiEnv::convertString(const string &input)
{
	string result;

	for (size_t i = 0; i < input.size(); ++i)
	{
		if (input[i] == '%' && i + 2 < input.size())
		{
			char hex[3] = {input[i + 1], input[i + 2], '\0'};
			int value;
			if (sscanf(hex, "%2x", &value) == 1)
			{
				result.push_back(static_cast<char>(value));
				i += 2;
			}
			else
			{
				result.push_back('%');
			}
		}
		else
		{
			result.push_back(input[i]);
		}
	}

	return result;
}

const string CgiEnv::getPathTranslate(ClientConnexion &client)
{
	const string pathInfo = client.getRequest().pathInfo;
	if (pathInfo.empty() || pathInfo == "/")
		return "";
	const string root = client.getRequest().route->getRoot();

	const string ret = root + convertString(pathInfo);
	return ret;
}

string CgiEnv::getMethod(ClientConnexion &client)
{
	string method;
	switch (client.getRequest().method)
	{
	case 0:
		method = "GET";
		break;
	case 1:
		method = "HEAD";
		break;
	case 2:
		method = "POST";
		break;
	case 3:
		method = "PUT";
		break;
	case 4:
		method = "DELETE";
		break;
	case 5:
		method = "CONNECT";
		break;
	case 6:
		method = "OPTIONS";
		break;
	case 7:
		method = "TRACE";
		break;
	case 8:
		method = "PATCH";
		break;
	default:
		break;
	}
	return method;
}
