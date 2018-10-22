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
	wpn_rifle,
	wpn_shotgun,
	wpn_secondary,
	wpn_melee
};

enum wpn_subtypes {
	wpn_normal,
	wpn_beam,
	wpn_throw = wpn_beam,
	wpn_bow,
	wpn_sniper
};

enum wpn_stat_firing {
	fire_none = -1,
	fire_multishot,
	fire_damage,
	fire_impact,
	fire_puncture,
	fire_slash,
	fire_crit,
	fire_mult,
	fire_cold,
	fire_electricity,
	fire_heat,
	fire_toxin,
	fire_proc,
	fire_proc_duration,
	fire_corpus,
	fire_corrupted,
	fire_grineer,
	fire_infested,
	fire_rof,
	fire_magazine,
	fire_ammo,
	fire_flight,
	fire_reload,
	fire_recoil,
	fire_zoom,
	fire_punch,
	fire_accuracy,
	fire_count
};

enum wpn_stat_melee {
	melee_none = -1,
	melee_damage,
	melee_impact,
	melee_puncture,
	melee_slash,
	melee_crit,
	melee_mult,
	melee_cold,
	melee_electricity,
	melee_heat,
	melee_toxin,
	melee_proc,
	melee_proc_duration,
	melee_corpus,
	melee_corrupted,
	melee_grineer,
	melee_infested,
	melee_rof,
	melee_ch_efficiency,
	melee_ch_damage,
	melee_finish_damage,
	melee_range,
	melee_slide_crit,
	melee_combo,
	melee_combocrit,
	melee_comboproc,
	melee_count
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

