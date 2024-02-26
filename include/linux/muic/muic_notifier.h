/*
 * include/linux/muic/muic_notifier.h
 *
 * header file supporting MUIC notifier call chain information
 *
 * Copyright (C) 2010 Samsung Electronics
 * Seung-Jin Hahn <sjin.hahn@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#ifndef __MUIC_NOTIFIER_H__
#define __MUIC_NOTIFIER_H__

#include <linux/muic/muic.h>
#include <linux/ccic/ccic_notifier.h>

/* MUIC notifier call chain command */
typedef enum {
	MUIC_NOTIFY_CMD_DETACH	= 0,
	MUIC_NOTIFY_CMD_ATTACH,
	MUIC_NOTIFY_CMD_LOGICALLY_DETACH,
	MUIC_NOTIFY_CMD_LOGICALLY_ATTACH,
	MUIC_PDIC_NOTIFY_CMD_ATTACH,
	MUIC_PDIC_NOTIFY_CMD_DETACH,
	PDIC_MUIC_NOTIFY_CMD_JIG_ATTACH,
	PDIC_MUIC_NOTIFY_CMD_JIG_DETACH,
} muic_notifier_cmd_t;

/* MUIC notifier call sequence,
 * largest priority number device will be called first. */
typedef enum {
	MUIC_NOTIFY_DEV_DOCK = 0,
	MUIC_NOTIFY_DEV_MHL,
	MUIC_NOTIFY_DEV_USB,
	MUIC_NOTIFY_DEV_TSP,
	MUIC_NOTIFY_DEV_CHARGER,
	MUIC_NOTIFY_DEV_PDIC,
	MUIC_NOTIFY_DEV_CPUIDLE,
	MUIC_NOTIFY_DEV_CPUFREQ,
	MUIC_NOTIFY_DEV_MANAGER,
	MUIC_NOTIFY_DEV_HSUART,
	MUIC_NOTIFY_DEV_CABLE_DATA,
} muic_notifier_device_t;

#if defined(CONFIG_MUIC_SUPPORT_KEYBOARDDOCK)
typedef enum {
	KEYBOARD_NOTIFY_DEV_TSP = 0,
	KEYBOARD_NOTIFY_DEV_WACOM,
} keyboard_notifier_device_t;
#endif

struct muic_notifier_struct {
	muic_attached_dev_t attached_dev;
	muic_notifier_cmd_t cmd;
	CC_NOTI_ATTACH_TYPEDEF cxt;
#if defined(CONFIG_USE_SECOND_MUIC)
	bool is_second_muic;
	struct mutex mutex;
#endif
	struct blocking_notifier_head notifier_call_chain;
};

#if defined(CONFIG_MUIC_SUPPORT_KEYBOARDDOCK)
struct keyboard_notifier_struct {
	muic_notifier_cmd_t cmd;
	struct blocking_notifier_head notifier_call_chain;
};
#endif

#define MUIC_NOTIFIER_BLOCK(name)	\
	struct notifier_block (name)

/* muic notifier init/notify function
 * this function is for JUST MUIC device driver.
 * DON'T use function anywhrer else!!
 */
extern void muic_notifier_attach_attached_dev(muic_attached_dev_t new_dev);
extern void muic_notifier_detach_attached_dev(muic_attached_dev_t cur_dev);
extern void muic_pdic_notifier_attach_attached_dev(muic_attached_dev_t new_dev);
extern void muic_pdic_notifier_detach_attached_dev(muic_attached_dev_t new_dev);
extern void muic_notifier_logically_attach_attached_dev(muic_attached_dev_t new_dev);
extern void muic_notifier_logically_detach_attached_dev(muic_attached_dev_t cur_dev);
extern void muic_notifier_chg_off(muic_attached_dev_t new_dev);
#if defined(CONFIG_MUIC_SUPPORT_KEYBOARDDOCK)
extern void keyboard_notifier_attach(void);
extern void keyboard_notifier_detach(void);
#endif
#if defined(CONFIG_CCIC_S2MU107) || defined(CONFIG_CCIC_S2MU106)
extern int muic_ccic_notifier_register(struct notifier_block *nb,
		notifier_fn_t notifier, muic_notifier_device_t listener);
extern int muic_ccic_notifier_unregister(struct notifier_block *nb);
#endif
/* muic notifier register/unregister API
 * for used any where want to receive muic attached device attach/detach. */
extern int muic_notifier_register(struct notifier_block *nb,
		notifier_fn_t notifier, muic_notifier_device_t listener);
extern int muic_notifier_unregister(struct notifier_block *nb);
#if defined(CONFIG_MUIC_SUPPORT_KEYBOARDDOCK)
extern int keyboard_notifier_register(struct notifier_block *nb, notifier_fn_t notifier,
			keyboard_notifier_device_t listener);
extern int keyboard_notifier_unregister(struct notifier_block *nb);
#endif
/* Choose a proper noti. interface for a test */
extern void muic_notifier_set_new_noti(bool flag);

#endif /* __MUIC_NOTIFIER_H__ */