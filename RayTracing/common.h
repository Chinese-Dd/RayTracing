#ifndef _COMMON_H_
#define _COMMON_H_

#include <math.h>
#include "stdlib.h"
/*������Ļ����*/
#define ImageWidth 1000
#define ImageHeight	1000

/*������Ļ�ߴ�*/
#define VIEW_WIDTH 10.0f
#define VIEW_HEIGHT 10.0f

/*ͼƬ����*/
#define JPG_PIXEL_WIDTH  640
#define JPG_PIXEL_HEIGHT	480

#define TOTALDEPTH 2 /*���ߵݹ����*/
#define BIG 1000000.0f /*�����*/
#define SMALL 0.0001f/*����С*/
#define DOT(A, B) (A.x*B.x+A.y*B.y+A.z*B.z) /*�������*/

/*�ཻ����,INTERSECTED_IN��ʾ���ߴ������ڲ��������������н��㣬MISS��ʾ���ߺ�����û�н���,INTERSECTED��ʾ���ߴ������ⲿ�������Һ������н���*/
enum INTERSECTION_TYPE { INTERSECTED_IN = -1, MISS = 0, INTERSECTED = 1 };

class GVector3
{
public:
	float x;
	float y;
	float z;

	GVector3();       //ȱʡ���캯��
	~GVector3(){};    //ȱʡ���캯��

	GVector3(float G_x, float G_y, float G_z);  // Ĭ�Ϲ��캯��

	void getInfo();
	void set(float a_x, float a_y, float a_z);
	float length();//��������
	float GVector3::dot(GVector3 a_v);//�����ĵ��
	float abs();//�����ľ���ֵ,��������ģ
	GVector3 negate();//����������
	GVector3 normalize();//������λ��
	float distance(const GVector3 &p);//2������֮��ľ���
	GVector3 GVector3::cross(GVector3 a_v);//��2�������Ĳ��
	friend GVector3 operator+ (const GVector3& v1, const GVector3& v2);/*�������ӷ�*/
	friend GVector3 operator- (const GVector3& v1, const GVector3& v2);/*����������,���������յ�ָ�򱻼��������յ�*/
	friend GVector3 operator* (const GVector3& v, float f);/*�����˸������������ĸ������ֱ�˸�����*/
	friend GVector3 operator* (float f, const GVector3& v);/*�������������������ĸ������ֱ�˸�����*/
	friend GVector3 operator* (const GVector3& v1, GVector3& v2);/*���������˷��������ĸ�������Ӧ���*/
	friend bool operator== (const GVector3& v1, GVector3& v2);/*�ж�2�������Ƿ����*/
};

/*��ɫ*/
typedef GVector3 Color;

/*����*/
typedef unsigned int Pixel;

#endif/*_COMMON_H_*/