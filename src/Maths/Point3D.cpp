/*----------------------------------------------------
**
**		Date: 2019
**		Author: Thendeo
**		Project: Particules
**		File: Triangle implementation file
**
----------------------------------------------------*/

#include "Point3D.h"

Point3D::Point3D()
	: m_PosX(0.0)
	, m_PosY(0.0)
	, m_PosZ(0.0)
{
}

Point3D::Point3D(double pPosX, double pPosY, double pPosZ)
{
	m_PosX = pPosX;
	m_PosY = pPosY;
	m_PosZ = pPosZ;
}

Point3D::~Point3D()
{
}

double Point3D::get_X()
{
	return m_PosX;
}

double Point3D::get_Y()
{
	return m_PosY;
}

double Point3D::get_Z()
{
	return m_PosZ;
}

void Point3D::set_X(double pPosX)
{
	m_PosX = pPosX;
}

void Point3D::set_Y(double pPosY)
{
	m_PosY = pPosY;
}

void Point3D::set_Z(double pPosZ)
{
	m_PosZ = pPosZ;
}
