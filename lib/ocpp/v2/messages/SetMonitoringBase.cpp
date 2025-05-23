// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 - 2025 Pionix GmbH and Contributors to EVerest
// This code is generated using the generator in 'src/code_generator/common`, please do not edit manually

#include <ocpp/v2/messages/SetMonitoringBase.hpp>

#include <optional>
#include <ostream>
#include <string>

using json = nlohmann::json;

namespace ocpp {
namespace v2 {

std::string SetMonitoringBaseRequest::get_type() const {
    return "SetMonitoringBase";
}

void to_json(json& j, const SetMonitoringBaseRequest& k) {
    // the required parts of the message
    j = json{
        {"monitoringBase", conversions::monitoring_base_enum_to_string(k.monitoringBase)},
    };
    // the optional parts of the message
    if (k.customData) {
        j["customData"] = k.customData.value();
    }
}

void from_json(const json& j, SetMonitoringBaseRequest& k) {
    // the required parts of the message
    k.monitoringBase = conversions::string_to_monitoring_base_enum(j.at("monitoringBase"));

    // the optional parts of the message
    if (j.contains("customData")) {
        k.customData.emplace(j.at("customData"));
    }
}

/// \brief Writes the string representation of the given SetMonitoringBaseRequest \p k to the given output stream \p os
/// \returns an output stream with the SetMonitoringBaseRequest written to
std::ostream& operator<<(std::ostream& os, const SetMonitoringBaseRequest& k) {
    os << json(k).dump(4);
    return os;
}

std::string SetMonitoringBaseResponse::get_type() const {
    return "SetMonitoringBaseResponse";
}

void to_json(json& j, const SetMonitoringBaseResponse& k) {
    // the required parts of the message
    j = json{
        {"status", conversions::generic_device_model_status_enum_to_string(k.status)},
    };
    // the optional parts of the message
    if (k.statusInfo) {
        j["statusInfo"] = k.statusInfo.value();
    }
    if (k.customData) {
        j["customData"] = k.customData.value();
    }
}

void from_json(const json& j, SetMonitoringBaseResponse& k) {
    // the required parts of the message
    k.status = conversions::string_to_generic_device_model_status_enum(j.at("status"));

    // the optional parts of the message
    if (j.contains("statusInfo")) {
        k.statusInfo.emplace(j.at("statusInfo"));
    }
    if (j.contains("customData")) {
        k.customData.emplace(j.at("customData"));
    }
}

/// \brief Writes the string representation of the given SetMonitoringBaseResponse \p k to the given output stream \p os
/// \returns an output stream with the SetMonitoringBaseResponse written to
std::ostream& operator<<(std::ostream& os, const SetMonitoringBaseResponse& k) {
    os << json(k).dump(4);
    return os;
}

} // namespace v2
} // namespace ocpp
