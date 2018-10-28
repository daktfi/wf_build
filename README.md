# wf_build
Calculates best builds and rivens for warframe weapons.

Does not include lots of actually important stuff (like procs and, most important, vs.faction damage optimization). However, given some game experience, you can quickly get starting point and the numbers "how good is good".

The riven calculation is brute force check of all possible riven buffs combinations followed by finding a best build with current combination "forced".

The best build is found (currently) as iterative search over all the available mods for one giving the best respective damage boost (single shot, burst dps, and sustained dps for firing weapons and dps at combo multiplier x1, x2, and x3 for melee).

Planning to adding some recursion to better catch synergy between mods (like take not the mod giving best damage boost, but the one for which exists other mod such that the PAIR gives best boost).

Also planning to add some faction-specific damage optimization, however, for that I need to easy and nice way to select right health types and their balance.

The first version was console (and was quite enough for me), however, normal GUI greatly simplifies many things, so here goes Qt (5.11 at the moment). If there are any license issues - I'll be happy to fix them the instant I got notified. MY CODE is free to use by everyone under GPLv3.

Also there's (incomplete and broken at the moment) translation to Russian. I think I need to add to db some localized names/descriptions for mods/weapons, just don't want to do it right now.

Build note: program store data in combo.sqlite sqlite database file. It must be in current path, so, consider disabling shadow build in QtCreator to simplify things.
