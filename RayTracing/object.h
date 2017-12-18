#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "common.h"
#include "CRAY.h"
#include <iostream>
#include <windows.h>

class Object
{
protected:
	float m_spec;/*���淴��ǿ��*/
	float m_refl;/*��������ǿ��*/
public:
	Object();
	~Object(){};
	float get_spec();//���㾵�淴��
	float get_refl();//���㻷���������
	float get_diffuse();//����������
	void reset(float spec, float refl);/*�������ó�Ա����ֵ*/
	virtual GVector3 get_color(GVector3 position) = 0;//��ȡ���������ɫ�Ĵ��������
	virtual GVector3 get_normal(GVector3 point) = 0;//��ȡ�������һ�㷨�ߵĴ��������
	virtual  INTERSECTION_TYPE is_intersected(Cray ray, float& distance) = 0;/*�жϹ����Ƿ��������ཻ�Ĵ��������*/
	int style;
};

class Sphere:public Object
{
private:
	Color m_color;/*��ɫ*/
	GVector3 m_center;/*����*/
	float m_radius;/*�뾶*/
public:
	Sphere();
	Sphere(GVector3 center, float radius);
	Sphere(GVector3 center, float radius, GVector3 color, float spec, float refl);
	~Sphere(){};
	GVector3 get_color(GVector3 position);
	GVector3 get_center();
	float get_radius();
	void reset_c_r(GVector3 cent, float radi, GVector3 color);/*�����������ĺͰ뾶����ɫ*/
	GVector3 get_normal(GVector3 point);/*��ȡ�������һ��ķ���*/
	INTERSECTION_TYPE is_intersected(Cray ray, float& dstance);/*�жϹ����Ƿ��������ཻ*/
};

class Triangle :public Object
{
private:
	Color m_color[3];/*��ɫ*/
	GVector3 position[3];/*��������λ��*/
	GVector3 m_normal;/*ƽ�巨����*/
	bool is_color_equal;/*����������ɫ�Ƿ�һ��*/
public:
	Triangle();
	Triangle(GVector3 nor,GVector3 pos1, GVector3 pos2, GVector3 pos3);
	Triangle(GVector3 nor, GVector3 pos1, GVector3 pos2, GVector3 pos3, GVector3 color1, GVector3 color2, GVector3 color3, float spec, float refl);
	GVector3 get_color(GVector3 position);
	float get_distance();
	bool get_equal();
	GVector3 get_normal(GVector3 position);/*��ȡƽ�����һ��ķ���*/
	bool InTriangle(const GVector3& orig, const GVector3& dir, float* t, float* u, float* v);/*�жϹ������Ƿ�����������*/
	INTERSECTION_TYPE is_intersected(Cray ray, float& dist);/*�жϹ����Ƿ���ƽ���ཻ*/
};

class PointLight
{
public:
	PointLight();
	GVector3 get_pos();
	Color get_col();
	void reset(GVector3 position, Color color);/*�������ù�Դλ�ú���ɫ*/
private:
	GVector3 m_position;/*λ��*/
	Color m_color;/*��ɫ*/
};

class Scene
{
private:
	int m_object_num;/*��������*/
	int m_light_num;/*��Դ����*/
	Color amb_light;/*������*/
public:
	Scene();
	~Scene();
	void init_scene();/*��ʼ������*/
	int get_object_num();/*��������������*/
	int get_light_num();/*�����й�Դ����*/
	void set_amblight(Color amb);/*���û�����*/
	Color get_amblight();/*��ȡ������*/
	PointLight *m_light;/*new�������÷�*/
	Object **m_obj;/*�洢������Ϊ�����ָ��,��Ҫ��ָ��ȥ����ָ��*/
	Object *get_objects(int idx);/*ȡ�����Ϊidx������*/
	PointLight get_light(int idx);/*ȡ�����Ϊidx�Ĺ�Դ*/
	int loadScene(char *filename);/*���س���*/
	//�ļ���������
	void parse_check(char *expected,char *found);
	void parse_doubles(FILE *file, char *check, double *p);
	void parse_radious(FILE *file,double *r);
	void parse_shininess(FILE *file, double *shin);
};


#endif 