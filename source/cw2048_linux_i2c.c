// vim:et:ts=4:sw=4:
/*
 * Copyright (C) 2013 Y-COM, Inc.
 *
 * Initial Code:
 * 	Gump Yang
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/i2c.h>

static char *busname= "i2c0";
static int i2cbus = 0;
static struct i2c_client *client;

module_param(i2cbus, int, S_IRUGO);
module_param(busname, charp, S_IRUGO);

static struct i2c_board_info cw2048b_info = {
    I2C_BOARD_INFO("cw2048b", 0x5a),
};

static int __init hello_init(void)
{
    struct i2c_adapter *i2c_adap;
    i2c_adap = i2c_get_adapter(i2cbus);
    client = i2c_new_device(i2c_adap, &cw2048b_info);
    i2c_put_adapter(i2c_adap);
    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
    return 0;
}
static void __exit hello_exit(void)
{
    i2c_unregister_device(client);
    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("yang cheng yangchengwork@gmail.com");
MODULE_DESCRIPTION("YUTONG CW2048B Driver");
MODULE_LICENSE("Dual BSD/GPL");
