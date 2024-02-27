/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryListing.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:57:30 by glamazer          #+#    #+#             */
/*   Updated: 2023/09/13 13:28:50 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTORYLISTING_HPP
#define DIRECTORYLISTING_HPP

#include "define.hpp"

namespace DirectoryListing
{
    inline string getHtml(const string &path, const string &target)
    {
        stringstream ss;

        ss << "<html>" << endl
           << "<head>" << endl
           << "<title>Index of " << target << "</title>" << endl
           << "</head>" << endl
           << "<body>" << endl
           << "<h1>Index of " << target << "</h1>" << endl
           << "<hr>" << endl
           << "<pre>" << endl;
        DIR *rep = opendir(path.c_str());
        if (rep != NULL)
        {
            struct dirent *ent = nullptr;
            while ((ent = readdir(rep)) != NULL)
            {
                struct stat buf;
                memset(&buf, 0, sizeof(buf));
                stat((path + "/" + ent->d_name).c_str(), &buf);

                string name = ent->d_name;
                if (name == ".")
                    continue;
                if ((buf.st_mode & S_IFDIR))
                {
                    name.append("/");
                }
                if (name != "../")
                {
                    ss << "<a href=\"" << target + "/" + name << "\">" << name << "</a>";
                    int space = 51 - name.size();
                    for (int i = 0; i < space; i++)
                    {
                        ss << " ";
                    }
                    char strTime[50];
                    strftime(strTime, 50, "%d-%b-%Y %H:%M", gmtime(&(buf.st_mtim.tv_sec)));
                    ss << strTime;
                    for (int i = 0; i < 18; i++)
                    {
                        ss << " ";
                    }
                    if ((buf.st_mode & S_IFDIR))
                    {
                        ss << " -";
                    }
                    else
                    {
                        ss << " " << buf.st_size;
                    }
                }
                else
                {
                    string path = target;
                    if (path.back() == '/')
                        path.pop_back();
                    path = path.substr(0, path.find_last_of('/'));
                    if (path.empty())
                        ss << "<a href=\""
                           << "../"
                           << "\">" << name << "</a>";
                    else
                        ss << "<a href=\"" << path << "\">" << name << "</a>";
                }
                ss << endl;
            }
            if (ent)
                free(ent);
            closedir(rep);
        }
        ss << "</pre>" << endl
           << "<hr>" << endl
           << "</body>" << endl
           << "</html>" << endl;

        return ss.str();
    }
}

#endif