['fujia_bomb'] = {
	['name'] = 'fujia_bomb',
	['package'] = 'misc',
	['count'] = 1,
	['layer'] = 0,
	['emission_time'] = 32*l,
	['min_life'] = 606*l,
	['max_life'] = 696*l,
	['min_hori'] = 0,
	['max_hori'] = 360,
	['min_vert'] = 90,
	['max_vert'] = 90,
	['min_spd'] = 0*k,
	['max_spd'] = 0*k,
	['gravity'] = 1200*g,
	['inertia'] = 10,
	['fadeout_time'] = 170*l,
	['bounce'] = false,
	['additive_blend'] = false,
	['start_radius'] = 0,
	['orient_to_movement'] = false,
	['orient_to_parent'] = false,
	['components'] = {
		{
			['name'] = 'flame_bomb',
			['start_scale'] = 79*s,
			['end_scale'] = 80*s,
			['min_rotate'] = -1*r,
			['max_rotate'] = -1*r,
			['start_z'] = 0,
		},
	},
},
['fujia_bomb_smog'] = {
	['name'] = 'fujia_bomb_smog',
	['package'] = 'misc',
	['count'] = 5,
	['layer'] = 2,
	['emission_time'] = 60*l,
	['min_life'] = 2000*l,
	['max_life'] = 3000*l,
	['min_hori'] = 0,
	['max_hori'] = 0,
	['min_vert'] = 60,
	['max_vert'] = 90,
	['min_spd'] = 75*k,
	['max_spd'] = 200*k,
	['gravity'] = 10*g,
	['inertia'] = 10,
	['fadeout_time'] = 5000*l,
	['bounce'] = false,
	['additive_blend'] = false,
	['start_radius'] = 30,
	['orient_to_movement'] = false,
	['orient_to_parent'] = true,
	['components'] = {
		{
			['name'] = 'd5',
			['start_scale'] = 30*s,
			['end_scale'] = 200*s,
			['min_rotate'] = -180*r,
			['max_rotate'] = 130*r,
			['start_z'] = 70,
		},
	},
},
['fujia_bomb_particle'] = {
	['name'] = 'fujia_bomb_particle',
	['package'] = 'misc',
	['count'] = 5,
	['layer'] = 0,
	['emission_time'] = 20*l,
	['min_life'] = 424*l,
	['max_life'] = 848*l,
	['min_hori'] = 0,
	['max_hori'] = 360,
	['min_vert'] = 40,
	['max_vert'] = 50,
	['min_spd'] = 1250*k,
	['max_spd'] = 1250*k,
	['gravity'] = 1109*g,
	['inertia'] = 10,
	['fadeout_time'] = 442*l,
	['bounce'] = true,
	['additive_blend'] = true,
	['start_radius'] = 0,
	['orient_to_movement'] = false,
	['orient_to_parent'] = false,
	['components'] = {
		{
			['name'] = 'firepoint',
			['start_scale'] = 48*s,
			['end_scale'] = 3*s,
			['min_rotate'] = -67*r,
			['max_rotate'] = 62*r,
			['start_z'] = 0,
		},
	},
},
['firetower_attack_before'] = {
	['name'] = 'firetower_attack_before',
	['package'] = 'misc',
	['count'] = 1,
	['layer'] = 0,
	['emission_time'] = 188*l,
	['min_life'] = 2334*l,
	['max_life'] = 2600*l,
	['min_hori'] = 0,
	['max_hori'] = 360,
	['min_vert'] = 60,
	['max_vert'] = 90,
	['min_spd'] = 0*k,
	['max_spd'] = 0*k,
	['gravity'] = 1200*g,
	['inertia'] = 10,
	['fadeout_time'] = 896*l,
	['bounce'] = false,
	['additive_blend'] = false,
	['start_radius'] = 0,
	['orient_to_movement'] = false,
	['orient_to_parent'] = false,
	['components'] = {
		{
			['name'] = 'fire_ball',
			['start_scale'] = 39*s,
			['end_scale'] = 142*s,
			['min_rotate'] = -180*r,
			['max_rotate'] = 180*r,
			['start_z'] = 0,
		},
	},
},
['Town_Hall_lv10'] = {
	['name'] = 'Town_Hall_lv10',
	['package'] = 'misc',
	['count'] = 3,
	['layer'] = 0,
	['emission_time'] = 1200*l,
	['min_life'] = 800*l,
	['max_life'] = 2000*l,
	['min_hori'] = 0,
	['max_hori'] = 360,
	['min_vert'] = 50,
	['max_vert'] = 60,
	['min_spd'] = 500*k,
	['max_spd'] = 450*k,
	['gravity'] = 5000*g,
	['inertia'] = 10,
	['fadeout_time'] = 800*l,
	['bounce'] = true,
	['additive_blend'] = true,
	['start_radius'] = 0,
	['orient_to_movement'] = false,
	['orient_to_parent'] = false,
	['components'] = {
		{
			['name'] = 'firepoint',
			['start_scale'] = 50*s,
			['end_scale'] = 5*s,
			['min_rotate'] = -180*r,
			['max_rotate'] = 180*r,
			['start_z'] = 0,
		},
	},
},
['huoyuansu_die'] = {
	['name'] = 'huoyuansu_die',
	['package'] = 'misc',
	['count'] = 7,
	['layer'] = 0,
	['emission_time'] = 68*l,
	['min_life'] = 400*l,
	['max_life'] = 1000*l,
	['min_hori'] = 0,
	['max_hori'] = 360,
	['min_vert'] = 50,
	['max_vert'] = 90,
	['min_spd'] = 125*k,
	['max_spd'] = 225*k,
	['gravity'] = 10*g,
	['inertia'] = 10,
	['fadeout_time'] = 1408*l,
	['bounce'] = false,
	['additive_blend'] = false,
	['start_radius'] = 0,
	['orient_to_movement'] = false,
	['orient_to_parent'] = false,
	['components'] = {
		{
			['name'] = 'flame',
			['start_scale'] = 3*s,
			['end_scale'] = 27*s,
			['min_rotate'] = 0*r,
			['max_rotate'] = 15*r,
			['start_z'] = 0,
		},
		{
			['name'] = 'grey_smoke',
			['start_scale'] = 9*s,
			['end_scale'] = 33*s,
			['min_rotate'] = 0*r,
			['max_rotate'] = 30*r,
			['start_z'] = 0,
		},
	},
},
['wuzhu_attack_hit_1'] = {
	['name'] = 'wuzhu_attack_hit_1',
	['package'] = 'misc',
	['count'] = 8,
	['layer'] = 0,
	['emission_time'] = 80*l,
	['min_life'] = 728*l,
	['max_life'] = 1212*l,
	['min_hori'] = 0,
	['max_hori'] = 360,
	['min_vert'] = 60,
	['max_vert'] = 70,
	['min_spd'] = 541.75*k,
	['max_spd'] = 750*k,
	['gravity'] = 5000*g,
	['inertia'] = 10,
	['fadeout_time'] = 1062*l,
	['bounce'] = true,
	['additive_blend'] = true,
	['start_radius'] = 0,
	['orient_to_movement'] = false,
	['orient_to_parent'] = false,
	['components'] = {
		{
			['name'] = 'firepoint',
			['start_scale'] = 27*s,
			['end_scale'] = 3*s,
			['min_rotate'] = -67*r,
			['max_rotate'] = 62*r,
			['start_z'] = 0,
		},
	},
},
['wuzhu_attack_particle'] = {
	['name'] = 'wuzhu_attack_particle',
	['package'] = 'misc',
	['count'] = 13,
	['layer'] = 0,
	['emission_time'] = 300*l,
	['min_life'] = 273*l,
	['max_life'] = 303*l,
	['min_hori'] = 0,
	['max_hori'] = 360,
	['min_vert'] = 40,
	['max_vert'] = 90,
	['min_spd'] = 75*k,
	['max_spd'] = 162.5*k,
	['gravity'] = 200*g,
	['inertia'] = 10,
	['fadeout_time'] = 322*l,
	['bounce'] = false,
	['additive_blend'] = false,
	['start_radius'] = 0,
	['orient_to_movement'] = false,
	['orient_to_parent'] = false,
	['components'] = {
		{
			['name'] = 'firepoint',
			['start_scale'] = 2*s,
			['end_scale'] = 30*s,
			['min_rotate'] = 0*r,
			['max_rotate'] = 60*r,
			['start_z'] = 0,
		},
		{
			['name'] = 'black_smoke',
			['start_scale'] = 2*s,
			['end_scale'] = 30*s,
			['min_rotate'] = 0*r,
			['max_rotate'] = 23*r,
			['start_z'] = 0,
		},
	},
},
['yini_attack'] = {
	['name'] = 'yini_attack',
	['package'] = 'misc',
	['count'] = 1,
	['layer'] = 0,
	['emission_time'] = 20*l,
	['min_life'] = 546*l,
	['max_life'] = 606*l,
	['min_hori'] = 0,
	['max_hori'] = 360,
	['min_vert'] = 60,
	['max_vert'] = 90,
	['min_spd'] = 0*k,
	['max_spd'] = 0*k,
	['gravity'] = 1200*g,
	['inertia'] = 10,
	['fadeout_time'] = 20*l,
	['bounce'] = false,
	['additive_blend'] = false,
	['start_radius'] = 0,
	['orient_to_movement'] = false,
	['orient_to_parent'] = false,
	['components'] = {
		{
			['name'] = 'lightning_line',
			['start_scale'] = 100*s,
			['end_scale'] = 100*s,
			['min_rotate'] = -180*r,
			['max_rotate'] = -180*r,
			['start_z'] = 0,
		},
	},
},
['wuzhu_attack_hit_2'] = {
	['name'] = 'wuzhu_attack_hit_2',
	['package'] = 'misc',
	['count'] = 3,
	['layer'] = 0,
	['emission_time'] = 80*l,
	['min_life'] = 122*l,
	['max_life'] = 636*l,
	['min_hori'] = 0,
	['max_hori'] = 360,
	['min_vert'] = 70,
	['max_vert'] = 90,
	['min_spd'] = 197*k,
	['max_spd'] = 318.25*k,
	['gravity'] = 10*g,
	['inertia'] = 10,
	['fadeout_time'] = 412*l,
	['bounce'] = false,
	['additive_blend'] = false,
	['start_radius'] = 0,
	['orient_to_movement'] = false,
	['orient_to_parent'] = false,
	['components'] = {
		{
			['name'] = 'flame',
			['start_scale'] = 3*s,
			['end_scale'] = 45*s,
			['min_rotate'] = -69*r,
			['max_rotate'] = 58*r,
			['start_z'] = 0,
		},
	},
},
['Army_Camp_fire'] = {
	['name'] = 'Army_Camp_fire',
	['package'] = 'misc',
	['count'] = 7,
	['layer'] = 0,
	['emission_time'] = 1200*l,
	['min_life'] = 400*l,
	['max_life'] = 1000*l,
	['min_hori'] = 0,
	['max_hori'] = 360,
	['min_vert'] = 50,
	['max_vert'] = 90,
	['min_spd'] = 125*k,
	['max_spd'] = 225*k,
	['gravity'] = 10*g,
	['inertia'] = 10,
	['fadeout_time'] = 956*l,
	['bounce'] = false,
	['additive_blend'] = false,
	['start_radius'] = 0,
	['orient_to_movement'] = false,
	['orient_to_parent'] = false,
	['components'] = {
		{
			['name'] = 'flame',
			['start_scale'] = 3*s,
			['end_scale'] = 30*s,
			['min_rotate'] = 0*r,
			['max_rotate'] = 15*r,
			['start_z'] = 0,
		},
		{
			['name'] = 'grey_smoke',
			['start_scale'] = 9*s,
			['end_scale'] = 33*s,
			['min_rotate'] = 0*r,
			['max_rotate'] = 30*r,
			['start_z'] = 0,
		},
	},
},
['yini_attack_before'] = {
	['name'] = 'yini_attack_before',
	['package'] = 'misc',
	['count'] = 1,
	['layer'] = 0,
	['emission_time'] = 80*l,
	['min_life'] = 454*l,
	['max_life'] = 576*l,
	['min_hori'] = 0,
	['max_hori'] = 360,
	['min_vert'] = 70,
	['max_vert'] = 90,
	['min_spd'] = 0*k,
	['max_spd'] = 0*k,
	['gravity'] = 10*g,
	['inertia'] = 10,
	['fadeout_time'] = 232*l,
	['bounce'] = false,
	['additive_blend'] = false,
	['start_radius'] = 0,
	['orient_to_movement'] = false,
	['orient_to_parent'] = false,
	['components'] = {
		{
			['name'] = 'lightning_ball',
			['start_scale'] = 82*s,
			['end_scale'] = 82*s,
			['min_rotate'] = 1*r,
			['max_rotate'] = 1*r,
			['start_z'] = 0,
		},
	},
},
