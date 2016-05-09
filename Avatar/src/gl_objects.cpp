#include "gl_objects.h"
#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<SDL.h>
#include<iostream>

GLuint Load2DTexture(GLsizei width, GLsizei height, int BytesPerPixel, const GLvoid *data) {
    GLuint texture_ID;
    glGenTextures(1,&texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    int color_mode = (BytesPerPixel == 3 ? GL_RGB : GL_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, color_mode, width, height, 0, color_mode, GL_UNSIGNED_BYTE, data);

    return texture_ID;
}

void DrawFrame(float origin_x, float origin_y, float origin_z, float frame_length) {
    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(origin_x, origin_y, origin_z);
        glVertex3f(origin_x+frame_length, origin_y, origin_z);

        glColor3f(0,0,1);
        glVertex3f(origin_x, origin_y,origin_z);
        glVertex3f(origin_x, origin_y+frame_length, origin_z);

        glColor3f(0,1,0);
        glVertex3f(origin_x,origin_y,origin_z);
        glVertex3f(origin_x, origin_y, origin_z+frame_length);
    glEnd();
}

void DrawCube(float origin_x, float origin_y, float origin_z, float half_side, GLuint texture) {
    GLfloat light_position[4] = {2.0, 1.5, 2.0, 0.0};
    GLfloat colorBronzeDiff[4] = {0.8, 0.6, 0.0, 1.0};
    GLfloat colorBronzeSpec[4] = {1.0, 1.0, 0.4, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBronzeDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, colorBronzeSpec);

    glBegin(GL_QUADS);

        glBindTexture(GL_TEXTURE_2D, texture);

        //front
        glColor3f(1,1,1);
        glTexCoord2d(1,0);
        glVertex3f(origin_x+half_side, origin_y-half_side, origin_z+half_side);
        glTexCoord2d(1,1);
        glVertex3f(origin_x+half_side, origin_y+half_side, origin_z+half_side);
        glTexCoord2d(0,1);
        glVertex3f(origin_x-half_side, origin_y+half_side, origin_z+half_side);
        glTexCoord2d(0,0);
        glVertex3f(origin_x-half_side, origin_y-half_side, origin_z+half_side);

        //back
        glColor3f(1,1,1);
        glTexCoord2d(1,1);
        glVertex3f(origin_x+half_side, origin_y+half_side, origin_z-half_side);
        glTexCoord2d(1,0);
        glVertex3f(origin_x+half_side, origin_y-half_side, origin_z-half_side);
        glTexCoord2d(0,0);
        glVertex3f(origin_x-half_side, origin_y-half_side, origin_z-half_side);
        glTexCoord2d(0,1);
        glVertex3f(origin_x-half_side, origin_y+half_side, origin_z-half_side);

        //left
        glColor3f(1,1,1);
        glTexCoord2d(0,0);
        glVertex3f(origin_x-half_side, origin_y-half_side, origin_z-half_side);
        glTexCoord2d(1,0);
        glVertex3f(origin_x-half_side, origin_y+half_side, origin_z-half_side);
        glTexCoord2d(1,1);
        glVertex3f(origin_x-half_side, origin_y+half_side, origin_z+half_side);
        glTexCoord2d(0,1);
        glVertex3f(origin_x-half_side, origin_y-half_side, origin_z+half_side);

        //right
        glColor3f(1,1,1);
        glTexCoord2d(1,0);
        glVertex3f(origin_x+half_side, origin_y+half_side, origin_z-half_side);
        glTexCoord2d(1,1);
        glVertex3f(origin_x+half_side, origin_y+half_side, origin_z+half_side);
        glTexCoord2d(0,1);
        glVertex3f(origin_x+half_side, origin_y-half_side, origin_z+half_side);
        glTexCoord2d(0,0);
        glVertex3f(origin_x+half_side, origin_y-half_side, origin_z-half_side);

        //top
        glColor3f(1,1,1);
        glTexCoord2d(0,1);
        glVertex3f(origin_x-half_side, origin_y+half_side, origin_z+half_side);
        glTexCoord2d(1,1);
        glVertex3f(origin_x+half_side, origin_y+half_side, origin_z+half_side);
        glTexCoord2d(1,0);
        glVertex3f(origin_x+half_side, origin_y+half_side, origin_z-half_side);
        glTexCoord2d(0,0);
        glVertex3f(origin_x-half_side, origin_y+half_side, origin_z-half_side);

        //bottom
        glColor3f(1,1,1);
        glTexCoord2d(1,1);
        glVertex3f(origin_x+half_side, origin_y-half_side, origin_z+half_side);
        glTexCoord2d(0,1);
        glVertex3f(origin_x-half_side, origin_y-half_side, origin_z+half_side);
        glTexCoord2d(0,0);
        glVertex3f(origin_x-half_side, origin_y-half_side, origin_z-half_side);
        glTexCoord2d(1,0);
        glVertex3f(origin_x+half_side, origin_y-half_side, origin_z-half_side);
    glEnd();
}

void FillWindowWithTexture(GLuint texture)
{
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
        glColor3f(1,1,1);

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexCoord2f(1,1);
        glVertex3f(0,0,0);

        glTexCoord2f(1,0);
        glVertex3f(0,1,0);

        glTexCoord2f(0,0);
        glVertex3f(1,1,0);

        glTexCoord2f(0,1);
        glVertex3f(1,0,0);

    glEnd();

}
