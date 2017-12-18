#include "common.h"
#include<math.h>
#include<iostream>
using namespace std;

//Ĭ�Ϲ��캯��
GVector3::GVector3()
{
	x = 0;
	y = 0;
	z = 0;
}

//���캯��
GVector3::GVector3(float G_x,float G_y,float G_z)
{
	x = G_x;
	y = G_y;
	z = G_z;
}
void GVector3::getInfo()
{
	cout << "x=" << x << "y=" << y << "z=" << z << endl;
	return;
}
float GVector3::length()
{
	return (float)sqrt(x*x + y*y + z*z);
}
float GVector3::abs()
{
	return sqrt(x*x+y*y+z*z);
}
/*�޸�������Ա����ֵ*/
void GVector3::set(float a_x, float a_y, float a_z)
{
	x = a_x;
	y = a_y;
	z = a_z;
}
GVector3 GVector3::negate()
{
	x *= -1;
	y *= -1;
	z *= -1;
	return GVector3(x, y, z);
}
GVector3 GVector3::normalize()
{
	float I = 1.0f/length();
	x *= I;
	y *= I;
	z *= I;
	return GVector3(x, y, z);
}
float GVector3::distance(const GVector3 &p)
{
	return  (*this - p).abs();
}
/*�������ӷ�*/
GVector3 operator + (const GVector3& v1, const GVector3& v2)
{
	return GVector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

/*����������*/
GVector3 operator- (const GVector3& v1, const GVector3& v2)
{
	return GVector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

/*�����˸������������ĸ������ֱ�˸�����*/
GVector3 operator* (const GVector3& v, float f)
{
	return GVector3(v.x * f, v.y * f, v.z * f);
}

/*�������������������ĸ������ֱ�˸�����*/
GVector3 operator* (float f, const GVector3& v)
{
	return GVector3(v.x * f, v.y * f, v.z * f);
}

/*���������˷��������ĸ�������Ӧ���*/
GVector3 operator* (const GVector3& v1, GVector3& v2)
{
	return GVector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

bool operator== (const GVector3& v1, GVector3& v2)
{
	if (v1.x == v2.x&&v1.y == v2.y&&v1.z == v2.z)
		return true;
	else
		return false;
}