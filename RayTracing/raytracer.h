#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "cray.h"
#include "common.h"
#include "object.h"

//���߸�����
class tracer
{
public:
	tracer();
	~tracer();
	GVector3 eye;//�۾�λ��
	Scene* m_scene;//����
	GVector3 trace(Cray ray, int depth);	//���߸��ٺ��������������е��������壬Ȼ�󷵻ؽ��㴦����ɫ
	void render();//����
	void save_jpg();
	Color screen_color[ImageWidth][ImageHeight];//�洢��Ļ�����ص㣨x,y��������ɫ
	unsigned char buffer[ImageWidth][ImageHeight][3];
};


#endif