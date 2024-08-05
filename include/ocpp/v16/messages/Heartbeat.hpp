// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 - 2023 Pionix GmbH and Contributors to EVerest
#ifndef OCPP_V16_HEARTBEAT_HPP
#define OCPP_V16_HEARTBEAT_HPP

#include <nlohmann/json_fwd.hpp>

#include <ocpp/v16/ocpp_types.hpp>

namespace ocpp {
namespace v16 {

/// \brief Contains a OCPP Heartbeat message
struct HeartbeatRequest : public ocpp::Message {

    /// \brief Provides the type of this Heartbeat message as a human readable string
    /// \returns the message type as a human readable string
    std::string get_type() const override;
};

/// \brief Conversion from a given HeartbeatRequest \p k to a given json object \p j
void to_json(json& j, const HeartbeatRequest& k);

/// \brief Conversion from a given json object \p j to a given HeartbeatRequest \p k
void from_json(const json& j, HeartbeatRequest& k);

/// \brief Writes the string representation of the given HeartbeatRequest \p k to the given output stream \p os
/// \returns an output stream with the HeartbeatRequest written to
std::ostream& operator<<(std::ostream& os, const HeartbeatRequest& k);

/// \brief Contains a OCPP HeartbeatResponse message
struct HeartbeatResponse : public ocpp::Message {
    ocpp::DateTime currentTime;

    /// \brief Provides the type of this HeartbeatResponse message as a human readable string
    /// \returns the message type as a human readable string
    std::string get_type() const override;
};

/// \brief Conversion from a given HeartbeatResponse \p k to a given json object \p j
void to_json(json& j, const HeartbeatResponse& k);

/// \brief Conversion from a given json object \p j to a given HeartbeatResponse \p k
void from_json(const json& j, HeartbeatResponse& k);

/// \brief Writes the string representation of the given HeartbeatResponse \p k to the given output stream \p os
/// \returns an output stream with the HeartbeatResponse written to
std::ostream& operator<<(std::ostream& os, const HeartbeatResponse& k);

} // namespace v16
} // namespace ocpp

#endif // OCPP_V16_HEARTBEAT_HPP
