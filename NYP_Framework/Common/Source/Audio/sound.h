#pragma once

#include <string>

struct Sound
{
	Sound(std::string fileDirectory, std::string name) 
	{
		m_fileDirectory = fileDirectory;
		m_fileName = name;
	};

	std::string m_fileDirectory;
	std::string m_fileName;
};