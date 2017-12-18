#include "OBJECT.h"
#include<iostream>
using namespace std;
//一般物体类
Object::Object()
{
	m_spec = 1.0f;
	m_refl = 1.0f;
}

float Object::get_spec()
{
	return m_spec;
}

float Object::get_refl()
{
	return m_refl;
}

float Object::get_diffuse()
{
	return 1.0f - m_spec;
}

void Object::reset(float spec, float refl)
{
	m_spec = spec;
	m_refl = refl;
}

//球体类 
Sphere::Sphere()
{
	m_color = GVector3(0, 0, 0);
	m_center = GVector3(0, 0, 0);
	m_radius = 1.0f;
	style = 0;
}

Sphere::Sphere(GVector3 cent, float radi)
{
	m_color = GVector3(0, 0, 0);
	m_center = cent;
	m_radius = radi;
	style = 0;
}

Sphere::Sphere(GVector3 cent, float radi, GVector3 color, float spec, float refl)
{
	m_center = cent;
	m_radius = radi;
	m_color = color;
	m_refl = refl;
	m_spec = spec;
	style = 0;
}

void Sphere::reset_c_r(GVector3 cent, float radi, GVector3 color)
{
	m_center = cent;
	m_radius = radi;
	m_color = color;
}
Color Sphere::get_color(GVector3 position)
{
	return m_color;
}

GVector3 Sphere::get_center()
{
	return m_center;
}

float Sphere::get_radius()
{
	return m_radius;
}

GVector3 Sphere::get_normal(GVector3 point)
{
	return (point - m_center);                //法线是球面上的该交点p和球心的c的差向量
}

INTERSECTION_TYPE  Sphere::is_intersected(Cray ray, float& distance)
{
	GVector3 v = ray.get_origin() - m_center;
	float a = DOT(ray.get_direction(), v);
	float b = (DOT(v, v) - m_radius*m_radius);
	float c = (a*a) - b;
	INTERSECTION_TYPE retval = MISS;
	if (c > 0)
	{
		c = sqrt(c);
		float temp1 = -a - c;
		float temp2 = -a + c;
		if (temp2 >0)
		{
			if (temp1 < 0)
			{
				if (temp2 < distance)
				{
					distance = temp2;
					retval = INTERSECTED_IN;
				}
			}
			else if (temp1>0)
			{
				if (temp1 < distance)
				{
					distance = temp1;
					retval = INTERSECTED;
				}
			}
		}
	}
	return retval;
}

/*三角形类*/
Triangle::Triangle()
{
	m_normal = GVector3(0, 0, 0);
	style = 2;
}

Triangle::Triangle(GVector3 nor,GVector3 po1, GVector3 po2, GVector3 po3)
{
	m_normal = nor;
	position[0] = po1;
	position[1] = po2;
	position[2] = po3;
	style = 2;
}

Triangle::Triangle(GVector3 nor, GVector3 po1, GVector3 po2, GVector3 po3, GVector3 color1, GVector3 color2, GVector3 color3, float spec, float refl)
{
	position[0] = po1;
	position[1] = po2;
	position[2] = po3;
	m_color[0] = color1;
	m_color[1] = color2;
	m_color[2] = color3;
	if (color1 == color2&&color1 == color3){
		is_color_equal = true;
	}
	else{
		is_color_equal = false;
	}
	m_normal = nor;
	m_refl = refl;
	m_spec = spec;
	style = 2;
}

GVector3 Triangle::get_color(GVector3 pos)
{
	//printf("获得颜色！\n");
	if (is_color_equal)
		return m_color[0];
	else{
		//printf("clolor:<%f,%f,%f>\n", m_color[0], m_color[1], m_color[2]);
		float dis1 = pos.distance(position[0]);
		float dis2 = pos.distance(position[1]);
		float dis3 = pos.distance(position[2]);
		float dis = dis1 + dis2 + dis3;
		dis1 /= dis;
		dis2 /= dis;
		dis3 /= dis;
		return dis1*m_color[0] + dis2*m_color[1] + dis3*m_color[2];
	}
}


/*点光源类*/
PointLight::PointLight()
{
	m_position = GVector3(0, 0, 0);
	m_color = GVector3(1, 1, 1);
}
Color PointLight::get_col()
{
	return m_color;
}

GVector3 PointLight::get_pos()
{
	return m_position;
}

void PointLight::reset(GVector3 position, Color color)
{
	m_color = color;
	m_position = position;
}

/*场景类*/
Scene::Scene()
{
	m_object_num = 0;
	m_light_num = 0;
}

Scene::~Scene()
{
	delete m_obj;
	delete m_light;
}

void Scene::init_scene()
{
   loadScene("screenfile.txt");
}

void Scene::set_amblight(Color amb)
{
	amb_light = amb;
}

Color Scene::get_amblight( )
{
	return amb_light;
}

int Scene::get_object_num()
{
	return m_object_num;
}

int Scene::get_light_num()
{
	return m_light_num;
}

Object *Scene::get_objects(int idx)
{
	return m_obj[idx];
}

PointLight Scene::get_light(int idx)
{
	return m_light[idx];
}

int  Scene::loadScene(char *filename)
{
	m_light = new PointLight[1500];
	m_obj = new Object*[1500];
	FILE *file = fopen(filename, "r");
	int Number_of_objects;
	fscanf(file, "%d", Number_of_objects);
	cout << "Number of objects:" << Number_of_objects << endl; //读取物体数
	double ambient_light[3];
	parse_doubles(file, "amb:", ambient_light);
	amb_light = Color(ambient_light[0], ambient_light[1], ambient_light[2]); //读取amb

	char type[25];
	int i;
	for (i = 0; i < Number_of_objects; i++)
	{
		fscanf(file, "%s", type);
		if (strcmp(type, "triangle") == 0)
		{
			cout << "Find triangle" << endl;
			int j;
			double pos[3][3], nor[3], dif[3][3], spe[3], shi;
			for (j = 0; j < 3; j++)
			{
				parse_doubles(file, "pos:", pos[j]);
				parse_doubles(file, "nor:", nor);
				parse_doubles(file, "pos:", dif[j]);
				parse_doubles(file, "spe:", spe);
				parse_doubles(file, "shi:", &shi);
			}
			m_obj[m_object_num] = new Triangle(GVector3(nor[0], nor[1], nor[2]), GVector3(pos[0][1], pos[0][2], pos[0][3]), GVector3(pos[1][1], pos[1][2], pos[1][3]), GVector3(pos[2][1], pos[2][2], pos[2][3]), GVector3(dif[0][1], dif[0][2], dif[0][3]), GVector3(dif[1][1], dif[1][2], dif[1][3]), GVector3(dif[2][1], dif[2][2], dif[2][3]), float(spe[0]), 1.0);//疑问1,最后为啥shi设为1.0,待解决
			m_object_num++;
			if (m_object_num >= 10000)
			{
				cout << "Too many triangles, you should increase MAX_TRIANGLES!\n" << endl;
				exit(0);
			}
		}
			else if (strcmp(type, "sphere")==0)
			{
				double pos[3], rad, dif[3], spe[3], shi[3];
				parse_doubles(file, "pos:", pos);
				parse_doubles(file, "rad:", &rad);
				parse_doubles(file, "dif:", dif);
				parse_doubles(file, "spe:", spe);
				parse_doubles(file, "shi:", shi);
				m_obj[m_object_num] = new Sphere(GVector3(pos[0], pos[1], pos[2]), rad, GVector3(dif[0], dif[1], dif[2]), spe[0], 1);//疑问2,最后为啥shi设为1.0,待解决
				m_object_num++;
				if (m_object_num >= 10000)
				{
					cout << "Too many spheres, you should increase MAX_SPHERES!\n" << endl;
					exit(0);
				}
			}
			else if (stricmp(type, "light") == 0)
			{
				double pos[3], col[3];
				parse_doubles(file, "pos:", pos);
				parse_doubles(file, "col:", col);
				m_light[m_light_num].reset(GVector3(pos[0], pos[1], pos[2]), Color(col[0], col[1], col[2]));
				m_light_num++;
				if (m_light_num == 10)
				{
					printf("Too many lights, you should increase MAX_LIGHTS!\n");
					exit(0);
				}
			}
			else
			{
				printf("Unknown type in scene description:\n %s\n", type);
				exit(0);
			}
	}
	printf("对象数目:%d\n光源数目:%d\n", m_object_num, m_light_num);
	return 0;
}

void Scene::parse_check(char *expected, char *found)
{
	if (strcmp(expected, found))
	{
		char error[100];
		printf("Expected '%s ' found '%s '\n", expected, found);
		printf("Parse error, abnormal abortion\n");
		exit(0);
	}
}

void Scene::parse_doubles(FILE *file, char *check, double *p)
{
	char temp[100];
	fscanf(file, "%s", temp);
	parse_check(check, temp);
	fscanf(file, "%lf %lf %lf", &p[0], &p[1], &p[2]);
	printf("%s %lf %lf %lf", check,p[0], p[1], p[2]);
}

void Scene::parse_radious(FILE *file, double *r)
{
	char temp[100];
	fscanf(file, "%s", temp);
	parse_check("rad:", temp);
	fscanf(file, "%lf", &r);
	printf("rad: %f\n", *r);
}

void Scene::parse_shininess(FILE *file, double *shin)
{
	char temp[100];
	fscanf(file, "%s", temp);
	parse_check("shi", temp);
	fscanf(file, "%lf", shin);
	printf("shi: %f\n", *shin);
}


