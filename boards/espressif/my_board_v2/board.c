/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/init.h>
#include <zephyr/sys/printk.h>

static int board_early_init(void)
{
printk("Board Initialized\n");
return 0;
}

/* Run after kernel services are initialized, before main() starts */
SYS_INIT(board_early_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
