#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>

static const struct device *const sensor_dev = DEVICE_DT_GET_ONE(zephyr_led_sensor);

/* Subcommand: fetch -> calls sensor_sample_fetch() */
static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
ARG_UNUSED(argc);
ARG_UNUSED(argv);

if (!device_is_ready(sensor_dev)) {
shell_error(sh, "Device %s is not ready", sensor_dev->name);
return -ENODEV;
}

int ret = sensor_sample_fetch(sensor_dev);
if (ret < 0) {
shell_error(sh, "Failed to fetch sample: %d", ret);
return ret;
}

shell_print(sh, "Sample fetched successfully (LED toggled ON)");
return 0;
}

/* Subcommand: read -> calls sensor_channel_get() and prints result */
static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
ARG_UNUSED(argc);
ARG_UNUSED(argv);

if (!device_is_ready(sensor_dev)) {
shell_error(sh, "Device %s is not ready", sensor_dev->name);
return -ENODEV;
}

struct sensor_value val;
int ret = sensor_channel_get(sensor_dev, SENSOR_CHAN_ALL, &val);
if (ret < 0) {
shell_error(sh, "Failed to get channel: %d", ret);
return ret;
}

shell_print(sh, "Sensor reading: %d (LED toggled OFF)", val.val1);
return 0;
}

/* Subcommand: info -> prints device name and ready state */
static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
ARG_UNUSED(argc);
ARG_UNUSED(argv);

bool ready = device_is_ready(sensor_dev);

shell_print(sh, "Device Name: %s", sensor_dev->name);
shell_print(sh, "Ready State: %s", ready ? "Ready" : "Not Ready");
return 0;
}

/* Register subcommands under root command 'sensor' */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
SHELL_CMD(fetch, NULL, "Fetch sample from sensor (turns LED ON)", cmd_sensor_fetch),
SHELL_CMD(read, NULL, "Read channel value (turns LED OFF)", cmd_sensor_read),
SHELL_CMD(info, NULL, "Print device name and ready state", cmd_sensor_info),
SHELL_SUBCMD_SET_END
);

/* Register root command 'sensor' */
SHELL_CMD_REGISTER(sensor, &sub_sensor, "LED Sensor commands", NULL);

int main(void)
{
if (!device_is_ready(sensor_dev)) {
printk("Warning: LED Sensor not ready at boot\n");
} else {
printk("LED Sensor ready. Shell commands available.\n");
}

return 0;
}
