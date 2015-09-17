///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//BGM_Point2D.h
//2차원 배열상의 위치를 나타내기위한 클래스

#ifndef __BGM_POINT_2D_H__
#define __BGM_POINT_2D_H__

namespace BGM
{
	class Point2D
	{
	private:
		union { int x; int row; };
		union { int y; int col; };
	public:
		Point2D();
		Point2D(int x_row, int y_col);
		Point2D(const Point2D& p);

		void setX(int x);
		int getX() const;
		void setY(int y);
		int getY() const;
		void setRow(int row);
		int getRow() const;
		void setCol(int col);
		int getCol() const;
		void set(int x_row, int y_col); 

		bool operator==(const Point2D& p) const;
		bool operator!=(const Point2D& p) const;
	};
}

#endif

///////////////////////////////////////////////////////////////////////////
//BGM_Point2D.cpp

////#include "BGM_Point2D.h"
namespace BGM
{
	Point2D::Point2D()
		: x(0), y(0)
	{
	}

	Point2D::Point2D(int x_row, int y_col)
		: x(x_row), y(y_col)
	{
	}

	Point2D::Point2D(const Point2D& p)
		: x(p.x), y(p.y)
	{
	}

	void Point2D::setX(int x)
	{
		this->x = x;
	}

	int Point2D::getX() const
	{
		return x;
	}

	void Point2D::setY(int y)
	{
		this->y = y;
	}

	int Point2D::getY() const
	{
		return y;
	}

	void Point2D::setRow(int row)
	{
		this->row = row;
	}

	int Point2D::getRow() const
	{
		return row;
	}

	void Point2D::setCol(int col)
	{
		this->col = col;
	}

	int Point2D::getCol() const
	{
		return col;
	}

	void Point2D::set(int x_row, int y_col)
	{
		x = x_row;
		y = y_col;
	}

	bool Point2D::operator==(const Point2D& p) const
	{
		return (x==p.x) && (y==p.y);
	}

	bool Point2D::operator!=(const Point2D& p) const
	{
		return (x!=p.x) || (y!=p.y);
	}
}
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//BGM_Fasta.h
//fasta파일을 읽어 문자열(벡터로 표현)로 저장하는 클래스

#ifndef __BGM_FASTA_H__
#define __BGM_FASTA_H__

#include <vector>

namespace BGM
{
	class Fasta
	{
	private:
		std::vector<char> DNA;

	public:
		Fasta();
		~Fasta();
		void readFile(const char *fileName);
		const std::vector<char>& getDNA() const;
	};
}

#endif

///////////////////////////////////////////////////////////////////////////
//BGM_Fasta.cpp

#include <iostream>
#include <fstream>
////#include <exception>
////#include "BGM_Fasta.h"
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

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//BGM_LocalAlignment.h
//LocalAlignment : dynamic programming 방식으로 두 문자열의 local lcs를 구하는 클래스
//CommonSequence : local common sequence의 정보를 나타내는 클래스
//					align matrix내의 시작점, 종료점, 점수를 나타낸다.

#ifndef __BGM_LOCAL_ALIGNMENT_H__
#define __BGM_LOCAL_ALIGNMENT_H__

#include <vector>
#include <list>
////#include "BGM_Point2D.h"

namespace BGM
{
	class CommonSequence
	{
	private:
		int length;
		Point2D begin;
		Point2D end;
	public:
		CommonSequence(int length, const Point2D& begin, const Point2D& end);
		int getLength() const;
		const Point2D& getBegin() const;
		const Point2D& getEnd() const;

		bool operator<(const CommonSequence& cs) const;
		bool operator>(const CommonSequence& cs) const;
	};

	class LocalAlignment
	{
		enum PENALTY { MATCHED=3, MISSMATCHED=-2, GAP_FIRST=-5, GAP=-2 };
		
	private:
		const std::vector<char> str1;
		const std::vector<char> str2;
		int **alignMatrix;

		int numOfCS;
		std::list<CommonSequence> lcs; //점수 상위 6개의 common sequence를 저장

	private:
		Point2D traceOrigin(int row, int col); //align matrix에서 인자 값을 common sequence 종료점으로 간주하여 시작점을 찾는다.
		void registerLcs(int length, int endRow, int endCol); //lcs를 갱신한다.

	public:
		LocalAlignment(const std::vector<char>& str1, const std::vector<char>& str2);
		~LocalAlignment();
		void align(); //align matrix 작성, lcs갱신

		//이하 출력용 함수들
		int getNumOfCS() const;
		int getLCSLength(int rank);
		CommonSequence* getLCS(int rank);
		std::vector< std::vector<char>* >* getLCSVector(int rank) const;
		void printAlignMatrix() const;		
	};
}

#endif

///////////////////////////////////////////////////////////////////////////
//BGM_LocalAlignment.cpp

#include <iostream>
#include <fstream>
////#include "BGM_LocalAlignment.h"
namespace BGM
{
	CommonSequence::CommonSequence(int length, const Point2D& begin, const Point2D& end)
		: length(length), begin(begin), end(end)
	{
	}

	int CommonSequence::getLength() const
	{
		return length;
	}

	const Point2D& CommonSequence::getBegin() const
	{
		return begin;
	}

	const Point2D& CommonSequence::getEnd() const
	{
		return end;
	}

	bool CommonSequence::operator<(const CommonSequence& cs) const
	{
		return length < cs.length;
	}

	bool CommonSequence::operator>(const CommonSequence& cs) const
	{
		return length > cs.length;
	}
	
	
	
	LocalAlignment::LocalAlignment(const std::vector<char>& str1, const std::vector<char>& str2)
		: str1(str1), str2(str2), numOfCS(0)
	{
		alignMatrix = new int* [str1.size()+1];
		for(unsigned int i=0; i<str1.size()+1; i++)
			alignMatrix[i] = new int[str2.size()+2];

		lcs.push_back(CommonSequence(0, Point2D(0,0), Point2D(0,0)));
	}

	LocalAlignment::~LocalAlignment()
	{
		for(unsigned int i=0; i<str1.size()+1; i++)
			delete alignMatrix[i];
		delete alignMatrix;
	}

	void LocalAlignment::align()
	{
		alignMatrix[0][0] = 0;

		//matrix initializing
		for(int i=0; i<=str1.size(); i++)
			alignMatrix[i][0] = 0;
		for(int i=0; i<=str2.size(); i++)
			alignMatrix[0][i] = 0;

		int max,match, gapStr1, gapStr2, zero=0, minLength=0;
		PENALTY lastPenalty = MATCHED;
		//fill align matrix
		for(unsigned int i=1; i<=str1.size(); i++)
		{
			for(unsigned int j=1; j<=str2.size(); j++)
			{
				match = alignMatrix[i-1][j-1] + ((str1[i-1]==str2[j-1]) ? MATCHED : MISSMATCHED);
				max = match;
				lastPenalty = (str1[i-1] == str2[j-1]) ? MATCHED : MISSMATCHED;

				gapStr1 = alignMatrix[i-1][j] + ((lastPenalty!=GAP_FIRST) ? GAP_FIRST : GAP);
				if(max < gapStr1)
				{
					max = gapStr1;
					lastPenalty = ((lastPenalty!=GAP_FIRST) ? GAP_FIRST : GAP);
				}
				gapStr2 = alignMatrix[i][j-1] + ((lastPenalty!=GAP_FIRST) ? GAP_FIRST : GAP);
				if(max < gapStr2)
				{
					max = gapStr2;
					lastPenalty = ((lastPenalty!=GAP_FIRST) ? GAP_FIRST : GAP);
				}

				if(max < 0)
					max = 0;

				alignMatrix[i][j] = max;
				
				//최대 local 후보군을 선정
				if(lastPenalty==MATCHED && (lcs.begin())->getLength()<max)
					registerLcs(max, i, j);
			}
		}
		//lcs.begin();
	}

	Point2D LocalAlignment::traceOrigin(int row, int col)
	{
		Point2D origin(row, col);
		int i = row;
		int j = col;
		int quitCount = 5;

		while(i>0 && j>0 && quitCount>0 && alignMatrix[i][j]>0)
		{
			//match로 간주하고 진행
			if(alignMatrix[i][j] == alignMatrix[i-1][j-1]+MATCHED)
			{
				i--;
				j--;
				quitCount = 5;
				origin.set(i, j);
			}
			else
			{
				//missmatch 판정
				if(alignMatrix[i-1][j-1]>=alignMatrix[i-1][j] && alignMatrix[i-1][j-1]>=alignMatrix[i][j-1])
				{
					i--;
					j--;
				}
				//gap으로 판정
				else if(alignMatrix[i-1][j] >= alignMatrix[i][j-1])
					i--;
				else
					j--;
				//5연속 불일치를 판단
				quitCount--;
			}
			
			//기존의 lcs원소가 현재 처리중인 lcs의 substring일 때 lcs에서 이를 제거한다.
			std::list<CommonSequence>::iterator itr;
			for(itr=lcs.begin(); itr!=lcs.end() && (*itr).getEnd()!=Point2D(i,j); itr++);
			if(itr!=lcs.end())
			{
				origin = (*itr).getBegin();
				lcs.erase(itr);
				numOfCS--;
				break;
			}
		}

		return origin;
	}

	void LocalAlignment::registerLcs(int length, int row, int col)
	{
		Point2D origin = traceOrigin(row, col);

		//std::list<CommonSequence>::iterator it;
		//for(it=lcs.begin(); it!=lcs.end() && (*it).getBegin()!=origin; it++);
		//if(it!=lcs.end() && (*it).getLength()<length)
		//	lcs.erase(it);

		numOfCS++;
		lcs.push_front(CommonSequence(length, origin, Point2D(row, col)));
		lcs.sort();
		if(lcs.size() > 6)
			lcs.erase(lcs.begin());
	}

	int LocalAlignment::getNumOfCS() const
	{
		return numOfCS;
	}

	int LocalAlignment::getLCSLength(int rank)
	{
		std::list<CommonSequence>::const_iterator itr = lcs.end();
		for(int i=0; i<rank; i++)
			itr--;
		return (*itr).getLength();
	}
	
	CommonSequence* LocalAlignment::getLCS(int rank)
	{
		CommonSequence* cs;
		std::list<CommonSequence>::iterator itr = lcs.end();
		for(int i=0; i<rank; i++)
			itr--;
		cs = &(*itr);
		return cs;
	}

	std::vector< std::vector<char>* >* LocalAlignment::getLCSVector(int rank) const
	{
		std::vector< std::vector<char>* >* result = new std::vector< std::vector<char>* >;
		std::vector<char> buffer1;
		std::vector<char> buffer2;
		std::vector<char>* lcs1 = new std::vector<char>;
		std::vector<char>* lcs2 = new std::vector<char>;
		
		std::list<CommonSequence>::const_iterator itr = lcs.end();
		for(int i=0; i<rank; i++)
			itr--;

		Point2D begin = (*itr).getBegin();
		Point2D end = (*itr).getEnd();
		int i = end.getRow();
		int j = end.getCol();

		buffer1.push_back(str1[i-1]);
		buffer2.push_back(str2[j-1]);
		while(i>0 && j>0 && Point2D(i,j)!=begin)
		{
			//match
			if(alignMatrix[i][j] == alignMatrix[i-1][j-1]+MATCHED)
			{
				buffer1.push_back(str1[i-1]);
				buffer2.push_back(str2[j-1]);
				i--;
				j--;				
			}
			else
			{
				//missmatch
				if(alignMatrix[i-1][j-1]>=alignMatrix[i-1][j] && alignMatrix[i-1][j-1]>=alignMatrix[i][j-1])
				{
					buffer1.push_back(str1[i-1]);
					buffer2.push_back(str2[j-1]);
					i--;
					j--;					
				}
				//str2 gap
				else if(alignMatrix[i-1][j] >= alignMatrix[i][j-1])
				{
					buffer1.push_back(str1[i-1]);
					buffer2.push_back('-');
					i--;					
				}
				//str1 gap
				else
				{
					buffer1.push_back('-');
					buffer2.push_back(str2[j-1]);
					j--;					
				}
			}
		}

		for(int i=buffer1.size()-1; i>=0; i--)
			lcs1->push_back(buffer1[i]);
		for(int i=buffer2.size()-1; i>=0; i--)
			lcs2->push_back(buffer2[i]);

		result->push_back(lcs1);
		result->push_back(lcs2);

		return result;
	}

	void LocalAlignment::printAlignMatrix() const
	{
		std::ofstream ofs("alignMatrix.csv");
		ofs << " , ,";
		for(int i=0; i<str1.size(); i++)
			ofs << str1[i] << ',';
		ofs << std::endl;
		ofs << ',';
		for(int i=0; i<str1.size(); i++)
			ofs << alignMatrix[i][0] << ',';

		ofs << std:: endl;
		for(int j=0; j<str2.size(); j++)
		{
			ofs << str2[j] << ',';
			for(int i=0; i<str1.size(); i++)
				ofs << alignMatrix[i][j+1] << ',';
			ofs << std::endl;
		}

		ofs.close();
	}
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//main.cpp

#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
////#include "BGM_Fasta.h"
////#include "BGM_LocalAlignment.h"

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
	
	vector< vector<char>* >* lcsVector;
	CommonSequence* lcsInfo;
	for(int i=1; i<=5; i++)
	{
		lcsInfo = la.getLCS(i);
		ofs << "부분서열 : 점수(" << lcsInfo->getLength() << ")" << endl;
		ofs << "\t서열1 : " << (lcsInfo->getBegin()).getRow()-1 << " ... " << (lcsInfo->getEnd()).getRow()-1 << endl;
		ofs << "\t서열2 : " << (lcsInfo->getBegin()).getCol()-1 << " ... " << (lcsInfo->getEnd()).getCol()-1 << endl;
		lcsVector = la.getLCSVector(i);
		ofs << "\t\tdata1\t";
		for(int j=0; j<(*lcsVector)[0]->size(); j++)
			ofs << (*((*lcsVector)[0]))[j];
		ofs << endl << "\t\tdata2\t";
		for(int j=0; j<(*lcsVector)[1]->size(); j++)
			ofs << (*((*lcsVector)[1]))[j];
		ofs << endl;

		delete (*lcsVector)[0];
		delete (*lcsVector)[1];
		delete lcsVector;
	}
	ofs.close();
	return 0;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////