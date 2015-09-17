#include <iostream>
#include <exception>
#include <vector>
#include <ctime>
#include <fstream>
#include "BGM_Fasta.h"
#include "BGM_LocalAlignment.h"


using namespace std;
using BGM::Fasta;
using BGM::LocalAlignment;
using BGM::CommonSequence;

int main(int argc, char* argv[]) {
	if(argc != 3)
	{
		cout << "error : wrong ipnut. [data1] [data2]" << endl;
		return 0;
	}
	
	char *file1 = argv[1];
	char *file2 = argv[2];
	Fasta fasta1;
	Fasta fasta2;
	fasta1.readFile(file1);
	fasta2.readFile(file2);

	ofstream ofs("output.align");
	
	vector<char> dna1 = fasta1.getDNA();
	vector<char> dna2 = fasta2.getDNA();

	LocalAlignment la(dna1, dna2);

	clock_t start, end;
	start = clock();
	la.align();
	end = clock();

	ofs << "동작 시간 : " << (float)(end-start)/CLOCKS_PER_SEC << "sec" << endl;
	ofs << "서열1 : " << file1 << ' ' << la.getNumOfCS() << endl;
	ofs << "서열2 : " << file2 << ' ' << la.getNumOfCS() << endl;
	
	vector< vector<char> >* lcsVector;
	CommonSequence* lcsInfo;
	for(int i=1; i<=5; i++)
	{
		lcsInfo = la.getLCS(i);
		ofs << "부분서열 : 점수(" << lcsInfo->getLength() << ")" << endl;
		ofs << "\t서열1 : " << (lcsInfo->getBegin()).getRow()-1 << " ... " << (lcsInfo->getEnd()).getRow()-1 << endl;
		ofs << "\t서열2 : " << (lcsInfo->getBegin()).getCol()-1 << " ... " << (lcsInfo->getEnd()).getCol()-1 << endl;
		lcsVector = la.getLCSVector(i);
		ofs << "\t\tdata1\t";
		for(int j=0; j<(*lcsVector)[0].size(); j++)
			ofs << ((*lcsVector)[0])[j];
		ofs << endl << "\t\tdata2\t";
		for(int j=0; j<(*lcsVector)[1].size(); j++)
			ofs << ((*lcsVector)[1])[j];
		ofs << endl;

		//delete[] lcsVector;
	}
	ofs.close();
	return 0;
}