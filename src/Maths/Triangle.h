/*----------------------------------------------------
**
**		Date: 2019
**		Author: Alexis LAFONT
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

private:

	Point3D m_A;
	Point3D m_B;
	Point3D m_C;
};