/*
 *  linux/lib/string.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/*
 * stupid library routines.. The optimized versions should generally be found
 * as inline code in <asm-xx/string.h>
 *
 * These are buggy as well..
 *
 * * Fri Jun 25 1999, Ingo Oeser <ioe@informatik.tu-chemnitz.de>
 * -  Added strsep() which will replace strtok() soon (because strsep() is
 *    reentrant and should be faster). Use only strsep() in new code, please.
 */

#include <linux/types.h>
#include <linux/string.h>

#ifndef __HAVE_ARCH_STRCPY
/**
 * strcpy - Copy a %NUL terminated string
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 */
char * strcpy(char * dest,const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}
#endif

#ifndef __HAVE_ARCH_STRNCMP
/**
 * strncmp - Compare two length-limited strings
 * @cs: One string
 * @ct: Another string
 * @count: The maximum number of bytes to compare
 */
int strncmp(const char * cs,const char * ct,size_t count)
{
	register signed char __res = 0;

	while (count) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
		count--;
	}

	return __res;
}
#endif

#ifndef __HAVE_ARCH_STRLEN
/**
 * strlen - Find the length of a string
 * @s: The string to be sized
 */
size_t strlen(const char * s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}
#endif

#ifndef __HAVE_ARCH_MEMCPY
/**
 * memcopy - Copy one area of memory to another
 * @src: Where to copy from
 * @dest: Where to copy to
 * @count: The size of the area.
 */
char * memcpy(char * dest, const char * src, int count)
{
	char *tmp = dest;

	while (count--)
		*tmp++ = *src++;

	return dest;
}
#endif

#ifndef __HAVE_ARCH_WCOPY
/**
 * wcopy - Copy one area of memory to another(short by short)
 * @src: Where to copy from
 * @dest: Where to copy to
 * @count: The size of the area.
 *
 * Note that this is the same as memcpy(), with the arguments reversed.
 *
 * You should not use this function to access IO space, use memcpy_toio()
 * or memcpy_fromio() instead.
 */
ushort * wcopy(const ushort * src, ushort * dest, int count)
{
	ushort *tmp = dest;

	while (count--)
		*tmp++ = *src++;

	return dest;
}
#endif

#ifndef __HAVE_ARCH_LCOPY
/**
 * lcopy - Copy one area of memory to another(long by long)
 * @src: Where to copy from
 * @dest: Where to copy to
 * @count: The size of the area.
 *
 * Note that this is the same as memcpy(), with the arguments reversed.
 *
 * You should not use this function to access IO space, use memcpy_toio()
 * or memcpy_fromio() instead.
 */
ulong * lcopy(const ulong * src, ulong * dest, int count)
{
	ulong *tmp = dest;

	while (count--)
		*tmp++ = *src++;

	return dest;
}
#endif

#ifndef __HAVE_ARCH_BCOPY
/**
 * bcopy - Copy one area of memory to another
 * @src: Where to copy from
 * @dest: Where to copy to
 * @count: The size of the area.
 *
 * Note that this is the same as memcpy(), with the arguments reversed.
 * memcpy() is the standard, bcopy() is a legacy BSD function.
 *
 * You should not use this function to access IO space, use memcpy_toio()
 * or memcpy_fromio() instead.
 */
char * bcopy(const char * src, char * dest, int count)
{
	char *tmp = dest;

	while (count--)
		*tmp++ = *src++;

	return dest;
}
#endif
