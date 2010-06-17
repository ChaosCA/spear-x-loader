/*
 *  linux/include/asm-arm/posix_types.h
 *
 *  Copyright (C) 1996-1998 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *   27-06-1996	RMK	Created
 */
#ifndef __ARCH_ARM_POSIX_TYPES_H
#define __ARCH_ARM_POSIX_TYPES_H

/*
 * This file is generally used by user-level software, so you need to
 * be a little careful about namespace pollution etc.  Also, we cannot
 * assume GCC is being used.
 */

typedef long			__kernel_off_t;
typedef unsigned int		__kernel_size_t;

#ifdef __GNUC__
typedef long long		__kernel_loff_t;
#endif

#endif
