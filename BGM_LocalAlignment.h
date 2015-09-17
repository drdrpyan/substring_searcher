#ifndef __BGM_LOCAL_ALIGNMENT_H__
#define __BGM_LOCAL_ALIGNMENT_H__

#include <vector>
#include <list>
#include "BGM_Point2D.h"

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
		std::list<CommonSequence> lcs;

	private:
		Point2D traceOrigin(int row, int col);
		void registerLcs(int length, int endRow, int endCol);

	public:
		LocalAlignment(const std::vector<char>& str1, const std::vector<char>& str2);
		~LocalAlignment();
		void align();

		int getNumOfCS() const;
		int getLCSLength(int rank);
		CommonSequence* getLCS(int rank);
		std::vector< std::vector<char> >* getLCSVector(int rank) const;
		void printAlignMatrix() const;		
	};
}

#endif