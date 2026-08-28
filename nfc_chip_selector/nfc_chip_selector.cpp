/*
 * Copyright (C) 2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>

#include <string>

namespace {

constexpr char kChipNamePath[] = "/sys/nfc/chip_name";
constexpr char kChipProperty[] = "vendor.nfc.chip";

}  // namespace

int main() {
    std::string chip;
    if (!android::base::ReadFileToString(kChipNamePath, &chip)) {
        LOG(ERROR) << "Could not read " << kChipNamePath;
        android::base::SetProperty(kChipProperty, "unknown");
        return 1;
    }

    chip = android::base::Trim(chip);
    if (chip != "thn31" && chip != "pn557") {
        LOG(ERROR) << "Unsupported NFC chip: " << chip;
        android::base::SetProperty(kChipProperty, "unknown");
        return 1;
    }

    if (!android::base::SetProperty(kChipProperty, chip)) {
        LOG(ERROR) << "Could not set " << kChipProperty;
        return 1;
    }

    return 0;
}
