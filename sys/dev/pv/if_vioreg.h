/*	$OpenBSD: if_vio.c,v 1.26 2023/11/10 15:51:24 bluhm Exp $	*/

/*
 * Copyright (c) 2012 Stefan Fritsch, Alexander Fiveg.
 * Copyright (c) 2010 Minoura Makoto.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * if_vioreg.h:
 */
/* Configuration registers */
#define VIRTIO_NET_CONFIG_MAC		0 /* 8bit x 6byte */
#define VIRTIO_NET_CONFIG_STATUS	6 /* 16bit */

/* Feature bits */
#define VIRTIO_NET_F_CSUM			(1ULL<<0)
#define VIRTIO_NET_F_GUEST_CSUM			(1ULL<<1)
#define VIRTIO_NET_F_CTRL_GUEST_OFFLOADS        (1ULL<<2)
#define VIRTIO_NET_F_MTU                        (1ULL<<3)
#define VIRTIO_NET_F_MAC			(1ULL<<5)
#define VIRTIO_NET_F_GSO			(1ULL<<6)
#define VIRTIO_NET_F_GUEST_TSO4			(1ULL<<7)
#define VIRTIO_NET_F_GUEST_TSO6			(1ULL<<8)
#define VIRTIO_NET_F_GUEST_ECN			(1ULL<<9)
#define VIRTIO_NET_F_GUEST_UFO			(1ULL<<10)
#define VIRTIO_NET_F_HOST_TSO4			(1ULL<<11)
#define VIRTIO_NET_F_HOST_TSO6			(1ULL<<12)
#define VIRTIO_NET_F_HOST_ECN			(1ULL<<13)
#define VIRTIO_NET_F_HOST_UFO			(1ULL<<14)
#define VIRTIO_NET_F_MRG_RXBUF			(1ULL<<15)
#define VIRTIO_NET_F_STATUS			(1ULL<<16)
#define VIRTIO_NET_F_CTRL_VQ			(1ULL<<17)
#define VIRTIO_NET_F_CTRL_RX			(1ULL<<18)
#define VIRTIO_NET_F_CTRL_VLAN			(1ULL<<19)
#define VIRTIO_NET_F_CTRL_RX_EXTRA		(1ULL<<20)
#define VIRTIO_NET_F_GUEST_ANNOUNCE		(1ULL<<21)
#define VIRTIO_NET_F_MQ				(1ULL<<22)
#define VIRTIO_NET_F_CTRL_MAC_ADDR		(1ULL<<23)

/*
 * Config(8) flags. The lowest byte is reserved for generic virtio stuff.
 */

/* Workaround for vlan related bug in qemu < version 2.0 */
#define CONFFLAG_QEMU_VLAN_BUG		(1<<8)

static const struct virtio_feature_name virtio_net_feature_names[] = {
#if VIRTIO_DEBUG
	{ VIRTIO_NET_F_CSUM,			"CSum" },
	{ VIRTIO_NET_F_GUEST_CSUM,		"GuestCSum" },
	{ VIRTIO_NET_F_CTRL_GUEST_OFFLOADS,	"CtrlGuestOffl" },
	{ VIRTIO_NET_F_MTU,			"MTU", },
	{ VIRTIO_NET_F_MAC,			"MAC" },
	{ VIRTIO_NET_F_GSO,			"GSO" },
	{ VIRTIO_NET_F_GUEST_TSO4,		"GuestTSO4" },
	{ VIRTIO_NET_F_GUEST_TSO6,		"GuestTSO6" },
	{ VIRTIO_NET_F_GUEST_ECN,		"GuestECN" },
	{ VIRTIO_NET_F_GUEST_UFO,		"GuestUFO" },
	{ VIRTIO_NET_F_HOST_TSO4,		"HostTSO4" },
	{ VIRTIO_NET_F_HOST_TSO6,		"HostTSO6" },
	{ VIRTIO_NET_F_HOST_ECN,		"HostECN" },
	{ VIRTIO_NET_F_HOST_UFO,		"HostUFO" },
	{ VIRTIO_NET_F_MRG_RXBUF,		"MrgRXBuf" },
	{ VIRTIO_NET_F_STATUS,			"Status" },
	{ VIRTIO_NET_F_CTRL_VQ,			"CtrlVQ" },
	{ VIRTIO_NET_F_CTRL_RX,			"CtrlRX" },
	{ VIRTIO_NET_F_CTRL_VLAN,		"CtrlVLAN" },
	{ VIRTIO_NET_F_CTRL_RX_EXTRA,		"CtrlRXExtra" },
	{ VIRTIO_NET_F_GUEST_ANNOUNCE,		"GuestAnnounce" },
	{ VIRTIO_NET_F_MQ,			"MQ" },
	{ VIRTIO_NET_F_CTRL_MAC_ADDR,		"CtrlMAC" },
#endif
	{ 0,				NULL }
};

/* Status */
#define VIRTIO_NET_S_LINK_UP	1

/* Packet header structure */
struct virtio_net_hdr {
	uint8_t		flags;
	uint8_t		gso_type;
	uint16_t	hdr_len;
	uint16_t	gso_size;
	uint16_t	csum_start;
	uint16_t	csum_offset;

	/* only present if VIRTIO_NET_F_MRG_RXBUF is negotiated */
	uint16_t	num_buffers;
} __packed;

struct virtio_net_hdr_legacy {
	uint8_t		flags;
	uint8_t		gso_type;
	uint16_t	hdr_len;
	uint16_t	gso_size;
	uint16_t	csum_start;
	uint16_t	csum_offset;
} __packed;

#define VIRTIO_NET_HDR_F_NEEDS_CSUM	1 /* flags */
#define VIRTIO_NET_HDR_F_DATA_VALID	2 /* flags */
#define VIRTIO_NET_HDR_GSO_NONE		0 /* gso_type */
#define VIRTIO_NET_HDR_GSO_TCPV4	1 /* gso_type */
#define VIRTIO_NET_HDR_GSO_UDP		3 /* gso_type */
#define VIRTIO_NET_HDR_GSO_TCPV6	4 /* gso_type */
#define VIRTIO_NET_HDR_GSO_ECN		0x80 /* gso_type, |'ed */

#define VIRTIO_NET_MAX_GSO_LEN		(65536+ETHER_HDR_LEN)

/* Control virtqueue */
struct virtio_net_ctrl_cmd {
	uint8_t	class;
	uint8_t	command;
} __packed;
#define VIRTIO_NET_CTRL_RX		0
# define VIRTIO_NET_CTRL_RX_PROMISC	0
# define VIRTIO_NET_CTRL_RX_ALLMULTI	1

#define VIRTIO_NET_CTRL_MAC		1
# define VIRTIO_NET_CTRL_MAC_TABLE_SET	0

#define VIRTIO_NET_CTRL_VLAN		2
# define VIRTIO_NET_CTRL_VLAN_ADD	0
# define VIRTIO_NET_CTRL_VLAN_DEL	1

struct virtio_net_ctrl_status {
	uint8_t	ack;
} __packed;
#define VIRTIO_NET_OK			0
#define VIRTIO_NET_ERR			1

struct virtio_net_ctrl_rx {
	uint8_t	onoff;
} __packed;

struct virtio_net_ctrl_mac_tbl {
	uint32_t nentries;
	uint8_t macs[][ETHER_ADDR_LEN];
} __packed;

struct virtio_net_ctrl_vlan {
	uint16_t id;
} __packed;
