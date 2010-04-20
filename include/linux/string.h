#ifndef _LINUX_STRING_H_
#define _LINUX_STRING_H_

#include <linux/types.h>	/* for size_t */
#include <linux/stddef.h>	/* for NULL */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Include machine specific inline routines
 */
#include <asm/string.h>

#ifndef __HAVE_ARCH_STRCPY
extern char * strcpy(char *,const char *);
#endif
#ifndef __HAVE_ARCH_STRNCMP
extern int strncmp(const char *,const char *,__kernel_size_t);
#endif
#ifndef __HAVE_ARCH_STRLEN
extern __kernel_size_t strlen(const char *);
#endif
#ifndef __HAVE_ARCH_MEMCPY
char * memcpy(char *, const char *, int);
#endif
#ifndef __HAVE_ARCH_BCOPY
char * bcopy(const char *, char *, int);
#endif
#ifndef __HAVE_ARCH_WCOPY
ushort * wcopy(const ushort *, ushort *, int);
#endif
#ifndef __HAVE_ARCH_LCOPY
ulong * lcopy(const ulong *, ulong *, int);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _LINUX_STRING_H_ */
