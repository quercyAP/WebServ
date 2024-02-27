/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckFile.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 11:28:37 by glamazer          #+#    #+#             */
/*   Updated: 2023/09/19 14:04:20 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKFILE_HPP
#define CHECKFILE_HPP

#include <sys/types.h>
#include <sys/stat.h>

namespace CheckFile
{
	inline bool isFile(const char *path)
	{
		struct stat path_stat;
		if (stat(path, &path_stat) != 0)
			return false;
		return S_ISREG(path_stat.st_mode);
	}

	inline bool isDir(const char *path)
	{
		struct stat path_stat;
		if (stat(path, &path_stat) != 0)
			return false;
		return S_ISDIR(path_stat.st_mode);
	}

	inline size_t getFileSize(const char *path)
	{
		struct stat path_stat;
		if (stat(path, &path_stat) != 0)
			return 0;
		return path_stat.st_size;
	}

	inline string appendIndex(const string &path, const vector<string> &index)
	{
		string ret;
		vector<string>::const_iterator it = index.begin();
		for (; it != index.end(); it++)
		{
			ret = path + "/" + *it;
			if (isFile((ret).c_str()))
				return ret;
		}
		return "";
	}

	inline bool removeDirectory(const char *path)
	{
		DIR *dir = opendir(path);
		if (dir == NULL)
			return false;

		struct dirent *entry;
		while ((entry = readdir(dir)) != NULL)
		{
			std::string entryName = entry->d_name;
			if (entryName != "." && entryName != "..")
			{
				std::string fullPath = std::string(path) + "/" + entryName;
				if (entry->d_type == DT_DIR)
				{
					if (!removeDirectory(fullPath.c_str()))
						return false;
				}
				else
				{
					if (remove(fullPath.c_str()) != 0)
					{
						std::cerr << "Erreur lors de la suppression du fichier : " << strerror(errno) << std::endl;
						return false;
					}
				}
			}
		}

		closedir(dir);
		if (rmdir(path) != 0)
		{
			std::cerr << "Erreur lors de la suppression du dossier : " << strerror(errno) << std::endl;
			return false;
		}

		return true;
	}
}

#endif
