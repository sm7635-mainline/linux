// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2024, Qualcomm Innovation Center, Inc. All rights reserved.
 * Copyright (c) 2025, Danila Tikhonov <danila@jiaxyga.com>
 */

#include <linux/clk-provider.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>

#include <dt-bindings/clock/qcom,sm7635-camcc.h>

#include "clk-alpha-pll.h"
#include "clk-branch.h"
#include "clk-rcg.h"
#include "clk-regmap.h"
#include "common.h"
#include "gdsc.h"
#include "reset.h"

enum {
	DT_BI_TCXO,
	DT_SLEEP_CLK,
};

enum {
	P_BI_TCXO,
	P_CAMCC_PLL0_OUT_EVEN,
	P_CAMCC_PLL0_OUT_MAIN,
	P_CAMCC_PLL0_OUT_ODD,
	P_CAMCC_PLL1_OUT_EVEN,
	P_CAMCC_PLL1_OUT_MAIN,
	P_CAMCC_PLL2_OUT_MAIN,
	P_CAMCC_PLL3_OUT_EVEN,
	P_CAMCC_PLL4_OUT_EVEN,
	P_CAMCC_PLL4_OUT_MAIN,
	P_CAMCC_PLL5_OUT_EVEN,
	P_CAMCC_PLL5_OUT_MAIN,
	P_CAMCC_PLL6_OUT_EVEN,
	P_CAMCC_PLL6_OUT_MAIN,
	P_SLEEP_CLK,
};

static const struct pll_vco lucid_ole_vco[] = {
	{ 249600000, 2300000000, 0 },
};

static const struct pll_vco rivian_ole_vco[] = {
	{ 777000000, 1285000000, 0 },
};

/* 1200.0 MHz Configuration */
static const struct alpha_pll_config camcc_pll0_config = {
	.l = 0x3e,
	.alpha = 0x8000,
	.config_ctl_val = 0x20485699,
	.config_ctl_hi_val = 0x00182261,
	.config_ctl_hi1_val = 0x82aa299c,
	.test_ctl_val = 0x00000000,
	.test_ctl_hi_val = 0x00000003,
	.test_ctl_hi1_val = 0x00009000,
	.test_ctl_hi2_val = 0x00000034,
	.user_ctl_val = 0x00008400,
	.user_ctl_hi_val = 0x00000005,
};

static struct clk_alpha_pll camcc_pll0 = {
	.offset = 0x0,
	.vco_table = lucid_ole_vco,
	.num_vco = ARRAY_SIZE(lucid_ole_vco),
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_LUCID_OLE],
	.clkr = {
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_pll0",
			.parent_data = &(const struct clk_parent_data) {
				.index = DT_BI_TCXO,
			},
			.num_parents = 1,
			.ops = &clk_alpha_pll_lucid_evo_ops,
		},
	},
};

static const struct clk_div_table post_div_table_camcc_pll0_out_even[] = {
	{ 0x1, 2 },
	{ }
};

static struct clk_alpha_pll_postdiv camcc_pll0_out_even = {
	.offset = 0x0,
	.post_div_shift = 10,
	.post_div_table = post_div_table_camcc_pll0_out_even,
	.num_post_div = ARRAY_SIZE(post_div_table_camcc_pll0_out_even),
	.width = 4,
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_LUCID_OLE],
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_pll0_out_even",
		.parent_hws = (const struct clk_hw*[]) {
			&camcc_pll0.clkr.hw,
		},
		.num_parents = 1,
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_alpha_pll_postdiv_lucid_ole_ops,
	},
};

static const struct clk_div_table post_div_table_camcc_pll0_out_odd[] = {
	{ 0x2, 3 },
	{ }
};

static struct clk_alpha_pll_postdiv camcc_pll0_out_odd = {
	.offset = 0x0,
	.post_div_shift = 14,
	.post_div_table = post_div_table_camcc_pll0_out_odd,
	.num_post_div = ARRAY_SIZE(post_div_table_camcc_pll0_out_odd),
	.width = 4,
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_LUCID_OLE],
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_pll0_out_odd",
		.parent_hws = (const struct clk_hw*[]) {
			&camcc_pll0.clkr.hw,
		},
		.num_parents = 1,
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_alpha_pll_postdiv_lucid_ole_ops,
	},
};

/* 600.0 MHz Configuration */
static const struct alpha_pll_config camcc_pll1_config = {
	.l = 0x1f,
	.alpha = 0x4000,
	.config_ctl_val = 0x20485699,
	.config_ctl_hi_val = 0x00182261,
	.config_ctl_hi1_val = 0x82aa299c,
	.test_ctl_val = 0x00000000,
	.test_ctl_hi_val = 0x00000003,
	.test_ctl_hi1_val = 0x00009000,
	.test_ctl_hi2_val = 0x00000034,
	.user_ctl_val = 0x00000400,
	.user_ctl_hi_val = 0x00000005,
};

static struct clk_alpha_pll camcc_pll1 = {
	.offset = 0x1000,
	.vco_table = lucid_ole_vco,
	.num_vco = ARRAY_SIZE(lucid_ole_vco),
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_LUCID_OLE],
	.clkr = {
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_pll1",
			.parent_data = &(const struct clk_parent_data) {
				.index = DT_BI_TCXO,
			},
			.num_parents = 1,
			.ops = &clk_alpha_pll_lucid_evo_ops,
		},
	},
};

static const struct clk_div_table post_div_table_camcc_pll1_out_even[] = {
	{ 0x1, 2 },
	{ }
};

static struct clk_alpha_pll_postdiv camcc_pll1_out_even = {
	.offset = 0x1000,
	.post_div_shift = 10,
	.post_div_table = post_div_table_camcc_pll1_out_even,
	.num_post_div = ARRAY_SIZE(post_div_table_camcc_pll1_out_even),
	.width = 4,
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_LUCID_OLE],
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_pll1_out_even",
		.parent_hws = (const struct clk_hw*[]) {
			&camcc_pll1.clkr.hw,
		},
		.num_parents = 1,
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_alpha_pll_postdiv_lucid_ole_ops,
	},
};

/* 960.0 MHz Configuration */
static const struct alpha_pll_config camcc_pll2_config = {
	.l = 0x32,
	.alpha = 0x0,
	.config_ctl_val = 0x10000030,
	.config_ctl_hi_val = 0x80890263,
	.config_ctl_hi1_val = 0x00000217,
	.user_ctl_val = 0x00000001,
	.user_ctl_hi_val = 0x00100000,
};

static struct clk_alpha_pll camcc_pll2 = {
	.offset = 0x2000,
	.vco_table = rivian_ole_vco,
	.num_vco = ARRAY_SIZE(rivian_ole_vco),
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_RIVIAN_EVO],
	.clkr = {
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_pll2",
			.parent_data = &(const struct clk_parent_data) {
				.index = DT_BI_TCXO,
			},
			.num_parents = 1,
			.ops = &clk_alpha_pll_rivian_evo_ops,
		},
	},
};

/* 600.0 MHz Configuration */
static const struct alpha_pll_config camcc_pll3_config = {
	.l = 0x1f,
	.alpha = 0x4000,
	.config_ctl_val = 0x20485699,
	.config_ctl_hi_val = 0x00182261,
	.config_ctl_hi1_val = 0x82aa299c,
	.test_ctl_val = 0x00000000,
	.test_ctl_hi_val = 0x00000003,
	.test_ctl_hi1_val = 0x00009000,
	.test_ctl_hi2_val = 0x00000034,
	.user_ctl_val = 0x00000400,
	.user_ctl_hi_val = 0x00000005,
};

static struct clk_alpha_pll camcc_pll3 = {
	.offset = 0x3000,
	.vco_table = lucid_ole_vco,
	.num_vco = ARRAY_SIZE(lucid_ole_vco),
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_LUCID_OLE],
	.clkr = {
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_pll3",
			.parent_data = &(const struct clk_parent_data) {
				.index = DT_BI_TCXO,
			},
			.num_parents = 1,
			.ops = &clk_alpha_pll_lucid_evo_ops,
		},
	},
};

static const struct clk_div_table post_div_table_camcc_pll3_out_even[] = {
	{ 0x1, 2 },
	{ }
};

static struct clk_alpha_pll_postdiv camcc_pll3_out_even = {
	.offset = 0x3000,
	.post_div_shift = 10,
	.post_div_table = post_div_table_camcc_pll3_out_even,
	.num_post_div = ARRAY_SIZE(post_div_table_camcc_pll3_out_even),
	.width = 4,
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_LUCID_OLE],
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_pll3_out_even",
		.parent_hws = (const struct clk_hw*[]) {
			&camcc_pll3.clkr.hw,
		},
		.num_parents = 1,
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_alpha_pll_postdiv_lucid_ole_ops,
	},
};

/* 700.0 MHz Configuration */
static const struct alpha_pll_config camcc_pll4_config = {
	.l = 0x24,
	.alpha = 0x7555,
	.config_ctl_val = 0x20485699,
	.config_ctl_hi_val = 0x00182261,
	.config_ctl_hi1_val = 0x82aa299c,
	.test_ctl_val = 0x00000000,
	.test_ctl_hi_val = 0x00000003,
	.test_ctl_hi1_val = 0x00009000,
	.test_ctl_hi2_val = 0x00000034,
	.user_ctl_val = 0x00000400,
	.user_ctl_hi_val = 0x00000005,
};

static struct clk_alpha_pll camcc_pll4 = {
	.offset = 0x4000,
	.vco_table = lucid_ole_vco,
	.num_vco = ARRAY_SIZE(lucid_ole_vco),
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_LUCID_OLE],
	.clkr = {
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_pll4",
			.parent_data = &(const struct clk_parent_data) {
				.index = DT_BI_TCXO,
			},
			.num_parents = 1,
			.ops = &clk_alpha_pll_lucid_evo_ops,
		},
	},
};

static const struct clk_div_table post_div_table_camcc_pll4_out_even[] = {
	{ 0x1, 2 },
	{ }
};

static struct clk_alpha_pll_postdiv camcc_pll4_out_even = {
	.offset = 0x4000,
	.post_div_shift = 10,
	.post_div_table = post_div_table_camcc_pll4_out_even,
	.num_post_div = ARRAY_SIZE(post_div_table_camcc_pll4_out_even),
	.width = 4,
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_LUCID_OLE],
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_pll4_out_even",
		.parent_hws = (const struct clk_hw*[]) {
			&camcc_pll4.clkr.hw,
		},
		.num_parents = 1,
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_alpha_pll_postdiv_lucid_ole_ops,
	},
};

/* 700.0 MHz Configuration */
static const struct alpha_pll_config camcc_pll5_config = {
	.l = 0x24,
	.alpha = 0x7555,
	.config_ctl_val = 0x20485699,
	.config_ctl_hi_val = 0x00182261,
	.config_ctl_hi1_val = 0x82aa299c,
	.test_ctl_val = 0x00000000,
	.test_ctl_hi_val = 0x00000003,
	.test_ctl_hi1_val = 0x00009000,
	.test_ctl_hi2_val = 0x00000034,
	.user_ctl_val = 0x00000400,
	.user_ctl_hi_val = 0x00000005,
};

static struct clk_alpha_pll camcc_pll5 = {
	.offset = 0x5000,
	.vco_table = lucid_ole_vco,
	.num_vco = ARRAY_SIZE(lucid_ole_vco),
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_LUCID_OLE],
	.clkr = {
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_pll5",
			.parent_data = &(const struct clk_parent_data) {
				.index = DT_BI_TCXO,
			},
			.num_parents = 1,
			.ops = &clk_alpha_pll_lucid_evo_ops,
		},
	},
};

static const struct clk_div_table post_div_table_camcc_pll5_out_even[] = {
	{ 0x1, 2 },
	{ }
};

static struct clk_alpha_pll_postdiv camcc_pll5_out_even = {
	.offset = 0x5000,
	.post_div_shift = 10,
	.post_div_table = post_div_table_camcc_pll5_out_even,
	.num_post_div = ARRAY_SIZE(post_div_table_camcc_pll5_out_even),
	.width = 4,
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_LUCID_OLE],
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_pll5_out_even",
		.parent_hws = (const struct clk_hw*[]) {
			&camcc_pll5.clkr.hw,
		},
		.num_parents = 1,
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_alpha_pll_postdiv_lucid_ole_ops,
	},
};

/* 700.0 MHz Configuration */
static const struct alpha_pll_config camcc_pll6_config = {
	.l = 0x24,
	.alpha = 0x7555,
	.config_ctl_val = 0x20485699,
	.config_ctl_hi_val = 0x00182261,
	.config_ctl_hi1_val = 0x82aa299c,
	.test_ctl_val = 0x00000000,
	.test_ctl_hi_val = 0x00000003,
	.test_ctl_hi1_val = 0x00009000,
	.test_ctl_hi2_val = 0x00000034,
	.user_ctl_val = 0x00000400,
	.user_ctl_hi_val = 0x00000005,
};

static struct clk_alpha_pll camcc_pll6 = {
	.offset = 0x6000,
	.vco_table = lucid_ole_vco,
	.num_vco = ARRAY_SIZE(lucid_ole_vco),
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_LUCID_OLE],
	.clkr = {
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_pll6",
			.parent_data = &(const struct clk_parent_data) {
				.index = DT_BI_TCXO,
			},
			.num_parents = 1,
			.ops = &clk_alpha_pll_lucid_evo_ops,
		},
	},
};

static const struct clk_div_table post_div_table_camcc_pll6_out_even[] = {
	{ 0x1, 2 },
	{ }
};

static struct clk_alpha_pll_postdiv camcc_pll6_out_even = {
	.offset = 0x6000,
	.post_div_shift = 10,
	.post_div_table = post_div_table_camcc_pll6_out_even,
	.num_post_div = ARRAY_SIZE(post_div_table_camcc_pll6_out_even),
	.width = 4,
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_LUCID_OLE],
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_pll6_out_even",
		.parent_hws = (const struct clk_hw*[]) {
			&camcc_pll6.clkr.hw,
		},
		.num_parents = 1,
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_alpha_pll_postdiv_lucid_ole_ops,
	},
};

static const struct parent_map camcc_parent_map_0[] = {
	{ P_BI_TCXO, 0 },
	{ P_CAMCC_PLL0_OUT_MAIN, 1 },
	{ P_CAMCC_PLL0_OUT_ODD, 5 },
	{ P_CAMCC_PLL0_OUT_EVEN, 6 },
};

static const struct clk_parent_data camcc_parent_data_0[] = {
	{ .index = DT_BI_TCXO },
	{ .hw = &camcc_pll0.clkr.hw },
	{ .hw = &camcc_pll0_out_odd.clkr.hw },
	{ .hw = &camcc_pll0_out_even.clkr.hw },
};

static const struct parent_map camcc_parent_map_1[] = {
	{ P_BI_TCXO, 0 },
	{ P_CAMCC_PLL2_OUT_MAIN, 4 },
};

static const struct clk_parent_data camcc_parent_data_1[] = {
	{ .index = DT_BI_TCXO },
	{ .hw = &camcc_pll2.clkr.hw },
};

static const struct parent_map camcc_parent_map_2[] = {
	{ P_BI_TCXO, 0 },
	{ P_CAMCC_PLL0_OUT_MAIN, 1 },
	{ P_CAMCC_PLL1_OUT_MAIN, 2 },
	{ P_CAMCC_PLL1_OUT_EVEN, 3 },
	{ P_CAMCC_PLL0_OUT_ODD, 5 },
	{ P_CAMCC_PLL0_OUT_EVEN, 6 },
};

static const struct clk_parent_data camcc_parent_data_2[] = {
	{ .index = DT_BI_TCXO },
	{ .hw = &camcc_pll0.clkr.hw },
	{ .hw = &camcc_pll1.clkr.hw },
	{ .hw = &camcc_pll1_out_even.clkr.hw },
	{ .hw = &camcc_pll0_out_odd.clkr.hw },
	{ .hw = &camcc_pll0_out_even.clkr.hw },
};

static const struct parent_map camcc_parent_map_3[] = {
	{ P_BI_TCXO, 0 },
	{ P_CAMCC_PLL0_OUT_ODD, 5 },
	{ P_CAMCC_PLL0_OUT_EVEN, 6 },
};

static const struct clk_parent_data camcc_parent_data_3[] = {
	{ .index = DT_BI_TCXO },
	{ .hw = &camcc_pll0_out_odd.clkr.hw },
	{ .hw = &camcc_pll0_out_even.clkr.hw },
};

static const struct parent_map camcc_parent_map_4[] = {
	{ P_BI_TCXO, 0 },
	{ P_CAMCC_PLL0_OUT_MAIN, 1 },
	{ P_CAMCC_PLL0_OUT_EVEN, 6 },
};

static const struct clk_parent_data camcc_parent_data_4[] = {
	{ .index = DT_BI_TCXO },
	{ .hw = &camcc_pll0.clkr.hw },
	{ .hw = &camcc_pll0_out_even.clkr.hw },
};

static const struct parent_map camcc_parent_map_5[] = {
	{ P_BI_TCXO, 0 },
	{ P_CAMCC_PLL0_OUT_MAIN, 1 },
	{ P_CAMCC_PLL3_OUT_EVEN, 5 },
	{ P_CAMCC_PLL0_OUT_EVEN, 6 },
};

static const struct clk_parent_data camcc_parent_data_5[] = {
	{ .index = DT_BI_TCXO },
	{ .hw = &camcc_pll0.clkr.hw },
	{ .hw = &camcc_pll3_out_even.clkr.hw },
	{ .hw = &camcc_pll0_out_even.clkr.hw },
};

static const struct parent_map camcc_parent_map_6[] = {
	{ P_SLEEP_CLK, 0 },
};

static const struct clk_parent_data camcc_parent_data_6_ao[] = {
	{ .index = DT_SLEEP_CLK },
};

static const struct parent_map camcc_parent_map_7[] = {
	{ P_BI_TCXO, 0 },
	{ P_CAMCC_PLL0_OUT_MAIN, 1 },
	{ P_CAMCC_PLL4_OUT_EVEN, 2 },
	{ P_CAMCC_PLL4_OUT_MAIN, 3 },
	{ P_CAMCC_PLL0_OUT_ODD, 5 },
	{ P_CAMCC_PLL0_OUT_EVEN, 6 },
};

static const struct clk_parent_data camcc_parent_data_7[] = {
	{ .index = DT_BI_TCXO },
	{ .hw = &camcc_pll0.clkr.hw },
	{ .hw = &camcc_pll4_out_even.clkr.hw },
	{ .hw = &camcc_pll4.clkr.hw },
	{ .hw = &camcc_pll0_out_odd.clkr.hw },
	{ .hw = &camcc_pll0_out_even.clkr.hw },
};

static const struct parent_map camcc_parent_map_8[] = {
	{ P_BI_TCXO, 0 },
	{ P_CAMCC_PLL0_OUT_MAIN, 1 },
	{ P_CAMCC_PLL5_OUT_EVEN, 2 },
	{ P_CAMCC_PLL5_OUT_MAIN, 3 },
	{ P_CAMCC_PLL0_OUT_ODD, 5 },
	{ P_CAMCC_PLL0_OUT_EVEN, 6 },
};

static const struct clk_parent_data camcc_parent_data_8[] = {
	{ .index = DT_BI_TCXO },
	{ .hw = &camcc_pll0.clkr.hw },
	{ .hw = &camcc_pll5_out_even.clkr.hw },
	{ .hw = &camcc_pll5.clkr.hw },
	{ .hw = &camcc_pll0_out_odd.clkr.hw },
	{ .hw = &camcc_pll0_out_even.clkr.hw },
};

static const struct parent_map camcc_parent_map_9[] = {
	{ P_BI_TCXO, 0 },
	{ P_CAMCC_PLL0_OUT_MAIN, 1 },
	{ P_CAMCC_PLL6_OUT_EVEN, 2 },
	{ P_CAMCC_PLL6_OUT_MAIN, 3 },
	{ P_CAMCC_PLL0_OUT_ODD, 5 },
	{ P_CAMCC_PLL0_OUT_EVEN, 6 },
};

static const struct clk_parent_data camcc_parent_data_9[] = {
	{ .index = DT_BI_TCXO },
	{ .hw = &camcc_pll0.clkr.hw },
	{ .hw = &camcc_pll6_out_even.clkr.hw },
	{ .hw = &camcc_pll6.clkr.hw },
	{ .hw = &camcc_pll0_out_odd.clkr.hw },
	{ .hw = &camcc_pll0_out_even.clkr.hw },
};

static const struct parent_map camcc_parent_map_10[] = {
	{ P_BI_TCXO, 0 },
};

static const struct clk_parent_data camcc_parent_data_10[] = {
	{ .index = DT_BI_TCXO },
};

static const struct freq_tbl ftbl_camcc_bps_clk_src[] = {
	F(300000000, P_CAMCC_PLL1_OUT_EVEN, 1, 0, 0),
	F(410000000, P_CAMCC_PLL1_OUT_EVEN, 1, 0, 0),
	F(460000000, P_CAMCC_PLL1_OUT_EVEN, 1, 0, 0),
	F(600000000, P_CAMCC_PLL1_OUT_EVEN, 1, 0, 0),
	F(700000000, P_CAMCC_PLL1_OUT_EVEN, 1, 0, 0),
	{ }
};

static struct clk_rcg2 camcc_bps_clk_src = {
	.cmd_rcgr = 0x1a004,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_2,
	.freq_tbl = ftbl_camcc_bps_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_bps_clk_src",
		.parent_data = camcc_parent_data_2,
		.num_parents = ARRAY_SIZE(camcc_parent_data_2),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_camcc_camnoc_axi_clk_src[] = {
	F(150000000, P_CAMCC_PLL0_OUT_EVEN, 4, 0, 0),
	F(240000000, P_CAMCC_PLL0_OUT_EVEN, 2.5, 0, 0),
	F(300000000, P_CAMCC_PLL0_OUT_EVEN, 2, 0, 0),
	F(400000000, P_CAMCC_PLL0_OUT_ODD, 1, 0, 0),
	{ }
};

static struct clk_rcg2 camcc_camnoc_axi_clk_src = {
	.cmd_rcgr = 0x2401c,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_0,
	.freq_tbl = ftbl_camcc_camnoc_axi_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_camnoc_axi_clk_src",
		.parent_data = camcc_parent_data_0,
		.num_parents = ARRAY_SIZE(camcc_parent_data_0),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_camcc_cci_0_clk_src[] = {
	F(37500000, P_CAMCC_PLL0_OUT_EVEN, 16, 0, 0),
	F(50000000, P_CAMCC_PLL0_OUT_EVEN, 12, 0, 0),
	F(100000000, P_CAMCC_PLL0_OUT_EVEN, 6, 0, 0),
	{ }
};

static struct clk_rcg2 camcc_cci_0_clk_src = {
	.cmd_rcgr = 0x21004,
	.mnd_width = 8,
	.hid_width = 5,
	.parent_map = camcc_parent_map_3,
	.freq_tbl = ftbl_camcc_cci_0_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_cci_0_clk_src",
		.parent_data = camcc_parent_data_3,
		.num_parents = ARRAY_SIZE(camcc_parent_data_3),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static struct clk_rcg2 camcc_cci_1_clk_src = {
	.cmd_rcgr = 0x22004,
	.mnd_width = 8,
	.hid_width = 5,
	.parent_map = camcc_parent_map_3,
	.freq_tbl = ftbl_camcc_cci_0_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_cci_1_clk_src",
		.parent_data = camcc_parent_data_3,
		.num_parents = ARRAY_SIZE(camcc_parent_data_3),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_camcc_cphy_rx_clk_src[] = {
	F(200000000, P_CAMCC_PLL0_OUT_EVEN, 3, 0, 0),
	F(300000000, P_CAMCC_PLL0_OUT_EVEN, 2, 0, 0),
	F(400000000, P_CAMCC_PLL0_OUT_EVEN, 1.5, 0, 0),
	{ }
};

static struct clk_rcg2 camcc_cphy_rx_clk_src = {
	.cmd_rcgr = 0x1c05c,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_0,
	.freq_tbl = ftbl_camcc_cphy_rx_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_cphy_rx_clk_src",
		.parent_data = camcc_parent_data_0,
		.num_parents = ARRAY_SIZE(camcc_parent_data_0),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_camcc_cre_clk_src[] = {
	F(300000000, P_CAMCC_PLL0_OUT_EVEN, 2, 0, 0),
	F(400000000, P_CAMCC_PLL0_OUT_ODD, 1, 0, 0),
	F(600000000, P_CAMCC_PLL0_OUT_MAIN, 2, 0, 0),
	{ }
};

static struct clk_rcg2 camcc_cre_clk_src = {
	.cmd_rcgr = 0x27004,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_2,
	.freq_tbl = ftbl_camcc_cre_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_cre_clk_src",
		.parent_data = camcc_parent_data_2,
		.num_parents = ARRAY_SIZE(camcc_parent_data_2),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_camcc_csi0phytimer_clk_src[] = {
	F(300000000, P_CAMCC_PLL0_OUT_EVEN, 2, 0, 0),
	{ }
};

static struct clk_rcg2 camcc_csi0phytimer_clk_src = {
	.cmd_rcgr = 0x19004,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_0,
	.freq_tbl = ftbl_camcc_csi0phytimer_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_csi0phytimer_clk_src",
		.parent_data = camcc_parent_data_0,
		.num_parents = ARRAY_SIZE(camcc_parent_data_0),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static struct clk_rcg2 camcc_csi1phytimer_clk_src = {
	.cmd_rcgr = 0x19028,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_0,
	.freq_tbl = ftbl_camcc_csi0phytimer_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_csi1phytimer_clk_src",
		.parent_data = camcc_parent_data_0,
		.num_parents = ARRAY_SIZE(camcc_parent_data_0),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static struct clk_rcg2 camcc_csi2phytimer_clk_src = {
	.cmd_rcgr = 0x1904c,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_0,
	.freq_tbl = ftbl_camcc_csi0phytimer_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_csi2phytimer_clk_src",
		.parent_data = camcc_parent_data_0,
		.num_parents = ARRAY_SIZE(camcc_parent_data_0),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static struct clk_rcg2 camcc_csi3phytimer_clk_src = {
	.cmd_rcgr = 0x19070,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_0,
	.freq_tbl = ftbl_camcc_csi0phytimer_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_csi3phytimer_clk_src",
		.parent_data = camcc_parent_data_0,
		.num_parents = ARRAY_SIZE(camcc_parent_data_0),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_camcc_fast_ahb_clk_src[] = {
	F(100000000, P_CAMCC_PLL0_OUT_EVEN, 6, 0, 0),
	F(150000000, P_CAMCC_PLL0_OUT_EVEN, 4, 0, 0),
	F(200000000, P_CAMCC_PLL0_OUT_MAIN, 6, 0, 0),
	F(240000000, P_CAMCC_PLL0_OUT_MAIN, 5, 0, 0),
	{ }
};

static struct clk_rcg2 camcc_fast_ahb_clk_src = {
	.cmd_rcgr = 0x1a030,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_0,
	.freq_tbl = ftbl_camcc_fast_ahb_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_fast_ahb_clk_src",
		.parent_data = camcc_parent_data_0,
		.num_parents = ARRAY_SIZE(camcc_parent_data_0),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_camcc_icp_clk_src[] = {
	F(400000000, P_CAMCC_PLL0_OUT_MAIN, 3, 0, 0),
	F(480000000, P_CAMCC_PLL0_OUT_MAIN, 2.5, 0, 0),
	F(600000000, P_CAMCC_PLL0_OUT_MAIN, 2, 0, 0),
	{ }
};

static struct clk_rcg2 camcc_icp_clk_src = {
	.cmd_rcgr = 0x20014,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_4,
	.freq_tbl = ftbl_camcc_icp_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_icp_clk_src",
		.parent_data = camcc_parent_data_4,
		.num_parents = ARRAY_SIZE(camcc_parent_data_4),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_camcc_mclk0_clk_src[] = {
	F(19200000, P_CAMCC_PLL2_OUT_MAIN, 1, 1, 50),
	F(24000000, P_CAMCC_PLL2_OUT_MAIN, 10, 1, 4),
	F(64000000, P_CAMCC_PLL2_OUT_MAIN, 15, 0, 0),
	{ }
};

static struct clk_rcg2 camcc_mclk0_clk_src = {
	.cmd_rcgr = 0x18004,
	.mnd_width = 8,
	.hid_width = 5,
	.parent_map = camcc_parent_map_1,
	.freq_tbl = ftbl_camcc_mclk0_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_mclk0_clk_src",
		.parent_data = camcc_parent_data_1,
		.num_parents = ARRAY_SIZE(camcc_parent_data_1),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static struct clk_rcg2 camcc_mclk1_clk_src = {
	.cmd_rcgr = 0x18024,
	.mnd_width = 8,
	.hid_width = 5,
	.parent_map = camcc_parent_map_1,
	.freq_tbl = ftbl_camcc_mclk0_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_mclk1_clk_src",
		.parent_data = camcc_parent_data_1,
		.num_parents = ARRAY_SIZE(camcc_parent_data_1),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static struct clk_rcg2 camcc_mclk2_clk_src = {
	.cmd_rcgr = 0x18044,
	.mnd_width = 8,
	.hid_width = 5,
	.parent_map = camcc_parent_map_1,
	.freq_tbl = ftbl_camcc_mclk0_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_mclk2_clk_src",
		.parent_data = camcc_parent_data_1,
		.num_parents = ARRAY_SIZE(camcc_parent_data_1),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static struct clk_rcg2 camcc_mclk3_clk_src = {
	.cmd_rcgr = 0x18064,
	.mnd_width = 8,
	.hid_width = 5,
	.parent_map = camcc_parent_map_1,
	.freq_tbl = ftbl_camcc_mclk0_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_mclk3_clk_src",
		.parent_data = camcc_parent_data_1,
		.num_parents = ARRAY_SIZE(camcc_parent_data_1),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static struct clk_rcg2 camcc_mclk4_clk_src = {
	.cmd_rcgr = 0x18084,
	.mnd_width = 8,
	.hid_width = 5,
	.parent_map = camcc_parent_map_1,
	.freq_tbl = ftbl_camcc_mclk0_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_mclk4_clk_src",
		.parent_data = camcc_parent_data_1,
		.num_parents = ARRAY_SIZE(camcc_parent_data_1),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_camcc_ope_0_clk_src[] = {
	F(300000000, P_CAMCC_PLL3_OUT_EVEN, 1, 0, 0),
	F(410000000, P_CAMCC_PLL3_OUT_EVEN, 1, 0, 0),
	F(520000000, P_CAMCC_PLL3_OUT_EVEN, 1, 0, 0),
	F(645000000, P_CAMCC_PLL3_OUT_EVEN, 1, 0, 0),
	F(700000000, P_CAMCC_PLL3_OUT_EVEN, 1, 0, 0),
	{ }
};

static struct clk_rcg2 camcc_ope_0_clk_src = {
	.cmd_rcgr = 0x1b004,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_5,
	.freq_tbl = ftbl_camcc_ope_0_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_ope_0_clk_src",
		.parent_data = camcc_parent_data_5,
		.num_parents = ARRAY_SIZE(camcc_parent_data_5),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_camcc_sleep_clk_src[] = {
	F(32000, P_SLEEP_CLK, 1, 0, 0),
	{ }
};

static struct clk_rcg2 camcc_sleep_clk_src = {
	.cmd_rcgr = 0x25044,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_6,
	.freq_tbl = ftbl_camcc_sleep_clk_src,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_sleep_clk_src",
		.parent_data = camcc_parent_data_6_ao,
		.num_parents = ARRAY_SIZE(camcc_parent_data_6_ao),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_camcc_slow_ahb_clk_src[] = {
	F(80000000, P_CAMCC_PLL0_OUT_EVEN, 7.5, 0, 0),
	{ }
};

static struct clk_rcg2 camcc_slow_ahb_clk_src = {
	.cmd_rcgr = 0x1a04c,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_0,
	.freq_tbl = ftbl_camcc_slow_ahb_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_slow_ahb_clk_src",
		.parent_data = camcc_parent_data_0,
		.num_parents = ARRAY_SIZE(camcc_parent_data_0),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_camcc_tfe_0_clk_src[] = {
	F(350000000, P_CAMCC_PLL4_OUT_EVEN, 1, 0, 0),
	F(570000000, P_CAMCC_PLL4_OUT_EVEN, 1, 0, 0),
	F(600000000, P_CAMCC_PLL4_OUT_EVEN, 1, 0, 0),
	F(725000000, P_CAMCC_PLL4_OUT_EVEN, 1, 0, 0),
	{ }
};

static struct clk_rcg2 camcc_tfe_0_clk_src = {
	.cmd_rcgr = 0x1c004,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_7,
	.freq_tbl = ftbl_camcc_tfe_0_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_tfe_0_clk_src",
		.parent_data = camcc_parent_data_7,
		.num_parents = ARRAY_SIZE(camcc_parent_data_7),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static struct clk_rcg2 camcc_tfe_0_csid_clk_src = {
	.cmd_rcgr = 0x1c030,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_0,
	.freq_tbl = ftbl_camcc_cphy_rx_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_tfe_0_csid_clk_src",
		.parent_data = camcc_parent_data_0,
		.num_parents = ARRAY_SIZE(camcc_parent_data_0),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_camcc_tfe_1_clk_src[] = {
	F(350000000, P_CAMCC_PLL5_OUT_EVEN, 1, 0, 0),
	F(570000000, P_CAMCC_PLL5_OUT_EVEN, 1, 0, 0),
	F(600000000, P_CAMCC_PLL5_OUT_EVEN, 1, 0, 0),
	F(725000000, P_CAMCC_PLL5_OUT_EVEN, 1, 0, 0),
	{ }
};

static struct clk_rcg2 camcc_tfe_1_clk_src = {
	.cmd_rcgr = 0x1d004,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_8,
	.freq_tbl = ftbl_camcc_tfe_1_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_tfe_1_clk_src",
		.parent_data = camcc_parent_data_8,
		.num_parents = ARRAY_SIZE(camcc_parent_data_8),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static struct clk_rcg2 camcc_tfe_1_csid_clk_src = {
	.cmd_rcgr = 0x1d030,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_0,
	.freq_tbl = ftbl_camcc_cphy_rx_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_tfe_1_csid_clk_src",
		.parent_data = camcc_parent_data_0,
		.num_parents = ARRAY_SIZE(camcc_parent_data_0),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_camcc_tfe_2_clk_src[] = {
	F(350000000, P_CAMCC_PLL6_OUT_EVEN, 1, 0, 0),
	F(570000000, P_CAMCC_PLL6_OUT_EVEN, 1, 0, 0),
	F(600000000, P_CAMCC_PLL6_OUT_EVEN, 1, 0, 0),
	F(725000000, P_CAMCC_PLL6_OUT_EVEN, 1, 0, 0),
	{ }
};

static struct clk_rcg2 camcc_tfe_2_clk_src = {
	.cmd_rcgr = 0x1e004,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_9,
	.freq_tbl = ftbl_camcc_tfe_2_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_tfe_2_clk_src",
		.parent_data = camcc_parent_data_9,
		.num_parents = ARRAY_SIZE(camcc_parent_data_9),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static struct clk_rcg2 camcc_tfe_2_csid_clk_src = {
	.cmd_rcgr = 0x1e030,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_0,
	.freq_tbl = ftbl_camcc_cphy_rx_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_tfe_2_csid_clk_src",
		.parent_data = camcc_parent_data_0,
		.num_parents = ARRAY_SIZE(camcc_parent_data_0),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_camcc_xo_clk_src[] = {
	F(19200000, P_BI_TCXO, 1, 0, 0),
	{ }
};

static struct clk_rcg2 camcc_xo_clk_src = {
	.cmd_rcgr = 0x25020,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = camcc_parent_map_10,
	.freq_tbl = ftbl_camcc_xo_clk_src,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "camcc_xo_clk_src",
		.parent_data = camcc_parent_data_10,
		.num_parents = ARRAY_SIZE(camcc_parent_data_10),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static struct clk_branch camcc_bps_ahb_clk = {
	.halt_reg = 0x1a064,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1a064,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_bps_ahb_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_slow_ahb_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_bps_areg_clk = {
	.halt_reg = 0x1a048,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1a048,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_bps_areg_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_fast_ahb_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_bps_clk = {
	.halt_reg = 0x1a01c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1a01c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_bps_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_bps_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_camnoc_atb_clk = {
	.halt_reg = 0x24040,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x24040,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_camnoc_atb_clk",
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_camnoc_axi_hf_clk = {
	.halt_reg = 0x24010,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x24010,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_camnoc_axi_hf_clk",
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_camnoc_axi_sf_clk = {
	.halt_reg = 0x24004,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x24004,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_camnoc_axi_sf_clk",
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_camnoc_nrt_axi_clk = {
	.halt_reg = 0x2404c,
	.halt_check = BRANCH_HALT_VOTED,
	.hwcg_reg = 0x2404c,
	.hwcg_bit = 1,
	.clkr = {
		.enable_reg = 0x2404c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_camnoc_nrt_axi_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_camnoc_axi_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_camnoc_rt_axi_clk = {
	.halt_reg = 0x24034,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x24034,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_camnoc_rt_axi_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_camnoc_axi_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_cci_0_clk = {
	.halt_reg = 0x2101c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x2101c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_cci_0_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_cci_0_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_cci_1_clk = {
	.halt_reg = 0x2201c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x2201c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_cci_1_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_cci_1_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_core_ahb_clk = {
	.halt_reg = 0x2501c,
	.halt_check = BRANCH_HALT_DELAY,
	.clkr = {
		.enable_reg = 0x2501c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_core_ahb_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_slow_ahb_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_cpas_ahb_clk = {
	.halt_reg = 0x23004,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x23004,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_cpas_ahb_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_slow_ahb_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_cre_ahb_clk = {
	.halt_reg = 0x27020,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x27020,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_cre_ahb_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_slow_ahb_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_cre_clk = {
	.halt_reg = 0x2701c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x2701c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_cre_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_cre_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_csi0phytimer_clk = {
	.halt_reg = 0x1901c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1901c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_csi0phytimer_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_csi0phytimer_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_csi1phytimer_clk = {
	.halt_reg = 0x19040,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x19040,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_csi1phytimer_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_csi1phytimer_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_csi2phytimer_clk = {
	.halt_reg = 0x19064,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x19064,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_csi2phytimer_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_csi2phytimer_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_csi3phytimer_clk = {
	.halt_reg = 0x19088,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x19088,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_csi3phytimer_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_csi3phytimer_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_csiphy0_clk = {
	.halt_reg = 0x19020,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x19020,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_csiphy0_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_cphy_rx_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_csiphy1_clk = {
	.halt_reg = 0x19044,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x19044,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_csiphy1_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_cphy_rx_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_csiphy2_clk = {
	.halt_reg = 0x19068,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x19068,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_csiphy2_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_cphy_rx_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_csiphy3_clk = {
	.halt_reg = 0x1908c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1908c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_csiphy3_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_cphy_rx_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_icp_atb_clk = {
	.halt_reg = 0x20004,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x20004,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_icp_atb_clk",
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_icp_clk = {
	.halt_reg = 0x2002c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x2002c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_icp_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_icp_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_icp_cti_clk = {
	.halt_reg = 0x20008,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x20008,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_icp_cti_clk",
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_icp_ts_clk = {
	.halt_reg = 0x2000c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x2000c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_icp_ts_clk",
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_mclk0_clk = {
	.halt_reg = 0x1801c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1801c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_mclk0_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_mclk0_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_mclk1_clk = {
	.halt_reg = 0x1803c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1803c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_mclk1_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_mclk1_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_mclk2_clk = {
	.halt_reg = 0x1805c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1805c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_mclk2_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_mclk2_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_mclk3_clk = {
	.halt_reg = 0x1807c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1807c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_mclk3_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_mclk3_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_mclk4_clk = {
	.halt_reg = 0x1809c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1809c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_mclk4_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_mclk4_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_ope_0_ahb_clk = {
	.halt_reg = 0x1b034,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1b034,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_ope_0_ahb_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_slow_ahb_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_ope_0_areg_clk = {
	.halt_reg = 0x1b030,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1b030,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_ope_0_areg_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_fast_ahb_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_ope_0_clk = {
	.halt_reg = 0x1b01c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1b01c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_ope_0_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_ope_0_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_soc_ahb_clk = {
	.halt_reg = 0x25018,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x25018,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_soc_ahb_clk",
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_sys_tmr_clk = {
	.halt_reg = 0x20038,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x20038,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_sys_tmr_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_xo_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_tfe_0_ahb_clk = {
	.halt_reg = 0x1c078,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1c078,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_tfe_0_ahb_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_slow_ahb_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_tfe_0_clk = {
	.halt_reg = 0x1c01c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1c01c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_tfe_0_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_tfe_0_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_tfe_0_cphy_rx_clk = {
	.halt_reg = 0x1c074,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1c074,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_tfe_0_cphy_rx_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_cphy_rx_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_tfe_0_csid_clk = {
	.halt_reg = 0x1c048,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1c048,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_tfe_0_csid_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_tfe_0_csid_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_tfe_1_ahb_clk = {
	.halt_reg = 0x1d058,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1d058,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_tfe_1_ahb_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_slow_ahb_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_tfe_1_clk = {
	.halt_reg = 0x1d01c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1d01c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_tfe_1_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_tfe_1_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_tfe_1_cphy_rx_clk = {
	.halt_reg = 0x1d054,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1d054,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_tfe_1_cphy_rx_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_cphy_rx_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_tfe_1_csid_clk = {
	.halt_reg = 0x1d048,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1d048,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_tfe_1_csid_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_tfe_1_csid_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_tfe_2_ahb_clk = {
	.halt_reg = 0x1e058,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1e058,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_tfe_2_ahb_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_slow_ahb_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_tfe_2_clk = {
	.halt_reg = 0x1e01c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1e01c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_tfe_2_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_tfe_2_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_tfe_2_cphy_rx_clk = {
	.halt_reg = 0x1e054,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1e054,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_tfe_2_cphy_rx_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_cphy_rx_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_tfe_2_csid_clk = {
	.halt_reg = 0x1e048,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x1e048,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_tfe_2_csid_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_tfe_2_csid_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch camcc_top_shift_clk = {
	.halt_reg = 0x25040,
	.halt_check = BRANCH_HALT_VOTED,
	.clkr = {
		.enable_reg = 0x25040,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "camcc_top_shift_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&camcc_xo_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_regmap *camcc_sm7635_clocks[] = {
	[CAMCC_BPS_AHB_CLK] = &camcc_bps_ahb_clk.clkr,
	[CAMCC_BPS_AREG_CLK] = &camcc_bps_areg_clk.clkr,
	[CAMCC_BPS_CLK] = &camcc_bps_clk.clkr,
	[CAMCC_BPS_CLK_SRC] = &camcc_bps_clk_src.clkr,
	[CAMCC_CAMNOC_ATB_CLK] = &camcc_camnoc_atb_clk.clkr,
	[CAMCC_CAMNOC_AXI_CLK_SRC] = &camcc_camnoc_axi_clk_src.clkr,
	[CAMCC_CAMNOC_AXI_HF_CLK] = &camcc_camnoc_axi_hf_clk.clkr,
	[CAMCC_CAMNOC_AXI_SF_CLK] = &camcc_camnoc_axi_sf_clk.clkr,
	[CAMCC_CAMNOC_NRT_AXI_CLK] = &camcc_camnoc_nrt_axi_clk.clkr,
	[CAMCC_CAMNOC_RT_AXI_CLK] = &camcc_camnoc_rt_axi_clk.clkr,
	[CAMCC_CCI_0_CLK] = &camcc_cci_0_clk.clkr,
	[CAMCC_CCI_0_CLK_SRC] = &camcc_cci_0_clk_src.clkr,
	[CAMCC_CCI_1_CLK] = &camcc_cci_1_clk.clkr,
	[CAMCC_CCI_1_CLK_SRC] = &camcc_cci_1_clk_src.clkr,
	[CAMCC_CORE_AHB_CLK] = &camcc_core_ahb_clk.clkr,
	[CAMCC_CPAS_AHB_CLK] = &camcc_cpas_ahb_clk.clkr,
	[CAMCC_CPHY_RX_CLK_SRC] = &camcc_cphy_rx_clk_src.clkr,
	[CAMCC_CRE_AHB_CLK] = &camcc_cre_ahb_clk.clkr,
	[CAMCC_CRE_CLK] = &camcc_cre_clk.clkr,
	[CAMCC_CRE_CLK_SRC] = &camcc_cre_clk_src.clkr,
	[CAMCC_CSI0PHYTIMER_CLK] = &camcc_csi0phytimer_clk.clkr,
	[CAMCC_CSI0PHYTIMER_CLK_SRC] = &camcc_csi0phytimer_clk_src.clkr,
	[CAMCC_CSI1PHYTIMER_CLK] = &camcc_csi1phytimer_clk.clkr,
	[CAMCC_CSI1PHYTIMER_CLK_SRC] = &camcc_csi1phytimer_clk_src.clkr,
	[CAMCC_CSI2PHYTIMER_CLK] = &camcc_csi2phytimer_clk.clkr,
	[CAMCC_CSI2PHYTIMER_CLK_SRC] = &camcc_csi2phytimer_clk_src.clkr,
	[CAMCC_CSI3PHYTIMER_CLK] = &camcc_csi3phytimer_clk.clkr,
	[CAMCC_CSI3PHYTIMER_CLK_SRC] = &camcc_csi3phytimer_clk_src.clkr,
	[CAMCC_CSIPHY0_CLK] = &camcc_csiphy0_clk.clkr,
	[CAMCC_CSIPHY1_CLK] = &camcc_csiphy1_clk.clkr,
	[CAMCC_CSIPHY2_CLK] = &camcc_csiphy2_clk.clkr,
	[CAMCC_CSIPHY3_CLK] = &camcc_csiphy3_clk.clkr,
	[CAMCC_FAST_AHB_CLK_SRC] = &camcc_fast_ahb_clk_src.clkr,
	[CAMCC_ICP_ATB_CLK] = &camcc_icp_atb_clk.clkr,
	[CAMCC_ICP_CLK] = &camcc_icp_clk.clkr,
	[CAMCC_ICP_CLK_SRC] = &camcc_icp_clk_src.clkr,
	[CAMCC_ICP_CTI_CLK] = &camcc_icp_cti_clk.clkr,
	[CAMCC_ICP_TS_CLK] = &camcc_icp_ts_clk.clkr,
	[CAMCC_MCLK0_CLK] = &camcc_mclk0_clk.clkr,
	[CAMCC_MCLK0_CLK_SRC] = &camcc_mclk0_clk_src.clkr,
	[CAMCC_MCLK1_CLK] = &camcc_mclk1_clk.clkr,
	[CAMCC_MCLK1_CLK_SRC] = &camcc_mclk1_clk_src.clkr,
	[CAMCC_MCLK2_CLK] = &camcc_mclk2_clk.clkr,
	[CAMCC_MCLK2_CLK_SRC] = &camcc_mclk2_clk_src.clkr,
	[CAMCC_MCLK3_CLK] = &camcc_mclk3_clk.clkr,
	[CAMCC_MCLK3_CLK_SRC] = &camcc_mclk3_clk_src.clkr,
	[CAMCC_MCLK4_CLK] = &camcc_mclk4_clk.clkr,
	[CAMCC_MCLK4_CLK_SRC] = &camcc_mclk4_clk_src.clkr,
	[CAMCC_OPE_0_AHB_CLK] = &camcc_ope_0_ahb_clk.clkr,
	[CAMCC_OPE_0_AREG_CLK] = &camcc_ope_0_areg_clk.clkr,
	[CAMCC_OPE_0_CLK] = &camcc_ope_0_clk.clkr,
	[CAMCC_OPE_0_CLK_SRC] = &camcc_ope_0_clk_src.clkr,
	[CAMCC_PLL0] = &camcc_pll0.clkr,
	[CAMCC_PLL0_OUT_EVEN] = &camcc_pll0_out_even.clkr,
	[CAMCC_PLL0_OUT_ODD] = &camcc_pll0_out_odd.clkr,
	[CAMCC_PLL1] = &camcc_pll1.clkr,
	[CAMCC_PLL1_OUT_EVEN] = &camcc_pll1_out_even.clkr,
	[CAMCC_PLL2] = &camcc_pll2.clkr,
	[CAMCC_PLL3] = &camcc_pll3.clkr,
	[CAMCC_PLL3_OUT_EVEN] = &camcc_pll3_out_even.clkr,
	[CAMCC_PLL4] = &camcc_pll4.clkr,
	[CAMCC_PLL4_OUT_EVEN] = &camcc_pll4_out_even.clkr,
	[CAMCC_PLL5] = &camcc_pll5.clkr,
	[CAMCC_PLL5_OUT_EVEN] = &camcc_pll5_out_even.clkr,
	[CAMCC_PLL6] = &camcc_pll6.clkr,
	[CAMCC_PLL6_OUT_EVEN] = &camcc_pll6_out_even.clkr,
	[CAMCC_SLEEP_CLK_SRC] = &camcc_sleep_clk_src.clkr,
	[CAMCC_SLOW_AHB_CLK_SRC] = &camcc_slow_ahb_clk_src.clkr,
	[CAMCC_SOC_AHB_CLK] = &camcc_soc_ahb_clk.clkr,
	[CAMCC_SYS_TMR_CLK] = &camcc_sys_tmr_clk.clkr,
	[CAMCC_TFE_0_AHB_CLK] = &camcc_tfe_0_ahb_clk.clkr,
	[CAMCC_TFE_0_CLK] = &camcc_tfe_0_clk.clkr,
	[CAMCC_TFE_0_CLK_SRC] = &camcc_tfe_0_clk_src.clkr,
	[CAMCC_TFE_0_CPHY_RX_CLK] = &camcc_tfe_0_cphy_rx_clk.clkr,
	[CAMCC_TFE_0_CSID_CLK] = &camcc_tfe_0_csid_clk.clkr,
	[CAMCC_TFE_0_CSID_CLK_SRC] = &camcc_tfe_0_csid_clk_src.clkr,
	[CAMCC_TFE_1_AHB_CLK] = &camcc_tfe_1_ahb_clk.clkr,
	[CAMCC_TFE_1_CLK] = &camcc_tfe_1_clk.clkr,
	[CAMCC_TFE_1_CLK_SRC] = &camcc_tfe_1_clk_src.clkr,
	[CAMCC_TFE_1_CPHY_RX_CLK] = &camcc_tfe_1_cphy_rx_clk.clkr,
	[CAMCC_TFE_1_CSID_CLK] = &camcc_tfe_1_csid_clk.clkr,
	[CAMCC_TFE_1_CSID_CLK_SRC] = &camcc_tfe_1_csid_clk_src.clkr,
	[CAMCC_TFE_2_AHB_CLK] = &camcc_tfe_2_ahb_clk.clkr,
	[CAMCC_TFE_2_CLK] = &camcc_tfe_2_clk.clkr,
	[CAMCC_TFE_2_CLK_SRC] = &camcc_tfe_2_clk_src.clkr,
	[CAMCC_TFE_2_CPHY_RX_CLK] = &camcc_tfe_2_cphy_rx_clk.clkr,
	[CAMCC_TFE_2_CSID_CLK] = &camcc_tfe_2_csid_clk.clkr,
	[CAMCC_TFE_2_CSID_CLK_SRC] = &camcc_tfe_2_csid_clk_src.clkr,
	[CAMCC_TOP_SHIFT_CLK] = &camcc_top_shift_clk.clkr,
	[CAMCC_XO_CLK_SRC] = &camcc_xo_clk_src.clkr,
};

static const struct qcom_reset_map camcc_sm7635_resets[] = {
	[CAMCC_BPS_BCR] = { 0x1a000 },
	[CAMCC_CAMNOC_BCR] = { 0x24000 },
	[CAMCC_CAMSS_TOP_BCR] = { 0x25000 },
	[CAMCC_CCI_0_BCR] = { 0x21000 },
	[CAMCC_CCI_1_BCR] = { 0x22000 },
	[CAMCC_CPAS_BCR] = { 0x23000 },
	[CAMCC_CRE_BCR] = { 0x27000 },
	[CAMCC_CSI0PHY_BCR] = { 0x19000 },
	[CAMCC_CSI1PHY_BCR] = { 0x19024 },
	[CAMCC_CSI2PHY_BCR] = { 0x19048 },
	[CAMCC_CSI3PHY_BCR] = { 0x1906c },
	[CAMCC_ICP_BCR] = { 0x20000 },
	[CAMCC_MCLK0_BCR] = { 0x18000 },
	[CAMCC_MCLK1_BCR] = { 0x18020 },
	[CAMCC_MCLK2_BCR] = { 0x18040 },
	[CAMCC_MCLK3_BCR] = { 0x18060 },
	[CAMCC_MCLK4_BCR] = { 0x18080 },
	[CAMCC_OPE_0_BCR] = { 0x1b000 },
	[CAMCC_TFE_0_BCR] = { 0x1c000 },
	[CAMCC_TFE_1_BCR] = { 0x1d000 },
	[CAMCC_TFE_2_BCR] = { 0x1e000 },
};

static const struct regmap_config camcc_sm7635_regmap_config = {
	.reg_bits = 32,
	.reg_stride = 4,
	.val_bits = 32,
	.max_register = 0x30728,
	.fast_io = true,
};

static struct qcom_cc_desc camcc_sm7635_desc = {
	.config = &camcc_sm7635_regmap_config,
	.clks = camcc_sm7635_clocks,
	.num_clks = ARRAY_SIZE(camcc_sm7635_clocks),
	.resets = camcc_sm7635_resets,
	.num_resets = ARRAY_SIZE(camcc_sm7635_resets),
};

static const struct of_device_id camcc_sm7635_match_table[] = {
	{ .compatible = "qcom,sm7635-camcc" },
	{ }
};
MODULE_DEVICE_TABLE(of, camcc_sm7635_match_table);

static int camcc_sm7635_probe(struct platform_device *pdev)
{
	struct regmap *regmap;

	regmap = qcom_cc_map(pdev, &camcc_sm7635_desc);
	if (IS_ERR(regmap))
		return PTR_ERR(regmap);

	clk_lucid_ole_pll_configure(&camcc_pll0, regmap, &camcc_pll0_config);
	clk_lucid_ole_pll_configure(&camcc_pll1, regmap, &camcc_pll1_config);
	clk_rivian_evo_pll_configure(&camcc_pll2, regmap, &camcc_pll2_config);
	clk_lucid_ole_pll_configure(&camcc_pll3, regmap, &camcc_pll3_config);
	clk_lucid_ole_pll_configure(&camcc_pll4, regmap, &camcc_pll4_config);
	clk_lucid_ole_pll_configure(&camcc_pll5, regmap, &camcc_pll5_config);
	clk_lucid_ole_pll_configure(&camcc_pll6, regmap, &camcc_pll6_config);

	/* Keep some clocks always-on */
	qcom_branch_set_clk_en(regmap, 0x25038); /* CAMCC_GDSC_CLK */
	qcom_branch_set_clk_en(regmap, 0x2505c); /* CAMCC_SLEEP_CLK */

	return qcom_cc_really_probe(&pdev->dev, &camcc_sm7635_desc, regmap);
}

static struct platform_driver camcc_sm7635_driver = {
	.probe = camcc_sm7635_probe,
	.driver = {
		.name = "camcc-sm7635",
		.of_match_table = camcc_sm7635_match_table,
	},
};

static int __init camcc_sm7635_init(void)
{
	return platform_driver_register(&camcc_sm7635_driver);
}
subsys_initcall(camcc_sm7635_init);

static void __exit camcc_sm7635_exit(void)
{
	platform_driver_unregister(&camcc_sm7635_driver);
}
module_exit(camcc_sm7635_exit);

MODULE_DESCRIPTION("QTI CAMCC SM7635 Driver");
MODULE_LICENSE("GPL");
