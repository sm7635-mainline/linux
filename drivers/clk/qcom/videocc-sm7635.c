// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2023, Qualcomm Innovation Center, Inc. All rights reserved.
 * Copyright (c) 2025, Danila Tikhonov <danila@jiaxyga.com>
 */

#include <linux/clk-provider.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/regmap.h>

#include <dt-bindings/clock/qcom,sm7635-videocc.h>

#include "clk-alpha-pll.h"
#include "clk-branch.h"
#include "clk-rcg.h"
#include "clk-regmap.h"
#include "clk-regmap-divider.h"
#include "common.h"
#include "gdsc.h"
#include "reset.h"

enum {
	DT_BI_TCXO,
	DT_BI_TCXO_AO,
	DT_SLEEP_CLK,
};

enum {
	P_BI_TCXO,
	P_SLEEP_CLK,
	P_VIDEOCC_PLL0_OUT_MAIN,
};

static const struct pll_vco lucid_ole_vco[] = {
	{ 249600000, 2300000000, 0 },
};

/* 604.8 MHz Configuration */
static const struct alpha_pll_config videocc_pll0_config = {
	.l = 0x1f,
	.alpha = 0x8000,
	.config_ctl_val = 0x20485699,
	.config_ctl_hi_val = 0x00182261,
	.config_ctl_hi1_val = 0x82aa299c,
	.test_ctl_val = 0x00000000,
	.test_ctl_hi_val = 0x00000003,
	.test_ctl_hi1_val = 0x00009000,
	.test_ctl_hi2_val = 0x00000034,
	.user_ctl_val = 0x00000000,
	.user_ctl_hi_val = 0x00000005,
};

static struct clk_alpha_pll videocc_pll0 = {
	.offset = 0x0,
	.vco_table = lucid_ole_vco,
	.num_vco = ARRAY_SIZE(lucid_ole_vco),
	.regs = clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_LUCID_OLE],
	.clkr = {
		.hw.init = &(const struct clk_init_data) {
			.name = "videocc_pll0",
			.parent_data = &(const struct clk_parent_data) {
				.index = DT_BI_TCXO,
			},
			.num_parents = 1,
			.ops = &clk_alpha_pll_lucid_evo_ops,
		},
	},
};

static const struct parent_map videocc_parent_map_0[] = {
	{ P_BI_TCXO, 0 },
};

static const struct clk_parent_data videocc_parent_data_0[] = {
	{ .index = DT_BI_TCXO },
};

static const struct clk_parent_data videocc_parent_data_0_ao[] = {
	{ .index = DT_BI_TCXO_AO },
};

static const struct parent_map videocc_parent_map_1[] = {
	{ P_BI_TCXO, 0 },
	{ P_VIDEOCC_PLL0_OUT_MAIN, 1 },
};

static const struct clk_parent_data videocc_parent_data_1[] = {
	{ .index = DT_BI_TCXO },
	{ .hw = &videocc_pll0.clkr.hw },
};

static const struct parent_map videocc_parent_map_2[] = {
	{ P_SLEEP_CLK, 0 },
};

static const struct clk_parent_data videocc_parent_data_2_ao[] = {
	{ .index = DT_SLEEP_CLK },
};

static const struct freq_tbl ftbl_videocc_ahb_clk_src[] = {
	F(19200000, P_BI_TCXO, 1, 0, 0),
	{ }
};

static struct clk_rcg2 videocc_ahb_clk_src = {
	.cmd_rcgr = 0x8030,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = videocc_parent_map_0,
	.freq_tbl = ftbl_videocc_ahb_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "videocc_ahb_clk_src",
		.parent_data = videocc_parent_data_0_ao,
		.num_parents = ARRAY_SIZE(videocc_parent_data_0_ao),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_videocc_mvs0_clk_src[] = {
	F(604800000, P_VIDEOCC_PLL0_OUT_MAIN, 1, 0, 0),
	F(720000000, P_VIDEOCC_PLL0_OUT_MAIN, 1, 0, 0),
	F(1014000000, P_VIDEOCC_PLL0_OUT_MAIN, 1, 0, 0),
	F(1098000000, P_VIDEOCC_PLL0_OUT_MAIN, 1, 0, 0),
	F(1332000000, P_VIDEOCC_PLL0_OUT_MAIN, 1, 0, 0),
	F(1656000000, P_VIDEOCC_PLL0_OUT_MAIN, 1, 0, 0),
	{ }
};

static struct clk_rcg2 videocc_mvs0_clk_src = {
	.cmd_rcgr = 0x8000,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = videocc_parent_map_1,
	.freq_tbl = ftbl_videocc_mvs0_clk_src,
	//.enable_safe_config = true,
	//.flags = HW_CLK_CTRL_MODE,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "videocc_mvs0_clk_src",
		.parent_data = videocc_parent_data_1,
		.num_parents = ARRAY_SIZE(videocc_parent_data_1),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static const struct freq_tbl ftbl_videocc_sleep_clk_src[] = {
	F(32000, P_SLEEP_CLK, 1, 0, 0),
	{ }
};

static struct clk_rcg2 videocc_sleep_clk_src = {
	.cmd_rcgr = 0x8128,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = videocc_parent_map_2,
	.freq_tbl = ftbl_videocc_sleep_clk_src,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "videocc_sleep_clk_src",
		.parent_data = videocc_parent_data_2_ao,
		.num_parents = ARRAY_SIZE(videocc_parent_data_2_ao),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static struct clk_rcg2 videocc_xo_clk_src = {
	.cmd_rcgr = 0x810c,
	.mnd_width = 0,
	.hid_width = 5,
	.parent_map = videocc_parent_map_0,
	.freq_tbl = ftbl_videocc_ahb_clk_src,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "videocc_xo_clk_src",
		.parent_data = videocc_parent_data_0,
		.num_parents = ARRAY_SIZE(videocc_parent_data_0),
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_rcg2_ops,
	},
};

static struct clk_regmap_div videocc_mvs0_div_clk_src = {
	.reg = 0x80c4,
	.shift = 0,
	.width = 4,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "videocc_mvs0_div_clk_src",
		.parent_hws = (const struct clk_hw*[]) {
			&videocc_mvs0_clk_src.clkr.hw,
		},
		.num_parents = 1,
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_regmap_div_ro_ops,
	},
};

static struct clk_regmap_div videocc_mvs0c_div2_div_clk_src = {
	.reg = 0x8070,
	.shift = 0,
	.width = 4,
	.clkr.hw.init = &(const struct clk_init_data) {
		.name = "videocc_mvs0c_div2_div_clk_src",
		.parent_hws = (const struct clk_hw*[]) {
			&videocc_mvs0_clk_src.clkr.hw,
		},
		.num_parents = 1,
		.flags = CLK_SET_RATE_PARENT,
		.ops = &clk_regmap_div_ro_ops,
	},
};

static struct clk_branch videocc_mvs0_clk = {
	.halt_reg = 0x80b8,
	.halt_check = BRANCH_HALT_VOTED,
	.hwcg_reg = 0x80b8,
	.hwcg_bit = 1,
	.clkr = {
		.enable_reg = 0x80b8,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "videocc_mvs0_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&videocc_mvs0_div_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch videocc_mvs0_shift_clk = {
	.halt_reg = 0x8144,
	.halt_check = BRANCH_HALT_VOTED,
	.hwcg_reg = 0x8144,
	.hwcg_bit = 1,
	.clkr = {
		.enable_reg = 0x8144,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "videocc_mvs0_shift_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&videocc_xo_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch videocc_mvs0c_clk = {
	.halt_reg = 0x8064,
	.halt_check = BRANCH_HALT,
	.clkr = {
		.enable_reg = 0x8064,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "videocc_mvs0c_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&videocc_mvs0c_div2_div_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_branch videocc_mvs0c_shift_clk = {
	.halt_reg = 0x8148,
	.halt_check = BRANCH_HALT_VOTED,
	.hwcg_reg = 0x8148,
	.hwcg_bit = 1,
	.clkr = {
		.enable_reg = 0x8148,
		.enable_mask = BIT(0),
		.hw.init = &(const struct clk_init_data) {
			.name = "videocc_mvs0c_shift_clk",
			.parent_hws = (const struct clk_hw*[]) {
				&videocc_xo_clk_src.clkr.hw,
			},
			.num_parents = 1,
			.flags = CLK_SET_RATE_PARENT,
			.ops = &clk_branch2_ops,
		},
	},
};

static struct clk_regmap *videocc_sm7635_clocks[] = {
	[VIDEOCC_AHB_CLK_SRC] = &videocc_ahb_clk_src.clkr,
	[VIDEOCC_MVS0_CLK] = &videocc_mvs0_clk.clkr,
	[VIDEOCC_MVS0_CLK_SRC] = &videocc_mvs0_clk_src.clkr,
	[VIDEOCC_MVS0_DIV_CLK_SRC] = &videocc_mvs0_div_clk_src.clkr,
	[VIDEOCC_MVS0_SHIFT_CLK] = &videocc_mvs0_shift_clk.clkr,
	[VIDEOCC_MVS0C_CLK] = &videocc_mvs0c_clk.clkr,
	[VIDEOCC_MVS0C_DIV2_DIV_CLK_SRC] = &videocc_mvs0c_div2_div_clk_src.clkr,
	[VIDEOCC_MVS0C_SHIFT_CLK] = &videocc_mvs0c_shift_clk.clkr,
	[VIDEOCC_PLL0] = &videocc_pll0.clkr,
	[VIDEOCC_SLEEP_CLK_SRC] = &videocc_sleep_clk_src.clkr,
	[VIDEOCC_XO_CLK_SRC] = &videocc_xo_clk_src.clkr,
};

static const struct qcom_reset_map videocc_sm7635_resets[] = {
	[VIDEOCC_INTERFACE_BCR] = { 0x80f0 },
	[VIDEOCC_MVS0_BCR] = { 0x80a0 },
	[VIDEOCC_MVS0C_CLK_ARES] = { .reg = 0x8064, .bit = 2, .udelay = 1000 },
	[VIDEOCC_MVS0C_BCR] = { 0x8048 },
};

static const struct regmap_config videocc_sm7635_regmap_config = {
	.reg_bits = 32,
	.reg_stride = 4,
	.val_bits = 32,
	.max_register = 0x9f50,
	.fast_io = true,
};

static struct qcom_cc_desc videocc_sm7635_desc = {
	.config = &videocc_sm7635_regmap_config,
	.clks = videocc_sm7635_clocks,
	.num_clks = ARRAY_SIZE(videocc_sm7635_clocks),
	.resets = videocc_sm7635_resets,
	.num_resets = ARRAY_SIZE(videocc_sm7635_resets),
};

static const struct of_device_id videocc_sm7635_match_table[] = {
	{ .compatible = "qcom,sm7635-videocc" },
	{ }
};
MODULE_DEVICE_TABLE(of, videocc_sm7635_match_table);

static int videocc_sm7635_probe(struct platform_device *pdev)
{
	struct regmap *regmap;
	int ret;

	ret = devm_pm_runtime_enable(&pdev->dev);
	if (ret)
		return ret;

	ret = pm_runtime_resume_and_get(&pdev->dev);
	if (ret)
		return ret;

	regmap = qcom_cc_map(pdev, &videocc_sm7635_desc);
	if (IS_ERR(regmap)) {
		pm_runtime_put(&pdev->dev);
		return PTR_ERR(regmap);
	}

	clk_lucid_ole_pll_configure(&videocc_pll0, regmap, &videocc_pll0_config);

	/* Keep some clocks always-on */
	qcom_branch_set_clk_en(regmap, 0x80f4); /* VIDEOCC_AHB_CLK */
	qcom_branch_set_clk_en(regmap, 0x8140); /* VIDEOCC_SLEEP_CLK */
	qcom_branch_set_clk_en(regmap, 0x8124); /* VIDEOCC_XO_CLK */

	return qcom_cc_really_probe(&pdev->dev, &videocc_sm7635_desc, regmap);
}

static struct platform_driver videocc_sm7635_driver = {
	.probe = videocc_sm7635_probe,
	.driver = {
		.name = "videocc-sm7635",
		.of_match_table = videocc_sm7635_match_table,
	},
};

static int __init videocc_sm7635_init(void)
{
	return platform_driver_register(&videocc_sm7635_driver);
}
subsys_initcall(videocc_sm7635_init);

static void __exit videocc_sm7635_exit(void)
{
	platform_driver_unregister(&videocc_sm7635_driver);
}
module_exit(videocc_sm7635_exit);

MODULE_DESCRIPTION("QTI VIDEOCC SM7635 Driver");
MODULE_LICENSE("GPL");
