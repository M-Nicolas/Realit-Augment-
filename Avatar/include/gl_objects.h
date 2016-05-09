#ifndef _GL_OBJECTS_H
#define _GL_OBJECTS_H_

#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glu.h>

GLuint Load2DTexture(GLsizei width, GLsizei height, int BytesPerPixel, const GLvoid *data);
void DrawFrame(float origin_x, float origin_y, float origin_z, float frame_length);
void DrawCube(float origin_x, float origin_y, float origin_z, float half_side, GLuint texture);
void FillWindowWithTexture(GLuint texture);

#endif
