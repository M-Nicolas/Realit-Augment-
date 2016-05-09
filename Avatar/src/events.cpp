#include "events.h"

CEvent::CEvent(){}
CEvent::~CEvent(){}

void CEvent::OnEvent(SDL_Event* Event) {
    switch(Event->type) {
    case SDL_KEYDOWN: {
        OnKeyDown(Event->key.keysym.sym, Event->key.keysym.mod, Event->key.keysym.unicode);
        break;
    }
    case SDL_MOUSEBUTTONDOWN: {
        switch(Event->button.button) {
        case SDL_BUTTON_LEFT: {
            OnLButtonDown(Event->button.x, Event->button.y);
            break;
        }
        case SDL_BUTTON_WHEELUP: {
            OnMouseWheelUp();
            break;
        }
        case SDL_BUTTON_WHEELDOWN: {
            OnMouseWheelDown();
            break;
        }
    }
    case SDL_MOUSEBUTTONUP: {
        switch(Event->button.button) {
            case SDL_BUTTON_LEFT: {
                OnLButtonUp(Event->button.x, Event->button.y);
                break;
            }
        }
    }
    case SDL_MOUSEMOTION: {
        OnMouseMove(Event->motion.x, Event->motion.y, Event->motion.xrel, Event->motion.yrel,
                            (Event->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT))!=0,
                            (Event->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT))!=0,
                            (Event->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE))!=0);
    }
    }
    }
}

void CEvent::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode){}
void CEvent::OnMouseWheelUp() {}
void CEvent::OnMouseWheelDown() {}
void CEvent::OnLButtonDown(int mX, int mY) {}
void CEvent::OnLButtonUp(int mX, int mY) {}
void CEvent::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle) {}
void CEvent::OnResize(int w, int h){}
void CEvent::OnExpose(){}
void CEvent::OnExit(){}
