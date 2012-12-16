#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xa7ac7bb, "module_layout" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x497950dd, "__register_chrdev" },
	{ 0x27e1a049, "printk" },
	{ 0xb6f8b7, "try_module_get" },
	{ 0x4f8b5ddb, "_copy_to_user" },
	{ 0xd315b31d, "module_put" },
	{ 0x1000e51, "schedule" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "8DF97648974D1B59D6209AA");
