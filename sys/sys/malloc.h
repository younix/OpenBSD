/*	$OpenBSD: malloc.h,v 1.125 2023/07/03 06:45:44 guenther Exp $	*/
/*	$NetBSD: malloc.h,v 1.39 1998/07/12 19:52:01 augustss Exp $	*/

/*
 * Copyright (c) 1987, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)malloc.h	8.5 (Berkeley) 5/3/95
 */

#ifndef _SYS_MALLOC_H_
#define	_SYS_MALLOC_H_

#include <sys/queue.h>

#define KERN_MALLOC_BUCKETS	1
#define KERN_MALLOC_BUCKET	2
#define KERN_MALLOC_KMEMNAMES	3
#define KERN_MALLOC_KMEMSTATS	4
#define KERN_MALLOC_MAXID	5

#define CTL_KERN_MALLOC_NAMES { \
	{ 0, 0 }, \
	{ "buckets", CTLTYPE_STRING }, \
	{ "bucket", CTLTYPE_NODE }, \
	{ "kmemnames", CTLTYPE_STRING }, \
	{ "kmemstat", CTLTYPE_NODE }, \
}

/*
 * flags to malloc
 */
enum malloc_flags {
	M_WAITOK	= 0x0001,
	M_NOWAIT	= 0x0002,
	M_CANFAIL	= 0x0004,
	M_ZERO		= 0x0008
};

/*
 * Types of memory to be allocated
 */
enum malloc_type {
	M_FREE = 0,	/* should be on free list */
	M_DEVBUF,	/* device driver memory */
	M_PCB,		/* protocol control blocks */
	M_RTABLE,	/* routing tables */
	M_PF,		/* packet filter structures */
	M_IFADDR,	/* interface addresses */
	M_IFGROUP,	/* interface groups */
	M_SYSCTL,	/* sysctl persistent buffers */
	M_COUNTERS,	/* per-CPU counters via counters_alloc(9) */
	M_IOCTLOPS,	/* ioctl data buffers */
	M_IOV,		/* large IOVs */
	M_MOUNT,	/* VFS mount structs */
	M_NFSREQ,	/* NFS request headers */
	M_NFSMNT,	/* NFS mount structures */
	M_LOG,		/* messages in kernel log stash */
	M_VNODE,	/* Dynamically allocated vnodes */
	M_DQUOT,	/* UFS quota entries */
	M_UFSMNT,	/* UFS mount structures */
	M_SHM,		/* SVID compatible shared memory segments */
	M_VMMAP,	/* VM map structures */
	M_SEM,		/* SVID compatible semaphores */
	M_DIRHASH,	/* UFS directory hash structures */
	M_ACPI,		/* ACPI structures */
	M_VMPMAP,	/* VM pmap data */
	M_FILEDESC,	/* open file descriptor tables */
	M_SIGIO,	/* sigio structures */
	M_PROC,		/* proc structures */
	M_SUBPROC,	/* proc sub-structures */
	M_MFSNODE,	/* MFS vnode private part */
	M_NETADDR,	/* export host address structures */
	M_NFSSVC,	/* NFS server structures */
	M_NFSD,		/* NFS server daemon structures */
	M_IPMOPTS,	/* internet multicast options */
	M_IPMADDR,	/* internet multicast addresses */
	M_IFMADDR,	/* link-level multicast addresses */
	M_MRTABLE,	/* multicast routing tables */
	M_ISOFSMNT,	/* ISOFS mount structures */
	M_ISOFSNODE,	/* ISOFS vnode private part */
	M_MSDOSFSMNT,	/* MSDOS FS mount structures */
	M_MSDOSFSFAT,	/* MSDOS FS FAT tables */
	M_MSDOSFSNODE,	/* MSDOS FS vnode private part */
	M_TTYS,		/* allocated tty structures */
	M_EXEC,		/* argument lists & other mem used by exec */
	M_MISCFSMNT,	/* miscellaneous FS mount structures */
	M_FUSEFS,	/* FUSE FS mount structures */
	M_PFKEY,	/* pfkey data */
	M_TDB,		/* transforms database */
	M_XDATA,	/* IPsec data */
	M_PAGEDEP,	/* file page dependencies */
	M_INODEDEP,	/* inode dependencies */
	M_NEWBLK,	/* new block allocation */
	M_INDIRDEP,	/* indirect block dependencies */
	M_VMSWAP,	/* VM swap structures */
	M_UVMAMAP,	/* UVM amap and related */
	M_UVMAOBJ,	/* UVM aobj and related */
	M_USB,		/* USB general */
	M_USBDEV,	/* USB device driver */
	M_USBHC,	/* USB host controller */
	M_WITNESS,	/* witness(4) memory */
	M_MEMDESC,	/* memory range */
	M_CRYPTO_DATA,	/* crypto(9) data buffers */
	M_CREDENTIALS,	/* ipsec(4) related credentials */

	/* KAME IPv6 */
	M_IP6OPT,	/* IPv6 options */
	M_IP6NDP,	/* IPv6 Neighbor Discovery structures */

	M_TEMP,		/* miscellaneous temporary data buffers */

	M_NTFSMNT,	/* NTFS mount structures */
	M_NTFSNTNODE,	/* NTFS ntnode information */
	M_NTFSFNODE,	/* NTFS fnode information */
	M_NTFSDIR,	/* NTFS directory buffers */
	M_NTFSNTHASH,	/* NTFS ntnode hash tables */
	M_NTFSNTVATTR,	/* NTFS file attribute information */
	M_NTFSRDATA,	/* NTFS resident data */
	M_NTFSDECOMP,	/* NTFS decompression temporary storage */
	M_NTFSRUN,	/* NTFS vrun storage */

	M_KEVENT,	/* kqueue(2) data structures */

	M_SYNCACHE,	/* SYN cache hash array */

	M_UDFMOUNT,	/* UDF mount structures */
	M_UDFFENTRY,	/* UDF file entries */
	M_UDFFID,	/* UDF file IDs */

	M_AGP,		/* AGP memory */

	M_DRM,		/* Direct Rendering Manager */

	M_LAST		/* is last type + 1 */
};

#define	INITKMEMNAMES { \
	"free",			/* M_FREE */ \
	"devbuf",		/* M_DEVBUF */ \
	"pcb",			/* M_PCB */ \
	"rtable",		/* M_RTABLE */ \
	"pf",			/* M_PF */ \
	"ifaddr",		/* M_IFADDR */ \
	"ifgroup",		/* M_IFGROUP */ \
	"sysctl",		/* M_SYSCTL */ \
	"counters",		/* M_COUNTERS */ \
	"ioctlops",		/* M_IOCTLOPS */ \
	"iov",			/* M_IOV */ \
	"mount",		/* M_MOUNT */ \
	"NFS req",		/* M_NFSREQ */ \
	"NFS mount",		/* M_NFSMNT */ \
	"log",			/* M_LOG */ \
	"vnodes",		/* M_VNODE */ \
	"UFS quota",		/* M_DQUOT */ \
	"UFS mount",		/* M_UFSMNT */ \
	"shm",			/* M_SHM */ \
	"VM map",		/* M_VMMAP */ \
	"sem",			/* M_SEM */ \
	"dirhash",		/* M_DIRHASH */ \
	"ACPI", 		/* M_ACPI */ \
	"VM pmap",		/* M_VMPMAP */ \
	"file desc",		/* M_FILEDESC */ \
	"sigio",		/* M_SIGIO */ \
	"proc",			/* M_PROC */ \
	"subproc",		/* M_SUBPROC */ \
	"MFS node",		/* M_MFSNODE */ \
	"Export Host",		/* M_NETADDR */ \
	"NFS srvsock",		/* M_NFSSVC */ \
	"NFS daemon",		/* M_NFSD */ \
	"ip_moptions",		/* M_IPMOPTS */ \
	"in_multi",		/* M_IPMADDR */ \
	"ether_multi",		/* M_IFMADDR */ \
	"mrt",			/* M_MRTABLE */ \
	"ISOFS mount",		/* M_ISOFSMNT */ \
	"ISOFS node",		/* M_ISOFSNODE */ \
	"MSDOSFS mount",	/* M_MSDOSFSMNT */ \
	"MSDOSFS fat",		/* M_MSDOSFSFAT */ \
	"MSDOSFS node",		/* M_MSDOSFSNODE */ \
	"ttys",			/* M_TTYS */ \
	"exec",			/* M_EXEC */ \
	"miscfs mount",		/* M_MISCFSMNT */ \
	"fusefs mount", 	/* M_FUSEFS */ \
	"pfkey data",		/* M_PFKEY */ \
	"tdb",			/* M_TDB */ \
	"xform_data",		/* M_XDATA */ \
	"pagedep",		/* M_PAGEDEP */ \
	"inodedep",		/* M_INODEDEP */ \
	"newblk",		/* M_NEWBLK */ \
	"indirdep",		/* M_INDIRDEP */ \
	"VM swap",		/* M_VMSWAP */ \
	"UVM amap",		/* M_UVMAMAP */ \
	"UVM aobj",		/* M_UVMAOBJ */ \
	"USB",			/* M_USB */ \
	"USB device",		/* M_USBDEV */ \
	"USB HC",		/* M_USBHC */ \
	"witness",		/* M_WITNESS */ \
	"memdesc",		/* M_MEMDESC */ \
	"crypto data",		/* M_CRYPTO_DATA */ \
	"IPsec creds",		/* M_CREDENTIALS */ \
	"ip6_options",		/* M_IP6OPT */ \
	"NDP",			/* M_IP6NDP */ \
	"temp",			/* M_TEMP */ \
	"NTFS mount",		/* M_NTFSMNT */ \
	"NTFS node",		/* M_NTFSNTNODE */ \
	"NTFS fnode",		/* M_NTFSFNODE */ \
	"NTFS dir",		/* M_NTFSDIR */ \
	"NTFS hash",		/* M_NTFSNTHASH */ \
	"NTFS attr",		/* M_NTFSNTVATTR */ \
	"NTFS data",		/* M_NTFSRDATA */ \
	"NTFS decomp",		/* M_NTFSDECOMP */ \
	"NTFS vrun",		/* M_NTFSRUN */ \
	"kqueue",		/* M_KEVENT */ \
	"SYN cache",		/* M_SYNCACHE */ \
	"UDF mount",		/* M_UDFMOUNT */ \
	"UDF file entry",	/* M_UDFFENTRY */ \
	"UDF file id",		/* M_UDFFID */ \
	"AGP Memory",		/* M_AGP */ \
	"DRM",			/* M_DRM */ \
}

struct kmemstats {
	long	ks_inuse;	/* # of packets of this type currently in use */
	long	ks_calls;	/* total packets of this type ever allocated */
	long 	ks_memuse;	/* total memory held in bytes */
	u_short	ks_limblocks;	/* number of times blocked for hitting limit */
	long	ks_maxused;	/* maximum number ever used */
	long	ks_limit;	/* most that are allowed to exist */
	long	ks_size;	/* sizes of this thing that are allocated */
	long	ks_spare;
};

/*
 * Array of descriptors that describe the contents of each page
 */
struct kmemusage {
	short ku_indx;		/* bucket index */
	union {
		u_short freecnt;/* for small allocations, free pieces in page */
		u_short pagecnt;/* for large allocations, pages alloced */
	} ku_un;
};
#define	ku_freecnt ku_un.freecnt
#define	ku_pagecnt ku_un.pagecnt

struct kmem_freelist;

/*
 * Set of buckets for each size of memory block that is retained
 */
struct kmembuckets {
	XSIMPLEQ_HEAD(, kmem_freelist) kb_freelist; /* list of free blocks */
	u_int64_t kb_calls;	/* total calls to allocate this size */
	u_int64_t kb_total;	/* total number of blocks allocated */
	u_int64_t kb_totalfree;	/* # of free elements in this bucket */
	u_int64_t kb_elmpercl;	/* # of elements in this sized allocation */
	u_int64_t kb_highwat;	/* high water mark */
	u_int64_t kb_couldfree;	/* over high water mark and could free */
};

/*
 * Constants for setting the parameters of the kernel memory allocator.
 *
 * 2 ** MINBUCKET is the smallest unit of memory that will be
 * allocated. It must be at least large enough to hold a pointer.
 *
 * Units of memory less or equal to MAXALLOCSAVE will permanently
 * allocate physical memory; requests for these size pieces of
 * memory are quite fast. Allocations greater than MAXALLOCSAVE must
 * always allocate and free physical memory; requests for these
 * size allocations should be done infrequently as they will be slow.
 *
 * Constraints: PAGE_SIZE <= MAXALLOCSAVE <= 2 ** (MINBUCKET + 14), and
 * MAXALLOCSIZE must be a power of two.
 */
#define MINBUCKET	4		/* 4 => min allocation of 16 bytes */

#ifdef _KERNEL

#define	MINALLOCSIZE	(1 << MINBUCKET)
#define	MAXALLOCSAVE	(2 * PAGE_SIZE)

/*
 * Turn virtual addresses into kmem map indices
 */
#define	kmemxtob(alloc)	(kmembase + (alloc) * PAGE_SIZE)
#define	btokmemx(addr)	(((caddr_t)(addr) - kmembase) / PAGE_SIZE)
#define	btokup(addr)	(&kmemusage[((caddr_t)(addr) - kmembase) >> PAGE_SHIFT])

extern struct kmemstats kmemstats[];
extern struct kmemusage *kmemusage;
extern char *kmembase;
extern struct kmembuckets bucket[];

void	*malloc(size_t, enum malloc_type, enum malloc_flags);
void	*mallocarray(size_t, size_t, enum malloc_type, enum malloc_flags);
void	free(void *, enum malloc_type, size_t);
int	sysctl_malloc(int *, u_int, void *, size_t *, void *, size_t,
	    struct proc *);

void	malloc_printit(int (*)(const char *, ...));

void	poison_mem(void *, size_t);
int	poison_check(void *, size_t, size_t *, uint32_t *);
uint32_t poison_value(void *);

#endif /* _KERNEL */
#endif /* !_SYS_MALLOC_H_ */
