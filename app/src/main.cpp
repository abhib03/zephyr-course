#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

/* Retrieve GPIO spec from the app_led Devicetree alias */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(app_led), gpios);

/* Fallback default if CONFIG_APP_HEARTBEAT_PERIOD_MS is not enabled in Kconfig */
#ifndef CONFIG_APP_HEARTBEAT_PERIOD_MS
#define CONFIG_APP_HEARTBEAT_PERIOD_MS 1000
#endif

int main(void)
{
int ret;

if (!gpio_is_ready_dt(&led)) {
printk("Error: LED GPIO device %s is not ready\n", led.port->name);
return 0;
}

ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
if (ret < 0) {
printk("Error: Failed to configure LED pin: %d\n", ret);
return 0;
}

printk("Heartbeat started with period %d ms\n", CONFIG_APP_HEARTBEAT_PERIOD_MS);

while (1) {
ret = gpio_pin_toggle_dt(&led);
if (ret < 0) {
printk("Error: Failed to toggle LED: %d\n", ret);
return 0;
}

k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
}

return 0;
}
