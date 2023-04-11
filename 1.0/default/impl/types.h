#ifndef HIDL_GENERATED_VENDOR_DEMO_VEHICLEVENDOREXTENSION_V1_0_TYPES_H
#define HIDL_GENERATED_VENDOR_DEMO_VEHICLEVENDOREXTENSION_V1_0_TYPES_H

#include <android/hardware/automotive/vehicle/2.0/types.h>

#include <string>
#include <vector>
namespace android {
namespace hardware {
template <typename T> using hidl_vec = std::vector<T>;
template <typename Enum>
using hidl_bitfield = typename std::underlying_type<Enum>::type;
using hidl_string = std::string;
} // namespace hardware
} // namespace android

namespace vendor {
namespace demo {
namespace vehiclevendorextension {
namespace V1_0 {

// Forward declaration for forward reference support:
enum class VehicleProperty : int32_t;

enum class VehicleProperty : int32_t {
    /**
     * Undefined property.
     */
    INVALID = 0 /* 0x00000000 */,

    VENDOR_TEST_1S_COUNTER = 557842433 ,

    VENDOR_TEST_500MS_COUNTER = 557842434 ,

    VENDOR_TEST_SYS_PROP = 557842435 ,
    
};
} // namespace V1_0
} // namespace vehiclevendorextension
} // namespace demo
} // namespace vendor
#endif // HIDL_GENERATED_VENDOR_APTIV_HARDWARE_VEHICLEVENDOREXTENSION_V1_0_TYPES_H
