// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generated with linux-mdss-dsi-panel-driver-generator from vendor device tree:
 * Copyright (c) 2013, The Linux Foundation. All rights reserved.
 * Copyright (c) 2025, Danila Tikhonov <danila@jiaxyga.com>
 */

#include <linux/backlight.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>

#include <video/mipi_display.h>

#include <drm/display/drm_dsc.h>
#include <drm/display/drm_dsc_helper.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_modes.h>
#include <drm/drm_panel.h>
#include <drm/drm_probe_helper.h>

struct vxn_rm69220 {
	struct drm_panel panel;
	struct mipi_dsi_device *dsi;
	struct drm_dsc_config dsc;
	struct gpio_desc *reset_gpio;
	struct regulator_bulk_data *supplies;
};

static const struct regulator_bulk_data vxn_rm69220_supplies[] = {
	{ .supply = "vci" },	/* 3p0 */
	{ .supply = "vddio" },	/* 1p8 */
	{ .supply = "vddic" },	/* 3p3 */
};

static inline
struct vxn_rm69220 *to_vxn_rm69220(struct drm_panel *panel)
{
	return container_of(panel, struct vxn_rm69220, panel);
}

static void vxn_rm69220_reset(struct vxn_rm69220 *ctx)
{
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	usleep_range(1000, 2000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	usleep_range(1000, 2000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	msleep(32);
}

static int vxn_rm69220_on(struct vxn_rm69220 *ctx)
{
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfe, 0x40);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xbf, 0x19);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xc0, 0xa6);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfe, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfa, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfe, 0xd2);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x97, 0x08);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, MIPI_DCS_SET_ADDRESS_MODE, 0x11);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x39, 0xab);
	mipi_dsi_dcs_set_pixel_format_multi(&dsi_ctx, 0x30);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x3b, 0x80);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, MIPI_DCS_SET_3D_CONTROL, 0x09);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x3f, 0x58);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, MIPI_DCS_SET_VSYNC_TIMING, 0x04);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x41, 0x38);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x42, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x43, 0x08);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x44, 0x02);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, MIPI_DCS_GET_SCANLINE, 0x1c);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x46, 0x02);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x47, 0x1c);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x48, 0x02);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x49, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x4a, 0x02);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x4b, 0x0e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x4d, 0x20);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x4e, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x4f, 0xbb);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x50, 0x00);
	mipi_dsi_dcs_set_display_brightness_multi(&dsi_ctx, 0x0007);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, MIPI_DCS_WRITE_CONTROL_DISPLAY, 0x0c);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x54, 0x0d);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, MIPI_DCS_WRITE_POWER_SAVE, 0xb7);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x56, 0x0c);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x58, 0xb7);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x59, 0x18);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x5a, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x5b, 0x10);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x5c, 0xf0);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x5d, 0x07);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, MIPI_DCS_SET_CABC_MIN_BRIGHTNESS, 0x10);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x5f, 0x20);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x60, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x61, 0x06);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x62, 0x0f);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x63, 0x0f);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x64, 0x33);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x65, 0x0e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x66, 0x1c);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x67, 0x2a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x68, 0x38);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x69, 0x46);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6a, 0x54);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6b, 0x62);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6c, 0x69);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6d, 0x70);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6e, 0x77);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x6f, 0x79);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x70, 0x7b);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x71, 0x7d);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x72, 0x7e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x73, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x74, 0x02);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x75, 0x22);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x76, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x77, 0x2a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x78, 0x40);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x79, 0x2a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x7a, 0xbe);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x7b, 0x3a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x7c, 0xfc);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x7d, 0x3a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x7e, 0xfa);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x7f, 0x3a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x80, 0xf8);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x81, 0x3b);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x82, 0x38);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x83, 0x3b);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x84, 0x78);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x85, 0x3b);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x86, 0xb6);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x87, 0x4b);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x88, 0xf6);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x89, 0x4c);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8a, 0x34);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8b, 0x4c);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8c, 0x74);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8d, 0x5c);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8e, 0x74);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8f, 0x8c);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x90, 0xf4);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x91, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x92, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x93, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x94, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x95, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x96, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfe, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x2f, 0x05);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfa, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfe, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xc1, 0x03);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xc2, 0x03);
	mipi_dsi_dcs_set_tear_on_multi(&dsi_ctx, MIPI_DSI_DCS_TEAR_MODE_VBLANK);
	mipi_dsi_dcs_set_display_brightness_multi(&dsi_ctx, 0xbb0d);
	mipi_dsi_dcs_exit_sleep_mode_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 80);
	mipi_dsi_dcs_set_display_on_multi(&dsi_ctx);
	mipi_dsi_usleep_range(&dsi_ctx, 10000, 11000);

	return dsi_ctx.accum_err;
}

static int vxn_rm69220_off(struct vxn_rm69220 *ctx)
{
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	mipi_dsi_dcs_set_display_off_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 120);
	mipi_dsi_dcs_enter_sleep_mode_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 100);

	return dsi_ctx.accum_err;
}

static int vxn_rm69220_prepare(struct drm_panel *panel)
{
	struct vxn_rm69220 *ctx = to_vxn_rm69220(panel);
	struct drm_dsc_picture_parameter_set pps;
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };
	int ret;

	ret = regulator_bulk_enable(ARRAY_SIZE(vxn_rm69220_supplies),
				    ctx->supplies);
	if (ret < 0)
		return ret;

	vxn_rm69220_reset(ctx);

	ret = vxn_rm69220_on(ctx);
	if (ret < 0) {
		gpiod_set_value_cansleep(ctx->reset_gpio, 1);
		goto err;
	}

	drm_dsc_pps_payload_pack(&pps, &ctx->dsc);
	mipi_dsi_picture_parameter_set_multi(&dsi_ctx, &pps);
	mipi_dsi_compression_mode_ext_multi(&dsi_ctx, true, MIPI_DSI_COMPRESSION_DSC, 0);

	mipi_dsi_msleep(&dsi_ctx, 28);

	if (dsi_ctx.accum_err < 0) {
		ret = dsi_ctx.accum_err;
		goto err;
	}

	return dsi_ctx.accum_err;
err:
	regulator_bulk_disable(ARRAY_SIZE(vxn_rm69220_supplies),
			ctx->supplies);
	return ret;
}

static int vxn_rm69220_unprepare(struct drm_panel *panel)
{
	struct vxn_rm69220 *ctx = to_vxn_rm69220(panel);
	struct device *dev = &ctx->dsi->dev;
	int ret;

	ret = vxn_rm69220_off(ctx);
	if (ret < 0)
		dev_err(dev, "Failed to un-initialize panel: %d\n", ret);

	gpiod_set_value_cansleep(ctx->reset_gpio, 1);

	return 0;
}

static const struct drm_display_mode vxn_rm69220_modes = {
	/* Let's initialize the highest frequency first */
	{ /* 120Hz mode */
		.clock = (1080 + 48 + 4 + 24) * (2392 + 108 + 4 + 16) * 120 / 1000,
		.hdisplay = 1080,
		.hsync_start = 1080 + 48,
		.hsync_end = 1080 + 48 + 4,
		.htotal = 1080 + 48 + 4 + 24,
		.vdisplay = 2392,
		.vsync_start = 2392 + 108,
		.vsync_end = 2392 + 108 + 4,
		.vtotal = 2392 + 108 + 4 + 16,
		.width_mm = 71,
		.height_mm = 157,
		.type = DRM_MODE_TYPE_DRIVER,
	},
	{ /* 90Hz mode */
		.clock = (1080 + 48 + 4 + 24) * (2392 + 108 + 4 + 16) * 90 / 1000,
		.hdisplay = 1080,
		.hsync_start = 1080 + 48,
		.hsync_end = 1080 + 48 + 4,
		.htotal = 1080 + 48 + 4 + 24,
		.vdisplay = 2392,
		.vsync_start = 2392 + 108,
		.vsync_end = 2392 + 108 + 4,
		.vtotal = 2392 + 108 + 4 + 16,
		.width_mm = 71,
		.height_mm = 157,
		.type = DRM_MODE_TYPE_DRIVER,
	},
	{ /* 60Hz mode */
		.clock = (1080 + 48 + 4 + 24) * (2392 + 108 + 4 + 16) * 60 / 1000,
		.hdisplay = 1080,
		.hsync_start = 1080 + 48,
		.hsync_end = 1080 + 48 + 4,
		.htotal = 1080 + 48 + 4 + 24,
		.vdisplay = 2392,
		.vsync_start = 2392 + 108,
		.vsync_end = 2392 + 108 + 4,
		.vtotal = 2392 + 108 + 4 + 16,
		.width_mm = 71,
		.height_mm = 157,
		.type = DRM_MODE_TYPE_DRIVER,
	},
	{ /* 30Hz mode */
		.clock = (1080 + 48 + 4 + 24) * (2392 + 108 + 4 + 16) * 30 / 1000,
		.hdisplay = 1080,
		.hsync_start = 1080 + 48,
		.hsync_end = 1080 + 48 + 4,
		.htotal = 1080 + 48 + 4 + 24,
		.vdisplay = 2392,
		.vsync_start = 2392 + 108,
		.vsync_end = 2392 + 108 + 4,
		.vtotal = 2392 + 108 + 4 + 16,
		.width_mm = 71,
		.height_mm = 157,
		.type = DRM_MODE_TYPE_DRIVER,
	},
};

static int vxn_rm69220_get_modes(struct drm_panel *panel,
					    struct drm_connector *connector)
{
	int count = 0;

	for (int i = 0; i < ARRAY_SIZE(vxn_rm69220_modes); i++)
		count += drm_connector_helper_get_modes_fixed(connector,
						    &vxn_rm69220_modes[i]);

	return count;
}

static const struct drm_panel_funcs vxn_rm69220_panel_funcs = {
	.prepare = vxn_rm69220_prepare,
	.unprepare = vxn_rm69220_unprepare,
	.get_modes = vxn_rm69220_get_modes,
};

static int vxn_rm69220_bl_update_status(struct backlight_device *bl)
{
	struct mipi_dsi_device *dsi = bl_get_data(bl);
	u16 brightness = backlight_get_brightness(bl);
	int ret;

	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	ret = mipi_dsi_dcs_set_display_brightness_large(dsi, brightness);
	if (ret < 0)
		return ret;

	dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	return 0;
}

// TODO: Check if /sys/class/backlight/.../actual_brightness actually returns
// correct values. If not, remove this function.
static int vxn_rm69220_bl_get_brightness(struct backlight_device *bl)
{
	struct mipi_dsi_device *dsi = bl_get_data(bl);
	u16 brightness;
	int ret;

	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	ret = mipi_dsi_dcs_get_display_brightness_large(dsi, &brightness);
	if (ret < 0)
		return ret;

	dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	return brightness;
}

static const struct backlight_ops vxn_rm69220_bl_ops = {
	.update_status = vxn_rm69220_bl_update_status,
	.get_brightness = vxn_rm69220_bl_get_brightness,
};

static struct backlight_device *
vxn_rm69220_create_backlight(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	const struct backlight_properties props = {
		.type = BACKLIGHT_RAW,
		.brightness = 2047,
		.max_brightness = 4095,
	};

	return devm_backlight_device_register(dev, dev_name(dev), dev, dsi,
					      &vxn_rm69220_bl_ops, &props);
}

static int vxn_rm69220_probe(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct vxn_rm69220 *ctx;
	int ret;

	ctx = devm_kzalloc(dev, sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio))
		return dev_err_probe(dev, PTR_ERR(ctx->reset_gpio),
				     "Failed to get reset-gpios\n");

	ctx->dsi = dsi;
	mipi_dsi_set_drvdata(dsi, ctx);

	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_NO_EOT_PACKET |
			  MIPI_DSI_CLOCK_NON_CONTINUOUS | MIPI_DSI_MODE_LPM;

	drm_panel_init(&ctx->panel, dev, &vxn_rm69220_panel_funcs,
		       DRM_MODE_CONNECTOR_DSI);
	ctx->panel.prepare_prev_first = true;

	ctx->panel.backlight = vxn_rm69220_create_backlight(dsi);
	if (IS_ERR(ctx->panel.backlight))
		return dev_err_probe(dev, PTR_ERR(ctx->panel.backlight),
				     "Failed to create backlight\n");

	drm_panel_add(&ctx->panel);

	dsi->dsc = &ctx->dsc;
	ctx->dsc.dsc_version_major = 1;
	ctx->dsc.dsc_version_minor = 1;
	ctx->dsc.slice_height = 8;
	ctx->dsc.slice_width = 540;
	WARN_ON(1080 % ctx->dsc.slice_width);
	ctx->dsc.slice_count = 1080 / ctx->dsc.slice_width;
	ctx->dsc.bits_per_component = 10;
	ctx->dsc.bits_per_pixel = 8 << 4;
	ctx->dsc.block_pred_enable = true;

	ret = mipi_dsi_attach(dsi);
	if (ret < 0) {
		drm_panel_remove(&ctx->panel);
		return dev_err_probe(dev, ret, "Failed to attach to DSI host\n");
	}

	return 0;
}

static void vxn_rm69220_remove(struct mipi_dsi_device *dsi)
{
	struct vxn_rm69220 *ctx = mipi_dsi_get_drvdata(dsi);
	int ret;

	ret = mipi_dsi_detach(dsi);
	if (ret < 0)
		dev_err(&dsi->dev, "Failed to detach from DSI host: %d\n", ret);

	drm_panel_remove(&ctx->panel);
}

static const struct of_device_id vxn_rm69220_of_match[] = {
	{ .compatible = "vxn,rm69220" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, vxn_rm69220_of_match);

static struct mipi_dsi_driver vxn_rm69220_driver = {
	.probe = vxn_rm69220_probe,
	.remove = vxn_rm69220_remove,
	.driver = {
		.name = "panel-vxn-rm69220",
		.of_match_table = vxn_rm69220_of_match,
	},
};
module_mipi_dsi_driver(vxn_rm69220_driver);

MODULE_AUTHOR("Danila Tikhonov <danila@jiaxyga.com>");
MODULE_DESCRIPTION("DRM driver for VXN RM69220 video mode dsi panel");
MODULE_LICENSE("GPL");
