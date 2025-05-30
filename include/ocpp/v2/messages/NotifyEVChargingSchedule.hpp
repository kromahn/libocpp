// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 - 2025 Pionix GmbH and Contributors to EVerest
// This code is generated using the generator in 'src/code_generator/common`, please do not edit manually

#ifndef OCPP_V2_NOTIFYEVCHARGINGSCHEDULE_HPP
#define OCPP_V2_NOTIFYEVCHARGINGSCHEDULE_HPP

#include <nlohmann/json_fwd.hpp>
#include <optional>

#include <ocpp/common/types.hpp>
#include <ocpp/v2/ocpp_enums.hpp>
#include <ocpp/v2/ocpp_types.hpp>

namespace ocpp {
namespace v2 {

/// \brief Contains a OCPP NotifyEVChargingSchedule message
struct NotifyEVChargingScheduleRequest : public ocpp::Message {
    ocpp::DateTime timeBase;
    ChargingSchedule chargingSchedule;
    int32_t evseId;
    std::optional<int32_t> selectedChargingScheduleId;
    std::optional<bool> powerToleranceAcceptance;
    std::optional<CustomData> customData;

    /// \brief Provides the type of this NotifyEVChargingSchedule message as a human readable string
    /// \returns the message type as a human readable string
    std::string get_type() const override;
};

/// \brief Conversion from a given NotifyEVChargingScheduleRequest \p k to a given json object \p j
void to_json(json& j, const NotifyEVChargingScheduleRequest& k);

/// \brief Conversion from a given json object \p j to a given NotifyEVChargingScheduleRequest \p k
void from_json(const json& j, NotifyEVChargingScheduleRequest& k);

/// \brief Writes the string representation of the given NotifyEVChargingScheduleRequest \p k to the given output stream
/// \p os \returns an output stream with the NotifyEVChargingScheduleRequest written to
std::ostream& operator<<(std::ostream& os, const NotifyEVChargingScheduleRequest& k);

/// \brief Contains a OCPP NotifyEVChargingScheduleResponse message
struct NotifyEVChargingScheduleResponse : public ocpp::Message {
    GenericStatusEnum status;
    std::optional<StatusInfo> statusInfo;
    std::optional<CustomData> customData;

    /// \brief Provides the type of this NotifyEVChargingScheduleResponse message as a human readable string
    /// \returns the message type as a human readable string
    std::string get_type() const override;
};

/// \brief Conversion from a given NotifyEVChargingScheduleResponse \p k to a given json object \p j
void to_json(json& j, const NotifyEVChargingScheduleResponse& k);

/// \brief Conversion from a given json object \p j to a given NotifyEVChargingScheduleResponse \p k
void from_json(const json& j, NotifyEVChargingScheduleResponse& k);

/// \brief Writes the string representation of the given NotifyEVChargingScheduleResponse \p k to the given output
/// stream \p os \returns an output stream with the NotifyEVChargingScheduleResponse written to
std::ostream& operator<<(std::ostream& os, const NotifyEVChargingScheduleResponse& k);

} // namespace v2
} // namespace ocpp

#endif // OCPP_V2_NOTIFYEVCHARGINGSCHEDULE_HPP
