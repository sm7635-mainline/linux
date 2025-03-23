// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2023-2024, Qualcomm Innovation Center, Inc. All rights reserved.
 * Copyright (c) 2025, Danila Tikhonov <danila@jiaxyga.com>
 */

#include <linux/clk-provider.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>

#include <dt-bindings/clock/qcom,sm7635-gpucc.h>

#include "clk-alpha-pll.h"
#include "clk-branch.h"
#include "clk-rcg.h"
#include "clk-regmap.h"
#include "clk-regmap-divider.h"
#include "clk-regmap-mux.h"
#include "clk-regmap-phy-mux.h"
#include "gdsc.h"
#include "reset.h"

enum {
	DT_BI_TCXO,
	DT_GPLL0_OUT_MAIN,
	DT_GPLL0_OUT_MAIN_DIV,
};

enum {
	P_BI_TCXO,
	P_GPLL0_OUT_MAIN,
	P_GPLL0_OUT_MAIN_DIV,
	P_GPUCC_PLL0_OUT_EVEN,
	P_GPUCC_PLL0_OUT_MAIN,
	P_GPUCC_PLL0_OUT_ODD,
};

static const struct pll_vco lucid_ole_vco[] = {
	{ 249600000, 2300000000, 0 },
};

/* 700.0 MHz Configuration */
static const struct alpha_pll_config gpucc_pll0_config = {
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

static struct clk_alpha_pll gpucc_pll0 = {
	.offset = 0x0,
	.vco_table = lucid_ole_vco,
	.num_vco = ARRAY_SIZE(lucid_ole_vco),
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_LUCID_OLE],
	.clkr = {
		.hw.init = &(const struct clk_init_data) {
			.name = "gpucc_pll0",
			.parent_data = &(const struct clk_parent_data) {
				.index = DT_BI_TCXO,
			},
			.num_parents = 1,
			.ops = &clk_alpha_pll_lucid_evo_ops,
		},
	},
};

static const struct clk_div_table post_div_table_gpucc_pll0_out_even[] = {
	{ 0x1, 2 },
	{ }
};

static struct clk_alpha_pll_postdiv gpucc_pll0_out_even = {
	.offset = 0x0,
	.post_div_shift = 10,
	.post_div_table = post_div_table_gpucc_pll0_out_even,
	.num_post_div = ARRAY_SIZE(post_div_table_gpucc_pll0_out_even),
	.width = 4,
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_LUCID_OLE],
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "gpucc_pll0_out_even",
		.parent_hws = (const struct clk_hw*[]) {
			&gpucc_pll0.clkr.hw,
		},
		.num_parents = 1,
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_alpha_pll_postdiv_lucid_ole_ops,
	},
};

static const struct parent_map gpucc_parent_map_0[] = {
	{ P_BI_TCXO, 0 },
	{ P_GPLL0_OUT_MAIN, 5 },
	{ P_GPLL0_OUT_MAIN_DIV, 6 },
};

static const struct clk_parent_data gpucc_parent_data_0[] = {
	{ .index = DT_BI_TCXO },
	{ .index = DT_GPLL0_OUT_MAIN },
	{ .index = DT_GPLL0_OUT_MAIN_DIV },
};

static const struct parent_map gpucc_parent_map_1[] = {
	{ P_BI_TCXO, 0 },
	{ P_GPUCC_PLL0_OUT_MAIN, 1 },
	{ P_GPUCC_PLL0_OUT_EVEN, 2 },
	{ P_GPUCC_PLL0_OUT_ODD, 3 },
	{ P_GPLL0_OUT_MAIN, 5 },
	{ P_GPLL0_OUT_MAIN_DIV, 6 },
};

static const struct clk_parent_data gpucc_parent_data_1[] = {
	{ .index = DT_BI_TCXO },
	{ .hw = &gpucc_pll0.clkr.hw },
	{ .hw = &gpucc_pll0_out_even.clkr.hw },
	{ .hw = &gpucc_pll0.clkr.hw },
	{ .index = DT_GPLL0_OUT_MAIN },
	{ .index = DT_GPLL0_OUT_MAIN_DIV },
};

static const struct parent_map gpucc_parent_map_2[] = {
	{ P_BI_TCXO, 0 },
};

static const struct clk_parent_data gpucc_parent_data_2[] = {
	{ .index = DT_BI_TCXO },
};

static const struct freq_tbl ftbl_gpucc_ff_clk_src[] = {
	F(200000000, P_GPLL0_OUT_MAIN, 3, 0, 0),
	{ }
};

static struct clk_rcg2 gpucc_ff_clk_src = {
	.cmd_rcgr = 0x9474,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = gpucc_parent_map_0,
	.freq_tbl = ftbl_gpucc_ff_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "gpucc_ff_clk_src",
		.parent_data = gpucc_parent_data_0,
		.num_parents = ARRAY_SIZE(gpucc_parent_data_0),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_gpucc_gmu_clk_src[] = {
	F(19200000, P_BI_TCXO, 1, 0, 0),
	F(350000000, P_GPUCC_PLL0_OUT_EVEN, 1, 0, 0),
	F(650000000, P_GPUCC_PLL0_OUT_EVEN, 1, 0, 0),
	F(687500000, P_GPUCC_PLL0_OUT_EVEN, 1, 0, 0),
	{ }
};

static struct clk_rcg2 gpucc_gmu_clk_src = {
	.cmd_rcgr = 0x9318,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = gpucc_parent_map_1,
	.freq_tbl = ftbl_gpucc_gmu_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "gpucc_gmu_clk_src",
		.parent_data = gpucc_parent_data_1,
		.num_parents = ARRAY_SIZE(gpucc_parent_data_1),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_gpucc_hub_clk_src[] = {
	F(200000000, P_GPLL0_OUT_MAIN, 3, 0, 0),
	F(300000000, P_GPLL0_OUT_MAIN, 2, 0, 0),
	F(400000000, P_GPLL0_OUT_MAIN, 1.5, 0, 0),
	{ }
};

static struct clk_rcg2 gpucc_hub_clk_src = {
	.cmd_rcgr = 0x93ec,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = gpucc_parent_map_1,
	.freq_tbl = ftbl_gpucc_hub_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "gpucc_hub_clk_src",
		.parent_data = gpucc_parent_data_1,
		.num_parents = ARRAY_SIZE(gpucc_parent_data_1),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static struct clk_regmap_div gpucc_hub_div_clk_src = {
	.reg = 0x942c,
	.shift = 0,
	.width = 4,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "gpucc_hub_div_clk_src",
		.parent_hws = (const struct clk_hw*[]) {
			&gpucc_hub_clk_src.clkr.hw,
		},
		.num_parents = 1,
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_regmap_div_ro_ops,
	},
};

static struct clk_branch gpucc_ahb_clk = {
	.halt_reg = 0x90bc,
	.halt_check = BRANCH_HALT_DELAY,
	.clkr = {
		.enable_reg = 0x90bc,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "gpucc_ahb_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&gpucc_hub_div_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch gpucc_cx_accu_shift_clk = {
	.halt_reg = 0x910c,
	.halt_check = BRANCH_HALT_VOTED,
	.clkr = {
		.enable_reg = 0x910c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "gpucc_cx_accu_shift_clk",
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch gpucc_cx_ff_clk = {
	.halt_reg = 0x90ec,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x90ec,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "gpucc_cx_ff_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&gpucc_ff_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch gpucc_cx_gmu_clk = {
	.halt_reg = 0x90d4,
	.halt_check = BRANCH_HALT_VOTED,
	.clkr = {
		.enable_reg = 0x90d4,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "gpucc_cx_gmu_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&gpucc_gmu_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = /*CLK_DONT_HOLD_STATE | */CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_aon_ops,
		},
	},
};

static struct clk_branch gpucc_cxo_clk = {
	.halt_reg = 0x90e4,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x90e4,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "gpucc_cxo_clk",
			//.flags = CLK_DONT_HOLD_STATE,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch gpucc_dpm_clk = {
	.halt_reg = 0x9110,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x9110,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "gpucc_dpm_clk",
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch gpucc_freq_measure_clk = {
	.halt_reg = 0x900c,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x900c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "gpucc_freq_measure_clk",
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch gpucc_gx_accu_shift_clk = {
	.halt_reg = 0x9070,
	.halt_check = BRANCH_HALT_VOTED,
	.clkr = {
		.enable_reg = 0x9070,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "gpucc_gx_accu_shift_clk",
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch gpucc_gx_acd_ahb_ff_clk = {
	.halt_reg = 0x9068,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x9068,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "gpucc_gx_acd_ahb_ff_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&gpucc_ff_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch gpucc_gx_gmu_clk = {
	.halt_reg = 0x9060,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x9060,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "gpucc_gx_gmu_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&gpucc_gmu_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch gpucc_gx_rcg_ahb_ff_clk = {
	.halt_reg = 0x906c,
	.halt_check = BRANCH_HALT_VOTED,
	.clkr = {
		.enable_reg = 0x906c,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "gpucc_gx_rcg_ahb_ff_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&gpucc_ff_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch gpucc_hlos1_vote_gpu_smmu_clk = {
	.halt_reg = 0x7000,
	.halt_check = BRANCH_HALT_VOTED,
	.clkr = {
		.enable_reg = 0x7000,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "gpucc_hlos1_vote_gpu_smmu_clk",
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch gpucc_hub_aon_clk = {
	.halt_reg = 0x93e8,
	.halt_check = BRANCH_HALT_VOTED,
	.clkr = {
		.enable_reg = 0x93e8,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "gpucc_hub_aon_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&gpucc_hub_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_aon_ops,
		},
	},
};

static struct clk_branch gpucc_hub_cx_int_clk = {
	.halt_reg = 0x90e8,
	.halt_check = BRANCH_HALT_VOTED,
	.clkr = {
		.enable_reg = 0x90e8,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "gpucc_hub_cx_int_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&gpucc_hub_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = /*CLK_DONT_HOLD_STATE | */CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_aon_ops,
		},
	},
};

static struct clk_branch gpucc_memnoc_gfx_clk = {
	.halt_reg = 0x90f4,
	.halt_check = BRANCH_HALT_VOTED,
	.clkr = {
		.enable_reg = 0x90f4,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "gpucc_memnoc_gfx_clk",
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_regmap *gpucc_sm7635_clocks[] = {
	[GPUCC_AHB_CLK] = &gpucc_ahb_clk.clkr,
	[GPUCC_CX_ACCU_SHIFT_CLK] = &gpucc_cx_accu_shift_clk.clkr,
	[GPUCC_CX_FF_CLK] = &gpucc_cx_ff_clk.clkr,
	[GPUCC_CX_GMU_CLK] = &gpucc_cx_gmu_clk.clkr,
	[GPUCC_CXO_CLK] = &gpucc_cxo_clk.clkr,
	[GPUCC_DPM_CLK] = &gpucc_dpm_clk.clkr,
	[GPUCC_FF_CLK_SRC] = &gpucc_ff_clk_src.clkr,
	[GPUCC_FREQ_MEASURE_CLK] = &gpucc_freq_measure_clk.clkr,
	[GPUCC_GMU_CLK_SRC] = &gpucc_gmu_clk_src.clkr,
	[GPUCC_GX_ACCU_SHIFT_CLK] = &gpucc_gx_accu_shift_clk.clkr,
	[GPUCC_GX_ACD_AHB_FF_CLK] = &gpucc_gx_acd_ahb_ff_clk.clkr,
	[GPUCC_GX_GMU_CLK] = &gpucc_gx_gmu_clk.clkr,
	[GPUCC_GX_RCG_AHB_FF_CLK] = &gpucc_gx_rcg_ahb_ff_clk.clkr,
	[GPUCC_HLOS1_VOTE_GPU_SMMU_CLK] = &gpucc_hlos1_vote_gpu_smmu_clk.clkr,
	[GPUCC_HUB_AON_CLK] = &gpucc_hub_aon_clk.clkr,
	[GPUCC_HUB_CLK_SRC] = &gpucc_hub_clk_src.clkr,
	[GPUCC_HUB_CX_INT_CLK] = &gpucc_hub_cx_int_clk.clkr,
	[GPUCC_HUB_DIV_CLK_SRC] = &gpucc_hub_div_clk_src.clkr,
	[GPUCC_MEMNOC_GFX_CLK] = &gpucc_memnoc_gfx_clk.clkr,
	[GPUCC_PLL0] = &gpucc_pll0.clkr,
	[GPUCC_PLL0_OUT_EVEN] = &gpucc_pll0_out_even.clkr,
};

static const struct qcom_reset_map gpucc_sm7635_resets[] = {
	[GPUCC_CB_BCR] = { 0x93a0 },
	[GPUCC_CX_BCR] = { 0x907c },
	[GPUCC_FAST_HUB_BCR] = { 0x93e4 },
	[GPUCC_FF_BCR] = { 0x9470 },
	[GPUCC_GMU_BCR] = { 0x9314 },
	[GPUCC_GX_BCR] = { 0x905c },
	[GPUCC_RBCPR_BCR] = { 0x91e0 },
	[GPUCC_XO_BCR] = { 0x9000 },
};

static const struct regmap_config gpucc_sm7635_regmap_config = {
	.reg_bits = 32,
	.reg_stride = 4,
	.val_bits = 32,
	.max_register = 0x95e8,
	.fast_io = true,
};

static const struct qcom_cc_desc gpucc_sm7635_desc = {
	.config = &gpucc_sm7635_regmap_config,
	.clks = gpucc_sm7635_clocks,
	.num_clks = ARRAY_SIZE(gpucc_sm7635_clocks),
	.resets = gpucc_sm7635_resets,
	.num_resets = ARRAY_SIZE(gpucc_sm7635_resets),
};

static const struct of_device_id gpucc_sm7635_match_table[] = {
	{ .compatible = "qcom,sm7635-gpucc" },
	{ }
};
MODULE_DEVICE_TABLE(of, gpucc_sm7635_match_table);

static int gpucc_sm7635_probe(struct platform_device *pdev)
{
	struct regmap *regmap;

	regmap = qcom_cc_map(pdev, &gpucc_sm7635_desc);
	if (IS_ERR(regmap))
		return PTR_ERR(regmap);

	clk_lucid_ole_pll_configure(&gpucc_pll0, regmap, &gpucc_pll0_config);

	/* Keep some clocks always-on */
	qcom_branch_set_clk_en(regmap, 0x93a4); /* GPUCC_CB_CLK */
	qcom_branch_set_clk_en(regmap, 0x9008); /* GPUCC_CXO_AON_CLK */
	qcom_branch_set_clk_en(regmap, 0x9010); /* GPUCC_DEMET_CLK */
	qcom_branch_set_clk_en(regmap, 0x9064); /* GPUCC_GX_AHB_FF_CLK */
	qcom_branch_set_clk_en(regmap, 0x93a8); /* GPUCC_RSCC_HUB_AON_CLK */
	qcom_branch_set_clk_en(regmap, 0x9004); /* GPUCC_RSCC_XO_AON_CLK */
	qcom_branch_set_clk_en(regmap, 0x90cc); /* GPUCC_SLEEP_CLK */

	return qcom_cc_really_probe(&pdev->dev, &gpucc_sm7635_desc, regmap);
}

static struct platform_driver gpucc_sm7635_driver = {
	.probe = gpucc_sm7635_probe,
	.driver = {
		.name = "gpucc-sm7635",
		.of_match_table = gpucc_sm7635_match_table,
	},
};

static int __init gpucc_sm7635_init(void)
{
	return platform_driver_register(&gpucc_sm7635_driver);
}
subsys_initcall(gpucc_sm7635_init);

static void __exit gpucc_sm7635_exit(void)
{
	platform_driver_unregister(&gpucc_sm7635_driver);
}
module_exit(gpucc_sm7635_exit);

MODULE_DESCRIPTION("QTI GPUCC SM7635 Driver");
MODULE_LICENSE("GPL");
