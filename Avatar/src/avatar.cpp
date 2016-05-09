#define SDL_VIDEO_MODE_OPTIONS (SDL_RESIZABLE | SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER /*| SDL_OPENGL*/)
#define SDL_DEPTH 32

#define SCENE_ROTATION_STEP 5
#define CAMERA_Z_OFFSET 2
#define CAMERA_TRANSLATION_STEP 0.1

#define RDR_FRAME_LENGTH 1
#define RDR_CUBE_HALF_SIDE 0.5

#include "avatar.h"
#include "sdlglutils.h"
#include<iostream>

CAvatar::CAvatar() {
    should_be_running=true;
    needs_rendering=true;
    world_origin_x=0;
    world_origin_y=0;
    world_origin_z=0;
    window_width=640;
    window_height=480;
    window_title="Avatar Main Window";
    camera_aspect_ratio = ((float)window_width)/((float)window_height);
    camera_min_z = 0.1;
    camera_max_z = 10;
    camera_fovy = 60;
    world_rx=45;
    world_ry=45;
    camera_tx=world_origin_x;
    camera_ty=world_origin_y;
    camera_min_tz=world_origin_z+CAMERA_Z_OFFSET;
    camera_tz=camera_min_tz;
    sensor_mode=false;
}

CAvatar::~CAvatar() {}

int CAvatar::OnExecute(bool mode)
{
    if(OnInit(mode) == false)//Test sur le mode vidéo et la vue configurée
    {
        return -1;
    }

    SDL_Event event;
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, //Active la répétition de touche
                        SDL_DEFAULT_REPEAT_INTERVAL);

    should_be_running = true;

    while(should_be_running){
        while(SDL_PollEvent(&event))//renvoie 1 si un évènement est en attente
        {
            OnEvent(&event);
        }

        OnLoop();//vide
        OnRender();//Affiche une nouvelle image
        //du repère et du cube en mouvement
    }
    OnCleanup();//Libère la place à la prochaine image

    return 0;
}

void CAvatar::SetOrthoProjectionMatrix()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,1,0,1,-10,10);
}

bool CAvatar::OnInit(bool mode)
{
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH_LINE_WIDTH_GRANULARITY);
    glEnable(GL_LIGHT0);

    GLfloat light_position[4] = {5.0f,-5.0f, 5.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    if (mode) {
        sensor_mode = true;
        sensor.OnInit(sensor_mode);
    } else
        sensor_mode = false;

    char sdl_wdw_pos[] = "SDL_VIDEO_WINDOW_POS";
    char sdl_wdw_ctr[] = "SDL_VIDEO_CENTERED=1";
    putenv(sdl_wdw_pos);
    putenv(sdl_wdw_ctr);

    if(SDL_Init(SDL_INIT_EVERYTHING)<0)//Initialise toutes les fonctions SDL
    {
        return false;
    }

    window_width = 640;
    window_height = 480;
    window_title ="Avatar Main Window";

    SDL_WM_SetCaption(window_title, 0);//Met le titre
    sdl_pimage = SDL_SetVideoMode(//Activation du mode vidéo
                                  window_width, window_height,
                                  SDL_DEPTH, /*SDL_VIDEO_MODE_OPTIONS*/SDL_OPENGL);
    if(sdl_pimage == NULL)//La zone graphique est vide
        return false;


    glClearColor(0, 0, 0, 0);//Efface les buffers color
    glViewport(0, 0, window_width, window_height);//On adapte la vue à la fenêtre

    camera_aspect_ratio = ((float)window_width)/((float)window_height);
    camera_min_z = 0.1;
    camera_max_z = 10;
    camera_fovy = 60;
    SetPerspectiveProjectionMatrix();//Modifie l'affichage pour avoir la vue souhaitée


    //Configuration de la fenêtre
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE ,16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE ,8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE ,8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE ,8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE ,8);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS ,1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES ,2);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    identifiant_texture = loadTexture("../images/myimage.png");
    return true;
}

void CAvatar::OnCleanup()
{
    if(sdl_pimage)
        SDL_FreeSurface(sdl_pimage);//Libère les pixels de l'image
    SDL_Quit();//Tout effacer avant de quitter

}

void CAvatar::OnLoop(){}

void CAvatar::OnRender()
{
    if(!sensor_mode)
        DrawDemo();
    else
        DrawSensor();
}

void CAvatar::DrawDemo()
{

    //Déclaration des matériaux
    GLfloat colorBronzeDiff[4] = { 0.8, 0.6, 0.0, 1.0 };
    GLfloat colorBronzeSpec[4] = { 1.0, 1.0, 0.4, 1.0 };
    GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};

    //Application et activation des effets de lumières/matériaux
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBronzeDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, colorBronzeSpec);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

    if(! needs_rendering)//laisser true
        return;
    needs_rendering = true;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Efface les buffers color et depth
    glEnable(GL_DEPTH_TEST);//Met à jour les buffers depth

    GLfloat scaling[] = {1, 0, 0, 0, 0, 1.5, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    glMatrixMode(GL_MODELVIEW);//Modifie la matrice courante dans la vue de l'objet
    glLoadIdentity();//Remplace cette matrice par l'identité
    glTranslatef(-camera_tx, -camera_ty, -camera_tz);//Translation
    glRotatef(world_rx, 1, 0, 0);//Rotation
    glRotatef(world_ry, 0, 1, 0);//Rotation
    glMultMatrixf(scaling);//Multiplie la matrice courante par scaling

    DrawFrame(world_origin_x, world_origin_y, world_origin_z, RDR_FRAME_LENGTH);
    DrawCube(world_origin_x, world_origin_y, world_origin_z, RDR_CUBE_HALF_SIDE, identifiant_texture);

    SDL_GL_SwapBuffers();//Met à jour l'affichage de la nouvelle image
}

void CAvatar::DrawSensor()
{
    if (sensor.active_stream == color_stream)
        DrawColor();
    else
        DrawDepth();
}

void CAvatar::DrawDepth()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -2.5);

    openni::VideoFrameRef m_depthFrame;

    sensor.m_colorStream.readFrame(&m_depthFrame);
    if (! m_depthFrame.isValid())
        return;
    const openni::DepthPixel* pDepth = (const openni::DepthPixel*) m_depthFrame.getData();

    openni::VideoFrameRef m_colorFrame;

    sensor.m_colorStream.readFrame(&m_colorFrame);
    if(!m_colorFrame.isValid())
        return;
    const openni::RGB888Pixel* pImage = (const openni::RGB888Pixel*) m_colorFrame.getData();

    GLuint* texture;
    texture[0] = Load2DTexture(m_colorFrame.getWidth(), m_colorFrame.getHeight(), 3, pImage);

    int width = m_depthFrame.getWidth();
    int height = m_depthFrame.getHeight();

    float pWorldX, pWorldY, pWorldZ;

    DrawFrame(world_origin_x,world_origin_y, world_origin_z, RDR_FRAME_LENGTH);

    glPointSize(2);
    glBegin(GL_POINTS);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x <width; x++) {
            if ((x % 2 == 0) && (y * 2 == 0) && (*pDepth != 0) && (*pDepth < 2000)) {
                openni::CoordinateConverter::convertDepthToWorld(sensor.m_depthStream, x, y, *pDepth, &pWorldX, &pWorldY, &pWorldZ);
                glColor3f(pImage -> r/255.0, pImage -> g/255.0, pImage -> b/255.0);
                glVertex3f(pWorldX/1000.0, pWorldY / 1000.0, pWorldZ / 2000.0);
            }
            pDepth++;
            pImage++;
        }
    }
    glEnd();

    SDL_GL_SwapBuffers();
}

void CAvatar::DrawColor()
{
    openni::VideoFrameRef m_colorFrame;

    sensor.m_colorStream.readFrame(&m_colorFrame);

    if(!m_colorFrame.isValid())
        return;

    const openni::RGB888Pixel* pImage = (const openni::RGB888Pixel*) m_colorFrame.getData();
    GLuint* texture;
    texture[0] = Load2DTexture(m_colorFrame.getWidth(), m_colorFrame.getHeight(), 3, pImage);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0, 0, -1);
    FillWindowWithTexture(texture[0]);
    DrawFrame(world_origin_x,world_origin_y,world_origin_z, RDR_FRAME_LENGTH);

    SDL_GL_SwapBuffers();
}

void CAvatar::InitSceneConstants()
{
    world_rx=0;
    world_ry=0;

    camera_min_tz = world_origin_z + CAMERA_Z_OFFSET;
    camera_tx = world_origin_x;
    camera_ty = world_origin_y;
    camera_tz = camera_min_tz;
}

void CAvatar::SetPerspectiveProjectionMatrix()
{
    glMatrixMode(GL_PROJECTION);//Modification de la matrice courante
    //Matrice qui détient la vue 2D de l'objet 3D
    glLoadIdentity();//Matrice courante devient l'identité
    gluPerspective(camera_fovy, camera_aspect_ratio, //Ajuste la matrice courante
                   camera_min_z, camera_max_z); //à la vue que l'on souhaite
}

void CAvatar::OnEvent(SDL_Event* Event){
    CEvent::OnEvent(Event);
}

void CAvatar::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode){
    switch(sym)
    {
    case SDLK_ESCAPE:
        OnExit();
        break;

    case SDLK_SPACE:
        InitSceneConstants();
        needs_rendering=true;
        break;

    case SDLK_LEFT:
        if(mod & KMOD_SHIFT)
        {
            world_rx-=SCENE_ROTATION_STEP;
            if(world_ry<0)
                world_ry+=360;
        }
        else
            camera_tx-=CAMERA_TRANSLATION_STEP;
        needs_rendering = true;
        break;

    case SDLK_RIGHT:
        if(mod & KMOD_SHIFT)
        {
            world_rx+=SCENE_ROTATION_STEP;
            if(world_ry>0)
                world_ry-=360;
        }
        else
            camera_tx+=CAMERA_TRANSLATION_STEP;
        needs_rendering = true;
        break;

    case SDLK_UP:
        if(mod & KMOD_SHIFT)
        {
            world_ry+=SCENE_ROTATION_STEP;
            if(world_rx>0)
                world_rx-=360;
        }
        else
            camera_ty+=CAMERA_TRANSLATION_STEP;
        needs_rendering = true;
        break;

    case SDLK_DOWN:
        if(mod & KMOD_SHIFT)
        {
            world_ry+=SCENE_ROTATION_STEP;
            if(world_rx<0)
                world_rx+=360;
        }
        else
            camera_ty-=CAMERA_TRANSLATION_STEP;
        needs_rendering = true;
        break;

    case SDLK_q:
        camera_tz-=CAMERA_TRANSLATION_STEP;
        if(camera_tz<camera_min_tz)
            camera_tz=camera_min_tz;
        needs_rendering=true;
        break;

    case SDLK_a:
        camera_tz+=CAMERA_TRANSLATION_STEP;
        needs_rendering=true;
        break;

    case SDLK_c:
        sensor.SetActiveStream(color_stream);
        OnInit(sensor_mode);
        break;
    case SDLK_p:
        sensor.SetActiveStream(depth_stream1);
        OnInit(sensor_mode);
        break;
    case SDLK_s:
        OnInit(!sensor_mode);
        break;
    }
}

void CAvatar::OnLButtonDown(int mX, int mY) {
    //std::cout << "\nHERE !" <<std::endl;
}

void CAvatar::OnLButtonUp(int mX, int mY) {}

void CAvatar::OnMouseWheelDown() {
    camera_tz-=CAMERA_TRANSLATION_STEP;
    if(camera_tz<camera_min_tz)
        camera_tz=camera_min_tz;
    needs_rendering=true;
}

void CAvatar::OnMouseWheelUp() {
    camera_tz+=CAMERA_TRANSLATION_STEP;
    needs_rendering=true;
}

void CAvatar::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle) {
    if(Left){
        if(relX < 0)
            camera_tx-=CAMERA_TRANSLATION_STEP;
        else if(relX > 0)
            camera_tx+=CAMERA_TRANSLATION_STEP;
        else if(relY < 0)
            camera_ty-=CAMERA_TRANSLATION_STEP;
        else if(relY > 0)
            camera_ty+=CAMERA_TRANSLATION_STEP;
    }
    else if(Right) {
        if(relX < 0) {
            world_ry-=SCENE_ROTATION_STEP;
            if(world_rx<0)
                world_rx+=360;
        }
        else if(relX > 0) {
            world_ry+=SCENE_ROTATION_STEP;
            if(world_rx<0)
                world_rx+=360;
        }
        if(relY < 0) {
            world_rx-=SCENE_ROTATION_STEP;
            if(world_ry<0)
                world_ry+=360;
        }
        if(relY > 0) {
            world_rx+=SCENE_ROTATION_STEP;
            if(world_ry<0)
                world_ry+=360;
        }
    }
    needs_rendering=true;
}

void CAvatar::OnResize(int w, int h)
{
    window_width=w;
    window_height=h;

    SDL_FreeSurface(sdl_pimage);
    sdl_pimage=SDL_SetVideoMode(window_width, window_height, SDL_DEPTH, SDL_OPENGL);

    glViewport(0,0,window_width, window_height);

    camera_aspect_ratio=((float)window_width)/((float)window_height);
    SetPerspectiveProjectionMatrix();

    needs_rendering=true;
}

void CAvatar::OnExpose(){

}

void CAvatar::OnExit(){
    should_be_running=false;
}

