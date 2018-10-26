#include <QStringList>
#include <QVector>
#include "weapon.h"

QStringList
element_names =
		QStringList() << "None" << "Cold" << "Electricity" << "Heat" << "Toxin" << "Blast"
					  << "Corrosive" << "Gas" << "Magnetic" << "Radiation" << "Viral",
weapon_type_names =
		QStringList() << "Rifle" << "Shotgun" << "Secondary" << "Melee" << "Arch-Gun" << "Arch-Melee",
rifle_subtypes = QStringList() << "Normal" << "Beam" << "Bow" << "Sniper",
shotgun_subtypes = QStringList() << "Normal" << "Beam",
secondary_subtypes = QStringList() << "Normal" << "Beam",
melee_subtypes = QStringList() << "Normal" << "Throwable",
stats_firing =
		QStringList() << "Multishot" << "Damage" << "Impact" << "Puncture" << "Slash" << "Critical %"
					  << "Critical X" << "Cold" << "Electricity" << "Heat" << "Toxin" << "Status %"
					  << "Status Duration" << "vs Corpus" << "vs Corrupted" << "vs Grineer"
					  << "vs Infested" << "Fire Rate" << "Magazine" << "Ammo" << "Flight Speed"
					  << "Reload" << "Recoil" << "Zoom" << "Punch Through" << "Accuracy",
stats_melee =
		QStringList() << "Damage" << "Impact" << "Puncture" << "Slash" << "Critical %" << "Critical X"
					  << "Cold" << "Electricity" << "Heat" << "Toxin" << "Status %" << "Status Duration"
					  << "vs Corpus" << "vs Corrupted" << "vs Grineer" << "vs Infested" << "Attack Speed"
					  << "Channel Efficiency" << "Channel Damage" << "Finisher Damage" << "Range"
					  << "Slide Crit" << "Combo Duration" << "Combo Crit" << "Combo Status";

double
damage_eff_impact[12] = {
	-0.25,	// cloned
	0.0,	// ferrite
	0.0,	// alloy
	0.25,	// machinery
	-0.25,	// flesh
	0.50,	// shield
	0.15,	// proto
	0.0,	// robotic
	0.0,	// infested
	0.0,	// i.flesh
	0.0,	// fossil
	0.0,	// sinew
},
damage_eff_puncture[12] = {
	0.0,	// cloned
	0.50,	// ferrite
	0.15,	// alloy
	0.0,	// machinery
	0.0,	// flesh
	-0.20,	// shield
	-0.50,	// proto
	0.25,	// robotic
	0.0,	// infested
	0.0,	// i.flesh
	0.0,	// fossil
	0.25,	// sinew
},
damage_eff_slash[12] = {
	0.25,	// cloned
	-0.15,	// ferrite
	-0.50,	// alloy
	0.0,	// machinery
	0.25,	// flesh
	0.0,	// shield
	0.0,	// proto
	-0.25,	// robotic
	0.25,	// infested
	0.50,	// i.flesh
	0.15,	// fossil
	0.0,	// sinew
},
damage_eff_cold[12] = {
	0.0,	// cloned
	0.0,	// ferrite
	0.25,	// alloy
	0.0,	// machinery
	0.0,	// flesh
	0.50,	// shield
	0.0,	// proto
	0.0,	// robotic
	0.0,	// infested
	-0.50,	// i.flesh
	-0.25,	// fossil
	0.25,	// sinew
},
damage_eff_elecro[12] = {
	0.0,	// cloned
	0.0,	// ferrite
	-0.50,	// alloy
	0.50,	// machinery
	0.0,	// flesh
	0.0,	// shield
	0.0,	// proto
	0.50,	// robotic
	0.0,	// infested
	0.0,	// i.flesh
	0.0,	// fossil
	0.0,	// sinew
},
damage_eff_heat[12] = {
	0.25,	// cloned
	0.0,	// ferrite
	0.0,	// alloy
	0.0,	// machinery
	0.0,	// flesh
	0.0,	// shield
	-0.50,	// proto
	0.0,	// robotic
	0.25,	// infested
	0.50,	// i.flesh
	0.0,	// fossil
	0.0,	// sinew
},
damage_eff_toxin[12] = {
	0.0,	// cloned
	0.25,	// ferrite
	0.0,	// alloy
	-0.25,	// machinery
	0.50,	// flesh
	0.0,	// shield
	0.0,	// proto
	-0.25,	// robotic
	0.0,	// infested
	0.0,	// i.flesh
	-0.50,	// fossil
	0.0,	// sinew
},
damage_eff_blast[12] = {
	0.0,	// cloned
	-0.25,	// ferrite
	0.0,	// alloy
	0.75,	// machinery
	0.0,	// flesh
	0.0,	// shield
	0.0,	// proto
	0.0,	// robotic
	0.0,	// infested
	0.0,	// i.flesh
	0.50,	// fossil
	-0.50,	// sinew
},
damage_eff_corrosive[12] = {
	0.0,	// cloned
	0.75,	// ferrite
	0.0,	// alloy
	0.0,	// machinery
	0.0,	// flesh
	0.0,	// shield
	-0.50,	// proto
	0.0,	// robotic
	0.0,	// infested
	0.0,	// i.flesh
	0.75,	// fossil
	0.0,	// sinew
},
damage_eff_gas[12] = {
	-0.50,	// cloned
	0.0,	// ferrite
	0.0,	// alloy
	0.0,	// machinery
	-0.25,	// flesh
	0.0,	// shield
	0.0,	// proto
	0.0,	// robotic
	0.75,	// infested
	0.50,	// i.flesh
	0.0,	// fossil
	0.0,	// sinew
},
damage_eff_magnetic[12] = {
	0.0,	// cloned
	0.0,	// ferrite
	-0.50,	// alloy
	0.0,	// machinery
	0.0,	// flesh
	0.75,	// shield
	0.75,	// proto
	0.0,	// robotic
	0.0,	// infested
	0.0,	// i.flesh
	0.0,	// fossil
	0.0,	// sinew
},
damage_eff_radiation[12] = {
	0.0,	// cloned
	0.0,	// ferrite
	0.75,	// alloy
	0.0,	// machinery
	0.0,	// flesh
	-0.25,	// shield
	0.0,	// proto
	0.25,	// robotic
	-0.25,	// infested
	0.0,	// i.flesh
	-0.75,	// fossil
	0.50,	// sinew
},
damage_eff_viral[12] = {
	0.75,	// cloned
	0.0,	// ferrite
	0.0,	// alloy
	-0.25,	// machinery
	0.50,	// flesh
	0.0,	// shield
	0.0,	// proto
	0.0,	// robotic
	-0.50,	// infested
	0.0,	// i.flesh
	0.0,	// fossil
	0.0,	// sinew
},
riven_table_rifle[fire_count] =
{
	0.608,	// fire_multishot,
	1.114,	// fire_damage,
	0.810,	// fire_impact,
	0.810,	// fire_puncture,
	0.810,	// fire_slash,
	1.012,	// fire_crit,
	0.810,	// fire_mult,
	0.608,	// fire_cold,
	0.608,	// fire_electricity,
	0.608,	// fire_heat,
	0.608,	// fire_toxin,
	0.608,	// fire_proc,
	0.675,	// fire_proc_duration,
	0.304,	// fire_corpus,
	0.304,	// fire_corrupted,
	0.304,	// fire_grineer,
	0.304,	// fire_infested,
	0.405,	// fire_rof,
	0.338,	// fire_magazine,
	0.337,	// fire_ammo,
	0.608,	// fire_flight,
	0.338,	// fire_reload,
	-0.608,	// fire_recoil,
	0.405,	// fire_zoom,
	1.8,	// fire_punch,
	0.0,	// fire_accuracy,
},
riven_table_shotgun[fire_count] = {
	0.808,	// fire_multishot,
	1.112,	// fire_damage,
	0.810,	// fire_impact,
	0.810,	// fire_puncture,
	0.810,	// fire_slash,
	0.608,	// fire_crit,
	0.608,	// fire_mult,
	0.608,	// fire_cold,
	0.608,	// fire_electricity,
	0.608,	// fire_heat,
	0.608,	// fire_toxin,
	0.608,	// fire_proc,
	0.675,	// fire_proc_duration,
	0.304,	// fire_corpus,
	0.304,	// fire_corrupted,
	0.304,	// fire_grineer,
	0.304,	// fire_infested,
	0.608,	// fire_rof,
	0.338,	// fire_magazine,
	0.608,	// fire_ammo,
	0.608,	// fire_flight,
	0.338,	// fire_reload,
	-0.608,	// fire_recoil,
	0.0,	// fire_zoom,
	1.8,	// fire_punch,
	0.0,	// fire_accuracy,
},
riven_table_secondary[fire_count] = {
	0.808,	// fire_multishot,
	1.482,	// fire_damage,
	0.810,	// fire_impact,
	0.810,	// fire_puncture,
	0.810,	// fire_slash,
	1.012,	// fire_crit,
	0.608,	// fire_mult,
	0.608,	// fire_cold,
	0.608,	// fire_electricity,
	0.608,	// fire_heat,
	0.608,	// fire_toxin,
	0.608,	// fire_proc,
	0.675,	// fire_proc_duration,
	0.304,	// fire_corpus,
	0.304,	// fire_corrupted,
	0.304,	// fire_grineer,
	0.304,	// fire_infested,
	0.504,	// fire_rof,
	0.338,	// fire_magazine,
	0.608,	// fire_ammo,
	0.608,	// fire_flight,
	0.338,	// fire_reload,
	-0.608,	// fire_recoil,
	0.541,	// fire_zoom,
	1.8,	// fire_punch,
	0.0,	// fire_accuracy,
},
riven_table_melee[melee_count] = {
	1.112,	// melee_damage,
	0.808,	// melee_impact,
	0.808,	// melee_puncture,
	0.808,	// melee_slash,
	0.608,	// melee_crit,
	0.608,	// melee_mult,
	0.608,	// melee_cold,
	0.608,	// melee_electricity,
	0.608,	// melee_heat,
	0.608,	// melee_toxin,
	0.608,	// melee_proc,
	0.675,	// melee_proc_duration,
	0.304,	// melee_corpus,
	0.304,	// melee_corrupted,
	0.304,	// melee_grineer,
	0.304,	// melee_infested,
	0.371,	// melee_rof,
	0.608,	// melee_ch_efficiency,
	1.015,	// melee_ch_damage,
	0.808,	// melee_finish_damage,
	0.808,	// melee_range,
	0.608,	// melee_slide_crit,
	5.5,	// melee_combo,
	0.0,	// melee_combocrit,
	0.0,	// melee_comboproc,
};

//TODO: need some way to differentiate health types from armor types (only two armor types: ferrite and alloy)
//TODO: need decide on same default level of armor, likely 1k-2k

QVector<mod> mods_1, mods_burst, mods_sust, rm_1, rm_burst, rm_sust;
