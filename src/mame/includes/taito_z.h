// license:BSD-3-Clause
// copyright-holders:David Graves
/*************************************************************************

    Taito Z system

*************************************************************************/
#ifndef MAME_INCLUDES_TAITO_Z_H
#define MAME_INCLUDES_TAITO_Z_H

#pragma once

#include "audio/taitosnd.h"
#include "machine/eepromser.h"
#include "machine/taitoio.h"
#include "sound/flt_vol.h"
#include "video/tc0100scn.h"
#include "video/tc0110pcr.h"
#include "video/tc0150rod.h"
#include "video/tc0480scp.h"
#include "screen.h"


class taitoz_state : public driver_device
{
public:
	taitoz_state(const machine_config &mconfig, device_type type, const char *tag) :
		driver_device(mconfig, type, tag),
		m_spriteram(*this, "spriteram"),
		m_spritemap(*this, "spritemap"),
		m_maincpu(*this, "maincpu"),
		m_subcpu(*this, "sub"),
		m_tc0480scp(*this, "tc0480scp"),
		m_tc0150rod(*this, "tc0150rod"),
		m_tc0100scn(*this, "tc0100scn"),
		m_tc0110pcr(*this, "tc0110pcr"),
		m_tc0040ioc(*this, "tc0040ioc"),
		m_tc0220ioc(*this, "tc0220ioc"),
		m_tc0510nio(*this, "tc0510nio"),
		m_tc0140syt(*this, "tc0140syt"),
		m_screen(*this, "screen"),
		m_gfxdecode(*this, "gfxdecode"),
		m_filter(*this, {"2610.1.r", "2610.1.l", "2610.2.r", "2610.2.l"}),
		m_gas(*this, "GAS"),
		m_brake(*this, "BRAKE"),
		m_steer(*this, "STEER"),
		m_stickx(*this, "STICKX"),
		m_sticky(*this, "STICKY")
	{ }

	DECLARE_CUSTOM_INPUT_MEMBER(gas_pedal_r);
	DECLARE_CUSTOM_INPUT_MEMBER(brake_pedal_r);
	template <int axis> DECLARE_CUSTOM_INPUT_MEMBER(adstick_r);

	void bshark_base(machine_config &config);
	void bshark(machine_config &config);
	void bsharkjjs(machine_config &config);

protected:
	virtual void device_post_load() override;
	virtual void machine_start() override;
	virtual void machine_reset() override;

	void screen_config(machine_config &config, int vdisp_start, int vdisp_end);

	void cpua_ctrl_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	void coin_control_w(u8 data);
	void pancontrol_w(offs_t offset, u8 data);

	void bshark_draw_sprites_16x8(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int y_offs);

	/* memory pointers */
	required_shared_ptr<u16> m_spriteram;
	required_region_ptr<u16> m_spritemap;

	/* devices */
	required_device<cpu_device> m_maincpu;
	required_device<cpu_device> m_subcpu;
	optional_device<tc0480scp_device> m_tc0480scp;
	optional_device<tc0150rod_device> m_tc0150rod;
	optional_device<tc0100scn_device> m_tc0100scn;
	optional_device<tc0110pcr_device> m_tc0110pcr;
	optional_device<tc0040ioc_device> m_tc0040ioc;
	optional_device<tc0220ioc_device> m_tc0220ioc;
	optional_device<tc0510nio_device> m_tc0510nio;
	optional_device<tc0140syt_device> m_tc0140syt;  // bshark & spacegun miss the CPUs which shall use TC0140
	required_device<screen_device> m_screen;
	required_device<gfxdecode_device> m_gfxdecode;
	optional_device_array<filter_volume_device, 4> m_filter;
	optional_ioport m_gas;
	optional_ioport m_brake;
	optional_ioport m_steer;
	optional_ioport m_stickx;
	optional_ioport m_sticky;

	/* misc */
	u16      m_cpua_ctrl;

private:
	u32 screen_update_bshark(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void parse_cpu_control();

	void bshark_cpub_map(address_map &map);
	void bshark_map(address_map &map);
	void bsharkjjs_map(address_map &map);
};


class taitoz_z80_sound_state : public taitoz_state
{
public:
	taitoz_z80_sound_state(const machine_config &mconfig, device_type type, const char *tag) :
		taitoz_state(mconfig, type, tag),
		m_audiocpu(*this, "audiocpu"),
		m_z80bank(*this, "z80bank")
	{
	}

	void aquajack(machine_config &config);

protected:
	virtual void machine_start() override;

	u16 dblaxle_steer_input_r(offs_t offset);

	u32 screen_update_chasehq(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);

	void z80_sound_map(address_map &map);

	required_device<cpu_device> m_audiocpu;

private:
	u16 aquajack_unknown_r();
	void sound_bankswitch_w(u8 data);

	void chasehq_draw_sprites_16x16(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int y_offs);
	void aquajack_draw_sprites_16x8(screen_device &screen, bitmap_ind16 &bitmap,const rectangle &cliprect,int y_offs);
	u32 screen_update_aquajack(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);

	void aquajack_map(address_map &map);
	void aquajack_cpub_map(address_map &map);

	required_memory_bank m_z80bank;
};


class contcirc_state : public taitoz_z80_sound_state
{
public:
	contcirc_state(const machine_config &mconfig, device_type type, const char *tag) :
		taitoz_z80_sound_state(mconfig, type, tag),
		m_shutter_out(*this, "shutter%u", 0U)
	{
	}

	void contcirc(machine_config &config);
	void enforce(machine_config &config);

protected:
	virtual void machine_start() override;
	virtual void video_start() override;

private:
	u8 contcirc_input_bypass_r();
	void contcirc_out_w(u8 data);
	DECLARE_WRITE_LINE_MEMBER(scope_vblank);

	void contcirc_draw_sprites_16x8(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int y_offs);
	u32 screen_update_contcirc(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);

	void contcirc_map(address_map &map);
	void contcirc_cpub_map(address_map &map);
	void enforce_map(address_map &map);
	void enforce_cpub_map(address_map &map);

	output_finder<2> m_shutter_out;

	int      m_road_palbank;
	int      m_shutter_toggle;
	u8       m_shutter_control;
};


class chasehq_state : public taitoz_z80_sound_state
{
public:
	chasehq_state(const machine_config &mconfig, device_type type, const char *tag) :
		taitoz_z80_sound_state(mconfig, type, tag),
		m_unknown_io(*this, "UNK%u", 1U),
		m_lamps(*this, "lamp%u", 0U)
	{
	}

	void chasehq(machine_config &config);

protected:
	virtual void machine_start() override;

private:
	void chasehq_cpua_ctrl_w(offs_t offset, u16 data, u16 mem_mask = ~0);
	u8 chasehq_input_bypass_r();
	u16 chasehq_motor_r(offs_t offset);
	void chasehq_motor_w(offs_t offset, u16 data);

	void chasehq_map(address_map &map);
	void chasehq_cpub_map(address_map &map);

	required_ioport_array<4> m_unknown_io;
	output_finder<2> m_lamps;
};


class sci_state : public taitoz_z80_sound_state
{
public:
	sci_state(const machine_config &mconfig, device_type type, const char *tag) :
		taitoz_z80_sound_state(mconfig, type, tag)
	{
	}

	void sci(machine_config &config);
	void racingb(machine_config &config);

protected:
	enum
	{
		TIMER_TAITOZ_INTERRUPT6,
	};

	virtual void device_timer(emu_timer &timer, device_timer_id id, int param) override;
	virtual void machine_start() override;
	virtual void machine_reset() override;
	virtual void video_start() override;

private:
	u16 sci_steer_input_r(offs_t offset);
	u16 sci_spriteframe_r();
	void sci_spriteframe_w(u16 data);

	INTERRUPT_GEN_MEMBER(sci_interrupt);

	void sci_draw_sprites_16x8(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect, int y_offs);
	u32 screen_update_sci(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	u32 screen_update_racingb(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);

	void sci_map(address_map &map);
	void sci_cpub_map(address_map &map);
	void racingb_map(address_map &map);
	void racingb_cpub_map(address_map &map);

	int      m_sci_spriteframe;
	s32      m_sci_int6;
};


class nightstr_state : public taitoz_z80_sound_state
{
public:
	nightstr_state(const machine_config &mconfig, device_type type, const char *tag) :
		taitoz_z80_sound_state(mconfig, type, tag),
		m_motor_dir(*this, "Motor_%u_Direction", 1U),
		m_motor_speed(*this, "Motor_%u_Speed", 1U),
		m_motor_debug(*this, "motor_debug")
	{
	}

	void nightstr(machine_config &config);

protected:
	virtual void machine_start() override;

private:
	void nightstr_motor_w(offs_t offset, u16 data);

	void nightstr_map(address_map &map);
	void nightstr_cpub_map(address_map &map);

	output_finder<3> m_motor_dir;
	output_finder<3> m_motor_speed;
	output_finder<> m_motor_debug;
};


class spacegun_state : public taitoz_state
{
public:
	spacegun_state(const machine_config &mconfig, device_type type, const char *tag) :
		taitoz_state(mconfig, type, tag),
		m_eeprom(*this, "eeprom"),
		m_io_eepromout(*this, "EEPROMOUT"),
		m_recoil(*this, "Player%u_Gun_Recoil", 1U)
	{
	}

	void spacegun(machine_config &config);

protected:
	virtual void machine_start() override;

private:
	void spacegun_eeprom_w(u8 data);
	void spacegun_gun_output_w(u16 data);

	void spacegun_draw_sprites_16x8(screen_device &screen, bitmap_ind16 &bitmap,const rectangle &cliprect,int y_offs);
	u32 screen_update_spacegun(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);

	void spacegun_map(address_map &map);
	void spacegun_cpub_map(address_map &map);

	required_device<eeprom_serial_93cxx_device> m_eeprom;
	required_ioport m_io_eepromout;
	output_finder<2> m_recoil;

	u8       m_eep_latch;
};


class dblaxle_state : public taitoz_z80_sound_state
{
public:
	dblaxle_state(const machine_config &mconfig, device_type type, const char *tag) :
		taitoz_z80_sound_state(mconfig, type, tag),
		m_wheel_vibration(*this, "Wheel_Vibration")
	{
	}

	void dblaxle(machine_config &config);

protected:
	virtual void machine_start() override;

private:
	void dblaxle_cpua_ctrl_w(offs_t offset, u16 data, u16 mem_mask = ~0);

	u32 screen_update_dblaxle(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);

	void dblaxle_map(address_map &map);
	void dblaxle_cpub_map(address_map &map);

	output_finder<> m_wheel_vibration;
};

#endif // MAME_INCLUDES_TAITO_Z_H
