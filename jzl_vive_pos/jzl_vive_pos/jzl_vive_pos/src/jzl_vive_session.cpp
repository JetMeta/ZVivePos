/*
**	JetZux Ltd. 2017
**  https://github.com/JetMeta
*/

#include "jzl_pch.h"
#include "jzl_vive_session.h"

JzlViveSession::JzlViveSession()
{
}

JzlViveSession::~JzlViveSession()
{

}

int JzlViveSession::init()
{
    vr::HmdError err;
    vr_system = vr::VR_Init(&err, vr::EVRApplicationType::VRApplication_Scene);

    if (vr_system)
    {
        return 1;
    }

    return 0;
}

void JzlViveSession::uninit()
{
    if (vr_system)
    {
        vr::VR_Shutdown();
    }
}

void JzlViveSession::get_pos_info()
{
    if (vr_system)
    {
        vr::TrackedDevicePose_t pose_array[vr::k_unMaxTrackedDeviceCount];
        vr_system->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseOrigin::TrackingUniverseStanding, 0.0f, pose_array, vr::k_unMaxTrackedDeviceCount);

        int idx = 0;
        for (vr::TrackedDeviceIndex_t i = 0; i < vr::k_unMaxTrackedDeviceCount; ++i)
        {
            vr::TrackedDevicePose_t pose = pose_array[i];
            if (pose.bPoseIsValid)
            {
                const vr::TrackedDeviceClass dev_class = vr_system->GetTrackedDeviceClass(i);

                std::cout << "idx:" << idx++ << "\t" << "seq:" << i << "\t" << get_track_dev_name(dev_class) << std::endl;

                dump_vr_matrix(pose.mDeviceToAbsoluteTracking);

                std::cout << std::endl;
            }
        }
    }
}

const char* JzlViveSession::get_track_dev_name(const vr::TrackedDeviceClass dev_class) const
{
    switch (dev_class)
    {
    case vr::TrackedDeviceClass::TrackedDeviceClass_HMD:
        return "HMD";
    case vr::TrackedDeviceClass_Controller:
        return "Controller";
    case vr::TrackedDeviceClass::TrackedDeviceClass_TrackingReference:
        return "TrackingReference";
    case vr::TrackedDeviceClass::TrackedDeviceClass_GenericTracker:
        return "GenericTracker";
    }

    return "Unknown";
}

void JzlViveSession::dump_vr_matrix(vr::HmdMatrix34_t& m)const
{
    std::cout << "pos :" << std::endl;
    std::cout << "\t" << m.m[0][3] << "\t" << m.m[1][3] << "\t" << m.m[2][3] << std::endl;

    std::cout << "rot :" << std::endl;
    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            std::cout << "\t";
            std::cout << m.m[i][j];
        }
        std::cout << std::endl;
    }
}
