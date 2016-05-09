#ifndef _AVATAR_H_
#define _AVATAR_H_
#include "gl_objects.h"
#include "events.h"
#include "sensor.h"
#include<SDL/SDL.h>
#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glu.h>

class CAvatar : public CEvent
{
    bool should_be_running;
    bool needs_rendering;
    int window_width;
    int window_height;
    const char* window_title;

    SDL_Surface* sdl_pimage;

    float world_rx;
    float world_ry;

    float world_origin_x;
    float world_origin_y;
    float world_origin_z;

    float camera_tx;
    float camera_ty;
    float camera_tz;

    float camera_min_tz;
    float camera_aspect_ratio;
    float camera_min_z;
    float camera_max_z;
    float camera_fovy;

    GLuint identifiant_texture;

    bool sensor_mode;
    CSensor sensor;

public:
    CAvatar();
    ~CAvatar();
    int OnExecute(bool mode);
    bool OnInit(bool mode);
    void OnCleanup();
    void OnLoop();
    void OnRender();
    void InitSceneConstants();
    void SetPerspectiveProjectionMatrix();
    void SetOrthoProjectionMatrix();
    void DrawDemo();
    void DrawSensor();
    void DrawDepth();
    void DrawColor();

    void OnEvent(SDL_Event* Event);
    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void OnMouseWheelUp();
    void OnMouseWheelDown();
    void OnLButtonUp(int mX, int mY);
    void OnLButtonDown(int mX, int mY);
    void OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle);
    void OnResize(int w, int h);
    void OnExpose();
    void OnExit();
};

#endif

