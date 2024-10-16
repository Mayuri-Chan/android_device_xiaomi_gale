/*
 * Copyright (C) 2021 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <aidl/android/hardware/power/BnPower.h>
#include <android-base/file.h>
#include <android-base/logging.h>
#include <sys/ioctl.h>
// defines from drivers/input/touchscreen/xiaomi/xiaomi_touch.h
#define SET_CUR_VALUE 0
#define TOUCH_DOUBLETAP_MODE 14
#define TOUCH_DEV_PATH "/dev/xiaomi-touch"
#define TOUCH_ID 0
#define TOUCH_MAGIC 0x5400
#define TOUCH_IOC_SETMODE TOUCH_MAGIC + SET_CUR_VALUE
namespace aidl {
namespace android {
namespace hardware {
namespace power {
namespace impl {
namespace mediatek {
using ::aidl::android::hardware::power::Mode;
bool isDeviceSpecificModeSupported(Mode type, bool* _aidl_return) {
    switch (type) {
        case Mode::DOUBLE_TAP_TO_WAKE:
            *_aidl_return = true;
            return true;
        default:
            return false;
    }
}
bool setDeviceSpecificMode(Mode type, bool enabled) {
    switch (type) {
        case Mode::DOUBLE_TAP_TO_WAKE: {
            int fd = open(TOUCH_DEV_PATH, O_RDWR);
            // Xiaomi has two competing ABIs, no idea how to detect it
            int arg[3] = {TOUCH_ID, TOUCH_DOUBLETAP_MODE, enabled ? 1 : 0};
            int arg2[3] = {TOUCH_DOUBLETAP_MODE, enabled ? 1 : 0};
            ioctl(fd, TOUCH_IOC_SETMODE, &arg);
            ioctl(fd, TOUCH_IOC_SETMODE, &arg2);
            close(fd);
            return true;
        }
        default:
            return false;
    }
}
}  // namespace mediatek
}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace android
}  // namespace aidl
