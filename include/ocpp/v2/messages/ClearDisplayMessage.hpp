// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 - 2025 Pionix GmbH and Contributors to EVerest
// This code is generated using the generator in 'src/code_generator/common`, please do not edit manually

#ifndef OCPP_V2_CLEARDISPLAYMESSAGE_HPP
#define OCPP_V2_CLEARDISPLAYMESSAGE_HPP

#include <nlohmann/json_fwd.hpp>
#include <optional>

#include <ocpp/common/types.hpp>
#include <ocpp/v2/ocpp_enums.hpp>
#include <ocpp/v2/ocpp_types.hpp>

namespace ocpp {
namespace v2 {

/// \brief Contains a OCPP ClearDisplayMessage message
struct ClearDisplayMessageRequest : public ocpp::Message {
    int32_t id;
    std::optional<CustomData> customData;

    /// \brief Provides the type of this ClearDisplayMessage message as a human readable string
    /// \returns the message type as a human readable string
    std::string get_type() const override;
};

/// \brief Conversion from a given ClearDisplayMessageRequest \p k to a given json object \p j
void to_json(json& j, const ClearDisplayMessageRequest& k);

/// \brief Conversion from a given json object \p j to a given ClearDisplayMessageRequest \p k
void from_json(const json& j, ClearDisplayMessageRequest& k);

/// \brief Writes the string representation of the given ClearDisplayMessageRequest \p k to the given output stream \p
/// os \returns an output stream with the ClearDisplayMessageRequest written to
std::ostream& operator<<(std::ostream& os, const ClearDisplayMessageRequest& k);

/// \brief Contains a OCPP ClearDisplayMessageResponse message
struct ClearDisplayMessageResponse : public ocpp::Message {
    ClearMessageStatusEnum status;
    std::optional<StatusInfo> statusInfo;
    std::optional<CustomData> customData;

    /// \brief Provides the type of this ClearDisplayMessageResponse message as a human readable string
    /// \returns the message type as a human readable string
    std::string get_type() const override;
};

/// \brief Conversion from a given ClearDisplayMessageResponse \p k to a given json object \p j
void to_json(json& j, const ClearDisplayMessageResponse& k);

/// \brief Conversion from a given json object \p j to a given ClearDisplayMessageResponse \p k
void from_json(const json& j, ClearDisplayMessageResponse& k);

/// \brief Writes the string representation of the given ClearDisplayMessageResponse \p k to the given output stream \p
/// os \returns an output stream with the ClearDisplayMessageResponse written to
std::ostream& operator<<(std::ostream& os, const ClearDisplayMessageResponse& k);

} // namespace v2
} // namespace ocpp

#endif // OCPP_V2_CLEARDISPLAYMESSAGE_HPP
