#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "common.h"
#include "raytracer.h"
#include "object.h"
#define MAX_TRIANGLES 2000
#define MAX_SPHERES 10
#define MAX_LIGHTS 10
#define MODE_DISPLAY 1
#define MODE_JPEG 2
Tracer tc;

/*��ʾ����*/
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	float dx = 2.0 / ImageWidth;/*����,��Ļԭ��Ϊ(0,0),���Ͻ�Ϊ(-1,-1)*/
	float dy = 2.0 / ImageHeight;
	float x = -1.0f;/*ʵ�ʳ�ʼλ��*/
	float y = -1.0f;
	glBegin(GL_POINTS);
	for (int i = 0; i<ImageHeight; i++)
	{
		x = -1.0f;
		for (int j = 0; j<ImageWidth; j++)
		{
			glColor3f(tc.screen_color[i][j].x, tc.screen_color[i][j].y, tc.screen_color[i][j].z);
			glVertex2f(x, y);
			x += dx;
		}
		y += dy;
	}
	glEnd();
	glFlush();
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h); //�ı���ʾ������ʼλ��Ϊ�ͻ��˴������½ǣ�������ԭ�㣩
	glMatrixMode(GL_PROJECTION); //�޸�ͶӰ����
	glLoadIdentity();//���뵥λ��
	gluPerspective(28.0, w / h, 1.0, 10.0);/*�ӽǣ�ʵ�ʴ��ڵ��ݺ�ȼ�x/y�������棬Զ����*/
	glMatrixMode(GL_MODELVIEW);//�޸�ģ����ͼ
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);/*�۲���λ��.�۾������壬���Ϸ���*/
}

/*��ʼ������*/
void init()
{
	glClearColor(1, 1, 1, 0);/*����ɫ����ɫ*/
	clock_t  start, finish;
	start = clock();//��ȡ��ʼʱ��
	tc.render();/*����*/
	finish = clock();//��ȡ����ʱ��
	float time = (float)(finish - start) / 1000;
	printf("\n��Ⱦʱ��:%lf\n", time);
	tc.save_jpg();
}

/*������*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("RayTracing");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
