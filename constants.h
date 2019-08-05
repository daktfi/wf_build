#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QStringList>
#include <QVector>
#include <weapon.h>

extern QStringList
    element_names, weapon_type_names, rifle_subtypes, shotgun_subtypes,
    secondary_subtypes, melee_subtypes, stats_firing, stats_melee;

extern double
    damage_eff_impact[12], damage_eff_puncture[12], damage_eff_slash[12],
    damage_eff_cold[12], damage_eff_elecro[12], damage_eff_heat[12],
    damage_eff_toxin[12], damage_eff_blast[12], damage_eff_corrosive[12],
    damage_eff_gas[12], damage_eff_magnetic[12], damage_eff_radiation[12],
    damage_eff_viral[12],
    riven_table_rifle[fire_count], riven_table_shotgun[fire_count],
    riven_table_secondary[fire_count], riven_table_archgun[fire_count],
    riven_table_melee[melee_count];

extern QVector<mod> mods_1, mods_burst, mods_sust, rm_1, rm_burst, rm_sust;

#endif // CONSTANTS_H

