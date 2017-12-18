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
	float distance = INFINITY;/*初始化无限大距离*/
	int dep = depth;/*递归深度*/
	Object* aim;/*交点物体*/
	GVector3 n;/*交点处的法向量*/
	GVector3 point;/*交点*/
	GVector3 dire_eye;/*交点到眼睛的方向*/
	GVector3 dire_ls;/*交点到光源的方向*/

	Color hit_color = m_scene->get_amblight();/*交点处的颜色*/
	int res_hit;/*相交情况结果*/

	/*遍历场景中的每一个物体,获取其中的一个离眼睛最近的点所在物体*/
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
	    point = ray.get_Point(distance);/*获取交点*/
	    n = aim->get_normal(point);/*交点处的法向量*/
	    n.normalize();/*单位化*/
	    /*如果在物体内与物体相交*/
	    if (res_hit == INTERSECTED_IN)
	     {
	     	n =-1*n;
	    }
	    dire_eye = eye - point; /*交点到眼睛的方向*/
	    dire_eye.normalize();
	    for (int j = 0; j < m_scene->get_light_num();j++)
	    {
		PointLight light = m_scene->get_light(j);
		dire_ls = light.get_pos()-point;/*交点到光源的方向*/
		dire_ls.normalize();

		Cray dire_ls_ray(point + dire_ls*SMALL, dire_ls);/*构造交点到光源的射线*/
		float shade = 1.0f;/*初始阴影,1.0表示交点不在阴影中*/
		float distance = INFINITY;/*初始化无限大距离*/
		/*测试是否有物体遮挡住光源*/
		for (int k = 0; k < m_scene->get_object_num(); k++)
		{
			Object *temp = m_scene->get_objects(k);
			if (temp->is_intersected(dire_ls_ray, distance))
			{
				shade = 0.0f;
				break;
			}
		}
		/*漫反射*/
		if (aim->get_diffuse() > 0)
		{
			float cos = DOT(dire_ls, n);/*光线与交点处法线的夹角的余弦*/
			if (cos > 0)
			{
				float diffuse = cos*aim->get_diffuse()*shade;
				/*将漫反射光加到光线颜色中*/
				hit_color = hit_color + diffuse*light.get_col()*aim->get_color(point);
			}
		}

		/*镜面反射*/
		if (aim->get_spec() > 0)
		{
			/*Phong高光*/
			GVector3 h = 2 * DOT(n, dire_ls)*n - dire_ls;/*反射光线*/
			float cos = DOT(h, dire_eye);/*反射光线与视线方向的夹角余弦*/
			if (cos > 0)
			{
				float specular = powf(cos, 20)*aim->get_spec()*shade;
				hit_color = hit_color + specular*light.get_col();
			}
		}
	}

	/*反射光线*/
	if (aim->get_refl() > 0 && dep < TOTALDEPTH)
	{
		GVector3 refl = 2 * DOT(n, dire_ls)*n - dire_ls;//反射光线
		hit_color = hit_color + trace(Cray(point + refl*0.0001f, refl), ++dep);
	}
}
	else
	{
		return Color(1.0,1.0,1.0);
	}
	return hit_color;/*没有相交时，是初始黑色；有相交时，是交点处的颜色*/
}
void tracer::render()
{
	eye.set(0.0f, 0.0f, 4.0f);/*眼睛位置*/
	m_scene->init_scene();/*读入场景*/

	  /*虚拟屏幕坐标增量*/
	  float dx = (float)(VIEW_WIDTH / ImageWidth); //试验2,是否需要static
	  float dy = (float)(VIEW_HEIGHT / ImageHeight);//试验2,是否需要static

	/*虚拟屏幕初始坐标*/
	float view_x = -VIEW_WIDTH / 2.0;  //坐标原点在中间,左上角为(-3,-3),右下角为(3,3)
	float view_y = -VIEW_HEIGHT / 2.0;
	float percent;
	int a = 1;
	int i, j;
#pragma omp parallel for private(i,j,view_x,view_y) //下面的for循环要分成多个线程跑，每个线程都要保存变量括号中的拷贝
	/*遍历所有像素点*/
	for (i = 0; i < ImageWidth; i++)
	{
		for (j = 0; j < ImageHeight; j++)
		{
			view_x = -VIEW_WIDTH / 2.0 + dx*i;
			view_y = -VIEW_HEIGHT / 2.0 + dy*j;
			GVector3 dir = GVector3(view_x, view_y, 0) - eye;/*眼睛到像素点的方向,即射线方向*/
			dir.normalize();
			Cray ray(eye, dir);/*创造从眼睛出发的射线*/
			screen_color[i][j] = trace(ray, 1);
		}
		percent = (i *10)% 64;//等价于i%6.4
		if (percent == 0)
		{
			char t = 8;/*退格,\b的ASCII码,\b表示将输出位置左移一位*/
			for (int k = 0; k<11; k++)
			{
				std::cout << t;
			}
			std::cout << "进度：" << a++ << "%";
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

