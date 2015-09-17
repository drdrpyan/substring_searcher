#include "BGM_Point2D.h"

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