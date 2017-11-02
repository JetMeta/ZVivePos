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
    const char* get_track_dev_class(const int,const vr::TrackedDeviceClass) const;
    void dump_vr_matrix(vr::HmdMatrix34_t&)const;
	void dump_controller_property(const int)const;
	const std::string get_string_property(const int, const vr::ETrackedDeviceProperty)const;

private:
    vr::IVRSystem* vr_system = nullptr;
	int left_controller_idx = -1;
	int right_controller_idx = -1;
};
