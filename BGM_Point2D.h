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