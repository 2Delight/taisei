/*
 * This software is licensed under the terms of the MIT License.
 * See COPYING for further information.
 * ---
 * Copyright (c) 2011-2019, Lukas Weber <laochailan@web.de>.
 * Copyright (c) 2012-2019, Andrei Alexeyev <akari@taisei-project.org>.
 */

#include "taisei.h"

#include "stage5.h"
#include "draw.h"
#include "background_anim.h"
#include "iku.h"
#include "spells/spells.h"
#include "timeline.h"

#include "global.h"
#include "portrait.h"
#include "common_tasks.h"

struct stage5_spells_s stage5_spells = {
	.mid = {
		.static_bomb = {
			{ 16, 17, 18, 19}, AT_SurvivalSpell, "High Voltage “Static Bomb”", 16, 10,
			TASK_INDIRECT_INIT(BossAttack, stage5_midboss_static_bomb),
			NULL, BOSS_DEFAULT_GO_POS, 0,
		},
	},

	.boss = {
		.atmospheric_discharge = {
			{ 0,  1,  2,  3}, AT_Spellcard, "High Voltage “Atmospheric Discharge”", 60, 44000,
			TASK_INDIRECT_INIT(BossAttack, stage5_spell_atmospheric_discharge),
			iku_spell_bg, BOSS_DEFAULT_GO_POS, 5,
		},
		.artificial_lightning = {
			{ 4,  5,  6,  7}, AT_Spellcard, "Charge Sign “Artificial Lightning”", 75, 60000,
			TASK_INDIRECT_INIT(BossAttack, stage5_spell_artificial_lightning),
			iku_spell_bg, BOSS_DEFAULT_GO_POS, 5,
		},
		.induction_field = {
			{12, 13, -1, -1}, AT_Spellcard, "Current Sign “Induction Field”", 60, 50000,
			TASK_INDIRECT_INIT(BossAttack, stage5_spell_induction),
			iku_spell_bg, BOSS_DEFAULT_GO_POS, 5,
		},
		.inductive_resonance = {
			{-1, -1, 14, 15}, AT_Spellcard, "Current Sign “Inductive Resonance”", 60, 50000,
			TASK_INDIRECT_INIT(BossAttack, stage5_spell_induction),
			iku_spell_bg, BOSS_DEFAULT_GO_POS, 5,
		},
		.natural_cathode = {
			{ 8,  9, 10, 11}, AT_Spellcard, "Spark Sign “Natural Cathode”", 60, 44000,
			TASK_INDIRECT_INIT(BossAttack, stage5_spell_natural_cathode),
			iku_spell_bg, BOSS_DEFAULT_GO_POS, 5,
		},
	},

	.extra.overload = {
		{ 0,  1,  2,  3}, AT_ExtraSpell, "Circuit Sign “Overload”", 120, 22000,
		TASK_INDIRECT_INIT(BossAttack, stage5_spell_overload),
		iku_spell_bg, BOSS_DEFAULT_GO_POS, 5,
	},
};

static void stage5_start(void) {
	stage5_drawsys_init();
	stage5_bg_init_fullstage();
	stage_start_bgm("stage5");
	stage_set_voltage_thresholds(255, 480, 860, 1250);
	INVOKE_TASK(stage5_timeline);
}

static void stage5_preload(void) {
	portrait_preload_base_sprite("iku", NULL, RESF_DEFAULT);
	portrait_preload_face_sprite("iku", "normal", RESF_DEFAULT);
	preload_resources(RES_BGM, RESF_OPTIONAL, "stage5", "stage5boss", NULL);
	preload_resources(RES_SPRITE, RESF_DEFAULT,
		"part/blast_huge_halo",
		"part/blast_huge_rays",
		"stage5/noise",
		"stage5/spell_bg",
		"stage5/spell_clouds",
		"stage5/spell_lightning",
	NULL);
	preload_resources(RES_TEXTURE, RESF_DEFAULT,
		"stage5/envmap",
	NULL);
	preload_resources(RES_MATERIAL, RESF_DEFAULT,
		"stage5/metal",
		"stage5/stairs",
		"stage5/wall",
	NULL);
	preload_resources(RES_MODEL, RESF_DEFAULT,
		"stage5/stairs",
		"stage5/wall",
		"stage5/metal",
	NULL);
	preload_resources(RES_SHADER_PROGRAM, RESF_DEFAULT,
		"pbr",
		"zbuf_fog",
	NULL);
	preload_resources(RES_ANIM, RESF_DEFAULT,
		"boss/iku",
		"boss/iku_mid",
	NULL);
	preload_resources(RES_SFX, RESF_OPTIONAL,
		"boom",
		"laser1",
		"enemydeath",
	NULL);
}

static void stage5_end(void) {
	stage5_drawsys_shutdown();
}

static void stage5_spellpractice_start(void) {
	stage5_drawsys_init();
	stage5_bg_init_spellpractice();

	global.boss = stage5_spawn_iku(BOSS_DEFAULT_SPAWN_POS);
	boss_add_attack_from_info(global.boss, global.stage->spell, true);
	boss_engage(global.boss);

	stage_start_bgm("stage5boss");
}

StageProcs stage5_procs = {
	.begin = stage5_start,
	.preload = stage5_preload,
	.end = stage5_end,
	.draw = stage5_draw,
	.shader_rules = stage5_bg_effects,
	.spellpractice_procs = &stage5_spell_procs,
};

StageProcs stage5_spell_procs = {
	.begin = stage5_spellpractice_start,
	.preload = stage5_preload,
	.end = stage5_end,
	.draw = stage5_draw,
	.shader_rules = stage5_bg_effects,
};
