#include"raytracer.h"
#include "common.h"
#include "picLibrary/pic.h"
tracer::tracer()
{
	m_scene = new Scene();
}
tracer::~tracer()
{
	delete m_scene;
}

GVector3 tracer::trace(Cray ray, int depth)
{
	float distance = INFINITY;/*��ʼ�����޴����*/
	int dep = depth;/*�ݹ����*/
	Object* aim;/*��������*/
	GVector3 n;/*���㴦�ķ�����*/
	GVector3 point;/*����*/
	GVector3 dire_eye;/*���㵽�۾��ķ���*/
	GVector3 dire_ls;/*���㵽��Դ�ķ���*/

	Color hit_color = m_scene->get_amblight();/*���㴦����ɫ*/
	int res_hit;/*�ཻ������*/

	/*���������е�ÿһ������,��ȡ���е�һ�����۾�����ĵ���������*/
	for (int i = 0; i < m_scene->get_object_num(); i++)
	{
		Object* obj = m_scene->get_objects(i);
		int res;
		if (res =obj->is_intersected(ray,distance))
		{
			aim = obj;
			res_hit = res;
		}
	}
	if (distance != INFINITY)
	{
	    point = ray.get_Point(distance);/*��ȡ����*/
	    n = aim->get_normal(point);/*���㴦�ķ�����*/
	    n.normalize();/*��λ��*/
	    /*������������������ཻ*/
	    if (res_hit == INTERSECTED_IN)
	     {
	     	n =-1*n;
	    }
	    dire_eye = eye - point; /*���㵽�۾��ķ���*/
	    dire_eye.normalize();
	    for (int j = 0; j < m_scene->get_light_num();j++)
	    {
		PointLight light = m_scene->get_light(j);
		dire_ls = light.get_pos()-point;/*���㵽��Դ�ķ���*/
		dire_ls.normalize();

		Cray dire_ls_ray(point + dire_ls*SMALL, dire_ls);/*���콻�㵽��Դ������*/
		float shade = 1.0f;/*��ʼ��Ӱ,1.0��ʾ���㲻����Ӱ��*/
		float distance = INFINITY;/*��ʼ�����޴����*/
		/*�����Ƿ��������ڵ�ס��Դ*/
		for (int k = 0; k < m_scene->get_object_num(); k++)
		{
			Object *temp = m_scene->get_objects(k);
			if (temp->is_intersected(dire_ls_ray, distance))
			{
				shade = 0.0f;
				break;
			}
		}
		/*������*/
		if (aim->get_diffuse() > 0)
		{
			float cos = DOT(dire_ls, n);/*�����뽻�㴦���ߵļнǵ�����*/
			if (cos > 0)
			{
				float diffuse = cos*aim->get_diffuse()*shade;
				/*���������ӵ�������ɫ��*/
				hit_color = hit_color + diffuse*light.get_col()*aim->get_color(point);
			}
		}

		/*���淴��*/
		if (aim->get_spec() > 0)
		{
			/*Phong�߹�*/
			GVector3 h = 2 * DOT(n, dire_ls)*n - dire_ls;/*�������*/
			float cos = DOT(h, dire_eye);/*������������߷���ļн�����*/
			if (cos > 0)
			{
				float specular = powf(cos, 20)*aim->get_spec()*shade;
				hit_color = hit_color + specular*light.get_col();
			}
		}
	}

	/*�������*/
	if (aim->get_refl() > 0 && dep < TOTALDEPTH)
	{
		GVector3 refl = 2 * DOT(n, dire_ls)*n - dire_ls;//�������
		hit_color = hit_color + trace(Cray(point + refl*0.0001f, refl), ++dep);
	}
}
	else
	{
		return Color(1.0,1.0,1.0);
	}
	return hit_color;/*û���ཻʱ���ǳ�ʼ��ɫ�����ཻʱ���ǽ��㴦����ɫ*/
}
void tracer::render()
{
	eye.set(0.0f, 0.0f, 4.0f);/*�۾�λ��*/
	m_scene->init_scene();/*���볡��*/

	  /*������Ļ��������*/
	  float dx = (float)(VIEW_WIDTH / ImageWidth); //����2,�Ƿ���Ҫstatic
	  float dy = (float)(VIEW_HEIGHT / ImageHeight);//����2,�Ƿ���Ҫstatic

	/*������Ļ��ʼ����*/
	float view_x = -VIEW_WIDTH / 2.0;  //����ԭ�����м�,���Ͻ�Ϊ(-3,-3),���½�Ϊ(3,3)
	float view_y = -VIEW_HEIGHT / 2.0;
	float percent;
	int a = 1;
	int i, j;
#pragma omp parallel for private(i,j,view_x,view_y) //�����forѭ��Ҫ�ֳɶ���߳��ܣ�ÿ���̶߳�Ҫ������������еĿ���
	/*�����������ص�*/
	for (i = 0; i < ImageWidth; i++)
	{
		for (j = 0; j < ImageHeight; j++)
		{
			view_x = -VIEW_WIDTH / 2.0 + dx*i;
			view_y = -VIEW_HEIGHT / 2.0 + dy*j;
			GVector3 dir = GVector3(view_x, view_y, 0) - eye;/*�۾������ص�ķ���,�����߷���*/
			dir.normalize();
			Cray ray(eye, dir);/*������۾�����������*/
			screen_color[i][j] = trace(ray, 1);
		}
		percent = (i *10)% 64;//�ȼ���i%6.4
		if (percent == 0)
		{
			char t = 8;/*�˸�,\b��ASCII��,\b��ʾ�����λ������һλ*/
			for (int k = 0; k<11; k++)
			{
				std::cout << t;
			}
			std::cout << "���ȣ�" << a++ << "%";
		}
	}
}

void tracer::save_jpg()
{
	for (int i = 0; i< ImageWidth; i++)
	{
		for (int j = 0; j< ImageHeight; j++)
		{
			buffer[ImageWidth - i - 1][j][0] = 256 * screen_color[i][j].x;
			buffer[ImageWidth - i - 1][j][1] = 256 * screen_color[i][j].y;
			buffer[ImageWidth - i - 1][j][2] = 256 * screen_color[i][j].z;
		}
	}
	Pic *in = NULL;
	char *filename = "result.jpg";
	in = pic_alloc(ImageWidth, ImageHeight, 3, NULL);
	printf("Saving JPEG file: %s\n", filename);

	memcpy(in->pix, buffer, 3 * ImageWidth *  ImageHeight);
	if (jpeg_write(filename, in))
		printf("File saved Successfully\n");
	else
		printf("Error in Saving\n");

	pic_free(in);
}

