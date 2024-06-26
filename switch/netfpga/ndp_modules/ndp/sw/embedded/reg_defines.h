//-
// Copyright (c) 2015 University of Cambridge
// All rights reserved.
//
// This software was developed by Stanford University and the University of Cambridge Computer Laboratory 
// under National Science Foundation under Grant No. CNS-0855268,
// the University of Cambridge Computer Laboratory under EPSRC INTERNET Project EP/H040536/1 and
// by the University of Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-11-C-0249 ("MRC2"), 
// as part of the DARPA MRC research programme.
//
// @NETFPGA_LICENSE_HEADER_START@
//
// Licensed to NetFPGA C.I.C. (NetFPGA) under one or more contributor
// license agreements.  See the NOTICE file distributed with this work for
// additional information regarding copyright ownership.  NetFPGA licenses this
// file to you under the NetFPGA Hardware-Software License, Version 1.0 (the
// "License"); you may not use this file except in compliance with the
// License.  You may obtain a copy of the License at:
//
//   http://www.netfpga-cic.org
//
// Unless required by applicable law or agreed to in writing, Work distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations under the License.
//
// @NETFPGA_LICENSE_HEADER_END@
/////////////////////////////////////////////////////////////////////////////////
// This is an automatically generated header definitions file
/////////////////////////////////////////////////////////////////////////////////

//######################################################
//# Definitions for MICROBLAZE_AXI_IIC
//######################################################
#define SUME_MICROBLAZE_AXI_IIC_BASEADDR 0x40800000
#define SUME_MICROBLAZE_AXI_IIC_HIGHADDR 0x4080FFFF
#define SUME_MICROBLAZE_AXI_IIC_SIZEADDR 0x10000


//######################################################
//# Definitions for MICROBLAZE_UARTLITE
//######################################################
#define SUME_MICROBLAZE_UARTLITE_BASEADDR 0x40600000
#define SUME_MICROBLAZE_UARTLITE_HIGHADDR 0x4060FFFF
#define SUME_MICROBLAZE_UARTLITE_SIZEADDR 0x10000


//######################################################
//# Definitions for MICROBLAZE_DLMB_BRAM
//######################################################
#define SUME_MICROBLAZE_DLMB_BRAM_BASEADDR 0x00000000
#define SUME_MICROBLAZE_DLMB_BRAM_HIGHADDR 0x0000FFFF
#define SUME_MICROBLAZE_DLMB_BRAM_SIZEADDR 0x10000


//######################################################
//# Definitions for MICROBLAZE_ILMB_BRAM
//######################################################
#define SUME_MICROBLAZE_ILMB_BRAM_BASEADDR 0x00000000
#define SUME_MICROBLAZE_ILMB_BRAM_HIGHADDR 0x0000FFFF
#define SUME_MICROBLAZE_ILMB_BRAM_SIZEADDR 0x10000


//######################################################
//# Definitions for MICROBLAZE_AXI_INTC
//######################################################
#define SUME_MICROBLAZE_AXI_INTC_BASEADDR 0x41200000
#define SUME_MICROBLAZE_AXI_INTC_HIGHADDR 0x4120FFFF
#define SUME_MICROBLAZE_AXI_INTC_SIZEADDR 0x10000


//######################################################
//# Definitions for INPUT_DRR_ARBITER
//######################################################
#define SUME_INPUT_DRR_ARBITER_BASEADDR 0x44010000
#define SUME_INPUT_DRR_ARBITER_HIGHADDR 0x44010FFF
#define SUME_INPUT_DRR_ARBITER_SIZEADDR 0x1000

#define SUME_INPUT_DRR_ARBITER_0_ID 0x44010000
#define SUME_INPUT_DRR_ARBITER_0_ID_DEFAULT 0x0000DA01
#define SUME_INPUT_DRR_ARBITER_0_ID_WIDTH 32
#define SUME_INPUT_DRR_ARBITER_0_VERSION 0x44010004
#define SUME_INPUT_DRR_ARBITER_0_VERSION_DEFAULT 0x1
#define SUME_INPUT_DRR_ARBITER_0_VERSION_WIDTH 32
#define SUME_INPUT_DRR_ARBITER_0_RESET 0x44010008
#define SUME_INPUT_DRR_ARBITER_0_RESET_DEFAULT 0x0
#define SUME_INPUT_DRR_ARBITER_0_RESET_WIDTH 16
#define SUME_INPUT_DRR_ARBITER_0_FLIP 0x4401000c
#define SUME_INPUT_DRR_ARBITER_0_FLIP_DEFAULT 0x0
#define SUME_INPUT_DRR_ARBITER_0_FLIP_WIDTH 32
#define SUME_INPUT_DRR_ARBITER_0_DEBUG 0x44010010
#define SUME_INPUT_DRR_ARBITER_0_DEBUG_DEFAULT 0x0
#define SUME_INPUT_DRR_ARBITER_0_DEBUG_WIDTH 32
#define SUME_INPUT_DRR_ARBITER_0_PKTIN 0x44010014
#define SUME_INPUT_DRR_ARBITER_0_PKTIN_DEFAULT 0x0
#define SUME_INPUT_DRR_ARBITER_0_PKTIN_WIDTH 32
#define SUME_INPUT_DRR_ARBITER_0_PKTOUT 0x44010018
#define SUME_INPUT_DRR_ARBITER_0_PKTOUT_DEFAULT 0x0
#define SUME_INPUT_DRR_ARBITER_0_PKTOUT_WIDTH 32

//######################################################
//# Definitions for OUTPUT_QUEUES
//######################################################
#define SUME_OUTPUT_QUEUES_BASEADDR 0x44030000
#define SUME_OUTPUT_QUEUES_HIGHADDR 0x44030FFF
#define SUME_OUTPUT_QUEUES_SIZEADDR 0x1000

#define SUME_OUTPUT_QUEUES_0_ID 0x44030000
#define SUME_OUTPUT_QUEUES_0_ID_DEFAULT 0x0000DA03
#define SUME_OUTPUT_QUEUES_0_ID_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_VERSION 0x44030004
#define SUME_OUTPUT_QUEUES_0_VERSION_DEFAULT 0x1
#define SUME_OUTPUT_QUEUES_0_VERSION_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_RESET 0x44030008
#define SUME_OUTPUT_QUEUES_0_RESET_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_RESET_WIDTH 16
#define SUME_OUTPUT_QUEUES_0_FLIP 0x4403000c
#define SUME_OUTPUT_QUEUES_0_FLIP_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_FLIP_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_DEBUG 0x44030010
#define SUME_OUTPUT_QUEUES_0_DEBUG_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_DEBUG_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTIN 0x44030014
#define SUME_OUTPUT_QUEUES_0_PKTIN_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTIN_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTOUT 0x44030018
#define SUME_OUTPUT_QUEUES_0_PKTOUT_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTOUT_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTSTOREDPORT0 0x4403001c
#define SUME_OUTPUT_QUEUES_0_PKTSTOREDPORT0_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTSTOREDPORT0_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_BYTESSTOREDPORT0 0x44030020
#define SUME_OUTPUT_QUEUES_0_BYTESSTOREDPORT0_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_BYTESSTOREDPORT0_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTREMOVEDPORT0 0x44030024
#define SUME_OUTPUT_QUEUES_0_PKTREMOVEDPORT0_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTREMOVEDPORT0_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_BYTESREMOVEDPORT0 0x44030028
#define SUME_OUTPUT_QUEUES_0_BYTESREMOVEDPORT0_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_BYTESREMOVEDPORT0_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTDROPPEDPORT0 0x4403002c
#define SUME_OUTPUT_QUEUES_0_PKTDROPPEDPORT0_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTDROPPEDPORT0_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_BYTESDROPPEDPORT0 0x44030030
#define SUME_OUTPUT_QUEUES_0_BYTESDROPPEDPORT0_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_BYTESDROPPEDPORT0_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTINQUEUEPORT0 0x44030034
#define SUME_OUTPUT_QUEUES_0_PKTINQUEUEPORT0_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTINQUEUEPORT0_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTSTOREDPORT1 0x44030038
#define SUME_OUTPUT_QUEUES_0_PKTSTOREDPORT1_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTSTOREDPORT1_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_BYTESSTOREDPORT1 0x4403003c
#define SUME_OUTPUT_QUEUES_0_BYTESSTOREDPORT1_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_BYTESSTOREDPORT1_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTREMOVEDPORT1 0x44030040
#define SUME_OUTPUT_QUEUES_0_PKTREMOVEDPORT1_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTREMOVEDPORT1_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_BYTESREMOVEDPORT1 0x44030044
#define SUME_OUTPUT_QUEUES_0_BYTESREMOVEDPORT1_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_BYTESREMOVEDPORT1_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTDROPPEDPORT1 0x44030048
#define SUME_OUTPUT_QUEUES_0_PKTDROPPEDPORT1_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTDROPPEDPORT1_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_BYTESDROPPEDPORT1 0x4403004c
#define SUME_OUTPUT_QUEUES_0_BYTESDROPPEDPORT1_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_BYTESDROPPEDPORT1_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTINQUEUEPORT1 0x44030050
#define SUME_OUTPUT_QUEUES_0_PKTINQUEUEPORT1_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTINQUEUEPORT1_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTSTOREDPORT2 0x44030054
#define SUME_OUTPUT_QUEUES_0_PKTSTOREDPORT2_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTSTOREDPORT2_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_BYTESSTOREDPORT2 0x44030058
#define SUME_OUTPUT_QUEUES_0_BYTESSTOREDPORT2_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_BYTESSTOREDPORT2_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTREMOVEDPORT2 0x4403005c
#define SUME_OUTPUT_QUEUES_0_PKTREMOVEDPORT2_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTREMOVEDPORT2_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_BYTESREMOVEDPORT2 0x44030060
#define SUME_OUTPUT_QUEUES_0_BYTESREMOVEDPORT2_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_BYTESREMOVEDPORT2_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTDROPPEDPORT2 0x44030064
#define SUME_OUTPUT_QUEUES_0_PKTDROPPEDPORT2_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTDROPPEDPORT2_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_BYTESDROPPEDPORT2 0x44030068
#define SUME_OUTPUT_QUEUES_0_BYTESDROPPEDPORT2_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_BYTESDROPPEDPORT2_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTINQUEUEPORT2 0x4403006c
#define SUME_OUTPUT_QUEUES_0_PKTINQUEUEPORT2_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTINQUEUEPORT2_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTSTOREDPORT3 0x44030070
#define SUME_OUTPUT_QUEUES_0_PKTSTOREDPORT3_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTSTOREDPORT3_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_BYTESSTOREDPORT3 0x44030074
#define SUME_OUTPUT_QUEUES_0_BYTESSTOREDPORT3_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_BYTESSTOREDPORT3_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTREMOVEDPORT3 0x44030078
#define SUME_OUTPUT_QUEUES_0_PKTREMOVEDPORT3_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTREMOVEDPORT3_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_BYTESREMOVEDPORT3 0x4403007c
#define SUME_OUTPUT_QUEUES_0_BYTESREMOVEDPORT3_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_BYTESREMOVEDPORT3_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTDROPPEDPORT3 0x44030080
#define SUME_OUTPUT_QUEUES_0_PKTDROPPEDPORT3_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTDROPPEDPORT3_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_BYTESDROPPEDPORT3 0x44030084
#define SUME_OUTPUT_QUEUES_0_BYTESDROPPEDPORT3_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_BYTESDROPPEDPORT3_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTINQUEUEPORT3 0x44030088
#define SUME_OUTPUT_QUEUES_0_PKTINQUEUEPORT3_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTINQUEUEPORT3_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTSTOREDPORT4 0x4403008c
#define SUME_OUTPUT_QUEUES_0_PKTSTOREDPORT4_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTSTOREDPORT4_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_BYTESSTOREDPORT4 0x44030090
#define SUME_OUTPUT_QUEUES_0_BYTESSTOREDPORT4_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_BYTESSTOREDPORT4_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTREMOVEDPORT4 0x44030094
#define SUME_OUTPUT_QUEUES_0_PKTREMOVEDPORT4_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTREMOVEDPORT4_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_BYTESREMOVEDPORT4 0x44030098
#define SUME_OUTPUT_QUEUES_0_BYTESREMOVEDPORT4_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_BYTESREMOVEDPORT4_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTDROPPEDPORT4 0x4403009c
#define SUME_OUTPUT_QUEUES_0_PKTDROPPEDPORT4_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTDROPPEDPORT4_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_BYTESDROPPEDPORT4 0x440300a0
#define SUME_OUTPUT_QUEUES_0_BYTESDROPPEDPORT4_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_BYTESDROPPEDPORT4_WIDTH 32
#define SUME_OUTPUT_QUEUES_0_PKTINQUEUEPORT4 0x440300a4
#define SUME_OUTPUT_QUEUES_0_PKTINQUEUEPORT4_DEFAULT 0x0
#define SUME_OUTPUT_QUEUES_0_PKTINQUEUEPORT4_WIDTH 32

//######################################################
//# Definitions for OUTPUT_PORT_LOOKUP
//######################################################
#define SUME_OUTPUT_PORT_LOOKUP_BASEADDR 0x44020000
#define SUME_OUTPUT_PORT_LOOKUP_HIGHADDR 0x44020FFF
#define SUME_OUTPUT_PORT_LOOKUP_SIZEADDR 0x1000

#define SUME_OUTPUT_PORT_LOOKUP_0_ID 0x44020000
#define SUME_OUTPUT_PORT_LOOKUP_0_ID_DEFAULT 0x0001DA02
#define SUME_OUTPUT_PORT_LOOKUP_0_ID_WIDTH 32
#define SUME_OUTPUT_PORT_LOOKUP_0_VERSION 0x44020004
#define SUME_OUTPUT_PORT_LOOKUP_0_VERSION_DEFAULT 0x1
#define SUME_OUTPUT_PORT_LOOKUP_0_VERSION_WIDTH 32
#define SUME_OUTPUT_PORT_LOOKUP_0_RESET 0x44020008
#define SUME_OUTPUT_PORT_LOOKUP_0_RESET_DEFAULT 0x0
#define SUME_OUTPUT_PORT_LOOKUP_0_RESET_WIDTH 16
#define SUME_OUTPUT_PORT_LOOKUP_0_FLIP 0x4402000c
#define SUME_OUTPUT_PORT_LOOKUP_0_FLIP_DEFAULT 0x0
#define SUME_OUTPUT_PORT_LOOKUP_0_FLIP_WIDTH 32
#define SUME_OUTPUT_PORT_LOOKUP_0_DEBUG 0x44020010
#define SUME_OUTPUT_PORT_LOOKUP_0_DEBUG_DEFAULT 0x0
#define SUME_OUTPUT_PORT_LOOKUP_0_DEBUG_WIDTH 32
#define SUME_OUTPUT_PORT_LOOKUP_0_PKTIN 0x44020014
#define SUME_OUTPUT_PORT_LOOKUP_0_PKTIN_DEFAULT 0x0
#define SUME_OUTPUT_PORT_LOOKUP_0_PKTIN_WIDTH 32
#define SUME_OUTPUT_PORT_LOOKUP_0_PKTOUT 0x44020018
#define SUME_OUTPUT_PORT_LOOKUP_0_PKTOUT_DEFAULT 0x0
#define SUME_OUTPUT_PORT_LOOKUP_0_PKTOUT_WIDTH 32
#define SUME_OUTPUT_PORT_LOOKUP_0_LUTHIT 0x4402001c
#define SUME_OUTPUT_PORT_LOOKUP_0_LUTHIT_DEFAULT 0x0
#define SUME_OUTPUT_PORT_LOOKUP_0_LUTHIT_WIDTH 32
#define SUME_OUTPUT_PORT_LOOKUP_0_LUTMISS 0x44020020
#define SUME_OUTPUT_PORT_LOOKUP_0_LUTMISS_DEFAULT 0x0
#define SUME_OUTPUT_PORT_LOOKUP_0_LUTMISS_WIDTH 32

//######################################################
//# Definitions for NF_10G_INTERFACE0
//######################################################
#define SUME_NF_10G_INTERFACE0_BASEADDR 0x44040000
#define SUME_NF_10G_INTERFACE0_HIGHADDR 0x44040FFF
#define SUME_NF_10G_INTERFACE0_SIZEADDR 0x1000

#define SUME_NF_10G_INTERFACE_SHARED_0_ID 0x44040000
#define SUME_NF_10G_INTERFACE_SHARED_0_ID_DEFAULT 0x00001F10
#define SUME_NF_10G_INTERFACE_SHARED_0_ID_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_VERSION 0x44040004
#define SUME_NF_10G_INTERFACE_SHARED_0_VERSION_DEFAULT 0x1
#define SUME_NF_10G_INTERFACE_SHARED_0_VERSION_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_RESET 0x44040008
#define SUME_NF_10G_INTERFACE_SHARED_0_RESET_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_RESET_WIDTH 16
#define SUME_NF_10G_INTERFACE_SHARED_0_FLIP 0x4404000c
#define SUME_NF_10G_INTERFACE_SHARED_0_FLIP_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_FLIP_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_DEBUG 0x44040010
#define SUME_NF_10G_INTERFACE_SHARED_0_DEBUG_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_DEBUG_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_INTERFACEID 0x44040014
#define SUME_NF_10G_INTERFACE_SHARED_0_INTERFACEID_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_INTERFACEID_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_PKTIN 0x44040018
#define SUME_NF_10G_INTERFACE_SHARED_0_PKTIN_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PKTIN_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_PKTOUT 0x4404001c
#define SUME_NF_10G_INTERFACE_SHARED_0_PKTOUT_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PKTOUT_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_MACSTATUSVECTOR 0x44040020
#define SUME_NF_10G_INTERFACE_SHARED_0_MACSTATUSVECTOR_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_MACSTATUSVECTOR_WIDTH 2
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUS 0x44040024
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUS_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUS_WIDTH 8
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR0 0x44040028
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR0_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR0_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR1 0x4404002c
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR1_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR1_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR2 0x44040030
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR2_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR2_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR3 0x44040034
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR3_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR3_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR4 0x44040038
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR4_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR4_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR5 0x4404003c
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR5_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR5_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR6 0x44040040
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR6_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR6_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR7 0x44040044
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR7_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR7_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR8 0x44040048
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR8_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR8_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR9 0x4404004c
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR9_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR9_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR10 0x44040050
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR10_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR10_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR11 0x44040054
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR11_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR11_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR12 0x44040058
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR12_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR12_WIDTH 32
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR13 0x4404005c
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR13_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_SHARED_0_PCSPMASTATUSVECTOR13_WIDTH 32

//######################################################
//# Definitions for NF_10G_INTERFACE1
//######################################################
#define SUME_NF_10G_INTERFACE1_BASEADDR 0x44050000
#define SUME_NF_10G_INTERFACE1_HIGHADDR 0x44050FFF
#define SUME_NF_10G_INTERFACE1_SIZEADDR 0x1000

#define SUME_NF_10G_INTERFACE_1_ID 0x44050000
#define SUME_NF_10G_INTERFACE_1_ID_DEFAULT 0x00001F10
#define SUME_NF_10G_INTERFACE_1_ID_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_VERSION 0x44050004
#define SUME_NF_10G_INTERFACE_1_VERSION_DEFAULT 0x1
#define SUME_NF_10G_INTERFACE_1_VERSION_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_RESET 0x44050008
#define SUME_NF_10G_INTERFACE_1_RESET_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_RESET_WIDTH 16
#define SUME_NF_10G_INTERFACE_1_FLIP 0x4405000c
#define SUME_NF_10G_INTERFACE_1_FLIP_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_FLIP_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_DEBUG 0x44050010
#define SUME_NF_10G_INTERFACE_1_DEBUG_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_DEBUG_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_INTERFACEID 0x44050014
#define SUME_NF_10G_INTERFACE_1_INTERFACEID_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_INTERFACEID_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_PKTIN 0x44050018
#define SUME_NF_10G_INTERFACE_1_PKTIN_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PKTIN_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_PKTOUT 0x4405001c
#define SUME_NF_10G_INTERFACE_1_PKTOUT_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PKTOUT_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_MACSTATUSVECTOR 0x44050020
#define SUME_NF_10G_INTERFACE_1_MACSTATUSVECTOR_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_MACSTATUSVECTOR_WIDTH 2
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUS 0x44050024
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUS_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUS_WIDTH 8
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR0 0x44050028
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR0_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR0_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR1 0x4405002c
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR1_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR1_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR2 0x44050030
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR2_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR2_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR3 0x44050034
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR3_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR3_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR4 0x44050038
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR4_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR4_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR5 0x4405003c
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR5_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR5_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR6 0x44050040
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR6_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR6_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR7 0x44050044
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR7_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR7_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR8 0x44050048
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR8_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR8_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR9 0x4405004c
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR9_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR9_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR10 0x44050050
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR10_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR10_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR11 0x44050054
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR11_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR11_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR12 0x44050058
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR12_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR12_WIDTH 32
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR13 0x4405005c
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR13_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_1_PCSPMASTATUSVECTOR13_WIDTH 32

//######################################################
//# Definitions for NF_10G_INTERFACE2
//######################################################
#define SUME_NF_10G_INTERFACE2_BASEADDR 0x44060000
#define SUME_NF_10G_INTERFACE2_HIGHADDR 0x44060FFF
#define SUME_NF_10G_INTERFACE2_SIZEADDR 0x1000

#define SUME_NF_10G_INTERFACE_2_ID 0x44060000
#define SUME_NF_10G_INTERFACE_2_ID_DEFAULT 0x00001F10
#define SUME_NF_10G_INTERFACE_2_ID_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_VERSION 0x44060004
#define SUME_NF_10G_INTERFACE_2_VERSION_DEFAULT 0x1
#define SUME_NF_10G_INTERFACE_2_VERSION_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_RESET 0x44060008
#define SUME_NF_10G_INTERFACE_2_RESET_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_RESET_WIDTH 16
#define SUME_NF_10G_INTERFACE_2_FLIP 0x4406000c
#define SUME_NF_10G_INTERFACE_2_FLIP_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_FLIP_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_DEBUG 0x44060010
#define SUME_NF_10G_INTERFACE_2_DEBUG_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_DEBUG_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_INTERFACEID 0x44060014
#define SUME_NF_10G_INTERFACE_2_INTERFACEID_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_INTERFACEID_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_PKTIN 0x44060018
#define SUME_NF_10G_INTERFACE_2_PKTIN_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PKTIN_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_PKTOUT 0x4406001c
#define SUME_NF_10G_INTERFACE_2_PKTOUT_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PKTOUT_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_MACSTATUSVECTOR 0x44060020
#define SUME_NF_10G_INTERFACE_2_MACSTATUSVECTOR_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_MACSTATUSVECTOR_WIDTH 2
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUS 0x44060024
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUS_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUS_WIDTH 8
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR0 0x44060028
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR0_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR0_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR1 0x4406002c
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR1_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR1_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR2 0x44060030
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR2_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR2_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR3 0x44060034
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR3_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR3_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR4 0x44060038
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR4_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR4_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR5 0x4406003c
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR5_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR5_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR6 0x44060040
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR6_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR6_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR7 0x44060044
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR7_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR7_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR8 0x44060048
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR8_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR8_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR9 0x4406004c
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR9_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR9_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR10 0x44060050
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR10_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR10_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR11 0x44060054
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR11_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR11_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR12 0x44060058
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR12_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR12_WIDTH 32
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR13 0x4406005c
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR13_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_2_PCSPMASTATUSVECTOR13_WIDTH 32

//######################################################
//# Definitions for NF_10G_INTERFACE3
//######################################################
#define SUME_NF_10G_INTERFACE3_BASEADDR 0x44070000
#define SUME_NF_10G_INTERFACE3_HIGHADDR 0x44070FFF
#define SUME_NF_10G_INTERFACE3_SIZEADDR 0x1000

#define SUME_NF_10G_INTERFACE_3_ID 0x44070000
#define SUME_NF_10G_INTERFACE_3_ID_DEFAULT 0x00001F10
#define SUME_NF_10G_INTERFACE_3_ID_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_VERSION 0x44070004
#define SUME_NF_10G_INTERFACE_3_VERSION_DEFAULT 0x1
#define SUME_NF_10G_INTERFACE_3_VERSION_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_RESET 0x44070008
#define SUME_NF_10G_INTERFACE_3_RESET_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_RESET_WIDTH 16
#define SUME_NF_10G_INTERFACE_3_FLIP 0x4407000c
#define SUME_NF_10G_INTERFACE_3_FLIP_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_FLIP_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_DEBUG 0x44070010
#define SUME_NF_10G_INTERFACE_3_DEBUG_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_DEBUG_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_INTERFACEID 0x44070014
#define SUME_NF_10G_INTERFACE_3_INTERFACEID_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_INTERFACEID_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_PKTIN 0x44070018
#define SUME_NF_10G_INTERFACE_3_PKTIN_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PKTIN_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_PKTOUT 0x4407001c
#define SUME_NF_10G_INTERFACE_3_PKTOUT_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PKTOUT_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_MACSTATUSVECTOR 0x44070020
#define SUME_NF_10G_INTERFACE_3_MACSTATUSVECTOR_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_MACSTATUSVECTOR_WIDTH 2
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUS 0x44070024
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUS_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUS_WIDTH 8
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR0 0x44070028
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR0_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR0_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR1 0x4407002c
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR1_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR1_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR2 0x44070030
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR2_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR2_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR3 0x44070034
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR3_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR3_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR4 0x44070038
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR4_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR4_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR5 0x4407003c
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR5_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR5_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR6 0x44070040
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR6_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR6_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR7 0x44070044
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR7_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR7_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR8 0x44070048
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR8_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR8_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR9 0x4407004c
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR9_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR9_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR10 0x44070050
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR10_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR10_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR11 0x44070054
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR11_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR11_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR12 0x44070058
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR12_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR12_WIDTH 32
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR13 0x4407005c
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR13_DEFAULT 0x0
#define SUME_NF_10G_INTERFACE_3_PCSPMASTATUSVECTOR13_WIDTH 32

//######################################################
//# Definitions for NF_RIFFA_DMA
//######################################################
#define SUME_NF_RIFFA_DMA_BASEADDR 0x44080000
#define SUME_NF_RIFFA_DMA_HIGHADDR 0x44080FFF
#define SUME_NF_RIFFA_DMA_SIZEADDR 0x1000

#define SUME_NF_RIFFA_DMA_0_ID 0x44080000
#define SUME_NF_RIFFA_DMA_0_ID_DEFAULT 0x00001FFA
#define SUME_NF_RIFFA_DMA_0_ID_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_VERSION 0x44080004
#define SUME_NF_RIFFA_DMA_0_VERSION_DEFAULT 0x1
#define SUME_NF_RIFFA_DMA_0_VERSION_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_RESET 0x44080008
#define SUME_NF_RIFFA_DMA_0_RESET_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_RESET_WIDTH 16
#define SUME_NF_RIFFA_DMA_0_FLIP 0x4408000c
#define SUME_NF_RIFFA_DMA_0_FLIP_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_FLIP_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_DEBUG 0x44080010
#define SUME_NF_RIFFA_DMA_0_DEBUG_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_DEBUG_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_RQPKT 0x44080014
#define SUME_NF_RIFFA_DMA_0_RQPKT_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_RQPKT_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_RCPKT 0x44080018
#define SUME_NF_RIFFA_DMA_0_RCPKT_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_RCPKT_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_CQPKT 0x4408001c
#define SUME_NF_RIFFA_DMA_0_CQPKT_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_CQPKT_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_CCPKT 0x44080020
#define SUME_NF_RIFFA_DMA_0_CCPKT_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_CCPKT_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_XGETXPKT 0x44080024
#define SUME_NF_RIFFA_DMA_0_XGETXPKT_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_XGETXPKT_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_XGERXPKT 0x44080028
#define SUME_NF_RIFFA_DMA_0_XGERXPKT_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_XGERXPKT_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_PCIERQ 0x4408002c
#define SUME_NF_RIFFA_DMA_0_PCIERQ_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_PCIERQ_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_PCIEPHY 0x44080030
#define SUME_NF_RIFFA_DMA_0_PCIEPHY_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_PCIEPHY_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_PCIECONFIG 0x44080034
#define SUME_NF_RIFFA_DMA_0_PCIECONFIG_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_PCIECONFIG_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_PCIECONFIG2 0x44080038
#define SUME_NF_RIFFA_DMA_0_PCIECONFIG2_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_PCIECONFIG2_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_PCIEERROR 0x4408003c
#define SUME_NF_RIFFA_DMA_0_PCIEERROR_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_PCIEERROR_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_PCIEMISC 0x44080040
#define SUME_NF_RIFFA_DMA_0_PCIEMISC_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_PCIEMISC_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_PCIETPH 0x44080044
#define SUME_NF_RIFFA_DMA_0_PCIETPH_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_PCIETPH_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_PCIEFC1 0x44080048
#define SUME_NF_RIFFA_DMA_0_PCIEFC1_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_PCIEFC1_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_PCIEFC2 0x4408004c
#define SUME_NF_RIFFA_DMA_0_PCIEFC2_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_PCIEFC2_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_PCIEFC3 0x44080050
#define SUME_NF_RIFFA_DMA_0_PCIEFC3_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_PCIEFC3_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_PCIEINTERRUPT 0x44080054
#define SUME_NF_RIFFA_DMA_0_PCIEINTERRUPT_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_PCIEINTERRUPT_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_PCIEMSIDATA 0x44080058
#define SUME_NF_RIFFA_DMA_0_PCIEMSIDATA_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_PCIEMSIDATA_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_PCIEMSIINT 0x4408005c
#define SUME_NF_RIFFA_DMA_0_PCIEMSIINT_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_PCIEMSIINT_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_PCIEMSIPENDINGSTATUS 0x44080060
#define SUME_NF_RIFFA_DMA_0_PCIEMSIPENDINGSTATUS_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_PCIEMSIPENDINGSTATUS_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_PCIEMSIPENDINGSTATUS2 0x44080064
#define SUME_NF_RIFFA_DMA_0_PCIEMSIPENDINGSTATUS2_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_PCIEMSIPENDINGSTATUS2_WIDTH 32
#define SUME_NF_RIFFA_DMA_0_PCIEINTERRUPT2 0x44080068
#define SUME_NF_RIFFA_DMA_0_PCIEINTERRUPT2_DEFAULT 0x0
#define SUME_NF_RIFFA_DMA_0_PCIEINTERRUPT2_WIDTH 32

