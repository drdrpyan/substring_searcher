#include <iostream>
#include <fstream>
#include <exception>
#include "BGM_Fasta.h"

namespace BGM
{
	Fasta::Fasta()
	{
	}

	Fasta::~Fasta()
	{
	}

	void Fasta::readFile(const char *fileName)
	{
		std::ifstream ifs(fileName);
		//if(!ifs.is_open())
		//{
		//	throw std::exception("file is not exists.\n");
		//}

		char c;
		
		//fasta 파일의 헤더 읽기
		do
		{
			ifs.get(c);
		}
		while(c != '\n');
		//fasta 파일의 본문 읽기
		while(ifs>>c)
		{
			DNA.push_back(c);
		}

		ifs.close();
	}


	const std::vector<char>& Fasta::getDNA() const
	{
		return DNA;
	}
}