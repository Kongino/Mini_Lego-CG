#ifndef __DRAW_LEGO__
#define __DRAW_LEGO__
#include <iostream>
#include <string>
#include <GL/freeglut.h>
#include <GL/glut.h>

#include "ObjParser.h"
#include "bmpfuncs.h"

void DrawLego(int index, int color);
void DrawLegoList(int index, int color, GLfloat Trans1, GLfloat Trans2, GLfloat Trans3, GLfloat Rot1, GLfloat Rot2);
void draw_obj(ObjParser* objParser);

ObjParser* Lego1;
ObjParser* Lego2;
ObjParser* Lego3;
ObjParser* Lego4;
ObjParser* Lego5;
ObjParser* Lego6;
ObjParser* Lego7;
ObjParser* Lego8;
ObjParser* Lego9;
ObjParser* Lego10;
ObjParser* Lego11;
ObjParser* Lego12;

void DrawLegoList(int index, int color, GLfloat Trans1, GLfloat Trans2, GLfloat Trans3, GLfloat Rot1, GLfloat Rot2)
{
	glPushMatrix();
	glTranslatef(Trans1, Trans2, Trans3);

	glRotatef(90 * Rot1, 0, 0, 1);
	glRotatef(90 * Rot2, 0, 1, 0);
	DrawLego(index, color);
	glPopMatrix();
}

void DrawLego(int index, int color)
{
	Lego1 = new ObjParser("img/Lego_1.obj");
	Lego2 = new ObjParser("img/Lego_2.obj");
	Lego3 = new ObjParser("img/Lego_3.obj");
	Lego4 = new ObjParser("img/Lego_4.obj");
	Lego5 = new ObjParser("img/Lego_5.obj");
	Lego6 = new ObjParser("img/Lego_6.obj");
	Lego7 = new ObjParser("img/Lego_7.obj");
	Lego8 = new ObjParser("img/Lego_8.obj");
	Lego9 = new ObjParser("img/Lego_9.obj");
	Lego10 = new ObjParser("img/Lego_10.obj");
	Lego11 = new ObjParser("img/Lego_11.obj");
	Lego12 = new ObjParser("img/Lego_12.obj");


	ObjParser* Obj[12] = { Lego1, Lego2, Lego3, Lego4, Lego5, Lego6, Lego7, Lego8, Lego9, Lego10, Lego11, Lego12 };

	float RGB[3] = { 0, 0, 0 };

	switch (color)
	{
	case 1:
		RGB[0] = 1;
		break;
	case 2:
		RGB[1] = 1;
		break;
	case 3:
		RGB[2] = 1;
		break;
	case 4:
		RGB[1] = 0.64;
		RGB[2] = 0.82;
		break;
	case 5:
		RGB[0] = 1;
		RGB[2] = 0.56;
		break;
	case 6:
		RGB[0] = 1;
		RGB[1] = 1;
		break;
	case 7:
		RGB[0] = 1;
		RGB[1] = 1;
		RGB[2] = 1;
		break;
	case 8:
		RGB[0] = 0.5;
		RGB[1] = 0.5;
		RGB[2] = 0.5;
		break;
	case 9:
		RGB[0] = 0.6;
		RGB[1] = 0;
		RGB[2] = 0.6;
		break;
	case 10:
		RGB[0] = 1.0;
		RGB[1] = 0.5;
		RGB[2] = 0;
		break;
	}

	glColor3f(RGB[0], RGB[1], RGB[2]);
	draw_obj(Obj[index - 1]);
}

void draw_obj(ObjParser* objParser)
{

	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3)
	{
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x,
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x,
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}

	glEnd();
}

#endif