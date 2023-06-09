#define LOG_TAG __FILE_NAME__
#include <log/log_main.h>

#include <android-base/properties.h>
#include <utils/SystemClock.h>

#include "Simulator.h"
#include "vendor/demo/vehiclevendorextension/1.0/types.h"

namespace {
const char* kVendorTestSysPropKey = "debug.vendor.demo.VENDOR_TEST_SYS_PROP";
}

namespace vendor {
namespace demo {
namespace vehiclevendorextension {
namespace V1_0 {

namespace impl {

Simulator::Simulator()
    : Runnable(std::bind(&Simulator::routine, this, _1))
{
    mVendorTest1sCounter.first = {
        .prop = static_cast<int32_t>(VehicleProperty::VENDOR_TEST_1S_COUNTER),
        .timestamp = android::elapsedRealtimeNano(),
        .areaId = 0,
        .status = vhal_v2_0::VehiclePropertyStatus::AVAILABLE,
        .value.int32Values{0}};

    mVendorTest500msCounter.first = {
        .prop = static_cast<int32_t>(VehicleProperty::VENDOR_TEST_500MS_COUNTER),
        .timestamp = android::elapsedRealtimeNano(),
        .areaId = 0,
        .status = vhal_v2_0::VehiclePropertyStatus::AVAILABLE,
        .value.int32Values{0}};

    mVendorTestSysProp.first = {
        .prop = static_cast<int32_t>(VehicleProperty::VENDOR_TEST_SYS_PROP),
        .timestamp = android::elapsedRealtimeNano(),
        .areaId = 0,
        .status = vhal_v2_0::VehiclePropertyStatus::AVAILABLE,
        .value.int32Values{-1}};
}

std::vector<vhal_v2_0::VehiclePropConfig> Simulator::getAllPropertyConfig() const
{
    return std::vector<vhal_v2_0::VehiclePropConfig>();
}

vhal_v2_0::StatusCode Simulator::setProperty(const vhal_v2_0::VehiclePropValue& value, bool updateStatus)
{
    ALOGD("VHAL %s(): prop: %d", __func__, value.prop);

    vhal_v2_0::StatusCode ret = vhal_v2_0::StatusCode::OK;

    ret = setIfSupportedProperty(value, updateStatus);

    if (ret == vhal_v2_0::StatusCode::OK)
    {
        onPropertyValue(value, updateStatus);
    }

    return ret;
}

void Simulator::routine(const std::atomic_bool& request_to_stop)
{
    vhal_v2_0::VehiclePropValue value;

    for (;;)
    {
        if (request_to_stop) break;

        simulateVendorTest1sCounter();
        simulateVendorTest500msCounter();
        simulateVendorTestSysProp();

        std::this_thread::sleep_for(100ms);
    }
}

vhal_v2_0::StatusCode Simulator::setIfSupportedProperty(const vhal_v2_0::VehiclePropValue& value, bool updateStatus)
{
    vhal_v2_0::StatusCode ret = vhal_v2_0::StatusCode::OK;

    switch (value.prop)
    {
    case static_cast<int32_t>(VehicleProperty::VENDOR_TEST_1S_COUNTER):
        ret = setVendorTest1sCounter(value, updateStatus);
        break;
    case static_cast<int32_t>(VehicleProperty::VENDOR_TEST_500MS_COUNTER):
        ret = setVendorTest500msCounter(value, updateStatus);
        break;
    case static_cast<int32_t>(VehicleProperty::VENDOR_TEST_SYS_PROP):
        ret = setVendorTestSysProp(value, updateStatus);
        break;
    default:
        break;
    }

    return ret;
}

vhal_v2_0::StatusCode Simulator::setVendorTest1sCounter(const vhal_v2_0::VehiclePropValue& value, bool updateStatus)
{
    static_cast<void>(updateStatus);

    {
        const std::lock_guard<std::mutex> lock(mVendorTest1sCounter.second);
        mVendorTest1sCounter.first = value;
    }

    return vhal_v2_0::StatusCode::OK;
}

vhal_v2_0::StatusCode Simulator::setVendorTest500msCounter(
    const vhal_v2_0::VehiclePropValue& value,
    bool updateStatus)
{
    static_cast<void>(updateStatus);

    {
        const std::lock_guard<std::mutex> lock(mVendorTest500msCounter.second);
        mVendorTest500msCounter.first = value;
    }

    return vhal_v2_0::StatusCode::OK;
}

vhal_v2_0::StatusCode Simulator::setVendorTestSysProp(const vhal_v2_0::VehiclePropValue& value, bool updateStatus)
{
    static_cast<void>(updateStatus);
    static_cast<void>(value);

    ALOGE("VHAL %s(): prop: %d, try to set READ property!", __func__, value.prop);

    return vhal_v2_0::StatusCode::ACCESS_DENIED;
}

void Simulator::simulateVendorTest1sCounter()
{
    static int32_t counter_100ms = 0;
    constexpr int32_t COUNTER_FOR_1S = 10;

    if (counter_100ms == COUNTER_FOR_1S)
    {
        vhal_v2_0::VehiclePropValue value;
        {
            const std::lock_guard<std::mutex> lock(mVendorTest1sCounter.second);
            ++mVendorTest1sCounter.first.value.int32Values[0];
            mVendorTest1sCounter.first.timestamp = android::elapsedRealtimeNano();
            mVendorTest1sCounter.first.status = vhal_v2_0::VehiclePropertyStatus::AVAILABLE;

            value = mVendorTest1sCounter.first;
        }
        onPropertyValue(value, true);
        counter_100ms = 0;
    }
    else
        ++counter_100ms;
}

void Simulator::simulateVendorTest500msCounter()
{
    static int32_t counter_100ms = 0;
    constexpr int32_t COUNTER_FOR_500MS = 5;

    if (counter_100ms == COUNTER_FOR_500MS)
    {
        vhal_v2_0::VehiclePropValue value;
        {
            const std::lock_guard<std::mutex> lock(mVendorTest500msCounter.second);
            ++mVendorTest500msCounter.first.value.int32Values[0];
            mVendorTest500msCounter.first.timestamp = android::elapsedRealtimeNano();
            mVendorTest500msCounter.first.status = vhal_v2_0::VehiclePropertyStatus::AVAILABLE;

            value = mVendorTest500msCounter.first;
        }
        onPropertyValue(value, true);
        counter_100ms = 0;
    }
    else
        ++counter_100ms;
}

void Simulator::simulateVendorTestSysProp()
{
    int32_t cur_value;

    {
        const std::lock_guard<std::mutex> lock(mVendorTestSysProp.second);
        cur_value = mVendorTestSysProp.first.value.int32Values[0];
    }

    int32_t new_value = android::base::GetIntProperty(
        kVendorTestSysPropKey,
        cur_value,
        std::numeric_limits<int32_t>::min(),
        std::numeric_limits<int32_t>::max());

    if (new_value != cur_value)
    {
        vhal_v2_0::VehiclePropValue value;
        {
            const std::lock_guard<std::mutex> lock(mVendorTestSysProp.second);
            mVendorTestSysProp.first.value.int32Values[0] = new_value;
            mVendorTestSysProp.first.timestamp = android::elapsedRealtimeNano();
            mVendorTestSysProp.first.status = vhal_v2_0::VehiclePropertyStatus::AVAILABLE;

            value = mVendorTestSysProp.first;
        }
        onPropertyValue(value, true);
    }
}

} // namespace impl

} // namespace V1_0
} // namespace vehiclevendorextension
} // namespace demo
} // namespace vendor
