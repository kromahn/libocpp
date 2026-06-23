// SPDX-License-Identifier: Apache-2.0
// Copyright Pionix GmbH and Contributors to EVerest

#include <gtest/gtest.h>

#include <ocpp/v16/utils.hpp>

namespace ocpp {
namespace v16 {
namespace utils {

TEST(V16UtilsTest, splits_signed_active_power_to_import) {
    EXPECT_DOUBLE_EQ(get_power_active_import_w(123.4), 123.4);
    EXPECT_DOUBLE_EQ(get_power_active_import_w(0.0), 0.0);
    EXPECT_DOUBLE_EQ(get_power_active_import_w(-123.4), 0.0);
}

TEST(V16UtilsTest, splits_signed_active_power_to_export) {
    EXPECT_DOUBLE_EQ(get_power_active_export_w(123.4), 0.0);
    EXPECT_DOUBLE_EQ(get_power_active_export_w(0.0), 0.0);
    EXPECT_DOUBLE_EQ(get_power_active_export_w(-123.4), 123.4);
}

TEST(V16UtilsTest, splits_signed_current_to_import) {
    EXPECT_DOUBLE_EQ(get_current_import_a(16.0), 16.0);
    EXPECT_DOUBLE_EQ(get_current_import_a(0.0), 0.0);
    EXPECT_DOUBLE_EQ(get_current_import_a(-16.0), 0.0);
}

TEST(V16UtilsTest, splits_signed_current_to_export) {
    EXPECT_DOUBLE_EQ(get_current_export_a(16.0), 0.0);
    EXPECT_DOUBLE_EQ(get_current_export_a(0.0), 0.0);
    EXPECT_DOUBLE_EQ(get_current_export_a(-16.0), 16.0);
}

} // namespace utils
} // namespace v16
} // namespace ocpp
