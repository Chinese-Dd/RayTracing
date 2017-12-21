#ifndef _COMMON_H_
#define _COMMON_H_

#include <math.h>
#include "stdlib.h"
/*虚拟屏幕像素*/
#define ImageWidth 1000
#define ImageHeight	1000

/*虚拟屏幕尺寸*/
#define VIEW_WIDTH 10.0f
#define VIEW_HEIGHT 10.0f

/*图片像素*/
#define JPG_PIXEL_WIDTH  640
#define JPG_PIXEL_HEIGHT	480

#define TOTALDEPTH 2 /*光线递归深度*/
#define BIG 1000000.0f /*无穷大*/
#define SMALL 0.0001f/*无穷小*/
#define DOT(A, B) (A.x*B.x+A.y*B.y+A.z*B.z) /*向量点乘*/

/*相交类型,INTERSECTED_IN表示射线从物体内部出发并和物体有交点，MISS表示射线和物体没有交点,INTERSECTED表示射线从物体外部出发并且和物体有交点*/
enum INTERSECTION_TYPE { INTERSECTED_IN = -1, MISS = 0, INTERSECTED = 1 };

class GVector3
{
public:
	float x;
	float y;
	float z;

	GVector3();       //缺省构造函数
	~GVector3(){};    //缺省析造函数

	GVector3(float G_x, float G_y, float G_z);  // 默认构造函数

	void getInfo();
	void set(float a_x, float a_y, float a_z);
	float length();//向量长度
	float GVector3::dot(GVector3 a_v);//向量的点积
	float abs();//向量的绝对值,即向量的模
	GVector3 negate();//向量反方向
	GVector3 normalize();//向量单位化
	float distance(const GVector3 &p);//2个向量之间的距离
	GVector3 GVector3::cross(GVector3 a_v);//求2个向量的差积
	friend GVector3 operator+ (const GVector3& v1, const GVector3& v2);/*两向量加法*/
	friend GVector3 operator- (const GVector3& v1, const GVector3& v2);/*两向量减法,差向量的终点指向被减向量的终点*/
	friend GVector3 operator* (const GVector3& v, float f);/*向量乘浮点数，向量的各分量分别乘浮点数*/
	friend GVector3 operator* (float f, const GVector3& v);/*浮点数乘向量，向量的各分量分别乘浮点数*/
	friend GVector3 operator* (const GVector3& v1, GVector3& v2);/*两个向量乘法，向量的各分量对应相乘*/
	friend bool operator== (const GVector3& v1, GVector3& v2);/*判断2个向量是否相等*/
};

/*颜色*/
typedef GVector3 Color;

/*像素*/
typedef unsigned int Pixel;

#endif/*_COMMON_H_*/