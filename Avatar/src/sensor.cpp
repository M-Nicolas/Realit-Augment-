#include <iostream>

#include "sensor.h"

using namespace std;

bool CSensor::OnInit(bool show_color_stream){
    if(openni::STATUS_OK!=openni::OpenNI::initialize()){ // init openni
        cout<<openni::OpenNI::getExtendedError()<<endl;
        return false;
    }

    if(openni::STATUS_OK!=m_device.open(openni::ANY_DEVICE)){ // ouverture périph
        cout<<openni::OpenNI::getExtendedError()<<endl;
        return false;
    }

    if(openni::STATUS_OK!=m_colorStream.create(m_device, openni::SENSOR_COLOR)){ // création flux vidéo
        cout<<openni::OpenNI::getExtendedError()<<endl;
        return false;
    }
    if(openni::STATUS_OK!=m_colorStream.start()){ //démarrage;
        cout<<openni::OpenNI::getExtendedError()<<endl;
        return false;
    }

    if(openni::STATUS_OK!=m_depthStream.create(m_device, openni::SENSOR_DEPTH)){
        cout<<openni::OpenNI::getExtendedError()<<endl;
        return false;
    }
    if(openni::STATUS_OK!=m_depthStream.start()){ //démarrage;
        cout<<openni::OpenNI::getExtendedError()<<endl;
        return false;
    }

    // init prop
    if(m_device.isImageRegistrationModeSupported(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR)){
        if(openni::STATUS_OK!=m_device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR)){
            cout<<openni::OpenNI::getExtendedError()<<endl;
            return false;
        }
    }


    if(openni::STATUS_OK!=m_device.setDepthColorSyncEnabled(true)){
        cout<<openni::OpenNI::getExtendedError()<<endl;
        return false;
    }


    if(!m_device.isValid()) return false; // validité des flux vidéos

    active_stream = show_color_stream? color_stream : depth_stream;

    return true;
}

void CSensor::switchStream(EActiveStream mode){
    if(mode==color_stream){
        if(openni::STATUS_OK!=m_device.setDepthColorSyncEnabled(true)){
            cout<<openni::OpenNI::getExtendedError()<<endl;
        }
    }
    else{
        if(openni::STATUS_OK!=m_device.setDepthColorSyncEnabled(false)){
            cout<<openni::OpenNI::getExtendedError()<<endl;
        }
    }
}

CSensor::CSensor(){

}


CSensor::~CSensor(){
    m_colorStream.stop();
    m_colorStream.destroy();

    m_depthStream.stop();
    m_depthStream.destroy();

    openni::OpenNI::shutdown();
}
