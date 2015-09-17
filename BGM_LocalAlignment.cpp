#include <iostream>
#include <fstream>
#include "BGM_LocalAlignment.h"

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
			if(alignMatrix[i][j] == alignMatrix[i-1][j-1]+MATCHED)
			{
				i--;
				j--;
				quitCount = 5;
				origin.set(i, j);
			}
			else
			{
				if(alignMatrix[i-1][j-1]>=alignMatrix[i-1][j] && alignMatrix[i-1][j-1]>=alignMatrix[i][j-1])
				{
					i--;
					j--;
				}
				else if(alignMatrix[i-1][j] >= alignMatrix[i][j-1])
					i--;
				else
					j--;

				quitCount--;
			}
			
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

	std::vector< std::vector<char> >* LocalAlignment::getLCSVector(int rank) const
	{
		std::vector< std::vector<char> >* result = new std::vector< std::vector<char> >;
		std::vector<char> buffer1;
		std::vector<char> buffer2;
		std::vector<char>& lcs1 = *(new std::vector<char>);
		std::vector<char>& lcs2 = *(new std::vector<char>);
		
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
			lcs1.push_back(buffer1[i]);
		for(int i=buffer2.size()-1; i>=0; i--)
			lcs2.push_back(buffer2[i]);

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