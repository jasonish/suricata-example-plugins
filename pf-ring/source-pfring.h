/* Copyright (C) 2007-2010 Open Information Security Foundation
 *
 * You can copy, redistribute or modify this Program under the terms of
 * the GNU General Public License version 2 as published by the Free
 * Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/**
 * \file
 *
 * \author William Metcalf <william.metcalf@gmail.com>
 */

#ifndef __SOURCE_PFRING_PLUGIN_H__
#define __SOURCE_PFRING_PLUGIN_H__

#define PFRING_IFACE_NAME_LENGTH 48

typedef struct PfringPluginThreadVars_ PfringPluginThreadVars;

/* PfringPluginIfaceConfig flags */
#define PFRING_CONF_FLAGS_CLUSTER (1 << 0)
#define PFRING_CONF_FLAGS_BYPASS  (1 << 1)

typedef struct PfringPluginIfaceConfig_
{
    uint32_t flags;

    /* cluster param */
    int cluster_id;
    unsigned int ctype;

    char iface[PFRING_IFACE_NAME_LENGTH];
    /* number of threads */
    int threads;

    const char *bpf_filter;

    ChecksumValidationMode checksum_mode;
    SC_ATOMIC_DECLARE(unsigned int, ref);
    void (*DerefFunc)(void *);
} PfringPluginIfaceConfig;

/**
 * \brief per packet Pfring vars
 *
 * This structure is used to pass packet metadata in callbacks.
 */
typedef struct PfringPluginPacketVars_
{
    PfringPluginThreadVars *ptv;
    uint32_t flow_id;
} PfringPluginPacketVars;


void TmModuleReceivePfringPluginRegister(int slot);
void TmModuleDecodePfringPluginRegister(int slot);

int PfringPluginConfGetThreads(void);
void PfringPluginLoadConfig(void);

/*
 * We don't have to use an enum that sucks in our code
 * these values must match with cluster_type in the kernel
 * include file pf_ring.h
 */
#define CLUSTER_FLOW 0
#define CLUSTER_ROUND_ROBIN 1
#define CLUSTER_FLOW_5_TUPLE 4
#define CLUSTER_INNER_FLOW         6
#define CLUSTER_INNER_FLOW_2_TUPLE 7
#define CLUSTER_INNER_FLOW_4_TUPLE 8
#define CLUSTER_INNER_FLOW_5_TUPLE 9
#endif /* __SOURCE_PFRING_H__ */
