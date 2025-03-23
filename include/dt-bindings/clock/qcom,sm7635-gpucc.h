/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2023, Qualcomm Innovation Center, Inc. All rights reserved.
 * Copyright (c) 2025, Danila Tikhonov <danila@jiaxyga.com>
 */

#ifndef _DT_BINDINGS_CLK_QCOM_GPUCC_SM7635H
#define _DT_BINDINGS_CLK_QCOM_GPUCC_SM7635H

/* GPUCC clocks */
#define GPUCC_PLL0			0
#define GPUCC_PLL0_OUT_EVEN		1
#define GPUCC_AHB_CLK			2
#define GPUCC_CB_CLK			3
#define GPUCC_CX_ACCU_SHIFT_CLK		4
#define GPUCC_CX_FF_CLK			5
#define GPUCC_CX_GMU_CLK		6
#define GPUCC_CXO_AON_CLK		7
#define GPUCC_CXO_CLK			8
#define GPUCC_DEMET_CLK			9
#define GPUCC_DEMET_DIV_CLK_SRC		10
#define GPUCC_DPM_CLK			11
#define GPUCC_FF_CLK_SRC		12
#define GPUCC_FREQ_MEASURE_CLK		13
#define GPUCC_GMU_CLK_SRC		14
#define GPUCC_GX_ACCU_SHIFT_CLK		15
#define GPUCC_GX_ACD_AHB_FF_CLK		16
#define GPUCC_GX_AHB_FF_CLK		17
#define GPUCC_GX_GMU_CLK		18
#define GPUCC_GX_RCG_AHB_FF_CLK		19
#define GPUCC_HLOS1_VOTE_GPU_SMMU_CLK	20
#define GPUCC_HUB_AON_CLK		21
#define GPUCC_HUB_CLK_SRC		22
#define GPUCC_HUB_CX_INT_CLK		23
#define GPUCC_HUB_DIV_CLK_SRC		24
#define GPUCC_MEMNOC_GFX_CLK		25
#define GPUCC_RSCC_HUB_AON_CLK		26
#define GPUCC_RSCC_XO_AON_CLK		27
#define GPUCC_SLEEP_CLK			28
#define GPUCC_XO_CLK_SRC		29
#define GPUCC_XO_DIV_CLK_SRC		30

/* GPUCC resets */
#define GPUCC_CB_BCR			0
#define GPUCC_CX_BCR			1
#define GPUCC_FAST_HUB_BCR		2
#define GPUCC_FF_BCR			3
#define GPUCC_GMU_BCR			4
#define GPUCC_GX_BCR			5
#define GPUCC_RBCPR_BCR			6
#define GPUCC_XO_BCR			7

#endif
