/*
 * Copyright (c) 2012-2013 Tristan Le Guern <leguern AT medu DOT se>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "tuntap.h"
#include "tuntap_private.h"

#include <sys/types.h>
#include <sys/ioctl.h>

#ifdef SunOS
# include <stropts.h>
#endif
#include <stdlib.h>
#include <string.h>

int
tuntap_set_mtu(struct device *dev, int mtu) {
	struct ifreq ifr;

	/* Only accept started device */
	if (dev->tun_fd == -1) {
		tuntap_log(TUNTAP_LOG_NOTICE, "Device is not started");
		return 0;
	}

	(void)memset(&ifr, '\0', sizeof ifr);
	(void)memcpy(ifr.ifr_name, dev->if_name, sizeof dev->if_name);
#if defined SunOS
	ifr.ifr_metric = mtu;
#else
	ifr.ifr_mtu = mtu;
#endif

	if (ioctl(dev->ctrl_sock, SIOCSIFMTU, &ifr) == -1) {
		tuntap_log(TUNTAP_LOG_ERR, "Can't set MTU");
		return -1;
	}
	return 0;
}

