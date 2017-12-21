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

/*显示函数*/
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	float dx = 2.0 / ImageWidth;/*增量,屏幕原点为(0,0),左上角为(-1,-1)*/
	float dy = 2.0 / ImageHeight;
	float x = -1.0f;/*实际初始位置*/
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

/*初始化函数*/
void init()
{
	glClearColor(1, 1, 1, 0);/*背景色，白色*/
	clock_t  start, finish;
	start = clock();//获取开始时间
	tc.render();/*绘制*/
	finish = clock();//获取结束时间
	float time = (float)(finish - start) / 1000;
	printf("\n渲染时间:%lfs\n", time);
	tc.save_jpg();
}

/*主函数*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("RayTracing");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
