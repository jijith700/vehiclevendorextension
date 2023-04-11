// Based on hardware/interfaces/automotive/vehicle/2.0/default/impl/vhal_v2_0/VehicleHalClient.cpp

#define LOG_TAG __FILE_NAME__
#include <log/log_main.h>

#include "VehicleHalClient.h"

#include <android-base/logging.h>

namespace vendor {
namespace demo {
namespace vehiclevendorextension {
namespace V1_0 {

namespace impl {

void VehicleHalClient::onPropertyValue(const vhal_v2_0::VehiclePropValue& value, bool updateStatus)
{
    if (!mPropCallback)
    {
        ALOGE("VHAL %s(): PropertyCallBackType is not registered!", __func__);
        return;
    }
    return mPropCallback(value, updateStatus);
}

void VehicleHalClient::registerPropertyValueCallback(PropertyCallBackType&& callback)
{
    if (mPropCallback)
    {
        ALOGW("VHAL %s(): Cannot register multiple callbacks!", __func__);
        return;
    }
    mPropCallback = std::move(callback);
}

} // namespace impl

} // namespace V1_0
} // namespace vehiclevendorextension
} // namespace demo
} // namespace vendor
