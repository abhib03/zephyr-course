/*
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_DRIVERS_SENSOR_LED_SENSOR_H_
#define ZEPHYR_INCLUDE_DRIVERS_SENSOR_LED_SENSOR_H_

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Custom extension API: change dynamic increment step in driver data.
 *
 * @param dev Pointer to the led_sensor device instance
 * @param step New increment step value
 * @return 0 on success, negative errno code on failure
 */
int led_sensor_set_step(const struct device *dev, int step);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_DRIVERS_SENSOR_LED_SENSOR_H_ */
