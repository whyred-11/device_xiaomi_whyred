/*
   Copyright (c) 2015, The Linux Foundation. All rights reserved.
   Copyright (C) 2016 The CyanogenMod Project.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <android-base/properties.h>
#include <unistd.h>

#include <fstream>
#include <vector>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "vendor_init.h"

using android::base::GetProperty;
using android::base::SetProperty;

std::vector<std::string> ro_props_default_source_order = {
        "", "odm.", "product.", "system.", "vendor.",
};

void property_override(char const prop[], char const value[], bool add = true) {
    prop_info* pi;

    pi = (prop_info*)__system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else if (add)
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void property_override_multifp(char const buildfp[], char const systemfp[],
      char const bootimagefp[], char const vendorfp[], char const value[])
{

    property_override(buildfp, value);
    property_override(systemfp, value);
    property_override(vendorfp, value);
    property_override(bootimagefp, value);
}

static void init_setup_model_properties() {
    const auto set_ro_product_prop = [](const std::string& source, const std::string& prop,
                                        const std::string& value) {
        auto prop_name = "ro.product." + source + prop;
        property_override(prop_name.c_str(), value.c_str(), false);
    };

    std::ifstream fin;
    std::string buf;

    fin.open("/proc/cmdline");
    while (std::getline(fin, buf, ' '))
        if (buf.find("androidboot.hwc") != std::string::npos) break;
    fin.close();

    if (buf.find("CN") != std::string::npos || buf.find("Global") != std::string::npos) {
        for (const auto& source : ro_props_default_source_order) {
            set_ro_product_prop(source, "model", "Redmi Note 5");
        }
    } else {
        for (const auto& source : ro_props_default_source_order) {
            set_ro_product_prop(source, "model", "Redmi Note 5 Pro");
        }
    }
}

void vendor_load_properties() {
    init_setup_model_properties();
}

	// fingerprint
	property_override("ro.build.description", "redfin-user 11 RD1A.201105.003.A1 6886512 release-keys");
	property_override_multifp("ro.build.fingerprint", "ro.system.build.fingerprint", "ro.vendor.build.fingerprint", "ro.bootimage.build.fingerprint", "google/redfin/redfin:11/RD1A.201105.003.A1/6886512:user/release-keys");

	// Magisk Hide
	property_override("ro.boot.verifiedbootstate", "green");
	property_override("ro.boot.vbmeta.device_state", "locked");
	property_override("ro.boot.veritymode", "permissive");
	property_override("ro.build.type", "user");
	property_override("ro.build.tags", "release-keys");
