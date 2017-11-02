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
		left_controller_idx = vr_system->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand);
		right_controller_idx = vr_system->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand);

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

                std::cout << "idx:" << idx++ << "\t" << "seq:" << i << "\t" << get_track_dev_class(i,dev_class) << std::endl;

                dump_vr_matrix(pose.mDeviceToAbsoluteTracking);

				if (dev_class == vr::TrackedDeviceClass_Controller)
				{
					dump_controller_property(i);
				}

                std::cout << std::endl;
            }
        }
    }
}

const char* JzlViveSession::get_track_dev_class(const int idx,const vr::TrackedDeviceClass dev_class) const
{
    switch (dev_class)
    {
    case vr::TrackedDeviceClass::TrackedDeviceClass_HMD:
        return "HMD";
    case vr::TrackedDeviceClass_Controller:
	{
		if (idx == left_controller_idx)
		{
			return "Controller - Left";
		}
		else
		{
			if (idx == right_controller_idx)
			{
				return "Controller - Right";
			}
			else
			{
				return "Controller";
			}
		}
	}

    case vr::TrackedDeviceClass::TrackedDeviceClass_TrackingReference:
        return "TrackingReference";

    case vr::TrackedDeviceClass::TrackedDeviceClass_GenericTracker:
        return "GenericTracker";

	case vr::TrackedDeviceClass::TrackedDeviceClass_DisplayRedirect:
		return "DisplayRedirect";

	case vr::TrackedDeviceClass::TrackedDeviceClass_Invalid:
		return "Invalid";
    }

    return "Unknown";
}

void JzlViveSession::dump_vr_matrix(vr::HmdMatrix34_t& m)const
{
	std::cout << std::setprecision(10);
    std::cout << "pos :" << std::endl;
    std::cout << "\t" <<  m.m[0][3] << "\t" << m.m[1][3] << "\t" << m.m[2][3] << std::endl;

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

	std::cout << std::defaultfloat;
}

void JzlViveSession::dump_controller_property(const int idx)const
{
	// generic
	const std::string tracking_system_name = get_string_property(idx, vr::Prop_TrackingSystemName_String);
	std::cout << tracking_system_name << std::endl;
	const std::string model_number = get_string_property(idx, vr::Prop_ModelNumber_String);
	std::cout << model_number << std::endl;
	const std::string serial_number = get_string_property(idx, vr::Prop_SerialNumber_String);
	std::cout << serial_number << std::endl;
	const std::string render_model_name = get_string_property(idx, vr::Prop_RenderModelName_String);
	std::cout << render_model_name << std::endl;
	const std::string manufacturer_name = get_string_property(idx, vr::Prop_ManufacturerName_String);
	std::cout << manufacturer_name << std::endl;
	const std::string tracking_firmware_version = get_string_property(idx, vr::Prop_TrackingFirmwareVersion_String);
	std::cout << tracking_firmware_version << std::endl;
	const std::string hardware_revision = get_string_property(idx, vr::Prop_HardwareRevision_String);
	std::cout << hardware_revision << std::endl;
	const std::string wireless_dongle_desc = get_string_property(idx, vr::Prop_AllWirelessDongleDescriptions_String);
	std::cout << wireless_dongle_desc << std::endl;
	const std::string driver_version = get_string_property(idx, vr::Prop_DriverVersion_String);
	std::cout << driver_version << std::endl;
	const std::string firmware_update_url = get_string_property(idx, vr::Prop_Firmware_ManualUpdateURL_String);
	std::cout << firmware_update_url << std::endl;

	const std::string firmware_programming_target = get_string_property(idx, vr::Prop_Firmware_ProgrammingTarget_String);
	std::cout << firmware_programming_target << std::endl;
	const std::string resource_root = get_string_property(idx, vr::Prop_ResourceRoot_String);
	std::cout << resource_root << std::endl;

	// controller
	const std::string attached_dev_id = get_string_property(idx, vr::Prop_AttachedDeviceId_String);
	std::cout << attached_dev_id << std::endl;
	const uint64_t supported_btns = vr_system->GetUint64TrackedDeviceProperty(idx, vr::Prop_SupportedButtons_Uint64);
	std::cout << supported_btns << std::endl;
	const int32_t dev_role = vr_system->GetInt32TrackedDeviceProperty(idx, vr::Prop_ControllerRoleHint_Int32);
	std::cout << dev_role << std::endl;	
}

const std::string JzlViveSession::get_string_property(const int idx, const vr::ETrackedDeviceProperty prop)const
{
	if (!vr_system)
	{
		return "";
	}

	const uint32_t buff_len = vr_system->GetStringTrackedDeviceProperty(idx, prop, NULL, 0, NULL);
	if (buff_len == 0)
		return "";

	std::string res;
	res.resize(buff_len);

	vr_system->GetStringTrackedDeviceProperty(idx, prop, &res[0], buff_len, NULL);

	return res;
}
