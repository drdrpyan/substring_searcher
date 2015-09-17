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
		
		//fasta ������ ��� �б�
		do
		{
			ifs.get(c);
		}
		while(c != '\n');
		//fasta ������ ���� �б�
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