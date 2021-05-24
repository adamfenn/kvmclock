/*-
 * Copyright (c) 2014, Bryan Venteicher <bryanv@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#ifndef X86_PVCLOCK
#define X86_PVCLOCK

#ifdef _KERNEL
#include <sys/timetc.h>
#endif /* _KERNEL */

#define	PVCLOCK_CDEVNAME		"pvclock"

struct pvclock_vcpu_time_info {
	uint32_t	version;
	uint32_t	pad0;
	uint64_t	tsc_timestamp;
	uint64_t	system_time;
	uint32_t	tsc_to_system_mul;
	int8_t		tsc_shift;
	uint8_t		flags;
	uint8_t		pad[2];
};

#define PVCLOCK_FLAG_TSC_STABLE		0x01
#define PVCLOCK_FLAG_GUEST_PASUED	0x02

#ifdef _KERNEL

typedef struct pvclock_vcpu_time_info *pvclock_get_curcpu_timeinfo_t(void *arg);

struct pvclock_wall_clock {
	uint32_t	version;
	uint32_t	sec;
	uint32_t	nsec;
};

struct pvclock_softc {
	struct timecounter		 tc;
	struct cdev			*cdev;
	pvclock_get_curcpu_timeinfo_t	*get_curcpu_ti;
	void				*get_curcpu_ti_arg;
	struct pvclock_vcpu_time_info	*ti_vcpu0_page;
	bool				 stable_flag_supported;
};

void		pvclock_resume(void);
uint64_t	pvclock_get_last_cycles(void);
uint64_t	pvclock_tsc_freq(struct pvclock_vcpu_time_info *ti);
uint64_t	pvclock_get_timecount(struct pvclock_vcpu_time_info *ti);
void		pvclock_get_wallclock(struct pvclock_wall_clock *wc,
		    struct timespec *ts);

void		pvclock_softc_init(struct pvclock_softc *sc,
		    const char *tc_name, int tc_quality, u_int tc_flags,
		    pvclock_get_curcpu_timeinfo_t *get_curcpu_ti,
		    void *get_curcpu_ti_arg,
		    struct pvclock_vcpu_time_info *ti_vcpu0_page,
		    bool stable_flag_supported);
void		pvclock_attach(device_t dev, struct pvclock_softc *sc);
int		pvclock_detach(device_t dev, struct pvclock_softc *sc);

#endif /* _KERNEL */

#endif
