#include <GL/glew.h>
#include <GL/glu.h>

#include "avatar.h"
#include "gl_objects.h"

#define SDL_VIDEO_MODE_OPTIONS (SDL_RESIZABLE | SDL_OPENGL)
#define SDL_DEPTH 32

#define SCENE_ROTATION_STEP 5
#define CAMERA_Z_OFFSET 2
#define CAMERA_TRANSLATION_STEP 0.1

#define RDR_FRAME_LENGTH 1
#define RDR_CUBE_HALF_SIDE 0.5

CAvatar::CAvatar(){
    this->should_be_running = false;
    this->needs_rendering = true;

    this->window_width = 0;
    this->window_height = 0;
    this->window_title = NULL;
    this->sdl_pimage = NULL;

    this->world_origin_x = 0;
    this->world_origin_y = 0;
    this->world_origin_z = 0;

    this->r1 = 30.0F;
    this->r2 = -50.0F;
    this->r3 = 20.0F;
    this->r4 = 0.0F;
    this->r5 = 0.0F;
    this->r6 = 20.0F;
    this->r7 = 0.0F;

    this->Surf_Test = NULL;
    this->sensor_mode = true;

    InitSceneConstants();
}

CAvatar::~CAvatar(){}

int CAvatar::OnExecute(bool use_sens){
    if(OnInit(use_sens) == false){
        return -1;
    }
    SDL_Event event;
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    should_be_running = true;

    while(should_be_running){
        while(SDL_PollEvent(&event)){
            OnEvent(&event);
        }
        OnLoop();
        OnRender(use_sens);
    }
    OnCleanup();

    return 0;
}

bool CAvatar::OnInit(bool use_sens){
    char sdl_wdw_pos[] = "SDL_VIDEO_WINDOW_POS";
    char sdl_wdw_ctr[] = "SDL_VIDEO_CENTERED=1";
    putenv(sdl_wdw_pos);
    putenv(sdl_wdw_ctr);

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        return false;
    }

    window_width = 640;
    window_height = 480;
    window_title = "Avatar Main Window";

    SDL_WM_SetCaption(window_title, 0);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    /*SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);*/

    // mise en place d'une fenetre SDL
    sdl_pimage = SDL_SetVideoMode(window_width, window_height, SDL_DEPTH, SDL_VIDEO_MODE_OPTIONS);
    if(sdl_pimage == NULL){
        return false;
    }



    glClearColor(0,0,0,0);
    glViewport(0,0,window_width, window_height);
    camera_aspect_ratio = ((float)window_width/(float)window_height);
    camera_min_z = 0.1;
    camera_max_z = 10;
    camera_fovy = 60;

    if(use_sens){
        if(!sensor.OnInit(sensor_mode)){
            return false;
        }
        if(sensor.active_stream == color_stream){
            glEnable(GL_TEXTURE_2D);
            glMatrixMode(GL_MODELVIEW);

            SetOrthoProjectionMatrix();

        } else {

        }
    }
    else{
        if((Surf_Test = CSurface::OnLoad("../images/texture.bmp")) == NULL) {
            return false;
        }

        glEnable(GL_TEXTURE_2D);
        // active la lumiere
        glEnable(GL_LIGHTING);
        glShadeModel(GL_SMOOTH);
        // active la source 0
        glEnable(GL_LIGHT0);
        int pos[] = {1,0,0,1};
        // définition de la position de la lumiere
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glLightiv(GL_LIGHT0, GL_POSITION, pos);
        glPopMatrix();
        texture = Load2DTexture(Surf_Test->w, Surf_Test->h, Surf_Test->format_version, Surf_Test->pixels);

        SetPerspectiveProjectionMatrix();
    }
    return true;
}

void CAvatar::OnCleanup(){
    if(sdl_pimage){
        SDL_FreeSurface(sdl_pimage);
    }
    SDL_FreeSurface(Surf_Test);
    SDL_Quit();
}

void CAvatar::OnLoop(){}

void CAvatar::OnRender(bool use_sens){
    if(use_sens)DrawSensor();
    else DrawDemo();
}

void CAvatar::DrawDemo(){
    // est-ce qu'il faut faire un nouveau rendu
    if(!needs_rendering)
        return;
    //needs_rendering = false;


    // nettoyage du buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    GLfloat scaling[] = {1, 0, 0, 0,
                         0, 1.5, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1};

    // obj -> obj trans -> camera -> ecran
    // comment on projecte les objets
    // dans OnRender car lorsque on va changer de vue il faudra la changer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-camera_tx, -camera_ty, -camera_tz);
    glRotatef(world_rx, 1, 0, 0);
    glRotatef(world_ry, 0, 1, 0);
    glMultMatrixf(scaling);

    // les transformations sont faites dans l'ordre inverse
    // scaling -> rot -> rot -> translate

    // dessin
    DrawFrame(world_origin_x, world_origin_y, world_origin_z, RDR_FRAME_LENGTH);

    glPushMatrix();
    glRotatef(r1,0.0F,1.0F,0.0F);
    glRotatef(r4,1.0F,0.0F,0.0F);
    glRotatef(r7,0.0F,0.0F,1.0F);
    glTranslatef(1.5F,0.0F,0.0F);
    glPushMatrix();
    glScalef(3.0F,1.0F,1.0F);
    DrawCubeTexture(world_origin_x, world_origin_y, world_origin_z, RDR_CUBE_HALF_SIDE,texture);
    glPopMatrix();
    glTranslatef(1.5F,0.0F,0.0F);
    glRotatef(r2,0.0F,1.0F,0.0F);
    glRotatef(r5,1.0F,0.0F,0.0F);
    glTranslatef(1.5F,0.0F,0.0F);
    glPushMatrix();
    glScalef(3.0F,0.8F,0.8F);
    DrawCubeTexture(world_origin_x, world_origin_y, world_origin_z, RDR_CUBE_HALF_SIDE,texture);
    glPopMatrix();
    glTranslatef(1.7F,0.0F,0.0F);
    glRotatef(r3,1.0F,0.0F,0.0F);
    glRotatef(r6,0.0F,0.0F,1.0F);
    glPushMatrix();
    glScalef(0.6F,0.6F,0.6F);
    DrawCubeTexture(world_origin_x, world_origin_y, world_origin_z, RDR_CUBE_HALF_SIDE,texture);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

    //DrawCubeTexture(world_origin_x, world_origin_y, world_origin_z, RDR_CUBE_HALF_SIDE,texture);

    SDL_GL_SwapBuffers();
}

void CAvatar::DrawSensor(){
    // est-ce qu'il faut faire un nouveau rendu
    if(!needs_rendering)
        return;
    //needs_rendering = false;

    // nettoyage du buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    GLfloat scaling[] = {1, 0, 0, 0,
                         0, 1.5, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1};

    // obj -> obj trans -> camera -> ecran
    // comment on projecte les objets
    // dans OnRender car lorsque on va changer de vue il faudra la changer
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-camera_tx, -camera_ty, -camera_tz);
    glRotatef(world_rx, 1, 0, 0);
    glRotatef(world_ry, 0, 1, 0);
    glMultMatrixf(scaling);

    // les transformations sont faites dans l'ordre inverse
    // scaling -> rot -> rot -> translate

    // dessin
    if(sensor.active_stream == color_stream){
        openni::VideoFrameRef m_colorFrame;
        sensor.m_colorStream.readFrame(&m_colorFrame);
        if(!m_colorFrame.isValid()){
            return;
        }
        const openni::RGB888Pixel* pImage = (const openni::RGB888Pixel*)m_colorFrame.getData();
        texture = Load2DTexture(m_colorFrame.getWidth(), m_colorFrame.getHeight(), 3, pImage);


        FillWindowsWithTexture(texture);

        glDeleteTextures(1, &texture);
    }
    else{
        //switchStream(depth_stream);
        openni::VideoFrameRef m_depthFrame;
        sensor.m_depthStream.readFrame(&m_depthFrame);
        if(!m_depthFrame.isValid()){
            return;
        }
        int width = m_depthFrame.getWidth();
        int height = m_depthFrame.getHeight();
        float min_depth = sensor.m_depthStream.getMinPixelValue();
        float max_depth = sensor.m_depthStream.getMaxPixelValue();

        const openni::DepthPixel* pDepth = (const openni::DepthPixel*)m_depthFrame.getData();
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
               // pImage++;
            }
        }
        glEnd();

    }


    SDL_GL_SwapBuffers();
}

void CAvatar::InitSceneConstants(){
    world_rx = 0;
    world_ry = 0;
    camera_min_tz = world_origin_z + CAMERA_Z_OFFSET;
    camera_tx = world_origin_x;
    camera_ty = world_origin_y;
    camera_tz = camera_min_tz;
}

void CAvatar::SetPerspectiveProjectionMatrix(){
    glMatrixMode(GL_PROJECTION);// pas dans OnRender car constant dans notre cas
    glLoadIdentity();
    // definition des angles de vue, du ratio et plage d'avancement/recul
    gluPerspective(camera_fovy,camera_aspect_ratio,camera_min_z,camera_max_z);
}

void CAvatar::SetOrthoProjectionMatrix(){
    glMatrixMode(GL_PROJECTION);// pas dans OnRender car constant dans notre cas
    glLoadIdentity();
    glOrtho(0,1,0,1,-10,10); // on peut reculer jusqu'en -10 et avancer jusqu'en +10
}

void CAvatar::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode){
    switch (sym) {
    case SDLK_ESCAPE:
        OnExit();
        break;
    case SDLK_SPACE:
        InitSceneConstants();
        needs_rendering = true;
        break;
    case SDLK_LEFT:
        if(mod & KMOD_SHIFT){
            world_ry -= SCENE_ROTATION_STEP;
            if(world_ry < 0){
                world_ry+=360;
            }
        }else{
            camera_tx-=CAMERA_TRANSLATION_STEP;
        }
        needs_rendering = true;
        break;
    case SDLK_RIGHT:
        if(mod & KMOD_SHIFT){
            world_ry += SCENE_ROTATION_STEP;
            if(world_ry > 360){
                world_ry-=360;
            }
        }else{
            camera_tx+=CAMERA_TRANSLATION_STEP;
        }
        needs_rendering = true;
        break;
    case SDLK_DOWN:
        if(mod & KMOD_SHIFT){
            world_rx -= SCENE_ROTATION_STEP;
            if(world_rx < 0){
                world_rx+=360;
            }
        }else{
            camera_ty-=CAMERA_TRANSLATION_STEP;
        }
        needs_rendering = true;
        break;
    case SDLK_UP:
        if(mod & KMOD_SHIFT){
            world_rx += SCENE_ROTATION_STEP;
            if(world_rx > 360){
                world_rx-=360;
            }
        }else{
            camera_ty+=CAMERA_TRANSLATION_STEP;
        }
        needs_rendering = true;
        break;
    case SDLK_q:
        camera_tz-=CAMERA_TRANSLATION_STEP;
        if(camera_tz < camera_min_tz){
            camera_tz = camera_min_tz;
        }
        needs_rendering = true;
        break;
    case SDLK_a:
        camera_tz+=CAMERA_TRANSLATION_STEP;
        needs_rendering = true;
        break;
    case SDLK_c:
        sensor_mode = true;
        InitSceneConstants();
        SetOrthoProjectionMatrix();
        needs_rendering = true;
        break;
    case SDLK_p:
        sensor_mode = false;
        InitSceneConstants();
        SetPerspectiveProjectionMatrix();
        needs_rendering = true;
        break;
    case SDLK_t:
        r1 += 5.0F;
        needs_rendering = true;
        break;
    case SDLK_y:
        r1 -= 5.0F;
        needs_rendering = true;
        break;
    case SDLK_g:
        r2 += 5.0F;
        needs_rendering = true;
        break;
    case SDLK_h:
        r2 -= 5.0F;
        needs_rendering = true;
        break;
    case SDLK_b:
        r3 += 5.0F;
        needs_rendering = true;
        break;
    case SDLK_n:
        r3 -= 5.0F;
        needs_rendering = true;
        break;
    case SDLK_i:
        r4 += 5.0F;
        needs_rendering = true;
        break;
    case SDLK_o:
        r4 -= 5.0F;
        needs_rendering = true;
        break;
    case SDLK_k:
        r5 += 5.0F;
        needs_rendering = true;
        break;
    case SDLK_l:
        r5 -= 5.0F;
        needs_rendering = true;
        break;
    case SDLK_e:
        r6 += 5.0F;
        needs_rendering = true;
        break;
    case SDLK_r:
        r6 -= 5.0F;
        needs_rendering = true;
        break;
    case SDLK_w:
        r7 += 5.0F;
        needs_rendering = true;
        break;
    case SDLK_x:
        r7 -= 5.0F;
        needs_rendering = true;
        break;
    default:
        break;
    }
}

void CAvatar::OnLButtonDown(int mX, int mY) {/*NOTHING*/}
void CAvatar::OnRButtonDown(int mX, int mY) {/*NOTHING*/}
void CAvatar::OnMButtonDown(int mX, int mY) {/*NOTHING*/}

// mX et mY : position de la souris dans la fenetre
// relX et relY : deplacement de la souris
void CAvatar::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle){
    if(Left){
        camera_tx-=relX;
        camera_ty-=relY;
    }
    if(Right){
        world_rx-=relY;
        world_ry-=relX;
    }
}

void CAvatar::OnMouseWheel(bool Up, bool Down) {
    if(Up){
        camera_tz-=CAMERA_TRANSLATION_STEP;
        if(camera_tz < camera_min_tz) camera_tz = camera_min_tz;
    }
    if(Down){
        camera_tz+=CAMERA_TRANSLATION_STEP;
    }
}

void CAvatar::OnResize(int w, int h){
    window_width=w;
    window_height=h;

    SDL_FreeSurface(sdl_pimage);
    sdl_pimage = SDL_SetVideoMode(window_width, window_height, SDL_DEPTH, SDL_VIDEO_MODE_OPTIONS);

    glViewport(0, 0, window_width, window_height);

    camera_aspect_ratio = ((float)window_width)/((float)window_height);
    SetPerspectiveProjectionMatrix();

    needs_rendering = true;
}

void CAvatar::OnExpose(){
    needs_rendering = true;
}

void CAvatar::OnExit(){
    should_be_running = false;
}
