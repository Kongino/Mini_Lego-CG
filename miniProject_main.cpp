
#include <gl/glut.h>
#include<GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "bmpfuncs.h"
#include "ObjParser.h"
#include "Draw_Lego.h"
#include "mapping.h"

void init();
void draw();
void keyboard(unsigned char key, int x, int y);
void specialkeyboard(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseWheel(int but, int dir, int x, int y);
void motion(int x, int y);
void main_menu_function(int option);
void sub_menu_function(int option);
void sub_menu_function2(int option);

void resize(int width, int height);
void preview_Lego();

int LegoIndex = 1;
int LegoColor = 1;
int LegoCount = 0;
int insertmode = 0;
int deleteCount = 0;

GLfloat LegoList[100][7];
GLfloat LegoTrans[3] = { 0, 0, 0 };
GLfloat LegoRot[3] = { 0, 0, 0 };

GLfloat saveSlot1[100][7];
int count_slot1 = 0;
int do_load_slot1 = 0;
GLfloat saveSlot2[100][7];
int count_slot2 = 0;
int do_load_slot2 = 0;

#define PI 3.14159265
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900

double theta = 45.0;
double phi = 45.0;
int radius = 50;
double upVector = cos(phi * PI / 180);

double eyex, eyey, eyez;
int MouseX = -1;
int MouseY = -1;

int current_width, current_height;


int main(int argc, char** argv)
{
	/* Window �ʱ�ȭ */
	int submenu1;
	int submenu2;

	printf("[1 2 3 4 5 6 7 8 9 0 - =] : ���� ���� ����\n");
	printf("[F1 F2                  ] : ���� ���� ����\n");
	printf("[�� �� �� ��            ] : ���� ��� ȸ��\n");
	printf("[W A S D Q Z            ] : ���� ��� �̵�\n");
	printf("[Insert                 ] : ���� �̸����� -> ���� ����ȭ������ �̵� -> ���� �Է�\n");
	printf("[PAGE_DOWN PAGE_UP      ] : ���� ���� ����, ������ ���� �ǵ�����\n");


	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("My GL Program");
	init(); // -> ����� �ʱ�ȭ �Լ�

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeyboard);

	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMouseWheelFunc(mouseWheel);

	submenu1 = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("slot1", 1);
	glutAddMenuEntry("slot2", 2);


	submenu2 = glutCreateMenu(sub_menu_function2);
	glutAddMenuEntry("slot1", 1);
	glutAddMenuEntry("slot2", 2);


	glutCreateMenu(main_menu_function);
	glutAddSubMenu("Save", submenu1);
	glutAddSubMenu("Load", submenu2);
	glutAddMenuEntry("Clear", 1);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutReshapeFunc(resize);

	glutDisplayFunc(draw); // draw() -> ���� �׸��� �Լ�

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Looping ����
	glutMainLoop();

	return 0;
}

void init(void)
{
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);

	// ȭ�� ��ǥ ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);


	GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat light_diffuse[] = { 0.7f, 0.7f, 0.7f,1.0f };
	GLfloat light_specular[] = { 1.0f,1.0f,1.0f,1.0f };

	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	GLfloat ambientMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseMaterial[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat specularMaterial[] = { 0.9f, 0.9f, 0.9f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 70);

	quadTextureMapping();
	glEnable(GL_TEXTURE_2D);

	//���� ����ġ�� 0�� ���� ����ġ �ѱ�
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);

	//��ü�� �÷��Ӽ�����
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

}

void resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)width / (double)height, 1, 500);
	current_width = width;
	current_height = height;
	glMatrixMode(GL_MODELVIEW);
}

void draw_axis()
{
	glLineWidth(4); // ��ǥ��β�
	glBegin(GL_LINES);

	glColor3f(1, 0, 0);  // x��
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);

	glColor3f(0, 1, 0); //y��
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);

	glColor3f(0, 0, 1); //z��
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);

	glEnd();
	glLineWidth(1); 
}

void draw(void)
{
	eyex = radius * sin(theta * PI / 180) * cos(phi * PI / 180);
	eyey = radius * sin(phi * PI / 180);
	eyez = radius * cos(theta * PI / 180) * cos(phi * PI / 180);
	upVector = cos(phi * PI / 180);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, upVector, 0);

	GLfloat light_position[] = { 30, 40, 50, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glViewport(0, 0, current_width, current_height);
	draw_axis();

	draw_quad();

	if (do_load_slot1 == 1) // saveSlot�� ����� �������� LegoList�� ����
	{
		for (int i = 0; i < count_slot1; i++)
		{
			for (int j = 0; j < 7; j++)
				LegoList[i][j] = saveSlot1[i][j];
		}
		LegoCount = count_slot1;
		do_load_slot1 = 0;
	}

	if (do_load_slot2 == 1)
	{
		for (int i = 0; i < count_slot2; i++)
		{
			for (int j = 0; j < 7; j++)
				LegoList[i][j] = saveSlot2[i][j];
		}
		LegoCount = count_slot2;
		do_load_slot2 = 0;
	}

	for (int i = 0; i < LegoCount; i++) // ���±��� �Էµ� ��� ������� Draw
	{
		DrawLegoList(LegoList[i][0], LegoList[i][1], LegoList[i][2], LegoList[i][3], LegoList[i][4], LegoList[i][5], LegoList[i][6]);
	}

	if (insertmode == 1) // ���� �̸����� ����
	{
		preview_Lego();
		LegoTrans[0] = 0;
		LegoTrans[1] = 0;
		LegoTrans[2] = 0;
		LegoRot[0] = 0;
		LegoRot[1] = 0;
	}
	if (insertmode == 2) // ���� ȭ�鿡�� ���� �����̴� �κ�.
	{
		glPushMatrix();
		glTranslatef(LegoTrans[0], LegoTrans[1], LegoTrans[2]);

		glRotatef(90 * LegoRot[0], 0, 0, 1);
		glRotatef(90 * LegoRot[1], 0, 1, 0);
		DrawLego(LegoIndex, LegoColor);

		glPopMatrix();
	}
	if (insertmode == -1) // ���� draw�� ���� LegoList�� ���
	{
		glPushMatrix();
		glTranslatef(LegoTrans[0], LegoTrans[1], LegoTrans[2]);

		glRotatef(90 * LegoRot[0], 0, 0, 1);
		glRotatef(90 * LegoRot[1], 0, 1, 0);
		DrawLego(LegoIndex, LegoColor);

		glPopMatrix();

		LegoList[LegoCount][0] = LegoIndex;
		LegoList[LegoCount][1] = LegoColor;
		LegoList[LegoCount][2] = LegoTrans[0];	
		LegoList[LegoCount][3] = LegoTrans[1];;
		LegoList[LegoCount][4] = LegoTrans[2];
		LegoList[LegoCount][5] = LegoRot[0];
		LegoList[LegoCount][6] = LegoRot[1];
		LegoCount++; // �������� ������� LegoList�� ���
		LegoTrans[0] = 0;
		LegoTrans[1] = 0;
		LegoTrans[2] = 0;
		LegoRot[0] = 0;
		LegoRot[1] = 0;
		LegoIndex = 1;
		LegoColor = 1; // ��� ���� ��ġ&ȸ�� �ʱ�ȭ
		
		if (deleteCount > 0)
			deleteCount = 0; // ���� �Է��� ����Ǹ� ������ ���� ���Է��� �ʱ�ȭ��
		
		// ���� ���� ������ �ʱ�ȭ
		insertmode = 0; // �۾� �� insertmode�� �ʱ���·�
	}

	glFlush();
	glutSwapBuffers(); // double buffering��

}

void preview_Lego()
{
	glLoadIdentity();
	gluLookAt(11, 11, 11, 0, 0, 0, 0, 1, 0);
	glViewport(current_width * 3/4, current_height * 3/4, current_width/4, current_height/4);
	draw_axis();
	DrawLego(LegoIndex, LegoColor);
	glFlush();
}

void mouse(int button, int state, int x, int y) {
	//printf("button : %d\n", button);
	//printf("state : %d\n", state);
	//printf("[%d], [%d]", x, y);
	if (state == 1)
	{
		MouseX = -1;
		MouseY = -1;
	} // Ŭ������ ���� ����
}
void motion(int x, int y)
{
	//printf("Mouse is moving! (%d, %d)\n", x, y);
	if (MouseX != -1 && MouseY != -1)
	{
		theta = theta + (MouseX-x);
		if (theta <= -360) theta = 0.0;

		phi = phi + (y-MouseY);
		if (abs((int)phi) % 360 == 0) phi = 0.0;
	}
	MouseX = x;
	MouseY = y;
	//���� ���콺 ��ġ�� ���� ��ġ���� ���� �� theta�� phi�� ����
	// �� �� �������� MouseX�� MouseY���� ����
	glutPostRedisplay();
}

void mouseWheel(int but, int dir, int x, int y)
{
	if (dir > 0) {
		if (radius > 2) radius--;
	}
	else {
		if (radius < 100) radius++;
	}
	glutPostRedisplay();
}

void main_menu_function(int option)
{
	if (option == 999)
		exit(0);
	if (option == 1) // Clear. �ʱ�ȭ
		LegoCount = 0;

	glutPostRedisplay();
}
void sub_menu_function(int option)
{
	if (option == 1)
	{
		for (int i = 0; i < LegoCount; i++)
		{
			for (int j = 0; j < 7; j++)
				saveSlot1[i][j] = LegoList[i][j];
		}
		count_slot1 = LegoCount;
		LegoCount = 0; // saveSlot�� ���� draw�� ������� ����. ���� �Ŀ� ȭ�� clear
	}
	if (option == 2)
	{
		for (int i = 0; i < LegoCount; i++)
		{
			for (int j = 0; j < 7; j++)
				saveSlot2[i][j] = LegoList[i][j];
		}
		count_slot2 = LegoCount;
		LegoCount = 0;
	}
	glutPostRedisplay();
}
void sub_menu_function2(int option)
{
	if (option == 1)
	{
		if (count_slot1 > 0)
			do_load_slot1 = 1; // �������� �������� load����
	}
	if (option == 2)
	{
		if (count_slot2 > 0)
			do_load_slot2 = 1;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	//printf("You pressed %c\n", key);
	
	switch (key)
	{
	case '1':
		LegoIndex = 1;
		break;
	case '2':
		LegoIndex = 2;
		break;
	case '3':
		LegoIndex = 3;
		break;
	case '4':
		LegoIndex = 4;
		break;
	case '5':
		LegoIndex = 5;
		break;
	case '6':
		LegoIndex = 6;
		break;
	case '7':
		LegoIndex = 7;
		break;
	case '8':
		LegoIndex = 8;
		break;
	case '9':
		LegoIndex = 9;
		break;
	case '0':
		LegoIndex = 10;
		break;
	case '-':
		LegoIndex = 11;
		break;
	case '=':
		LegoIndex = 12;
		break;

	case 'q':
		LegoTrans[1]++;
		break;
	case 'z':
		LegoTrans[1]--;
		break;
	case 'a':
		LegoTrans[2]++;
		break;
	case 'd':
		LegoTrans[2]--;
		break;
	case 's':
		LegoTrans[0]++;
		break;
	case 'w':
		LegoTrans[0]--;
		break;
	}
	glutPostRedisplay();

}

void specialkeyboard(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1:
	{
		LegoColor++;
		if (LegoColor > 10)
			LegoColor = 1;
		break;
	}
	case GLUT_KEY_F2:
	{
		LegoColor--;
		if (LegoColor < 1)
			LegoColor = 10;
		break;
	}

	case GLUT_KEY_INSERT:
		if (insertmode == 0)
			insertmode = 1; // ���� �̸����� ����
		else if (insertmode == 1)
			insertmode = 2; // ���� ȭ�鿡 ���� �Է�&�̵� ����
		else
			insertmode = -1; // ���� �������� ���� LegoList�� �߰�
		break;

	case GLUT_KEY_PAGE_DOWN:
		if (LegoCount > 0)
		{
			LegoCount--;
			deleteCount++; // ���� �ֱ� �Էµ� ���� ����
		}
		break;
		
	case GLUT_KEY_PAGE_UP:
		if (deleteCount > 0)
		{
			LegoCount++;
			deleteCount--; // ���� �ֱ� ������ ���� �ٽ� �Է�
		}
		break;

	case GLUT_KEY_UP:
		LegoRot[0]++;
		break;
	case GLUT_KEY_DOWN:
		LegoRot[0]--;
		break;
	case GLUT_KEY_RIGHT:
		LegoRot[1]++;
		break;
	case GLUT_KEY_LEFT:
		LegoRot[1]--;
		break;

	}
	glutPostRedisplay();
}