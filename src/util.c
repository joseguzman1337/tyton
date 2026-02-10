#include <linux/kallsyms.h>

#include "core.h"
#include "util.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,13,0)
struct module *get_module_from_addr(unsigned long addr){
	typedef struct module *(*module_address_fn)(unsigned long);
	module_address_fn fn = (module_address_fn)lookup_name("__module_address");
	if (!fn)
		return NULL;
	return fn(addr);
}
#else
struct module *get_module_from_addr(unsigned long addr){
	return  __module_address(addr);
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,7,0)

#include <linux/kprobes.h>

static struct kprobe kp;

unsigned long lookup_name(const char *name){
	kp.symbol_name = name;

	if (register_kprobe(&kp) < 0)
		return 0;

	unregister_kprobe(&kp);

	return (unsigned long)kp.addr;
}

#else

unsigned long lookup_name(const char *name){
	return kallsyms_lookup_name(name);
}

#endif
