#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>

int main()
{
const struct device *const dev = DEVICE_DT_GET_ONE(zephyr_led_sensor);

if (!device_is_ready(dev)) {
printk("LED sensor device not ready!\n");
return 0;
}

printk("Starting LED Sensor loop from C++...\n");

while (true) {
sensor_value val{};

/* Step 1: Fetch sample -> Turns LED ON */
sensor_sample_fetch(dev);
k_msleep(1000);

/* Step 2: Get channel -> Turns LED OFF */
sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
printk("Sensor reading: %d\n", val.val1);
k_msleep(1000);
}

return 0;
}
