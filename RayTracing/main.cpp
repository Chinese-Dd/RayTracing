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


void reshape(int w, int h)
{
	glViewport(0, 0, w, h); //改变显示区域，起始位置为客户端窗口左下角（非坐标原点）
	glMatrixMode(GL_PROJECTION); //修改投影矩阵
	glLoadIdentity();//导入单位阵
	gluPerspective(28.0, w / h, 1.0, 10.0);/*视角，实际窗口的纵横比即x/y，近截面，远截面*/
	glMatrixMode(GL_MODELVIEW);//修改模型视图
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);/*观察者位置.眼睛，物体，向上方向*/
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
	printf("\n渲染时间:%lf\n", time);
	tc.save_jpg();
}

/*主函数*/
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
