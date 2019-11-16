/*----------------------------------------------------
**
**		Date: 2019
**		Author: Alexis LAFONT
**		Project: Particules
**		File: 3DPoints header file
**
----------------------------------------------------*/

#pragma once

class Point3D
{
public:
	Point3D();
	Point3D(double pPosX, double pPosY, double pPosZ);
	~Point3D();

	double get_X();
	double get_Y();
	double get_Z();

	void set_X(double pPosX);
	void set_Y(double pPosY);
	void set_Z(double pPosZ);

private:

	double m_PosX;
	double m_PosY;
	double m_PosZ;
};