#ifndef _GL_OBJECTS_H_
#define _GL_OBGECTS_H_

#include "GL/gl.h"
#include "OpenNI.h"

// to draw the frame
void DrawFrame(float origin_x, float origin_y, float origin_z, float frame_length);

// to draw a cube
void DrawCube(float origin_x, float origin_y, float origin_z, float half_size);

// to draw a cube with a crazy texture
void DrawCubeTexture(float origin_x, float origin_y, float origin_z, float half_size, GLuint texture_ID);

// to draw an amazing arm
void DrawArm(float origin_x, float origin_y, float origin_z);

// to charge a texture
// width and height : the dimensions of the texture
// BytesPerPixel : the format of the texture
// data : the structure of the texture
GLuint Load2DTexture(GLsizei width, GLsizei height, int BytesPerPixel, const GLvoid * data);

void FillWindowsWithTexture(GLuint texture);
void FillWindowsDepth(int width, int height, float min_depth, float max_depth, openni::DepthPixel* pImage);

#endif
