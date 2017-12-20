#include "common.h"
#include "CRAY.h"
Cray::Cray(GVector3 origin, GVector3 direction)
{
	m_origin = origin;
	m_direction = direction;
}
const GVector3 &Cray::get_origin()
{
	return m_origin;
}
const GVector3 &Cray::get_direction()
{
	return m_direction;
}
void Cray::set_Origin(const GVector3 &origin)
{
	m_origin = origin;
}
void Cray::set_Direction(const GVector3 &direction)
{
	m_direction = direction;
}
GVector3 Cray::get_Point(float t)
{
	return m_origin +m_direction*t;
}