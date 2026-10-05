/*
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT zephyr_led_sensor

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(LED_SENSOR, CONFIG_SENSOR_LOG_LEVEL);

struct led_sensor_config {
struct gpio_dt_spec led;
};

struct led_sensor_data {
int dummy_val;
};

static int led_sensor_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
const struct led_sensor_config *cfg = dev->config;

gpio_pin_set_dt(&cfg->led, 1);
LOG_INF("sensor_sample_fetch: LED turned ON");

return 0;
}

static int led_sensor_channel_get(const struct device *dev,
  enum sensor_channel chan,
  struct sensor_value *val)
{
const struct led_sensor_config *cfg = dev->config;
struct led_sensor_data *data = dev->data;

gpio_pin_set_dt(&cfg->led, 0);
LOG_INF("sensor_channel_get: LED turned OFF");

val->val1 = data->dummy_val++;
val->val2 = 0;

return 0;
}

static const struct sensor_driver_api led_sensor_api = {
.sample_fetch = led_sensor_sample_fetch,
.channel_get = led_sensor_channel_get,
};

static int led_sensor_init(const struct device *dev)
{
const struct led_sensor_config *cfg = dev->config;

if (!gpio_is_ready_dt(&cfg->led)) {
LOG_ERR("LED GPIO device not ready");
return -ENODEV;
}

int ret = gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
if (ret < 0) {
LOG_ERR("Failed to configure LED pin: %d", ret);
return ret;
}

LOG_INF("LED Sensor initialized successfully");
return 0;
}

#define LED_SENSOR_INIT(inst)                                              \
static struct led_sensor_data led_sensor_data_##inst;                  \
static const struct led_sensor_config led_sensor_config_##inst = {     \
.led = GPIO_DT_SPEC_INST_GET(inst, led_gpios),                    \
};                                                                     \
DEVICE_DT_INST_DEFINE(inst,                                            \
      led_sensor_init,                                     \
      NULL,                                                \
      &led_sensor_data_##inst,                             \
      &led_sensor_config_##inst,                           \
      POST_KERNEL,                                         \
      CONFIG_SENSOR_INIT_PRIORITY,                         \
      &led_sensor_api);

DT_INST_FOREACH_STATUS_OKAY(LED_SENSOR_INIT)
