#include "gl_objects.h"

void DrawFrame(float origin_x, float origin_y, float origin_z, float frame_length){
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex3f(origin_x,origin_y,origin_z);
    glVertex3f(origin_x+frame_length, origin_y, origin_z);

    glColor3f(0,0,1);
    glVertex3f(origin_x,origin_y,origin_z);
    glVertex3f(origin_x, origin_y+frame_length, origin_z);

    glColor3f(0,1,0);
    glVertex3f(origin_x,origin_y,origin_z);
    glVertex3f(origin_x, origin_y, origin_z+frame_length);
    glEnd();
}

void DrawCube(float origin_x, float origin_y, float origin_z, float half_side){
    glBegin(GL_QUADS);
    // front
    glColor3f(0, 1, 0);
    glVertex3f(origin_x+half_side, origin_y-half_side, origin_z+half_side);
    glVertex3f(origin_x+half_side, origin_y+half_side, origin_z+half_side);
    glVertex3f(origin_x-half_side, origin_y+half_side, origin_z+half_side);
    glVertex3f(origin_x-half_side, origin_y-half_side, origin_z+half_side);
    // back
    glColor3f(0, 0.5, 0);
    glVertex3f(origin_x+half_side, origin_y+half_side, origin_z-half_side);
    glVertex3f(origin_x+half_side, origin_y-half_side, origin_z-half_side);
    glVertex3f(origin_x-half_side, origin_y-half_side, origin_z-half_side);
    glVertex3f(origin_x-half_side, origin_y+half_side, origin_z-half_side);
    // left
    glColor3f(0.5, 0, 0);
    glVertex3f(origin_x-half_side, origin_y-half_side, origin_z-half_side);
    glVertex3f(origin_x-half_side, origin_y+half_side, origin_z-half_side);
    glVertex3f(origin_x-half_side, origin_y+half_side, origin_z+half_side);
    glVertex3f(origin_x-half_side, origin_y-half_side, origin_z+half_side);
    // right
    glColor3f(1, 0, 0);
    glVertex3f(origin_x+half_side, origin_y+half_side, origin_z-half_side);
    glVertex3f(origin_x+half_side, origin_y+half_side, origin_z+half_side);
    glVertex3f(origin_x+half_side, origin_y-half_side, origin_z+half_side);
    glVertex3f(origin_x+half_side, origin_y-half_side, origin_z-half_side);
    // top
    glColor3f(0, 0, 1);
    glVertex3f(origin_x-half_side, origin_y+half_side, origin_z+half_side);
    glVertex3f(origin_x+half_side, origin_y+half_side, origin_z+half_side);
    glVertex3f(origin_x+half_side, origin_y+half_side, origin_z-half_side);
    glVertex3f(origin_x-half_side, origin_y+half_side, origin_z-half_side);
    // bottom
    glColor3f(0, 0, 0.5);
    glVertex3f(origin_x+half_side, origin_y-half_side, origin_z+half_side);
    glVertex3f(origin_x-half_side, origin_y-half_side, origin_z+half_side);
    glVertex3f(origin_x-half_side, origin_y-half_side, origin_z-half_side);
    glVertex3f(origin_x+half_side, origin_y-half_side, origin_z-half_side);
    glEnd();
}

void DrawCubeTexture(float origin_x, float origin_y, float origin_z, float half_side, GLuint texture_ID){
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glColor3f(1, 1, 1);

    //Déclaration des matériaux
    GLfloat diffuse[4] = { 0.8, 0.6, 0.0, 1.0 };
    GLfloat spec[4] = { 1.0, 1.0, 0.4, 1.0 };
    GLfloat mat[] = {0.3, 0.2, 0.2, 0.0};
    glBegin(GL_QUADS);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,spec);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
    glMaterialfv(GL_FRONT, GL_EMISSION,mat);

    // front
    glNormal3f(0,0,1);
    glTexCoord2f(1,1);
    glVertex3f(origin_x+half_side, origin_y-half_side, origin_z+half_side);
    glTexCoord2f(1,0);
    glVertex3f(origin_x+half_side, origin_y+half_side, origin_z+half_side);
    glTexCoord2f(0,0);
    glVertex3f(origin_x-half_side, origin_y+half_side, origin_z+half_side);
    glTexCoord2f(0,1);
    glVertex3f(origin_x-half_side, origin_y-half_side, origin_z+half_side);
    // back
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0);
    glVertex3f(origin_x+half_side, origin_y+half_side, origin_z-half_side);
    glTexCoord2f(0,1);
    glVertex3f(origin_x+half_side, origin_y-half_side, origin_z-half_side);
    glTexCoord2f(1,1);
    glVertex3f(origin_x-half_side, origin_y-half_side, origin_z-half_side);
    glTexCoord2f(1,0);
    glVertex3f(origin_x-half_side, origin_y+half_side, origin_z-half_side);
    // left
    glNormal3f(-1,0,0);
    glTexCoord2f(0,1);
    glVertex3f(origin_x-half_side, origin_y-half_side, origin_z-half_side);
    glTexCoord2f(0,0);
    glVertex3f(origin_x-half_side, origin_y+half_side, origin_z-half_side);
    glTexCoord2f(1,0);
    glVertex3f(origin_x-half_side, origin_y+half_side, origin_z+half_side);
    glTexCoord2f(1,1);
    glVertex3f(origin_x-half_side, origin_y-half_side, origin_z+half_side);
    // right
    glNormal3f(1,0,0);
    glTexCoord2f(1,0);
    glVertex3f(origin_x+half_side, origin_y+half_side, origin_z-half_side);
    glTexCoord2f(0,0);
    glVertex3f(origin_x+half_side, origin_y+half_side, origin_z+half_side);
    glTexCoord2f(0,1);
    glVertex3f(origin_x+half_side, origin_y-half_side, origin_z+half_side);
    glTexCoord2f(1,1);
    glVertex3f(origin_x+half_side, origin_y-half_side, origin_z-half_side);
    // top
    glNormal3f(0,1,0);
    glTexCoord2f(0,1);
    glVertex3f(origin_x-half_side, origin_y+half_side, origin_z+half_side);
    glTexCoord2f(1,1);
    glVertex3f(origin_x+half_side, origin_y+half_side, origin_z+half_side);
    glTexCoord2f(1,0);
    glVertex3f(origin_x+half_side, origin_y+half_side, origin_z-half_side);
    glTexCoord2f(0,0);
    glVertex3f(origin_x-half_side, origin_y+half_side, origin_z-half_side);
    // bottom
    glNormal3f(0,-1,0);
    glTexCoord2f(1,0);
    glVertex3f(origin_x+half_side, origin_y-half_side, origin_z+half_side);
    glTexCoord2f(0,0);
    glVertex3f(origin_x-half_side, origin_y-half_side, origin_z+half_side);
    glTexCoord2f(0,1);
    glVertex3f(origin_x-half_side, origin_y-half_side, origin_z-half_side);
    glTexCoord2f(1,1);
    glVertex3f(origin_x+half_side, origin_y-half_side, origin_z-half_side);
    glEnd();
}

void DrawArm(float origin_x, float origin_y, float origin_z){
    double half_size_bras = 0.7;
    double half_size_ab = 0.5;
    double half_size_main = 0.3;

    // bras
    glTranslatef(-half_size_bras, -half_size_bras, 0);
    DrawCube(origin_x, origin_y, origin_z, half_size_bras);

    // avant-bras
    glTranslatef(0, -half_size_bras-half_size_ab, 0);
    DrawCube(origin_x, origin_y, origin_z, half_size_ab);

    // main
    glTranslatef(0, -half_size_ab-half_size_main, 0);
    DrawCube(origin_x, origin_y, origin_z, half_size_main);
}

GLuint Load2DTexture(GLsizei width, GLsizei height, int BytesPerPixel, const GLvoid * data){
    GLuint texture_ID;
    glGenTextures(1,&texture_ID); // allocation, returns 1 texture names in texture_ID
    glBindTexture(GL_TEXTURE_2D, texture_ID); // creation of a named texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int color_mode = (BytesPerPixel == 3? GL_RGB : GL_RGBA);
    // The arguments describe the parameters of the texture image,
    // such as height,width,level-of-detail number and format.
    // The last three arguments describe how the image is represented in memory.
    glTexImage2D(GL_TEXTURE_2D, 0, color_mode, width, height, 0, color_mode, GL_UNSIGNED_BYTE, data);

    return texture_ID;
}

void FillWindowsWithTexture(GLuint texture){

    float origin_x =0.5;
    float origin_y = 0.4;
    float origin_z = 0;
    float half_side = 0.5;

    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1, 1, 1);

    float diffuse[] = {0.8,0.5,0.2,1.0};
    float spec[] = {0.9,0.6,0.3,1.0};
    float mat[] = {0.5,0.5,0.5,1.0};
    glBegin(GL_QUADS);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,spec);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
    glMaterialfv(GL_FRONT, GL_EMISSION,mat);

    glNormal3f(0,0,1);
    glTexCoord2f(1,1);
    glVertex3f(origin_x+half_side, origin_y-half_side, origin_z+half_side);
    glTexCoord2f(1,0);
    glVertex3f(origin_x+half_side, origin_y+half_side, origin_z+half_side);
    glTexCoord2f(0,0);
    glVertex3f(origin_x-half_side, origin_y+half_side, origin_z+half_side);
    glTexCoord2f(0,1);
    glVertex3f(origin_x-half_side, origin_y-half_side, origin_z+half_side);
    glEnd();
}

void FillWindowsDepth(int width, int height, float min_depth, float max_depth, openni::DepthPixel* pDepth) {
/*
    float pWorldX, pWorldY, pWorldZ;

    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glPointSize(2);
    glBegin(GL_POINTS);


    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            if((x%2 == 0) && (y % 2 == 0) && (*pDepth < 2000)) {
                openni::CoordinateConverter::convertDepthToWorld(sensor.m_depthStream, x, y, *pDepth, &pWorldX, &pWorldY, &pWorldZ);
                glColor3f(0, 1, 0);
                glVertex3f(pWorldX/400.0, pWorldY / 400.0, pWorldZ /  (max_depth-min_depth));

            }
            pDepth++;
            pImage++;
        }
    }
    glEnd();*/
}
