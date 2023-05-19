#ifndef _VDRM_PRINT_H_
#define _VDRM_PRINT_H_

#include <linux/printk.h>

#define NAME "vtux"

#define printv(s, ...) \
	({					\
		printk ("%s: ", NAME); 	\
		printk(KERN_CONT s, ##__VA_ARGS__);	\
	})





#endif /*_VDRM_PRINT_H_*/