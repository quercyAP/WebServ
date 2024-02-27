/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseHeader.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:37:01 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/09 08:33:42 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseHeader.hpp"

namespace ParseHeader
{
	namespace ParseMethod
	{
		enum ErrorCode
		{
			READ_OK,
			READ_ERROR,
			TOO_SHORT,
			TOO_LONG,
			METHOD_NOT_IMPLEMENTED,
		};
		ErrorCode read(int fd, ClientConnexion &clientConnexion)
		{
			string method;
			vector<string> methods = {"GET", "HEAD", "POST", "PUT", "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH"};
			char c;
			while (1)
			{
				int ret = recv(fd, &c, 1, MSG_DONTWAIT);
				if (ret == -1)
				{
					if (PARSE_HEADER_DEBUG)
						cout << "error code: READ_ERROR: " << strerror(errno) << endl;
					return READ_ERROR;
				}
				else if (ret == 0)
				{
					if (PARSE_HEADER_DEBUG)
						cout << "error code: TOO_SHORT" << endl;

					return TOO_SHORT;
				}
				// cout << "c: " << c << endl;
				clientConnexion.incHeaderByteRead();
				if (c == ' ')
				{
					int i = 0;
					for (vector<string>::const_iterator it = methods.begin(); it != methods.end(); it++)
					{
						if (method == *it)
						{
							clientConnexion.setRequestMethod((ClientConnexion::Method)(i));
							clientConnexion.setMethodIsParsed(true);
							if (PARSE_HEADER_DEBUG)
								cout << "READ_OK: Method " << methods[clientConnexion.getRequest().method]
									 << " headerBytesRead: " << clientConnexion.getHeaderByteRead()
									 << endl;

							return READ_OK;
						}
						i++;
					}
					if (PARSE_HEADER_DEBUG)
						cout << "error code: METHOD_NOT_IMPLEMENTED" << endl;

					return METHOD_NOT_IMPLEMENTED;
				}
				method.push_back(c);
				if (clientConnexion.getHeaderByteRead() > 4096)
				{
					if (PARSE_HEADER_DEBUG)
						cout << "error code: TOO_LONG" << endl;
					return TOO_LONG;
				}
			}
		}
	}

	namespace ParseTarget
	{
		enum ErrorCode
		{
			READ_OK,
			READ_ERROR,
			TOO_SHORT,
			TOO_LONG,
		};
		ErrorCode read(int fd, ClientConnexion &clientConnexion)
		{
			string target;
			char c;
			while (1)
			{
				int ret = recv(fd, &c, 1, MSG_DONTWAIT);
				if (ret == -1)
				{
					
					if (PARSE_HEADER_DEBUG)
						cout << "error code: READ_ERROR: " << strerror(errno) << endl;
					return READ_ERROR;
				}
				else if (ret == 0)
				{
					if (PARSE_HEADER_DEBUG)
						cout << "error code: TOO_SHORT" << endl;

					return TOO_SHORT;
				}
				// cout << "c: " << c << endl;
				clientConnexion.incHeaderByteRead();
				if (c == ' ')
				{

					clientConnexion.setRequestTarget(target);
					clientConnexion.setTargetIsParsed(true);
					size_t pos = target.find('?');
					if (pos != string::npos)
					{
						clientConnexion.setRequestUrl(target.substr(0, pos));
						clientConnexion.setRequestQuery(target.substr(pos + 1));
					}
					else
					{
						clientConnexion.setRequestUrl(target);
					}

					if (PARSE_HEADER_DEBUG)
						cout << "READ_OK: Target: " << clientConnexion.getRequest().target
							 << " url: " << clientConnexion.getRequest().url
							 << " query: " << clientConnexion.getRequest().query
							 << " headerBytesRead: " << clientConnexion.getHeaderByteRead()
							 << endl;

					return READ_OK;
				}
				target.push_back(c);
				if (clientConnexion.getHeaderByteRead() > 4096)
				{
					if (PARSE_HEADER_DEBUG)
						cout << "error code: TOO_LONG" << endl;

					return TOO_LONG;
				}
			}
		}
	}

	namespace ParseVersion
	{
		enum ErrorCode
		{
			READ_OK,
			READ_ERROR,
			TOO_SHORT,
			TOO_LONG,
			VERSION_NOT_SUPPORTED,
		};
		ErrorCode read(int fd, ClientConnexion &clientConnexion)
		{
			string version;
			char c;
			while (1)
			{
				int ret = recv(fd, &c, 1, MSG_DONTWAIT);
				if (ret == -1)
				{
					
					if (PARSE_HEADER_DEBUG)
						cout << "error code: READ_ERROR: " << strerror(errno) << endl;
					return READ_ERROR;
				}
				else if (ret == 0)
				{
					if (PARSE_HEADER_DEBUG)
						cout << "error code: TOO_SHORT" << endl;

					return TOO_SHORT;
				}
				// cout << "c: " << c << endl;
				clientConnexion.incHeaderByteRead();
				// if (c == '\n')
				version.push_back(c);
				if (version.find("\r\n") != string::npos)
				{
					version.pop_back();
					version.pop_back();
					if (version == "HTTP/1.1")
					{
						clientConnexion.setRequestVersion(version);
						clientConnexion.setVersionIsParsed(true);
						if (PARSE_HEADER_DEBUG)
							cout << "READ_OK: Version :" << clientConnexion.getRequest().version
								 << " headerBytesRead: " << clientConnexion.getHeaderByteRead() << endl;

						return READ_OK;
					}
					else
					{
						if (PARSE_HEADER_DEBUG)
							cout << "error code: VERSION_NOT_SUPPORTED" << endl;

						return VERSION_NOT_SUPPORTED;
					}
				}
				if (clientConnexion.getHeaderByteRead() > 4096)
				{
					if (PARSE_HEADER_DEBUG)
						cout << "error code: TOO_LONG" << endl;

					return TOO_LONG;
				}
			}
		}
	}

	namespace ParseHeaders
	{
		enum ErrorCode
		{
			READ_OK,
			READ_ERROR,
			TOO_SHORT,
			TOO_LONG,
			COLON_MISSING,
		};
		ErrorCode read(int fd, ClientConnexion &clientConnexion)
		{
			string buffer;
			string key;
			string value;
			char c;
			while (1)
			{
				int ret = recv(fd, &c, 1, MSG_DONTWAIT);
				if (ret == -1)
				{
					
					if (PARSE_HEADER_DEBUG)
						cout << "error code: READ_ERROR: " << strerror(errno) << endl;
					return READ_ERROR;
				}
				else if (ret == 0)
				{
					if (PARSE_HEADER_DEBUG)
						cout << "error code: TOO_SHORT" << endl;

					return TOO_SHORT;
				}
				// cout << "c: " << c << endl;
				clientConnexion.incHeaderByteRead();
				if (c == ' ' && key.empty())
				{
					if (buffer.back() == ':')
						buffer.pop_back();
					else
					{
						if (PARSE_HEADER_DEBUG)
							cout << "error code: COLON_MISSING" << endl;

						return COLON_MISSING;
					}
					key = buffer;
					buffer.clear();
				}
				else
				{
					buffer.push_back(c);
					if (buffer.find("\r\n") != string::npos && !key.empty())
					{
						buffer.pop_back();
						buffer.pop_back();
						value = buffer;
						buffer.clear();
						clientConnexion.setRequestHeader(key, value);
						if (PARSE_HEADER_DEBUG)
							cout << "READ_OK: Key: " << key << " Value: " << value << endl;

						key.clear();
						value.clear();
					}
					else if (buffer.find("\r\n") != string::npos)
					{
						clientConnexion.setKeyValueIsParsed(true);
						if (PARSE_HEADER_DEBUG)
							cout << "READ_OK" << endl;

						return READ_OK;
					}
				}
				if (clientConnexion.getHeaderByteRead() > 4096)
				{
					if (PARSE_HEADER_DEBUG)
						cout << "error code: TOO_LONG" << endl;

					return TOO_LONG;
				}
			}
		}
	}

	int parseHeader(ClientConnexion &clientConnexion, pollfd &pollfd, Server &server, int pollIndex)
	{
		if (PARSE_HEADER_DEBUG)
			cout << "# -- Method is parsed? " << clientConnexion.isMethodParsed() << endl;

		if (clientConnexion.isMethodParsed() == false)
		{
			switch (ParseHeader::ParseMethod::read(pollfd.fd, clientConnexion))
			{
			case ParseHeader::ParseMethod::READ_OK:
				break;
			case ParseHeader::ParseMethod::READ_ERROR:
				if (ClientConnexion::closeCon())
					server.closeClientConnexion(pollIndex);
				return 1;
			case ParseHeader::ParseMethod::TOO_SHORT:
				clientConnexion.setHeaderStream(Response::header400(clientConnexion, "Method parsing error"));
				pollfd.events = POLLOUT;
				return 1;
			case ParseHeader::ParseMethod::TOO_LONG:
				clientConnexion.setHeaderStream(Response::header400(clientConnexion, "Method parsing error"));
				pollfd.events = POLLOUT;
				return 1;
			case ParseHeader::ParseMethod::METHOD_NOT_IMPLEMENTED:
				clientConnexion.setHeaderStream(Response::header501(clientConnexion));
				pollfd.events = POLLOUT;
				return 1;
			default:
				break;
			}
		}

		if (PARSE_HEADER_DEBUG)
			cout << "# -- Target is parsed? " << clientConnexion.isTargetParsed() << endl;

		if (clientConnexion.isTargetParsed() == false)
		{
			switch (ParseHeader::ParseTarget::read(pollfd.fd, clientConnexion))
			{
			case ParseHeader::ParseTarget::READ_OK:
				break;
			case ParseHeader::ParseTarget::READ_ERROR:
				if (ClientConnexion::closeCon())
					server.closeClientConnexion(pollIndex);
				return 1;
			case ParseHeader::ParseTarget::TOO_SHORT:
				clientConnexion.setHeaderStream(Response::header400(clientConnexion, "Target parsing error"));
				pollfd.events = POLLOUT;
				return 1;
			case ParseHeader::ParseTarget::TOO_LONG:
				clientConnexion.setHeaderStream(Response::header400(clientConnexion, "Target parsing error"));
				pollfd.events = POLLOUT;
				return 1;
			default:
				break;
			}
		}

		if (PARSE_HEADER_DEBUG)
			cout << "# -- Version is parsed? " << clientConnexion.isVersionParsed() << endl;

		if (clientConnexion.isVersionParsed() == false)
		{
			switch (ParseHeader::ParseVersion::read(pollfd.fd, clientConnexion))
			{
			case ParseHeader::ParseVersion::READ_OK:
				break;
			case ParseHeader::ParseVersion::READ_ERROR:
				if (ClientConnexion::closeCon())
					server.closeClientConnexion(pollIndex);
				return 1;
			case ParseHeader::ParseVersion::TOO_SHORT:
				clientConnexion.setHeaderStream(Response::header400(clientConnexion, "Version parsing error"));
				pollfd.events = POLLOUT;
				return 1;
			case ParseHeader::ParseVersion::TOO_LONG:
				clientConnexion.setHeaderStream(Response::header400(clientConnexion, "Version parsing error"));
				pollfd.events = POLLOUT;
				return 1;
			case ParseHeader::ParseVersion::VERSION_NOT_SUPPORTED:
				clientConnexion.setHeaderStream(Response::header505(clientConnexion));
				pollfd.events = POLLOUT;
				return 1;
			default:
				break;
			}
		}
		if (PARSE_HEADER_DEBUG)
			cout << "# -- KeyValue is parsed? " << clientConnexion.isKeyValueParsed() << endl;

		if (clientConnexion.isKeyValueParsed() == false)
		{
			switch (ParseHeader::ParseHeaders::read(pollfd.fd, clientConnexion))
			{
			case ParseHeader::ParseHeaders::READ_OK:
				break;
			case ParseHeader::ParseHeaders::READ_ERROR:
				if (ClientConnexion::closeCon())
					server.closeClientConnexion(pollIndex);
				return 1;
			case ParseHeader::ParseHeaders::TOO_SHORT:
				clientConnexion.setHeaderStream(Response::header400(clientConnexion, "Header parsing error"));
				pollfd.events = POLLOUT;
				return 1;
			case ParseHeader::ParseHeaders::TOO_LONG:
				clientConnexion.setHeaderStream(Response::header400(clientConnexion, "Header parsing error"));
				pollfd.events = POLLOUT;
				return 1;
			case ParseHeader::ParseHeaders::COLON_MISSING:
				clientConnexion.setHeaderStream(Response::header400(clientConnexion, "Header parsing error, colon missing"));
				pollfd.events = POLLOUT;
				return 1;
			default:
				break;
			}
		}

		
		return 0;
	}
}
