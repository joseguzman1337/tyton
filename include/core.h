#ifndef CORE_H
#define CORE_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>

/*
 * Compat: module_mutex is no longer exported since kernel ~5.13.
 * Resolve it dynamically via lookup_name (defined in util.c).
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,13,0)
extern unsigned long lookup_name(const char *);
#define module_mutex (*((struct mutex *)lookup_name("module_mutex")))
#endif

#endif /* CORE_H */
