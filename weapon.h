#ifndef WEAPON_H
#define WEAPON_H
#include <QString>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

enum elements {
	element_none = 0,
	element_cold,
	element_electricity,
	element_heat,
	element_toxin,
	element_blast,
	element_corrosive,
	element_gas,
	element_magnetic,
	element_radiation,
	element_viral,
	element_count
};

enum weapon_types {
	wpn_none = -1,
	wpn_rifle,		// 0
	wpn_shotgun,	// 1
	wpn_secondary,	// 2
	wpn_melee,		// 3
	wpn_archgun,	// 4
	wpn_archmelee,	// 5
	wpn_primary		// 6
};

enum wpn_subtypes {
	wpn_normal,
	wpn_beam,
	wpn_throw = wpn_beam,
	wpn_bow,
	wpn_sniper
};

enum wpn_stat {
	stat_none = -1,
	fire_none = stat_none,
	melee_none = stat_none,
	stat_damage,
	fire_damage = stat_damage,
	melee_damage = stat_damage,
	stat_impact,
	fire_impact = stat_impact,
	melee_impact = stat_impact,
	stat_puncture,
	fire_puncture = stat_puncture,
	melee_puncture = stat_puncture,
	stat_slash,
	fire_slash = stat_slash,
	melee_slash = stat_slash,
	stat_crit,
	fire_crit = stat_crit,
	melee_crit = stat_crit,
	stat_mult,
	fire_mult = stat_mult,
	melee_mult = stat_mult,
	stat_cold,
	fire_cold = stat_cold,
	melee_cold = stat_cold,
	stat_electricity,
	fire_electricity = stat_electricity,
	melee_electricity = stat_electricity,
	stat_heat,
	fire_heat = stat_heat,
	melee_heat = stat_heat,
	stat_toxin,
	fire_toxin = stat_toxin,
	melee_toxin = stat_toxin,
	stat_proc,
	fire_proc = stat_proc,
	melee_proc = stat_proc,
	stat_proc_duration,
	fire_proc_duration = stat_proc_duration,
	melee_proc_duration = stat_proc_duration,
	stat_corpus,
	fire_corpus = stat_corpus,
	melee_corpus = stat_corpus,
	stat_corrupted,
	fire_corrupted = stat_corrupted,
	melee_corrupted = stat_corrupted,
	stat_grineer,
	fire_grineer = stat_grineer,
	melee_grineer = stat_grineer,
	stat_infested,
	fire_infested = stat_infested,
	melee_infested = stat_infested,
	stat_rof,
	fire_rof = stat_rof,
	melee_rof = stat_rof,
	fire_multishot,
	melee_ch_efficiency = fire_multishot,
	fire_magazine,
	melee_ch_damage = fire_magazine,
	fire_ammo,
	melee_finish_damage = fire_ammo,
	fire_flight,
	melee_range = fire_flight,
	fire_reload,
	melee_slide_crit = fire_reload,
	fire_recoil,
	melee_combo = fire_recoil,
	fire_zoom,
	melee_combocrit	= fire_zoom,
	fire_punch,
	melee_comboproc = fire_punch,
	fire_accuracy,
	melee_count = fire_accuracy,
	fire_count,
	stat_count = fire_count
};

class weapon
{
public:
	weapon() {}

	weapon( const QSqlQuery &query )
	{	// wpn_name, impact, puncture, slash, element, ele_dmg,
		// crit, mult, status, rof, rnd, pellets, reload, magazine,
		// ammo, wpn_type, subtype, dispo, desc, regen
		name = query.value( 0 ).toString();
		impact = query.value( 1 ).toDouble();
		puncture = query.value( 2 ).toDouble();
		slash = query.value( 3 ).toDouble();
		element = elements( query.value( 4 ).toInt() );
		elemental = query.value( 5 ).toDouble();
		crit = query.value( 6 ).toDouble();
		mult = query.value( 7 ).toDouble();
		proc = query.value( 8 ).toDouble();
		rof = query.value( 9 ).toDouble();
		rnd = query.value( 10 ).toInt();
		pellets = query.value( 11 ).toInt();
		reload = query.value( 12 ).toDouble();
		magazine = query.value( 13 ).toInt();
		ammo = query.value( 14 ).toInt();
		type = query.value( 15 ).toInt();
		subtype = query.value( 16 ).toInt();
		dispo = query.value( 17 ).toDouble();
		desc = query.value( 18 ).toString();
		regen = query.value( 19 ).toDouble();
	}


	~weapon() {}

	QString name, desc;
	double impact = 0, puncture = 0, slash = 0, elemental = 0, crit = 0,
		   mult = 0, proc = 0, rof = 0, reload = 0, dispo = 1.0, regen = 0,
		   ele_dmg[element_count] = {0}, dps_1 = 0, dps_burst = 0, dps_sust = 0,
		   dps_x10 = 0, dps_x15 = 0, dps_x20 = 0, dps_x25 = 0, dps_x30 = 0,
		   proc_x10 = 0, proc_x15 = 0, proc_x20 = 0, proc_x25 = 0, proc_x30 = 0,
		   pellets = 1;
	int magazine = 0, ammo = 0, type = wpn_rifle, subtype = wpn_normal, rnd = 1;
	elements element = element_none;
};

class mod
{
public:
	mod() {}
	mod( const QSqlQuery &query )
	{  //mod_name, stat1, val1, stat2, val2, stat3, val3, stat4, val4, mod_type, subtype, exact_wpn, desc
		name = query.value( 0 ).toString();
		stat1 = query.value( 1 ).toInt();
		val1 = query.value( 2 ).toDouble();
		stat2 = query.value( 3 ).toInt();
		val2 = query.value( 4 ).toDouble();
		stat3 = query.value( 5 ).toInt();
		val3 = query.value( 6 ).toDouble();
		stat4 = query.value( 7 ).toInt();
		val4 = query.value( 8 ).toDouble();
		type = query.value( 9 ).toInt();
		subtype = query.value( 10 ).toInt();
		exact_wpn = query.value( 11 ).toString();
		desc = query.value( 12 ).toString();
	}

	~mod() {}

	QString name, exact_wpn, desc;
	int type = -1, subtype = -1, stat1 = -1, stat2 = -1,
		stat3 = -1, stat4 = -1;
	double val1 = 0, val2 = 0, val3 = 0, val4 = 0;
};

#endif // WEAPON_H

