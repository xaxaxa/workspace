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
	{ 0x7ba4171d, "module_layout" },
	{ 0x1edfa577, "get_sb_nodev" },
	{ 0x3cd5d1d, "slab_buffer_size" },
	{ 0xba1b52e5, "malloc_sizes" },
	{ 0x105e2727, "__tracepoint_kmalloc" },
	{ 0x7e9bea3f, "kmem_cache_alloc_notrace" },
	{ 0xea147363, "printk" },
	{ 0xb4390f9a, "mcount" },
	{ 0x715405eb, "path_lookup" },
	{ 0x6f685371, "register_filesystem" },
	{ 0x37a0cba, "kfree" },
	{ 0x392b70ec, "unregister_filesystem" },
	{ 0x4d43ca7b, "generic_shutdown_super" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "89633D36768E5745D1C845D");

static const struct rheldata _rheldata __used
__attribute__((section(".rheldata"))) = {
	.rhel_major = 6,
	.rhel_minor = 3,
};
