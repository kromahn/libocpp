// SPDX-License-Identifier: Apache-2.0
// Copyright Pionix GmbH and Contributors to EVerest

#include <algorithm>

#include <ocpp/common/utils.hpp>
#include <ocpp/v16/utils.hpp>

namespace ocpp {
namespace v16 {
namespace utils {

size_t get_message_size(const ocpp::Call<StopTransactionRequest>& call) {
    return json(call).at(CALL_PAYLOAD).dump().length();
}

void drop_transaction_data(size_t max_message_size, ocpp::Call<StopTransactionRequest>& call) {
    auto& transaction_data = call.msg.transactionData.value();
    while (get_message_size(call) > max_message_size && transaction_data.size() > 2) {
        for (size_t i = 1; i < transaction_data.size() - 1; i = i + 2) {
            transaction_data.erase(transaction_data.begin() + i);
        }
    }
}

bool is_critical(const std::string& security_event) {
    if (security_event == ocpp::security_events::FIRMWARE_UPDATED) {
        return true;
    } else if (security_event == ocpp::security_events::SETTINGSYSTEMTIME) {
        return true;
    } else if (security_event == ocpp::security_events::STARTUP_OF_THE_DEVICE) {
        return true;
    } else if (security_event == ocpp::security_events::RESET_OR_REBOOT) {
        return true;
    } else if (security_event == ocpp::security_events::SECURITYLOGWASCLEARED) {
        return true;
    } else if (security_event == ocpp::security_events::MEMORYEXHAUSTION) {
        return true;
    } else if (security_event == ocpp::security_events::TAMPERDETECTIONACTIVATED) {
        return true;
    }

    return false;
}

double get_power_active_import_w(double signed_power_w) {
    return std::max(signed_power_w, 0.0);
}

double get_power_active_export_w(double signed_power_w) {
    return std::max(-signed_power_w, 0.0);
}

double get_current_import_a(double signed_current_a) {
    return std::max(signed_current_a, 0.0);
}

double get_current_export_a(double signed_current_a) {
    return std::max(-signed_current_a, 0.0);
}

} // namespace utils
} // namespace v16
} // namespace ocpp
