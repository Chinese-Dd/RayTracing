#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "common.h"
#include "CRAY.h"
#include <iostream>
#include <windows.h>

class Object
{
protected:
	float m_spec;/*镜面反射强度*/
	float m_refl;/*环境反射强度*/
public:
	Object();
	~Object(){};
	float get_spec();//计算镜面反射
	float get_refl();//计算环境反射光线
	float get_diffuse();//计算漫反射
	void reset(float spec, float refl);/*重新设置成员变量值*/
	virtual GVector3 get_color(GVector3 position) = 0;//获取物体表面颜色的纯虚基函数
	virtual GVector3 get_normal(GVector3 point) = 0;//获取物体表面一点法线的纯虚基函数
	virtual  INTERSECTION_TYPE is_intersected(Cray ray, float& distance) = 0;/*判断光线是否与物体相交的纯虚基函数*/
	int style;
};

class Sphere:public Object
{
private:
	Color m_color;/*颜色*/
	GVector3 m_center;/*球心*/
	float m_radius;/*半径*/
public:
	Sphere();
	Sphere(GVector3 center, float radius);
	Sphere(GVector3 center, float radius, GVector3 color, float spec, float refl);
	~Sphere(){};
	GVector3 get_color(GVector3 position);
	GVector3 get_center();
	float get_radius();
	void reset_c_r(GVector3 cent, float radi, GVector3 color);/*重新设置球心和半径和颜色*/
	GVector3 get_normal(GVector3 point);/*获取球体表面一点的法线*/
	INTERSECTION_TYPE is_intersected(Cray ray, float& dstance);/*判断光线是否与球体相交*/
};

class Triangle :public Object
{
private:
	Color m_color[3];/*颜色*/
	GVector3 position[3];/*三个定点位置*/
	GVector3 m_normal;/*平板法向量*/
	bool is_color_equal;/*三个顶点颜色是否一样*/
public:
	Triangle();
	Triangle(GVector3 nor,GVector3 pos1, GVector3 pos2, GVector3 pos3);
	Triangle(GVector3 nor, GVector3 pos1, GVector3 pos2, GVector3 pos3, GVector3 color1, GVector3 color2, GVector3 color3, float spec, float refl);
	GVector3 get_color(GVector3 position);
	float get_distance();
	bool get_equal();
	GVector3 get_normal(GVector3 position);/*获取平板表面一点的法线*/
	bool InTriangle(const GVector3& orig, const GVector3& dir, float* t, float* u, float* v);/*判断光线是是否在三角形内*/
	INTERSECTION_TYPE is_intersected(Cray ray, float& dist);/*判断光线是否与平板相交*/
};

class PointLight
{
public:
	PointLight();
	GVector3 get_pos();
	Color get_col();
	void reset(GVector3 position, Color color);/*重新设置光源位置和颜色*/
private:
	GVector3 m_position;/*位置*/
	Color m_color;/*颜色*/
};

class Scene
{
private:
	int m_object_num;/*物体数量*/
	int m_light_num;/*光源数量*/
	Color amb_light;/*环境光*/
public:
	Scene();
	~Scene();
	void init_scene();/*初始化场景*/
	int get_object_num();/*场景中物体数量*/
	int get_light_num();/*场景中光源数量*/
	void set_amblight(Color amb);/*设置环境光*/
	Color get_amblight();/*获取环境光*/
	PointLight *m_light;/*new的正常用法*/
	Object **m_obj;/*存储的内容为物体的指针,需要用指针去操作指针*/
	Object *get_objects(int idx);/*取出编号为idx的物体*/
	PointLight get_light(int idx);/*取出编号为idx的光源*/
	int loadScene(char *filename);/*加载场景*/
	//文件分析函数
	void parse_check(char *expected,char *found);
	void parse_doubles(FILE *file, char *check, double *p);
	void parse_radious(FILE *file,double *r);
	void parse_shininess(FILE *file, double *shin);
};


#endif 