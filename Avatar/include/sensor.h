#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "OpenNI.h"

enum EActiveStream{
    color_stream = 0,
    depth_stream = 1
};

class CSensor{
public:
    CSensor();
    ~CSensor();


    openni::Device m_device;
    openni::VideoStream m_colorStream;
    openni::VideoStream m_depthStream;
    EActiveStream active_stream;

    void switchStream(EActiveStream mode);

    bool OnInit(bool show_color_stream);
};

#endif
