#ifndef _AVATAR_H_
#define _AVATAR_H_

#include <SDL/SDL.h>
#include "GL/gl.h"
#include "events.h"
#include "surface.h"
#include "sensor.h"

class CAvatar : public CEvent{
public:
    CAvatar();
    ~CAvatar();
    int OnExecute(bool use_sens);

    //Arm rotation angle
    float r1;
    float r2;
    float r3;
    float r4;

    // initialisation of the avatar (frame, opengl, camera, texture, lighting, matrix modelview)
    bool OnInit(bool use_sens);

    // to free the frame and the texture before exit
    void OnCleanup();

    // empty
    void OnLoop();

    // use each time we need to refresh the frame
    void OnRender(bool use_sens);

    // to draw the cube / arm
    void DrawDemo();

    // to use the sensor into the frame
    void DrawSensor();

    /* event gestion */
    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void OnLButtonDown(int mX, int mY);
    void OnRButtonDown(int mX, int mY);
    void OnMButtonDown(int mX, int mY);
    void OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);
    void OnMouseWheel(bool Up, bool Down);
    void OnResize(int w, int h);
    void OnExpose();
    void OnExit();
    /* event gestion */

    // to init/ re-init the camera and the world position
    void InitSceneConstants();

    // to set the matriw projection
    void SetPerspectiveProjectionMatrix();
    void SetOrthoProjectionMatrix();

private:
    bool use_sensor;
    bool sensor_mode;
    bool should_be_running;
    bool needs_rendering;

    int window_width;
    int window_height;

    float world_rx;
    float world_ry;
    float world_origin_z;
    float world_origin_y;
    float world_origin_x;

    double camera_min_tz;
    double camera_tx;
    double camera_ty;
    double camera_tz;
    double camera_min_z;
    double camera_max_z;
    double camera_aspect_ratio;
    double camera_fovy;

    double avant_bras_x;
    double avant_bras_y;
    double avant_bras_z;
    double avant_bras_shape;
    double avant_bras_angle;

    double bras_x;
    double bras_y;
    double bras_z;
    double bras_shape;
    double bras_angle;

    double main_x;
    double main_y;
    double main_z;
    double main_shape;
    double main_angle;

    const char* window_title;

    SDL_Surface*    Surf_Test;
    SDL_Surface* sdl_pimage;

    CSensor sensor;

    GLuint texture;
};

#endif
