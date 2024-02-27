/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PythonCgiHandler.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/05 10:44:54 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PythonCgiHandler.hpp"

namespace PythonCgiHandler
{

	int executor(ClientConnexion &clientConnexion, pollfd &pollFd, Server &server, int i)
	{

		if (clientConnexion.getRequest().method == ClientConnexion::POST)
		{
			if (clientConnexion.getRequest().headers.find("Content-Length") != clientConnexion.getRequest().headers.end())
			{
				const long int uploadSize = strtol(clientConnexion.getRequest().headers.at("Content-Length").c_str(), nullptr, 10);
				const long int maxSize = clientConnexion.getRequest().route->getMaxBody().empty() ? LONG_MAX : strtol(clientConnexion.getRequest().route->getMaxBody().c_str(), nullptr, 10);
				if (uploadSize > 0L && uploadSize < LONG_MAX && uploadSize > LONG_MIN)
				{
					if (uploadSize > maxSize)
					{
						clientConnexion.setHeaderStream(Response::header413(clientConnexion));
						clientConnexion.setCgiIsInit(true);
						pollFd.events = POLLOUT;
					}
					else
					{
						clientConnexion.setKeepAlive(true);
						clientConnexion.setHeaderStream(Response::header100());
						clientConnexion.setCgiContinueToSend(true);
						pollFd.events |= POLLOUT;
					}
				}
				else
				{
					clientConnexion.setHeaderStream(Response::header400(clientConnexion, "Content-Length is invalid"));
					clientConnexion.setCgiIsInit(true);
					pollFd.events = POLLOUT;
				}
			}
			else
			{
				clientConnexion.setHeaderStream(Response::header411(clientConnexion));
				clientConnexion.setCgiIsInit(true);
				pollFd.events = POLLOUT;
			}
		}

		const string pythonFile = CheckFile::appendIndex(clientConnexion.getRequest().route->getRoot(), clientConnexion.getRequest().route->getIndex());
		if (pythonFile == "")
		{
			if (CGI_DEBUG)
				cerr << "No python file found" << endl;
			clientConnexion.setHeaderStream(Response::header404(clientConnexion));
			pollFd.events = POLLOUT;
			return 1;
		}

		clientConnexion.setCgiEnv(CgiEnv::getCgiEnv(clientConnexion));
		vector<const char *> env;
		for (vector<string>::const_iterator it = clientConnexion.getCgiEnv().begin(); it != clientConnexion.getCgiEnv().end(); ++it)
		{
			env.push_back((*it).c_str());
		}
		env.push_back(NULL);

		int tmp_pipeOut[2];
		int tmp_pipeIn[2];
		int tmp_pipeErr[2];
		if (CGI_DEBUG)
			cout << "Init CGI" << endl;

		CgiEnv::getCgiEnv(clientConnexion);

		if (clientConnexion.getCgi().pid == 0)
		{
			if (pipe(tmp_pipeOut) == -1)
			{
				if (CGI_DEBUG)
					cerr << "Pipe out failed: " << strerror(errno) << endl;
				clientConnexion.setHeaderStream(Response::header500(clientConnexion, "pipe out failed"));
				pollFd.events = POLLOUT;
				return 1;
			}
			clientConnexion.setCgiPipeOut(tmp_pipeOut);
			if (pipe(tmp_pipeErr) == -1)
			{
				if (CGI_DEBUG)
					cerr << "Pipe err failed: " << strerror(errno) << endl;
				clientConnexion.setHeaderStream(Response::header500(clientConnexion, "pipe err failed"));
				pollFd.events = POLLOUT;
				return 1;
			}
			clientConnexion.setCgiPipeErr(tmp_pipeErr);
			if (pipe(tmp_pipeIn) == -1)
			{
				if (CGI_DEBUG)
					cerr << "Pipe in failed: " << strerror(errno) << endl;
				clientConnexion.setHeaderStream(Response::header500(clientConnexion, "pipe in failed"));
				pollFd.events = POLLOUT;
				return 1;
			}
			clientConnexion.setCgiPipeIn(tmp_pipeIn);
			clientConnexion.setCgiPid(fork());
			clientConnexion.setCgiTimeAsForked(time(nullptr));
		}

		if (clientConnexion.getCgi().pid < 0)
		{
			if (CGI_DEBUG)
				cerr << "Fork failed: " << strerror(errno) << endl;
			clientConnexion.setHeaderStream(Response::header500(clientConnexion, "fork failed"));
			pollFd.events = POLLOUT;
			return 1;
		}

		// Parent process
		else if (clientConnexion.getCgi().pid > 0)
		{
			close(clientConnexion.getCgi().pipeOut[WRITE]);
			close(clientConnexion.getCgi().pipeErr[WRITE]);
			close(clientConnexion.getCgi().pipeIn[READ]);
			if (clientConnexion.getRequest().method != ClientConnexion::POST)
			{
				close(clientConnexion.getCgi().pipeIn[WRITE]);
				pollFd.events = POLLOUT;
				clientConnexion.setCgiIsInit(true);
				return 0;
			}

			clientConnexion.setCgiIsInit(true);
			return 0;
		}

		// child process
		else
		{
			close(clientConnexion.getCgi().pipeIn[WRITE]);
			close(clientConnexion.getCgi().pipeOut[READ]);
			close(clientConnexion.getCgi().pipeErr[READ]);
			dup2(clientConnexion.getCgi().pipeIn[READ], STDIN_FILENO);
			dup2(clientConnexion.getCgi().pipeOut[WRITE], STDOUT_FILENO);
			dup2(clientConnexion.getCgi().pipeErr[WRITE], STDERR_FILENO);

			char *args[] = {
				(char *)clientConnexion.getRequest().route->getPythonPath().c_str(),
				(char *)pythonFile.c_str(),
				nullptr};

			if (execve((char *)clientConnexion.getRequest().route->getPythonPath().c_str(), args, (char *const *)env.data()) == -1)
			{
				if (CGI_DEBUG)
					cerr << "execve failed: " << strerror(errno) << endl;
				exit(1);
			}
			close(clientConnexion.getCgi().pipeOut[WRITE]);
			close(clientConnexion.getCgi().pipeErr[WRITE]);
			exit(0);
		}
	}

	int handler(ClientConnexion &clientConnexion, pollfd &pollFd, Server &server, int i)
	{
		return executor(clientConnexion, pollFd, server, i);
	}
}
