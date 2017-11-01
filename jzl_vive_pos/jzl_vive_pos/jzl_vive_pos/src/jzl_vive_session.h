/*
**	JetZux Ltd. 2017
**  https://github.com/JetMeta
*/

#include "jzl_openvr_hdr.h"

#pragma once

class JzlViveSession
{
public:
    JzlViveSession();
    ~JzlViveSession();

    int init();
    void uninit();
    void get_pos_info();

private:
    DISALLOW_COPY_AND_ASSIGN(JzlViveSession);
    const char* get_track_dev_name(const vr::TrackedDeviceClass) const;
    void dump_vr_matrix(vr::HmdMatrix34_t&)const;

private:
    vr::IVRSystem* vr_system = nullptr;
};
