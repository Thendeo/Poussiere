/*----------------------------------------------------
**
**		Date: 2019
**		Author: Thendeo
**		Project: Particules
**		File: triangle header file
**
----------------------------------------------------*/

#pragma once

#include "Point3D.h"

class Triangle
{
public:
	Triangle();
	~Triangle();


	const Point3D get_A();
	const Point3D get_B();
	const Point3D get_C();

	void set_A(Point3D p_A);
	void set_B(Point3D p_B);
	void set_C(Point3D p_C);
private:

	Point3D m_A;
	Point3D m_B;
	Point3D m_C;
};