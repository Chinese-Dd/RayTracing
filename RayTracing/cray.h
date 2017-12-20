#ifndef _CRAY_H_
#define _CRAY_H_

#include "common.h"

//…‰œﬂ¿‡
class Cray
{
private:
	GVector3 m_origin;;
	GVector3 m_direction;
public:
	Cray();
	~Cray(){};
	Cray(GVector3 origin, GVector3 direction);
	const GVector3 &get_origin();
	const GVector3 &get_direction();
	void set_Origin(const GVector3 &origin);
	void set_Direction(const GVector3 & direction);
	GVector3 get_Point(float t);
};
#endif 