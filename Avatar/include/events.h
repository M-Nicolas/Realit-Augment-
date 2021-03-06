#ifndef _EVENTS_H_
#define _EVENTS_H_

#include <SDL.h>

class CEvent
{
public:
    CEvent();
    virtual ~CEvent();
    void OnEvent(SDL_Event* Event);
    virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    virtual void OnLButtonDown(int mX, int mY);
    virtual void OnRButtonDown(int mX, int mY);
    virtual void OnMButtonDown(int mX, int mY);
    virtual void OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);
    virtual void OnMouseWheel(bool Up, bool Down);
    virtual void OnResize(int w, int h);
    virtual void OnExpose();
    virtual void OnExit();
};

#endif
