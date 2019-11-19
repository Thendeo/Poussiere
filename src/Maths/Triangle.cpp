/*----------------------------------------------------
**
**		Date: 2019
**		Author: Alexis LAFONT
**		Project: Particules
**		File: Triangle implementation file
**
----------------------------------------------------*/

#include "Triangle.h"

Triangle::Triangle()
	: m_A(1.0, 0.0, 0.0)
, m_B(0.0, 0.0, 0.0)
, m_C(0.0, 1.0, 0.0)
{
}

Triangle::~Triangle()
{
}

const Point3D Triangle::get_A()
{
	return m_A;
}

const Point3D Triangle::get_B()
{
	return m_B;
}

const Point3D Triangle::get_C()
{
	return m_C;
}

void Triangle::set_A(Point3D p_A)
{
	m_A = p_A;
}

void Triangle::set_B(Point3D p_B)
{
	m_B = p_B;
}

void Triangle::set_C(Point3D p_C)
{
	m_C = p_C;
}
