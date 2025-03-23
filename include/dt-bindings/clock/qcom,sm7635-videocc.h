/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2023, Qualcomm Innovation Center, Inc. All rights reserved.
 * Copyright (c) 2025, Danila Tikhonov <danila@jiaxyga.com>
 */

#ifndef _DT_BINDINGS_CLK_QCOM_VIDEOCC_SM7635H
#define _DT_BINDINGS_CLK_QCOM_VIDEOCC_SM7635H

/* VIDEOCC clocks */
#define VIDEOCC_PLL0			0
#define VIDEOCC_AHB_CLK			1
#define VIDEOCC_AHB_CLK_SRC		2
#define VIDEOCC_MVS0_CLK		3
#define VIDEOCC_MVS0_CLK_SRC		4
#define VIDEOCC_MVS0_DIV_CLK_SRC	5
#define VIDEOCC_MVS0_SHIFT_CLK		6
#define VIDEOCC_MVS0C_CLK		7
#define VIDEOCC_MVS0C_DIV2_DIV_CLK_SRC  8
#define VIDEOCC_MVS0C_SHIFT_CLK		9
#define VIDEOCC_SLEEP_CLK		10
#define VIDEOCC_SLEEP_CLK_SRC		11
#define VIDEOCC_XO_CLK			12
#define VIDEOCC_XO_CLK_SRC		13

/* VIDEOCC resets */
#define VIDEOCC_INTERFACE_BCR		0
#define VIDEOCC_MVS0_BCR		1
#define VIDEOCC_MVS0C_CLK_ARES		2
#define VIDEOCC_MVS0C_BCR		3

#endif
