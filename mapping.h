#pragma once
#include <GL/freeglut.h>
#include <gl/glut.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmpfuncs.h"

void quadTextureMapping();
void draw_quad();

GLuint* texName = new GLuint();

void draw_quad()
{
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, *texName);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0, 0); glVertex3f(-500.0f, 0.0f, 500.0f);
	glTexCoord2f(5, 0); glVertex3f(500.0f, 0.0f, 500.0f);
	glTexCoord2f(5, 5); glVertex3f(500.0f, 0.0f, -500.0f);
	glTexCoord2f(0, 5); glVertex3f(-500.0f, 0.0f, -500.0f);
	glEnd();

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void quadTextureMapping()
{
	glGenTextures(1, texName);
	int width, height, channels;
	glBindTexture(GL_TEXTURE_2D, *texName);
	uchar* img = readImageData("img/EARTH.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}