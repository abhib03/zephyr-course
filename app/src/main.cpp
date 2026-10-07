#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor/led_sensor.h>
#include <zephyr/shell/shell.h>
#include <stdlib.h>
#include <errno.h>

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

shell_print(sh, "Sample fetched successfully (LED ON)");
return 0;
}

/* Subcommand: read -> calls sensor_channel_get() */
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
shell_error(sh, "Failed to read channel: %d", ret);
return ret;
}

shell_print(sh, "Sensor reading: %d (LED OFF)", val.val1);
return 0;
}

/* Subcommand: info -> prints device metadata */
static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
ARG_UNUSED(argc);
ARG_UNUSED(argv);

bool ready = device_is_ready(sensor_dev);

shell_print(sh, "Device Name: %s", sensor_dev->name);
shell_print(sh, "Ready State: %s", ready ? "Ready" : "Not Ready");
return 0;
}

/* Subcommand: set <value> -> validates argument and calls extension API */
static int cmd_sensor_set(const struct shell *sh, size_t argc, char **argv)
{
ARG_UNUSED(argc);

if (!device_is_ready(sensor_dev)) {
shell_error(sh, "Device %s is not ready", sensor_dev->name);
return -ENODEV;
}

char *endptr = NULL;
errno = 0;
long val = strtol(argv[1], &endptr, 10);

/* Check for parsing errors or invalid characters */
if (errno != 0 || endptr == argv[1] || *endptr != '\0') {
shell_error(sh, "Invalid argument '%s': must be a valid integer", argv[1]);
return -EINVAL;
}

/* Range check: must be positive and within reasonable limits (e.g., 1 to 1000) */
if (val < 1 || val > 1000) {
shell_error(sh, "Value %ld out of range (allowed: 1 to 1000)", val);
return -ERANGE;
}

int ret = led_sensor_set_step(sensor_dev, (int)val);
if (ret < 0) {
shell_error(sh, "Failed to set step size: %d", ret);
return ret;
}

shell_print(sh, "Step size successfully updated to %ld", val);
return 0;
}

/* Define static subcommands under 'sensor' */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
SHELL_CMD(fetch, NULL, "Fetch sample from sensor (turns LED ON)", cmd_sensor_fetch),
SHELL_CMD(read, NULL, "Read channel value (turns LED OFF)", cmd_sensor_read),
SHELL_CMD(info, NULL, "Print device name and ready state", cmd_sensor_info),
/* Enforce exactly 1 argument: argv[0]='set', argv[1]='<value>' -> mand=2, opt=0 */
SHELL_CMD_ARG(set, NULL, "Set step increment: sensor set <1-1000>", cmd_sensor_set, 2, 0),
SHELL_SUBCMD_SET_END
);

/* Register root command 'sensor' */
SHELL_CMD_REGISTER(sensor, &sub_sensor, "LED Sensor management commands", NULL);

int main(void)
{
if (!device_is_ready(sensor_dev)) {
printk("Warning: LED Sensor not ready at boot\n");
} else {
printk("LED Sensor ready. Type 'sensor --help' in shell.\n");
}

return 0;
}
