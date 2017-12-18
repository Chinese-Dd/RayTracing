#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "cray.h"
#include "common.h"
#include "object.h"

//光线跟踪类
class tracer
{
public:
	tracer();
	~tracer();
	GVector3 eye;//眼睛位置
	Scene* m_scene;//场景
	GVector3 trace(Cray ray, int depth);	//光线跟踪函数，遍历场景中的所有物体，然后返回交点处的颜色
	void render();//场景
	void save_jpg();
	Color screen_color[ImageWidth][ImageHeight];//存储屏幕上像素点（x,y）处的颜色
	unsigned char buffer[ImageWidth][ImageHeight][3];
};


#endif