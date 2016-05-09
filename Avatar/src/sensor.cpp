#include "sensor.h"
#include <iostream>
using namespace std;

CSensor::CSensor() {}

CSensor::~CSensor() {
    if (status != openni::STATUS_ERROR) {
        m_colorStream.stop();
        m_colorStream.destroy();

        m_depthStream.stop();
        m_depthStream.destroy();

        openni::OpenNI::shutdown();
    }
}

bool CSensor::OnInit(bool show_color_stream) {
    //On initialise OpenNI
    status = openni::OpenNI::initialize();
    m_device.open(openni::ANY_DEVICE);

    m_colorStream.create(m_device, openni::SENSOR_COLOR);
    m_colorStream.start();
    if (!m_colorStream.isValid()) {
        cout << openni::OpenNI::getExtendedError() << endl;
        return false;
    }

    m_depthStream.create(m_device,openni::SENSOR_DEPTH);
    m_depthStream.start();
    if (!m_depthStream.isValid()) {
        cout << openni::OpenNI::getExtendedError() << endl;
        return false;
    }

    if(m_device.isImageRegistrationModeSupported(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR))
        m_device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);

    if(active_stream == depth_stream1)
        m_device.setDepthColorSyncEnabled(true);

    if(show_color_stream)
        active_stream = color_stream;
}

bool CSensor::SetActiveStream(EActiveStream requested_stream) {
    if (active_stream != requested_stream)
        active_stream = requested_stream;

    return true;
}
