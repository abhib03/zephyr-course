#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor/led_sensor.h>
#include <zephyr/sys/printk.h>

int main()
{
const struct device *const dev = DEVICE_DT_GET_ONE(zephyr_led_sensor);

if (!device_is_ready(dev)) {
printk("LED sensor device not ready!\n");
return 0;
}

printk("Starting LED Sensor loop with custom extension API...\n");

int count = 0;

while (true) {
sensor_value val{};

/* Step 1: Fetch sample -> Turns LED ON */
sensor_sample_fetch(dev);
k_msleep(1000);

/* Step 2: Get channel -> Turns LED OFF and increments by step */
sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
printk("Sensor reading: %d\n", val.val1);
k_msleep(1000);

count++;

/* Demonstrate custom extension API altering dynamic struct */
if (count == 3) {
printk(">>> Calling custom extension API: led_sensor_set_step(dev, 10) <<<\n");
led_sensor_set_step(dev, 10);
}
}

return 0;
}
