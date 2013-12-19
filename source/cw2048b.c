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

#include "cw_low.h"

 extern uchar Main_authen_pw(void);
static int __init hello_init(void)
{
    uchar rwdata[2];
    int ret = 0;
    cw_gpio_init();

    Main_authen_pw();
    cm_SetUserZone(1,0);
    rwdata[0] = 0x55; rwdata[1] = 0xaa;
    ret = cm_WriteSmallZone(0x00, rwdata, 2);
    pr_info(KERN_DEBUG "%s:%d rw=%02x%02x ret=%d\n", __func__, __LINE__, 
            rwdata[0], rwdata[1], ret);
    rwdata[0] = 0; rwdata[1] = 0;
    ret = cm_ReadSmallZone(0x00, rwdata, 2);
    pr_info(KERN_DEBUG "%s:%d rw=%02x%02x ret=%d\n", __func__, __LINE__, 
            rwdata[0], rwdata[1], ret);
    return -1; // 0; //
}
static void __exit hello_exit(void)
{
    pr_info(KERN_DEBUG "%s:%d OK\n", __func__, __LINE__);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("yang cheng yangchengwork@gmail.com");
MODULE_DESCRIPTION("YUTONG CW2048B Driver");
MODULE_LICENSE("Dual BSD/GPL");
