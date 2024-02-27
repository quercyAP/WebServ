/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PythonOutCgiHandler.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 14:57:35 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/05 10:46:06 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PythonOutCgiHandler.hpp"

namespace PythonOutCgiHandler
{

	int handler(ClientConnexion &clientConnexion, pollfd &pollFd, Server &server, int pollIndex)
	{
		if (difftime(time(nullptr), clientConnexion.getCgiTimeAsForked()) > 15)
		{
			if (CGI_DEBUG)
				cerr << "Cgi timeout" << endl;
			clientConnexion.setHeaderStream(Response::header502(clientConnexion, "Python cgi timeout(15s): Python script took too long to execute and was killed."));
			kill(clientConnexion.getCgi().pid, SIGKILL);
			pollFd.events = POLLOUT;
			clientConnexion.setCgiFinish(true);
			return 0;
		}

		int tmp_status = 0;

		waitpid(clientConnexion.getCgi().pid, &tmp_status, WNOHANG);
		if (tmp_status != 0)
		{
			if (CGI_DEBUG)
				cerr << "Cgi Crashed: " << tmp_status << strerror(errno) << endl;
			clientConnexion.setHeaderStream(Response::header500(clientConnexion, "cgi exited with error status: " + to_string(tmp_status)));
			pollFd.events = POLLOUT;
			clientConnexion.setCgiFinish(true);
			return 0;
		}

#define BUFF_SIZE_CGI 16000
		char bufferOut[BUFF_SIZE_CGI] = {0};
		char bufferErr[BUFF_SIZE_CGI] = {0};
		int flagsErr = fcntl(clientConnexion.getCgi().pipeErr[READ], F_GETFL, 0);
		fcntl(clientConnexion.getCgi().pipeErr[READ], F_SETFL, flagsErr | O_NONBLOCK);
		int flagsOut = fcntl(clientConnexion.getCgi().pipeOut[READ], F_GETFL, 0);
		fcntl(clientConnexion.getCgi().pipeOut[READ], F_SETFL, flagsOut | O_NONBLOCK);
		int retOut = read(clientConnexion.getCgi().pipeOut[READ], bufferOut, BUFF_SIZE_CGI);
		int retErr = read(clientConnexion.getCgi().pipeErr[READ], bufferErr, BUFF_SIZE_CGI);

		if (retErr == 0 && clientConnexion.isCgiError() == true)
		{
			if (CGI_DEBUG)
				cout << "retErr == 0" << endl;
			clientConnexion.setHeaderStream(Response::header502(clientConnexion, clientConnexion.getTmpCgiErrorStream()->str()));
			delete clientConnexion.getTmpCgiErrorStream();
			clientConnexion.setCgiFinish(true);
			return 0;
		}

		if (retOut == 0 && clientConnexion.isStdOut() == true)
		{
			if (CGI_DEBUG)
				cout << "retOut OK == 0" << endl;
#define HEADER_SIZE_MAX 4096
			string header;
			vector<string> headers;
			char c = 0;
			int i = 0;
			while (42)
			{
				clientConnexion.getTmpCgiOutStream()->get(c);
				header.push_back(c);
				// cout << "cur: " << header << endl;
				if (header.find("\n\n") != string::npos || header.find("\r\n\r\n") != string::npos)
				{
					break;
				}

				i = i + clientConnexion.getTmpCgiOutStream()->gcount();
				// cout << "i: " << i << endl;
				if (i >= HEADER_SIZE_MAX)
				{
					if (CGI_DEBUG)
						cout << "header too big" << endl;
					clientConnexion.setHeaderStream(Response::header502(clientConnexion, "Python CGI Error: Header too big"));
					clientConnexion.setCgiFinish(true);
					return 0;
				}
			}
			stringstream ss(header);
			while (42)
			{
				string line;
				getline(ss, line);
				if (line.empty())
					break;
				headers.push_back(line);
			}
			stringstream ss2(headers[0]);
			string version;
			string code;
			ss2 >> version;
			if (version != "HTTP/1.1")
				version = "";
			ss2 >> code;
			long codeLong = strtol(code.c_str(), nullptr, 10);
			if (codeLong < 100L || codeLong > 599L)
				code = "";
			stringstream headerOut;
			if (code.empty() || version.empty())
				headerOut << "HTTP/1.1 200 OK\r\n";
			for (size_t i = 0; i < headers.size(); i++)
			{
				headerOut << headers[i] << "\r\n";
			}
			headerOut << "\r\n";
			clientConnexion.setHeaderStream(new stringstream(headerOut.str()));
			clientConnexion.setDownloadStream(clientConnexion.getTmpCgiOutStream());
			clientConnexion.setCgiFinish(true);
			return 0;
		}

		if (retOut == 0)
		{
			if (CGI_DEBUG)
				cout << "retOut No Ouptut == 0" << endl;
			clientConnexion.setHeaderStream(Response::header502(clientConnexion, "Python CGI Error: No output"));
			clientConnexion.setCgiFinish(true);
			return 0;
		}

		if (retErr > 0)
		{
			if (CGI_DEBUG)
			{
				cout << "retErr: " << retErr << endl;
				cout << "bufferErr: " << bufferErr << endl;
			}
			string str(bufferErr);
			if (clientConnexion.getTmpCgiErrorStream() == nullptr)
			{
				clientConnexion.setTmpCgiErrorStream(new stringstream());
				*clientConnexion.getTmpCgiErrorStream() << "Python CGI Error: " << endl;
				clientConnexion.setCgiError(true);
			}
			*clientConnexion.getTmpCgiErrorStream() << str << endl;
			return 1;
		}

		if (retOut > 0)
		{
			if (CGI_DEBUG)
			{
				cout << "retOut: " << retOut << endl;
				cout << "bufferOut: " << bufferOut << endl;
			}
			if (clientConnexion.getTmpCgiOutStream() == nullptr)
			{
				clientConnexion.setTmpCgiOutStream(new stringstream());
				clientConnexion.setCgiStdOut(true);
			}
			clientConnexion.getTmpCgiOutStream()->write(bufferOut, retOut);
		}
		if (retErr < 0 && errno == EAGAIN)
		{
			// cout << "Err not ready" << endl;
			return 1;
		}
		if (retOut < 0 && errno == EAGAIN)
		{
			// cout << "Out not ready" << endl;
			return 1;
		}
		if (CGI_DEBUG)
			cout << "rien " << retErr << " " << retOut << " " << strerror(errno) << endl;
		return 0;
	}

}
