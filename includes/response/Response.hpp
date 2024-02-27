/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdechand <tdechand@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by tdechand          #+#    #+#             */
/*   Updated: 2023/10/04 15:27:01 by tdechand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include "define.hpp"
#include "ClientConnexion.hpp"
#include "CheckFile.hpp"
#include "MimesTypes.hpp"

namespace Response
{
	namespace Tools
	{
		inline string generateHtmlMultRedirToString(vector<string> const &urls)
		{
			stringstream ss;
			ss << "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">" << endl
			   << "<html>" << endl
			   << "<head>" << endl
			   << "<title> 300 Multiple Choices</title>" << endl
			   << "</head>" << endl
			   << "<body>" << endl
			   << "<h1>Multiple Choice</h1>" << endl
			   << "<ul>" << endl;

			for (vector<string>::const_iterator it = urls.begin(); it != urls.end(); it++)
			{
				ss << "<li>" << endl
				   << "<a href=\"" << *it << "\">" << *it << "</a>" << endl;
			}
			ss << "</ul>" << endl
			   << "</body>" << endl
			   << "</html>" << endl;
			return ss.str();
		}
		inline string getDateNowToString()
		{
			time_t now = time(0);
			tm *gmtm = gmtime(&now);
			char *date = asctime(gmtm);
			return (string(date) + " GMT");
		}
		inline string generateDefaultHtmlError(const string &error, const string &message)
		{
			stringstream ss;

			ss << "<!DOCTYPE html>"<<endl
<<"<html lang=\"en\">"<<endl
<<"<head>"<<endl
    <<"<meta charset=\"UTF-8\">"<<endl
   <<"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"<<endl
    <<"<title>Erreur "<<error<<"</title>"<<endl
   << "<style>"<<endl
        <<"body {font-family: Arial, sans-serif; text-align: center; background-color: #f0f0f0;}"<<endl
		<<".container {margin: 100px auto; padding: 20px; background-color: #fff; border-radius: 5px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.2);}"<<endl
      <<"h1 {color: #f00;}"<<endl
   <<"</style>"<<endl
<<"</head>"<<endl
<<"<body>"<<endl
   <<"<div class=\"container\">"<<endl
        <<"<h1>Erreur "<<error<<"</h1>"<<endl
        <<"<p style=\"white-space: pre-line\">"<<message<<"</p>"<<endl
        <<"<p><a href=\"/\">Retour à la page d\'accueil</a></p>"<<endl
   <<"</div>"<<endl
<<"</body>"<<endl
<<"</html>"<<endl;

			// ss << "<!DOCTYPE html>" << endl
			//    << "<html lang=\"en\">" << endl
			//    << "<head>" << endl
			//    << "<meta charset=\"UTF-8\" />" << endl
			//    << "<title>Error " << error << "</title>" << endl

			//    << "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/normalize/5.0.0/normalize.min.css\"/>" << endl
			//    << "<style>" << endl
			//    << "#oopss {" << endl
			//    << "background: linear-gradient(-45deg, #00ff2f, #eaff00);" << endl
			//    << "position: fixed;" << endl
			//    << "left: 0px;" << endl
			//    << "top: 0;" << endl
			//    << "width: 100%;" << endl
			//    << "height: 100%;" << endl
			//    << "line-height: 1.5em;" << endl
			//    << "z-index: 9999;" << endl
			//    << "}" << endl
			//    << "#oopss #error-text {" << endl
			//    << "font-size: 40px;" << endl
			//    << "display: flex;" << endl
			//    << "flex-direction: column;" << endl
			//    << "align-items: center;" << endl
			//    << "font-family: \"Shabnam\", Tahoma, sans-serif;" << endl
			//    << "color: #000;" << endl
			//    << "direction: rtl;" << endl
			//    << "}" << endl
			//    << "#oopss #error-text img {" << endl
			//    << "margin: 85px auto 20px;" << endl
			//    << "height: 342px;" << endl
			//    << "}" << endl
			//    << "#oopss #error-text span {" << endl
			//    << "position: relative;" << endl
			//    << "font-size: 3.3em;" << endl
			//    << "font-weight: 900;" << endl
			//    << "margin-bottom: 50px;" << endl
			//    << "}" << endl
			//    << "#oopss #error-text p.p-a {" << endl
			//    << "font-size: 19px;" << endl
			//    << "margin: 30px 0 15px 0;" << endl
			//    << "}" << endl
			//    << "#oopss #error-text p.p-b {" << endl
			//    << "font-size: 15px;" << endl
			//    << "}" << endl
			//    << "#oopss #error-text .back {" << endl
			//    << "background: #fff;" << endl
			//    << "color: #000;" << endl
			//    << "font-size: 30px;" << endl
			//    << "text-decoration: none;" << endl
			//    << "margin: 2em auto 0;" << endl
			//    << "padding: 0.7em 2em;" << endl
			//    << "border-radius: 500px;" << endl
			//    << "box-shadow: 0 20px 70px 4px rgba(0, 0, 0, 0.1)," << endl
			//    << "inset 7px 33px 0 0px #fff300;" << endl
			//    << "font-weight: 900;" << endl
			//    << "transition: all 300ms ease;" << endl
			//    << "}" << endl
			//    << "#oopss #error-text .back:hover {" << endl
			//    << "-webkit-transform: translateY(-13px);" << endl
			//    << "transform: translateY(-13px);" << endl
			//    << "box-shadow: 0 35px 90px 4px rgba(0, 0, 0, 0.3), inset 0px 0 0 3px #000;" << endl
			//    << "}" << endl

			//    << ".btn-go-home{" << endl
			//    << "position: relative;" << endl
			//    << "z-index: 200;" << endl
			//    << "margin: 15px auto;" << endl
			//    << "width: 100px;" << endl
			//    << "padding: 10px 15px;" << endl
			//    << "border: 5px solid #000000;" << endl
			//    << "border-radius: 100px;" << endl
			//    << "font-weight: 800;" << endl
			//    << "display: block;" << endl
			//    << "color: black;" << endl
			//    << "text-align: center;" << endl
			//    << "text-decoration: none;" << endl
			//    << "letter-spacing : 2px;" << endl
			//    << "font-size: 22px;" << endl
			//    << "-webkit-transition: all 0.3s ease-in;" << endl
			//    << "-moz-transition: all 0.3s ease-in;" << endl
			//    << "-ms-transition: all 0.3s ease-in;" << endl
			//    << "-o-transition: all 0.3s ease-in;" << endl
			//    << "transition: all 0.3s ease-in;" << endl
			//    << "}" << endl

			//    << "@font-face {" << endl
			//    << "font-family: Shabnam;" << endl
			//    << "src: url(\"https://cdn.rawgit.com/ahmedhosna95/upload/ba6564f8/fonts/Shabnam/Shabnam-Bold.eot\");" << endl
			//    << "src: url(\"https://cdn.rawgit.com/ahmedhosna95/upload/ba6564f8/fonts/Shabnam/Shabnam-Bold.eot?#iefix\")" << endl
			//    << "format(\"embedded-opentype\")," << endl
			//    << "url(\"https://cdn.rawgit.com/ahmedhosna95/upload/ba6564f8/fonts/Shabnam/Shabnam-Bold.woff\")" << endl
			//    << "format(\"woff\")," << endl
			//    << "url(\"https://cdn.rawgit.com/ahmedhosna95/upload/ba6564f8/fonts/Shabnam/Shabnam-Bold.woff2\")" << endl
			//    << "format(\"woff2\")," << endl
			//    << "url(\"https://cdn.rawgit.com/ahmedhosna95/upload/ba6564f8/fonts/Shabnam/Shabnam-Bold.ttf\")" << endl
			//    << "format(\"truetype\");" << endl
			//    << "font-weight: bold;" << endl
			//    << "}" << endl
			//    << "@font-face {" << endl
			//    << "font-family: Shabnam;" << endl
			//    << "src: url(\"https://cdn.rawgit.com/ahmedhosna95/upload/ba6564f8/fonts/Shabnam/Shabnam.eot\");" << endl
			//    << "src: url(\"https://cdn.rawgit.com/ahmedhosna95/upload/ba6564f8/fonts/Shabnam/Shabnam.eot?#iefix\")" << endl
			//    << "format(\"embedded-opentype\")," << endl
			//    << "url(\"https://cdn.rawgit.com/ahmedhosna95/upload/ba6564f8/fonts/Shabnam/Shabnam.woff\")" << endl
			//    << "format(\"woff\")," << endl
			//    << "url(\"https://cdn.rawgit.com/ahmedhosna95/upload/ba6564f8/fonts/Shabnam/Shabnam.woff2\")" << endl
			//    << "format(\"woff2\")," << endl
			//    << "url(\"https://cdn.rawgit.com/ahmedhosna95/upload/ba6564f8/fonts/Shabnam/Shabnam.ttf\")" << endl
			//    << "format(\"truetype\");" << endl
			//    << "font-weight: normal;" << endl
			//    << "}" << endl
			//    << "</style>" << endl
			//    << "</head>" << endl

			//    << "<body>" << endl
			//    << "<div id=\"oopss\">" << endl
			//    << "<div id=\"error-text\">" << endl
			//    << "<img src=\"https://cdn.rawgit.com/ahmedhosna95/upload/1731955f/sad404.svg\" alt=\"" << error << "\"/>" << endl
			//    << "<span>Error " << error << "</span>" << endl
			//    <<"<pre style=\"white-space: break-spaces\">" <<message<<"</pre>" << endl
			//    << "<a href=\"/\" class=\"btn-go-home\">GO BACK HOME</a>" << endl
			//    << "</div>" << endl
			//    << "</div>" << endl
			//    << "</body>" << endl
			//    << "</html>" << endl;
			return ss.str();
		}
		inline void isDefaultPage(ClientConnexion &clientConnexion, const string code, stringstream *ss, const string &message)
		{
			
			string errorPath;
			if (clientConnexion.getRequest().route == nullptr)
				errorPath = clientConnexion.getConfig()->routes.at("/").getErrorIndex(code);
			else
				errorPath = clientConnexion.getRequest().route->getErrorIndex(code);
			if (errorPath.empty())
			{
				string html = Tools::generateDefaultHtmlError(code, message);
				int contentLength = html.length();

				*ss << "Content-Length: " << contentLength << "\r\n"
					<< "Content-Type: text/html\r\n\r\n"
					<< html;
			}
			else
			{
				string file = clientConnexion.getRequest().route->getErrorIndex(code);
				int contentLength = CheckFile::getFileSize(file.c_str());
				string contentType = MimesTypes::getContentType(file);

				*ss << "Content-Length: " << contentLength << "\r\n"
					<< "Content-Type: " + contentType + "\r\n\r\n";

				clientConnexion.setDownloadStream(new ifstream(file));
			}
		}
	}
	inline stringstream *header100()
	{
		return new stringstream("HTTP/1.1 100 Continue\r\n\r\n");
	}
	inline stringstream *header101()
	{
		return new stringstream("HTTP/1.1 101 Switching Protocols\r\n\r\n");
	}
	inline stringstream *header200(vector<string> const &headers)
	{
		stringstream *ss = new stringstream();
		*ss << "HTTP/1.1 200 OK\r\n";
		for (vector<string>::const_iterator it = headers.begin(); it != headers.end(); it++)
		{
			*ss << *it << "\r\n";
		}
		*ss << "\r\n";
		return ss;
	}
	inline stringstream *header201(string location)
	{
		stringstream *ss = new stringstream();
		*ss << "HTTP/1.1 201 Created\r\n";
		*ss << "Location: " << location << "\r\n";
		*ss << "\r\n";
		return ss;
	}
	inline stringstream *header202()
	{
		return new stringstream("HTTP/1.1 202 Accepted\r\n\r\n");
	}
	inline stringstream *header203()
	{
		return new stringstream("HTTP/1.1 203 Non-Authoritative Information\r\n\r\n");
	}
	inline stringstream *header204()
	{
		return new stringstream("HTTP/1.1 204 No Content\r\n\r\n");
	}
	inline stringstream *header205()
	{
		return new stringstream("HTTP/1.1 205 Reset Content\r\n\r\n");
	}
	inline stringstream *header206()
	{
		return new stringstream("HTTP/1.1 206 Partial Content\r\n\r\n");
	}
	inline stringstream *header300(vector<string> const &urls)
	{
		string html = Tools::generateHtmlMultRedirToString(urls);
		int contentLength = html.length();
		stringstream *ss = new stringstream();
		*ss << "HTTP/1.1 300 Multiple Choice\r\n"
			<< "Cache-Control: no-cache, no-store, must-revalidate\r\n"
			<< "Content-Length: " << contentLength << "\r\n"
			<< "Content-Type: text/html\r\n\r\n"
			<< html;
		return ss;
	}

	inline stringstream *header301(const string &url)
	{
		stringstream *ss = new stringstream();
		*ss << "HTTP/1.1 301 Moved Permanently\r\n"
			<< "Location: " << url << "\r\n\r\n";
		return ss;
	}
	inline stringstream *header302(const string &url)
	{
		stringstream *ss = new stringstream();
		*ss << "HTTP/1.1 302 Found\r\n"
			<< "Location: " << url << "\r\n\r\n";
		return ss;
	}
	inline stringstream *header303(const string &url)
	{
		stringstream *ss = new stringstream();
		*ss << "HTTP/1.1 303 See Other\r\n"
			<< "Location: " << url << "\r\n\r\n";
		return ss;
	}
	inline stringstream *header304()
	{
		stringstream *ss = new stringstream();
		*ss << "HTTP/1.1 304 Not Modified\r\n"
			<< "Content-Location: absolute-URI"
			<< "Etag: "
			<< "Vary: accept-encoding"
			<< "Cache-Control: max-age=0"
			<< "Expires: " << Tools::getDateNowToString()
			<< "\r\n\r\n";
		return ss;
	}
	inline stringstream *header307(const string &url)
	{
		stringstream *ss = new stringstream();
		*ss << "HTTP/1.1 307 Temporary Redirect\r\n"
			<< "Location: " << url << "\r\n\r\n";
		return ss;
	}
	inline stringstream *header308(const string &url)
	{
		stringstream *ss = new stringstream();
		*ss << "HTTP/1.1 308 Permanent Redirect\r\n"
			<< "Location: " << url << "\r\n\r\n";
		return ss;
	}

	inline stringstream *header400(ClientConnexion &clientConnexion, string message)
	{
		stringstream *ss = new stringstream();
		*ss << "HTTP/1.1 400 Bad Request\r\n";

		Tools::isDefaultPage(clientConnexion, "400", ss, message);

		return ss;
	}
	inline stringstream *header404(ClientConnexion &clientConnexion)
	{
		stringstream *ss = new stringstream;
		*ss << "HTTP/1.1 404 Not Found\r\n";

		Tools::isDefaultPage(clientConnexion, "404", ss, "");

		return ss;
	}
	inline stringstream *header405(ClientConnexion &clientConnexion)
	{
		stringstream *ss = new stringstream;
		*ss << "HTTP/1.1 405 Method Not Allowed\r\n";

		Tools::isDefaultPage(clientConnexion, "405", ss, "");

		return ss;
	}
	inline stringstream *header406(ClientConnexion &clientConnexion)
	{
		stringstream *ss = new stringstream;
		*ss << "HTTP/1.1 406 Not Acceptable\r\n";

		Tools::isDefaultPage(clientConnexion, "406", ss, "");

		return ss;
	}
	inline stringstream *header408(ClientConnexion &clientConnexion)
	{
		stringstream *ss = new stringstream;
		*ss << "HTTP/1.1 408 Request Timeout\r\n";

		Tools::isDefaultPage(clientConnexion, "408", ss, "");

		return ss;
	}
	inline stringstream *header411(ClientConnexion &clientConnexion)
	{
		stringstream *ss = new stringstream;
		*ss << "HTTP/1.1 411 Length Required\r\n";

		Tools::isDefaultPage(clientConnexion, "411", ss, "");

		return ss;
	}
	inline stringstream *header413(ClientConnexion &clientConnexion)
	{
		stringstream *ss = new stringstream;
		*ss << "HTTP/1.1 413 Payload Too Large\r\n";

		Tools::isDefaultPage(clientConnexion, "413", ss, "");

		return ss;
	}
	inline stringstream *header414(ClientConnexion &clientConnexion)
	{
		stringstream *ss = new stringstream;
		*ss << "HTTP/1.1 414 URI Too Long\r\n";

		Tools::isDefaultPage(clientConnexion, "414", ss, "");

		return ss;
	}
	inline stringstream *header500(ClientConnexion &clientConnexion, string message)
	{
		stringstream *ss = new stringstream();
		*ss << "HTTP/1.1 500 Internal Server Error\r\n";

		Tools::isDefaultPage(clientConnexion, "500", ss, message);

		return ss;
	}
	inline stringstream *header501(ClientConnexion &clientConnexion)
	{
		stringstream *ss = new stringstream();
		*ss << "HTTP/1.1 501 Not Implemented\r\n";

		Tools::isDefaultPage(clientConnexion, "501", ss, "");

		return ss;
	}
	inline stringstream *header502(ClientConnexion &clientConnexion, string message)
	{
		stringstream *ss = new stringstream();
		*ss << "HTTP/1.1 502 Bad Gateway\r\n";

		Tools::isDefaultPage(clientConnexion, "502", ss, message);

		return ss;
	}
	inline stringstream *header505(ClientConnexion &clientConnexion)
	{
		stringstream *ss = new stringstream();
		*ss << "HTTP/1.1 505 HTTP Version Not Supported\r\n";

		Tools::isDefaultPage(clientConnexion, "505", ss, "");

		return ss;
	}
}
#endif
