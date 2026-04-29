// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 - 2024 Pionix GmbH and Contributors to EVerest
#ifndef V16_UTILS_HPP
#define V16_UTILS_HPP

#include <ocpp/common/call_types.hpp>
#include <ocpp/v16/messages/StopTransaction.hpp>
#include <ocpp/v16/ocpp_types.hpp>
#include <ocpp/v16/types.hpp>

namespace ocpp {
namespace v16 {
namespace utils {

size_t get_message_size(const ocpp::Call<StopTransactionRequest>& call);

/// \brief Drops every second entry from transactionData as long as the message size of the \p call is greater than the
/// \p max_message_size
void drop_transaction_data(size_t max_message_size, ocpp::Call<StopTransactionRequest>& call);

/// \brief Determines if a given \p security_event is critical as defined in the OCPP 1.6 security whitepaper
bool is_critical(const std::string& security_event);

/// \brief Converts signed active power into OCPP 1.6 Power.Active.Import (always >= 0)
double get_power_active_import_w(double signed_power_w);

/// \brief Converts signed active power into OCPP 1.6 Power.Active.Export (always >= 0)
double get_power_active_export_w(double signed_power_w);

} // namespace utils
} // namespace v16
} // namespace ocpp

#endif
