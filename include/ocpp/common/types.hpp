// SPDX-License-Identifier: Apache-2.0
// Copyright Pionix GmbH and Contributors to EVerest

#ifndef OCPP_COMMON_TYPES_HPP
#define OCPP_COMMON_TYPES_HPP

#include <chrono>
#include <cstddef>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <date/date.h>
#include <date/tz.h>

#include <ocpp/common/cistring.hpp>
#include <ocpp/common/support_older_cpp_versions.hpp>
#include <ocpp/v16/ocpp_enums.hpp>
#include <ocpp/v2/ocpp_enums.hpp>

using json = nlohmann::json;

namespace ocpp {

/// \brief Parent structure for all OCPP messages supported by this implementation
struct Message {
    /// \brief Provides the type of the message
    /// \returns the message type as a string
    virtual std::string get_type() const = 0;
};

/// \brief Exception used when DateTime class is initialized by invalid timepoint string.
class TimePointParseException : public std::exception {
public:
    explicit TimePointParseException(const std::string& timepoint_str) :
        msg{"Timepoint string parsing failed. Could not convert: \"" + timepoint_str + "\" into DateTime."} {
    }

    const char* what() const noexcept override {
        return msg.c_str();
    }

private:
    std::string msg;
};

/// \brief Contains a DateTime implementation that can parse and create RFC 3339 compatible strings
class DateTimeImpl {
private:
    std::chrono::time_point<date::utc_clock> timepoint;

public:
    /// \brief Creates a new DateTimeImpl object with the current utc time
    DateTimeImpl();

    ~DateTimeImpl() = default;

    /// \brief Creates a new DateTimeImpl object from the given \p timepoint
    explicit DateTimeImpl(std::chrono::time_point<date::utc_clock> timepoint);

    /// \brief Creates a new DateTimeImpl object from the given \p timepoint_str
    explicit DateTimeImpl(const std::string& timepoint_str);

    /// \brief Converts this DateTimeImpl to a RFC 3339 compatible string
    /// \returns a RFC 3339 compatible string representation of the stored DateTime
    std::string to_rfc3339() const;

    /// \brief Sets the timepoint of this DateTimeImpl to the given \p timepoint_str
    void from_rfc3339(const std::string& timepoint_str);

    /// \brief Converts this DateTimeImpl to a std::chrono::time_point
    /// \returns a std::chrono::time_point
    std::chrono::time_point<date::utc_clock> to_time_point() const;

    /// \brief Assignment operator= to assign another DateTimeImpl \p dt to this DateTimeImpl
    DateTimeImpl& operator=(const DateTimeImpl& dt);

    /// \brief Conversion operatpr std::string returns a RFC 3339 compatible string representation of the stored
    /// DateTime
    operator std::string() {
        return this->to_rfc3339();
    }

    /// \brief Writes the given DateTimeImpl \p dt to the given output stream \p os as a RFC 3339 compatible string
    /// \returns an output stream with the DateTimeImpl as a RFC 3339 compatible string written to
    friend std::ostream& operator<<(std::ostream& os, const DateTimeImpl& dt);

    /// \brief Comparison operator> between two DateTimeImpl \p lhs and \p rhs
    friend bool operator>(const DateTimeImpl& lhs, const DateTimeImpl& rhs);

    /// \brief Comparison operator>= between two DateTimeImpl \p lhs and \p rhs
    friend bool operator>=(const DateTimeImpl& lhs, const DateTimeImpl& rhs);

    /// \brief Comparison operator< between two DateTimeImpl \p lhs and \p rhs
    friend bool operator<(const DateTimeImpl& lhs, const DateTimeImpl& rhs);

    /// \brief Comparison operator<= between two DateTimeImpl \p lhs and \p rhs
    friend bool operator<=(const DateTimeImpl& lhs, const DateTimeImpl& rhs);

    /// \brief Comparison operator== between two DateTimeImpl \p lhs and \p rhs
    friend bool operator==(const DateTimeImpl& lhs, const DateTimeImpl& rhs);
};

/// \brief Contains a DateTime implementation that can parse and create RFC 3339 compatible strings
class DateTime : public DateTimeImpl {
public:
    /// \brief Creates a new DateTime object with the current system time
    DateTime();

    ~DateTime() = default;

    /// \brief Creates a new DateTime object from the given \p timepoint
    explicit DateTime(std::chrono::time_point<date::utc_clock> timepoint);

    /// \brief Creates a new DateTime object from the given \p timepoint_str
    explicit DateTime(const std::string& timepoint_str);
};

/// \brief Base exception for when a conversion from string to enum or vice versa fails
class EnumConversionException : public std::out_of_range {
public:
    using std::out_of_range::out_of_range;
};

/// \brief Exception used when conversion from enum to string fails
class EnumToStringException : public EnumConversionException {
public:
    /// \brief Creates a new StringToEnumException
    /// \param enumValue value of enum that failed to convert
    /// \param type name of the enum trying to convert from
    template <typename T>
    explicit EnumToStringException(T enumValue, std::string_view type) :
        EnumConversionException{std::string{"No known conversion from value '"} +
                                std::to_string(static_cast<int>(enumValue)) + "' to " + type.data()} {
    }
};

/// \brief Exception used when conversion from string to enum fails
class StringToEnumException : public EnumConversionException {
public:
    /// \brief Creates a new StringToEnumException
    /// \param str input string that failed to convert
    /// \param type name of the enum trying to convert to
    StringToEnumException(std::string_view str, std::string_view type) :
        EnumConversionException{std::string{"Provided string '"} + str.data() + "' could not be converted to " +
                                type.data()} {
    }
};

/// \brief Contains the different connection states of the charge point
enum SessionStartedReason {
    EVConnected,
    Authorized
};
namespace conversions {
/// \brief Converts the given SessionStartedReason \p e to std::string
/// \returns a string representation of the SessionStartedReason
std::string session_started_reason_to_string(SessionStartedReason e);

/// \brief Converts the given std::string \p s to SessionStartedReason
/// \returns a SessionStartedReason from a string representation
SessionStartedReason string_to_session_started_reason(const std::string& s);
} // namespace conversions

/// \brief Writes the string representation of the given \p session_started_reason
/// to the given output stream \p os \returns an output stream with the SessionStartedReason written to
std::ostream& operator<<(std::ostream& os, const SessionStartedReason& session_started_reason);

struct Current {
    std::optional<float> DC; ///< DC current
    std::optional<float> L1; ///< AC L1 value only
    std::optional<float> L2; ///< AC L2 value only
    std::optional<float> L3; ///< AC L3 value only
    std::optional<float> N;  ///< AC Neutral value only

    /// \brief Conversion from a given Current \p k to a given json object \p j
    friend void to_json(json& j, const Current& k);

    /// \brief Conversion from a given json object \p j to a given Current \p k
    friend void from_json(const json& j, Current& k);

    // \brief Writes the string representation of the given Current \p k to the given output stream \p os
    /// \returns an output stream with the Current written to
    friend std::ostream& operator<<(std::ostream& os, const Current& k);
};
struct Voltage {
    std::optional<float> DC; ///< DC voltage
    std::optional<float> L1; ///< AC L1 value only
    std::optional<float> L2; ///< AC L2 value only
    std::optional<float> L3; ///< AC L3 value only

    /// \brief Conversion from a given Voltage \p k to a given json object \p j
    friend void to_json(json& j, const Voltage& k);

    /// \brief Conversion from a given json object \p j to a given Voltage \p k
    friend void from_json(const json& j, Voltage& k);

    // \brief Writes the string representation of the given Voltage \p k to the given output stream \p os
    /// \returns an output stream with the Voltage written to
    friend std::ostream& operator<<(std::ostream& os, const Voltage& k);
};
struct Frequency {
    float L1;                ///< AC L1 value
    std::optional<float> L2; ///< AC L2 value
    std::optional<float> L3; ///< AC L3 value

    /// \brief Conversion from a given Frequency \p k to a given json object \p j
    friend void to_json(json& j, const Frequency& k);

    /// \brief Conversion from a given json object \p j to a given Frequency \p k
    friend void from_json(const json& j, Frequency& k);

    // \brief Writes the string representation of the given Frequency \p k to the given output stream \p os
    /// \returns an output stream with the Frequency written to
    friend std::ostream& operator<<(std::ostream& os, const Frequency& k);
};
struct Power {
    float total;             ///< DC / AC Sum value
    std::optional<float> L1; ///< AC L1 value only
    std::optional<float> L2; ///< AC L2 value only
    std::optional<float> L3; ///< AC L3 value only

    /// \brief Conversion from a given Power \p k to a given json object \p j
    friend void to_json(json& j, const Power& k);

    /// \brief Conversion from a given json object \p j to a given Power \p k
    friend void from_json(const json& j, Power& k);

    // \brief Writes the string representation of the given Power \p k to the given output stream \p os
    /// \returns an output stream with the Power written to
    friend std::ostream& operator<<(std::ostream& os, const Power& k);
};
struct Energy {
    float total;             ///< DC / AC Sum value (which is relevant for billing)
    std::optional<float> L1; ///< AC L1 value only
    std::optional<float> L2; ///< AC L2 value only
    std::optional<float> L3; ///< AC L3 value only

    /// \brief Conversion from a given Energy \p k to a given json object \p j
    friend void to_json(json& j, const Energy& k);

    /// \brief Conversion from a given json object \p j to a given Energy \p k
    friend void from_json(const json& j, Energy& k);

    // \brief Writes the string representation of the given Energy \p k to the given output stream \p os
    /// \returns an output stream with the Energy written to
    friend std::ostream& operator<<(std::ostream& os, const Energy& k);
};
struct ReactivePower {
    float total;                 ///< VAR total
    std::optional<float> VARphA; ///< VAR phase A
    std::optional<float> VARphB; ///< VAR phase B
    std::optional<float> VARphC; ///< VAR phase C

    /// \brief Conversion from a given ReactivePower \p k to a given json object \p j
    friend void to_json(json& j, const ReactivePower& k);

    /// \brief Conversion from a given json object \p j to a given ReactivePower \p k
    friend void from_json(const json& j, ReactivePower& k);

    // \brief Writes the string representation of the given ReactivePower \p k to the given output stream \p os
    /// \returns an output stream with the ReactivePower written to
    friend std::ostream& operator<<(std::ostream& os, const ReactivePower& k);
};

struct Powermeter {
    std::string timestamp;                  ///< Timestamp of measurement
    Energy energy_Wh_import;                ///< Imported energy in Wh (from grid)
    std::optional<std::string> meter_id;    ///< A (user defined) meter if (e.g. id printed on the case)
    std::optional<bool> phase_seq_error;    ///< AC only: true for 3 phase rotation error (ccw)
    std::optional<Energy> energy_Wh_export; ///< Exported energy in Wh (to grid)
    std::optional<Power>
        power_W; ///< Instantaneous power in Watt. Negative values are exported, positive values imported Energy.
    std::optional<Voltage> voltage_V;      ///< Voltage in Volts
    std::optional<ReactivePower> VAR;      ///< Reactive power VAR
    std::optional<Current> current_A;      ///< Current in ampere
    std::optional<Frequency> frequency_Hz; ///< Grid frequency in Hertz

    /// \brief Conversion from a given Powermeter \p k to a given json object \p j
    friend void to_json(json& j, const Powermeter& k);

    /// \brief Conversion from a given json object \p j to a given Powermeter \p k
    friend void from_json(const json& j, Powermeter& k);

    // \brief Writes the string representation of the given Powermeter \p k to the given output stream \p os
    /// \returns an output stream with the Powermeter written to
    friend std::ostream& operator<<(std::ostream& os, const Powermeter& k);
};

struct StateOfCharge {
    float value;                         ///< State of Charge in percent
    std::optional<std::string> location; ///< Location of the State of Charge measurement

    /// \brief Conversion from a given StateOfCharge \p k to a given json object \p j
    friend void to_json(json& j, const StateOfCharge& k);

    /// \brief Conversion from a given json object \p j to a given StateOfCharge \p k
    friend void from_json(const json& j, StateOfCharge& k);

    // \brief Writes the string representation of the given StateOfCharge \p k to the given output stream \p os
    /// \returns an output stream with the StateOfCharge written to
    friend std::ostream& operator<<(std::ostream& os, const StateOfCharge& k);
};

struct Temperature {
    float value;                         ///< Temperature in degree Celsius
    std::optional<std::string> location; ///< Location of the Temperature measurement

    /// \brief Conversion from a given Temperature \p k to a given json object \p j
    friend void to_json(json& j, const Temperature& k);

    /// \brief Conversion from a given json object \p j to a given Temperature \p k
    friend void from_json(const json& j, Temperature& k);

    // \brief Writes the string representation of the given Temperature \p k to the given output stream \p os
    /// \returns an output stream with the Temperature written to
    friend std::ostream& operator<<(std::ostream& os, const Temperature& k);
};

struct RPM {
    float value;                         ///< RPM
    std::optional<std::string> location; ///< Location of the RPM measurement

    /// \brief Conversion from a given RPM \p k to a given json object \p j
    friend void to_json(json& j, const RPM& k);

    /// \brief Conversion from a given json object \p j to a given RPM \p k
    friend void from_json(const json& j, RPM& k);

    // \brief Writes the string representation of the given RPM \p k to the given output stream \p os
    /// \returns an output stream with the RPM written to
    friend std::ostream& operator<<(std::ostream& os, const RPM& k);
};

struct Measurement {
    Powermeter power_meter;                   ///< Powermeter data
    std::optional<StateOfCharge> soc_Percent; ///< State of Charge in percent
    std::vector<Temperature> temperature_C;   ///< Temperature in degree Celsius
    std::optional<RPM> rpm;                   ///< RPM

    /// \brief Conversion from a given Measurement \p k to a given json object \p j
    friend void to_json(json& j, const Measurement& k);

    /// \brief Conversion from a given json object \p j to a given Measurement \p k
    friend void from_json(const json& j, Measurement& k);

    // \brief Writes the string representation of the given Measurement \p k to the given output stream \p os
    /// \returns an output stream with the Measurement written to
    friend std::ostream& operator<<(std::ostream& os, const Measurement& k);
};

struct DisplayMessageContent {
    std::string message;
    std::optional<std::string> language;
    std::optional<v2::MessageFormatEnum> message_format;

    friend void from_json(const json& j, DisplayMessageContent& m);
    friend void to_json(json& j, const DisplayMessageContent& m);
};

///
/// \brief Type of an identifier string.
///
enum class IdentifierType {
    SessionId,    ///< \brief Identifier is the session id.
    IdToken,      ///< \brief Identifier is the id token.
    TransactionId ///< \brief Identifier is the transaction id.
};

struct DisplayMessage {
    std::optional<int32_t> id;
    std::optional<v2::MessagePriorityEnum> priority;
    std::optional<v2::MessageStateEnum> state;
    std::optional<DateTime> timestamp_from;
    std::optional<DateTime> timestamp_to;
    std::optional<std::string> identifier_id;
    std::optional<IdentifierType> identifier_type;
    DisplayMessageContent message;
    std::optional<std::string> qr_code;
};

struct RunningCostChargingPrice {
    std::optional<double> kWh_price;
    std::optional<double> hour_price;
    std::optional<double> flat_fee;

    friend void from_json(const json& j, RunningCostChargingPrice& c);
    friend void to_json(json& j, const RunningCostChargingPrice& c);
};

struct RunningCostIdlePrice {
    std::optional<uint32_t> idle_grace_minutes;
    std::optional<double> idle_hour_price;

    friend void from_json(const json& j, RunningCostIdlePrice& c);
    friend void to_json(json& j, const RunningCostIdlePrice& c);
};

enum class RunningCostState {
    Charging,
    Idle,
    Finished
};

namespace conversions {
RunningCostState string_to_running_cost_state(const std::string& state);
std::string running_cost_state_to_string(const RunningCostState& state);
} // namespace conversions

struct RunningCost {
    std::string transaction_id;
    std::optional<DateTime> timestamp;
    std::optional<uint32_t> meter_value;
    double cost;
    // Running cost state: "Charging" or "Idle". When this is the final price, state will be "Finished".
    RunningCostState state;
    std::optional<RunningCostChargingPrice> charging_price;
    std::optional<RunningCostIdlePrice> idle_price;
    std::optional<DateTime> next_period_at_time;
    std::optional<RunningCostChargingPrice> next_period_charging_price;
    std::optional<RunningCostIdlePrice> next_period_idle_price;
    std::optional<std::vector<DisplayMessageContent>> cost_messages;
    std::optional<std::string> qr_code_text;

    friend void from_json(const json& j, RunningCost& c);
};

struct TriggerMeterValue {
    std::optional<DateTime> at_time;
    std::optional<int> at_energy_kwh;
    std::optional<int> at_power_kw;
    std::vector<v16::ChargePointStatus> at_chargepoint_status;

    friend void from_json(const json& j, TriggerMeterValue& t);
};

enum class CaCertificateType {
    V2G,
    MO,
    CSMS,
    MF,
    OEM
};

namespace conversions {
/// \brief Converts the given CaCertificateType \p e to human readable string
/// \returns a string representation of the CaCertificateType
std::string ca_certificate_type_to_string(CaCertificateType e);

/// \brief Converts the given std::string \p s to CaCertificateType
/// \returns a CaCertificateType from a string representation
CaCertificateType string_to_ca_certificate_type(const std::string& s);
} // namespace conversions

/// \brief Writes the string representation of the given CaCertificateType
/// \param ca_certificate_type to the given output stream
/// \param os
/// \returns an output stream with the CaCertificateType written to
std::ostream& operator<<(std::ostream& os, const CaCertificateType& ca_certificate_type);

enum class CertificateValidationResult {
    Valid,
    Expired,
    InvalidSignature,
    IssuerNotFound,
    InvalidLeafSignature,
    InvalidChain,
    Unknown,
};

namespace conversions {
/// \brief Converts the given InstallCertificateResult \p e to human readable string
/// \returns a string representation of the InstallCertificateResult
std::string certificate_validation_result_to_string(CertificateValidationResult e);

/// \brief Converts the given std::string \p s to InstallCertificateResult
/// \returns a InstallCertificateResult from a string representation
CertificateValidationResult string_to_certificate_validation_result(const std::string& s);
} // namespace conversions

/// \brief Writes the string representation of the given InstallCertificateResult \p
/// install_certificate_result to the given output stream \p os \returns an output stream with the
/// InstallCertificateResult written to
std::ostream& operator<<(std::ostream& os, const CertificateValidationResult& certificate_validation_result);

enum class InstallCertificateResult {
    InvalidSignature,
    InvalidCertificateChain,
    InvalidFormat,
    InvalidCommonName,
    NoRootCertificateInstalled,
    Expired,
    CertificateStoreMaxLengthExceeded,
    WriteError,
    Accepted
};

namespace conversions {
/// \brief Converts the given InstallCertificateResult \p e to human readable string
/// \returns a string representation of the InstallCertificateResult
std::string install_certificate_result_to_string(InstallCertificateResult e);

/// \brief Converts the given std::string \p s to InstallCertificateResult
/// \returns a InstallCertificateResult from a string representation
InstallCertificateResult string_to_install_certificate_result(const std::string& s);
} // namespace conversions

/// \brief Writes the string representation of the given InstallCertificateResult \p
/// install_certificate_result to the given output stream \p os \returns an output stream with the
/// InstallCertificateResult written to
std::ostream& operator<<(std::ostream& os, const InstallCertificateResult& install_certificate_result);

enum class DeleteCertificateResult {
    Accepted,
    Failed,
    NotFound,
};

namespace conversions {
/// \brief Converts the given DeleteCertificateResult \p e to human readable string
/// \returns a string representation of the DeleteCertificateResult
std::string delete_certificate_result_to_string(DeleteCertificateResult e);

/// \brief Converts the given std::string \p s to DeleteCertificateResult
/// \returns a DeleteCertificateResult from a string representation
DeleteCertificateResult string_to_delete_certificate_result(const std::string& s);
} // namespace conversions

/// \brief Writes the string representation of the given DeleteCertificateResult \p
/// delete_certificate_result to the given output stream \p os \returns an output stream with the
/// DeleteCertificateResult written to
std::ostream& operator<<(std::ostream& os, const DeleteCertificateResult& delete_certificate_result);

// from: GetInstalledCertificateIdsResponse
enum class HashAlgorithmEnumType {
    SHA256,
    SHA384,
    SHA512,
};

namespace conversions {
/// \brief Converts the given HashAlgorithmEnumType \p e to human readable string
/// \returns a string representation of the HashAlgorithmEnumType
std::string hash_algorithm_enum_type_to_string(HashAlgorithmEnumType e);

/// \brief Converts the given std::string \p s to HashAlgorithmEnumType
/// \returns a HashAlgorithmEnumType from a string representation
HashAlgorithmEnumType string_to_hash_algorithm_enum_type(const std::string& s);
} // namespace conversions

/// \brief Writes the string representation of the given HashAlgorithmEnumType \p hash_algorithm_enum_type to the given
/// output stream \p os \returns an output stream with the HashAlgorithmEnumType written to
std::ostream& operator<<(std::ostream& os, const HashAlgorithmEnumType& hash_algorithm_enum_type);

struct CertificateHashDataType {
    HashAlgorithmEnumType hashAlgorithm;
    CiString<128> issuerNameHash;
    CiString<128> issuerKeyHash;
    CiString<40> serialNumber;
};
/// \brief Conversion from a given CertificateHashDataType \p k to a given json object \p j
void to_json(json& j, const CertificateHashDataType& k);

/// \brief Conversion from a given json object \p j to a given CertificateHashDataType \p k
void from_json(const json& j, CertificateHashDataType& k);

// \brief Writes the string representation of the given CertificateHashDataType \p k to the given output stream \p os
/// \returns an output stream with the CertificateHashDataType written to
std::ostream& operator<<(std::ostream& os, const CertificateHashDataType& k);

enum class CertificateType {
    V2GRootCertificate,
    MORootCertificate,
    CSMSRootCertificate,
    V2GCertificateChain,
    MFRootCertificate,
    OEMRootCertificate,
};

namespace conversions {
/// \brief Converts the given CertificateType \p e to human readable string
/// \returns a string representation of the CertificateType
std::string certificate_type_to_string(CertificateType e);

/// \brief Converts the given std::string \p s to CertificateType
/// \returns a CertificateType from a string representation
CertificateType string_to_certificate_type(const std::string& s);
} // namespace conversions

/// \brief Writes the string representation of the given CertificateType \p ceritficate_type to
/// the given output stream \p os \returns an output stream with the CertificateType written to
std::ostream& operator<<(std::ostream& os, const CertificateType& ceritficate_type);

struct CertificateHashDataChain {
    CertificateHashDataType certificateHashData;
    CertificateType certificateType;
    std::optional<std::vector<CertificateHashDataType>> childCertificateHashData;
};
/// \brief Conversion from a given CertificateHashDataChain \p k to a given json object \p j
void to_json(json& j, const CertificateHashDataChain& k);

/// \brief Conversion from a given json object \p j to a given CertificateHashDataChain \p k
void from_json(const json& j, CertificateHashDataChain& k);

// \brief Writes the string representation of the given CertificateHashDataChain \p k to the given output stream \p os
/// \returns an output stream with the CertificateHashDataChain written to
std::ostream& operator<<(std::ostream& os, const CertificateHashDataChain& k);

enum class OcppProtocolVersion {
    v16,
    v201,
    v21,
    Unknown,
};

namespace conversions {
/// \brief Converts the given OcppProtocolVersion \p e to human readable string
/// \returns a string representation of the OcppProtocolVersion
std::string ocpp_protocol_version_to_string(OcppProtocolVersion e);

/// \brief Converts the given std::string \p s to OcppProtocolVersion
/// \returns a OcppProtocolVersion from a string representation
OcppProtocolVersion string_to_ocpp_protocol_version(const std::string& s);
} // namespace conversions

/// \brief Writes the string representation of the given OcppProtocolVersion \p
/// ocpp_protocol_version to the given output stream \p os \returns an output stream with the
/// OcppProtocolVersion written to
std::ostream& operator<<(std::ostream& os, const OcppProtocolVersion& ocpp_protocol_version);

enum class CertificateSigningUseEnum {
    ChargingStationCertificate,
    V2GCertificate,
    ManufacturerCertificate,
    V2G20Certificate
};

namespace conversions {
/// \brief Converts the given CertificateSigningUseEnum \p e to human readable string
/// \returns a string representation of the CertificateSigningUseEnum
std::string certificate_signing_use_enum_to_string(CertificateSigningUseEnum e);

/// \brief Converts the given std::string \p s to CertificateSigningUseEnum
/// \returns a CertificateSigningUseEnum from a string representation
CertificateSigningUseEnum string_to_certificate_signing_use_enum(const std::string& s);
} // namespace conversions

/// \brief Writes the string representation of the given CertificateSigningUseEnum \p certificate_signing_use_enum to
/// the given output stream \p os \returns an output stream with the CertificateSigningUseEnum written to
std::ostream& operator<<(std::ostream& os, const CertificateSigningUseEnum& certificate_signing_use_enum);

/// \brief Struct for OCSPRequestData
struct OCSPRequestData {
    HashAlgorithmEnumType hashAlgorithm;
    std::string issuerNameHash;
    std::string issuerKeyHash;
    std::string serialNumber;
    std::string responderUrl;
};

enum class GetCertificateSignRequestStatus {
    Accepted,
    InvalidRequestedType, ///< Requested a CSR for non CSMS/V2G leafs
    KeyGenError,          ///< The key could not be generated with the requested/default parameters
    GenerationError,      ///< Any other error when creating the CSR
};

enum class GetCertificateInfoStatus {
    Accepted,
    Rejected,
    NotFound,
    NotFoundValid,
    PrivateKeyNotFound,
};

struct GetCertificateSignRequestResult {
    GetCertificateSignRequestStatus status;
    std::optional<std::string> csr;
};

struct CertificateOCSP {
    CertificateHashDataType hash;
    std::optional<fs::path> ocsp_path;
};

struct CertificateInfo {
    std::optional<fs::path> certificate_path;        // path to the full certificate chain
    std::optional<fs::path> certificate_single_path; // path to the single leaf certificate
    int certificate_count;                           // count of certs in the chain
    fs::path key_path;                               // path to private key of the leaf certificate
    std::optional<std::string> password;             // optional password for the private key
    std::vector<CertificateOCSP> ocsp;               // OCSP data if requested
};

struct GetCertificateInfoResult {
    GetCertificateInfoStatus status;
    std::optional<CertificateInfo> info;
};

enum class LeafCertificateType {
    CSMS, // Charging Station Management System
    V2G,  // Vehicle to grid
    MF,   // Manufacturer
    MO    // Mobility Operator
};

namespace conversions {
/// \brief Converts the given bool \p b to a string representation of "true" or "false"
/// \returns a string representation of the given bool
std::string bool_to_string(bool b);

/// \brief Converts the given string \p s to a bool value. "true" is converted into true, anything else to false
/// \returns a bool from the given string representation
bool string_to_bool(const std::string& s);

/// \brief Converts the given double \p d to a string representation with the given \p precision
/// \returns a string representation of the given double using the given precision
std::string double_to_string(double d, int precision);

/// \brief Converts the given double \p d to a string representation with a fixed precision of 2
/// \returns a string representation of the given double using a fixed precision of 2
std::string double_to_string(double d);
} // namespace conversions

enum class FirmwareStatusNotification {
    Downloaded,
    DownloadFailed,
    Downloading,
    DownloadScheduled,
    DownloadPaused,
    Idle,
    InstallationFailed,
    Installing,
    Installed,
    InstallRebooting,
    InstallScheduled,
    InstallVerificationFailed,
    InvalidSignature,
    SignatureVerified
};

namespace conversions {
/// \brief Converts GetCertificateSignRequestStatus to string
std::string generate_certificate_signing_request_status_to_string(const GetCertificateSignRequestStatus status);
} // namespace conversions

namespace conversions {

/// \brief Converts ocpp::FirmwareStatusNotification to v16::FirmwareStatus
v16::FirmwareStatus firmware_status_notification_to_firmware_status(const FirmwareStatusNotification status);

/// \brief Converts ocpp::FirmwareStatusNotification to v16::FirmwareStatusEnumType
v16::FirmwareStatusEnumType
firmware_status_notification_to_firmware_status_enum_type(const FirmwareStatusNotification status);

} // namespace conversions

namespace security {
// The security profiles defined in OCPP 2.0.1 resp. in the OCPP 1.6 security-whitepaper.
enum SecurityProfile { // no "enum class" because values are used in implicit `switch`-comparisons to `int
                       // security_profile`
    OCPP_1_6_ONLY_UNSECURED_TRANSPORT_WITHOUT_BASIC_AUTHENTICATION = 0,
    UNSECURED_TRANSPORT_WITH_BASIC_AUTHENTICATION = 1,
    TLS_WITH_BASIC_AUTHENTICATION = 2,
    TLS_WITH_CLIENT_SIDE_CERTIFICATES = 3,
};
} // namespace security

namespace security_events {

// This is the list of security events defined in OCPP 2.0.1 (and the 1.6 security whitepper).
// Security events that are marked critical should be pushed to the CSMS.
// This is a non-exhaustive list of security events, when a security event matches the description in the OCPP
// specification of one of the Security Events in this list, for interoperability reasons, the Security Event from this
// list shall be used, instead of adding a new (proprietary) Security Event.

inline const std::string FIRMWARE_UPDATED = "FirmwareUpdated"; // CRITICAL
inline const std::string FAILEDTOAUTHENTICATEATCSMS = "FailedToAuthenticateAtCsms";
inline const std::string CSMSFAILEDTOAUTHENTICATE = "CsmsFailedToAuthenticate";
inline const std::string CSRGENERATIONFAILED = "CSRGenerationFailed";
inline const std::string SETTINGSYSTEMTIME = "SettingSystemTime";         // CRITICAL
inline const std::string RESET_OR_REBOOT = "ResetOrReboot";               // CRITICAL
inline const std::string STARTUP_OF_THE_DEVICE = "StartupOfTheDevice";    // CRITICAL
inline const std::string SECURITYLOGWASCLEARED = "SecurityLogWasCleared"; // CRITICAL
inline const std::string RECONFIGURATIONOFSECURITYPARAMETERS = "ReconfigurationOfSecurityParameters";
inline const std::string MEMORYEXHAUSTION = "MemoryExhaustion"; // CRITICAL
inline const std::string INVALIDMESSAGES = "InvalidMessages";
inline const std::string ATTEMPTEDREPLAYATTACKS = "AttemptedReplayAttacks";
inline const std::string TAMPERDETECTIONACTIVATED = "TamperDetectionActivated"; // CRITICAL
inline const std::string INVALIDFIRMWARESIGNATURE = "InvalidFirmwareSignature";
inline const std::string INVALIDFIRMWARESIGNINGCERTIFICATE = "InvalidFirmwareSigningCertificate";
inline const std::string INVALIDCSMSCERTIFICATE = "InvalidCsmsCertificate";
inline const std::string INVALIDCENTRALSYSTEMCERTIFICATE = "InvalidCentralSystemCertificate";
inline const std::string INVALIDCHARGINGSTATIONCERTIFICATE = "InvalidChargingStationCertificate";
inline const std::string INVALIDCHARGEPOINTCERTIFICATE = "InvalidChargePointCertificate"; // for OCPP1.6
inline const std::string INVALIDTLSVERSION = "InvalidTLSVersion";
inline const std::string INVALIDTLSCIPHERSUITE = "InvalidTLSCipherSuite";
inline const std::string MAINTENANCELOGINACCEPTED = "MaintenanceLoginAccepted";
inline const std::string MAINTENANCELOGINFAILED = "MaintenanceLoginFailed";
} // namespace security_events

enum class MessageDirection {
    CSMSToChargingStation,
    ChargingStationToCSMS
};

enum class ConnectionFailedReason {
    InvalidCSMSCertificate = 0,
    FailedToAuthenticateAtCsms
};

///
/// \brief Reason why a websocket closes its connection
///
enum class WebsocketCloseReason : uint8_t {
    /// Normal closure
    Normal = 1,
    ForceTcpDrop,
    GoingAway,
    AbnormalClose,
    ServiceRestart
};

/// \brief This can be used to distinguish the different queue types
enum class QueueType {
    Normal,
    Transaction,
    None,
};

/// \brief Struct containing default limits for amps, watts and number of phases
struct CompositeScheduleDefaultLimits {
    int32_t amps;
    int32_t watts;
    int32_t number_phases;
};

/// \brief Status of a reservation check.
enum class ReservationCheckStatus {
    NotReserved,           ///< @brief No reservation of this evse and / or id token
    ReservedForToken,      ///< @brief Reservation for this token.
    ReservedForOtherToken, ///< @brief Reserved for other token and reservation has no parent token or parent token does
                           ///< not match.
    ReservedForOtherTokenAndHasParentToken, ///< @brief Reserved for other token but reservation has a parent token.
};

} // namespace ocpp

#endif
