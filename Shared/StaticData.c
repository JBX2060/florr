// Copyright (C) 2024  Paul Johnson

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.

// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <Shared/StaticData.h>

#include <math.h>
#include <stdio.h>

#include <Shared/Utilities.h>

// clang-format off
struct rr_petal_base_stat_scale const offensive[rr_rarity_id_max] = {
    {1.0, 1.0},
    {1.7, 2.0},
    {2.9, 4.0},
    {5.0, 8.0},
    {8.5,  16},
    {14.5, 48},
    {24.6,144},
    {42.0,432}
};

struct rr_petal_base_stat_scale const defensive[rr_rarity_id_max] = {
    {1.0, 1.0},
    {2.0, 1.7},
    {4.0, 2.9},
    {8.0, 5.0},
    {16,  8.5},
    {48, 14.5},
    {144,24.6},
    {432,42.0}
};

struct rr_petal_data RR_PETAL_DATA[rr_petal_id_max] = {
//   id                     min_rarity              scale       name     dmg      hp   clump   cd 2cd  count
    {rr_petal_id_none,      rr_rarity_id_common,    offensive,  "Secret",0.0f,   0.0f,   0.0f,   0,  0, {0,0,0,0,0,0,0,0}},
    {rr_petal_id_basic,     rr_rarity_id_common,    offensive,  "Petal", 10.0f,  15.0f,   0.0f,  50,  0, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_pellet,    rr_rarity_id_common,    offensive,  "Pellet",12.0f,   5.0f,   0.0f,  18,  0, {1,2,2,3,3,5,6,6}},
    {rr_petal_id_fossil,    rr_rarity_id_common,    offensive,  "Fossil", 5.0f, 100.0f,   0.0f, 100,  0, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_stinger,   rr_rarity_id_common,    offensive,  "Stinger",50.0f,   3.0f,  10.0f, 125,  0, {1,1,1,1,1,3,5,5}},
    {rr_petal_id_berry,     rr_rarity_id_rare,      offensive,  "Berry",  5.0f,   5.0f,  12.0f,  20,  0, {1,1,1,1,1,2,3,3}},
    {rr_petal_id_shell,     rr_rarity_id_rare,      offensive,  "Shell", 30.0f,  16.0f,   0.0f,  75, 13, {1,1,1,1,1,2,3,3}},
    {rr_petal_id_peas,      rr_rarity_id_rare,      offensive,  "Peas",  22.0f,  12.0f,   8.0f,  13, 12, {4,4,4,4,4,4,5,5}},
    {rr_petal_id_leaf,      rr_rarity_id_unusual,   offensive,  "Leaf",   9.0f,   8.0f,   8.0f,  38,  0, {1,1,1,1,1,2,2,2}},
    {rr_petal_id_egg,       rr_rarity_id_unusual,   defensive,  "Egg",    1.0f,  50.0f,   0.0f,  25,100, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_magnet,    rr_rarity_id_rare,      defensive,  "Magnet", 2.0f,  15.0f,   0.0f,  38,  0, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_uranium,   rr_rarity_id_rare,      offensive,  "Uranium",6.0f,  10.0f,   0.0f,  50, 25, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_feather,   rr_rarity_id_common,    defensive,  "Feather",1.0f,   3.0f,   0.0f,  25,  0, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_azalea,    rr_rarity_id_common,    defensive,  "Azalea", 5.0f,  15.0f,   0.0f,  50, 25, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_bone,      rr_rarity_id_common,    defensive,  "Bone",   2.5f,  25.0f,   0.0f,  68,  0, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_web,       rr_rarity_id_rare,      defensive,  "Web",    5.0f,   5.0f,   0.0f,  50, 13, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_seed,      rr_rarity_id_legendary, defensive,  "Seed",   1.0f, 100.0f,   0.0f,  63,  1, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_gravel,    rr_rarity_id_unusual,   offensive,  "Gravel", 9.0f,  20.0f,   0.0f,  20, 10, {1,2,2,2,3,3,4,4}},
    {rr_petal_id_club,      rr_rarity_id_common,    defensive,  "Club",   8.0f, 600.0f,   0.0f, 250,  0, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_crest,     rr_rarity_id_rare,      offensive,  "Crest",  0.0f,   0.0f,   0.0f,   0,  0, {0,0,0,0,0,0,0,0}},
    {rr_petal_id_droplet,   rr_rarity_id_common,    offensive,  "Droplet",15.0f,   5.0f,   0.0f,  37,  0, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_beak,      rr_rarity_id_unusual,   defensive,  "Beak",   5.0f,   5.0f,   0.0f,  55,  0, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_lightning, rr_rarity_id_unusual,   offensive,  "Lightning",13.0f,   1.0f,   0.0f,  63,  0, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_third_eye, rr_rarity_id_legendary, offensive,  "Third Eye",0.0f,   0.0f,   0.0f,   0,  0, {0,0,0,0,0,0,0,0}},
    {rr_petal_id_nest,      rr_rarity_id_legendary, defensive,  "Nest",   5.0f,  25.0f,   0.0f, 375,  1, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_fireball,  rr_rarity_id_unusual,   offensive,  "Fireball",200.0f,   1.0f,   0.0f, 500,  0, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_meat,      rr_rarity_id_common,    defensive,  "Meat",   0.0f, 300.0f,   0.0f, 175, 13, {1,1,1,1,1,1,1,1}},

    {rr_petal_id_mandible,  rr_rarity_id_common,    offensive,  "Mandible",5.0f,  10.0f,   0.0f,  75,  0, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_wax,       rr_rarity_id_unusual,   offensive,  "Wax",   10.0f,  10.0f,  10.0f,  38,  0, {2,2,2,2,2,2,2,2}},
    {rr_petal_id_sand,      rr_rarity_id_common,    offensive,  "Sand",  15.0f,  10.0f,  10.0f,  37,  0, {4,4,4,4,4,4,4,4}},
    {rr_petal_id_mint,      rr_rarity_id_unusual,   offensive,  "Mint",   5.0f,  10.0f,  10.0f,  50, 25, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_rice,      rr_rarity_id_common,    offensive,  "Rice",   5.0f,   0.5f,   0.0f,   1,  0, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_stick,     rr_rarity_id_rare,      offensive,  "Stick",  6.0f,   8.0f,   0.0f,  50,  0, {1,1,1,1,1,1,1,1}},
    {rr_petal_id_missile,   rr_rarity_id_rare,      offensive,  "Missile",2.0f,   0.5f,   0.0f,   3,  3, {1,1,1,1,1,1,1,1}},
};

char const *RR_PETAL_DESCRIPTIONS[rr_petal_id_max] = {
    0,
    "It's just a petal",
    "Low damage, but there's lots",
    "It came from a dino",
    "Ow that hurts",
    "Gives your other petals more energy",
    "Poor snail",
    "Splits in 4. Or maybe 5 if you're a pro",
    "Heals you gradually",
    "Spawns a pet dinosaur to protect you",
    "Increases loot pickup radius. Does not stack",
    "Does damage to the owner and enemies in a large range",
    "It's so light it increases your movement speed. Stacks diminishingly",
    "It heals you",
    "Gives the player armor. Does not stack",
    "It slows everything down",
    "What does this one do",
    "Tiny rocks that stay on the ground and trip dinos",
    "Heavy and sturdy",
    "Increases your maximum vision range. Does not stack",
    "This mysterious petal reverses your petal rotation",
    "Stuns mobs and prevents them from moving",
    "A stunning display",
    "Your petals hate it and want to move further away",
    "Home sweet home",
    "Nice ball bro",
    "Meat meta",
    "Does more damage if target hp is below 50%",
    "Made by the bees",
    "Very fine",
    "Remember to feed your pets"
};

struct rr_mob_data RR_MOB_DATA[rr_mob_id_max] = {
//   id,                                    min_rarity  max_rarity,             name              hp, dmg, size, danger, loot
    // Hell Creek mobs
    {rr_mob_id_triceratops,        rr_rarity_id_epic,   rr_rarity_id_ultimate, "Triceratops",      45, 15, 30.0f, 3,   {{rr_petal_id_leaf,    0.15},{rr_petal_id_fossil,    0.05}}},
    {rr_mob_id_trex,               rr_rarity_id_epic,   rr_rarity_id_ultimate, "T-Rex",            40, 25, 32.0f, 4,   {{rr_petal_id_stinger, 0.03},{rr_petal_id_egg,       0.05},{rr_petal_id_meat,      0.01}}},
    {rr_mob_id_fern,               rr_rarity_id_common, rr_rarity_id_ultimate, "Fern",             10,  5, 24.0f, 1,   {{rr_petal_id_leaf,     0.1},{rr_petal_id_azalea,    0.25}}},
    {rr_mob_id_tree,               rr_rarity_id_common, rr_rarity_id_ultimate, "Tree",            100,  5, 64.0f, 2.15,{{rr_petal_id_leaf,     2.5},{rr_petal_id_peas,       2.5},{rr_petal_id_seed,      0.05}}},
    {rr_mob_id_pteranodon,         rr_rarity_id_epic,   rr_rarity_id_ultimate, "Pteranodon",       40, 15, 20.0f, 5,   {{rr_petal_id_shell,   0.05},{rr_petal_id_beak,      0.15},{rr_petal_id_nest,      0.01}}},
    {rr_mob_id_dakotaraptor,       rr_rarity_id_common, rr_rarity_id_ultimate, "Dakotaraptor",     35, 10, 25.0f, 5,   {{rr_petal_id_crest,    0.1},{rr_petal_id_feather,    0.1},{rr_petal_id_pellet,    0.05}}},
    {rr_mob_id_pachycephalosaurus, rr_rarity_id_common, rr_rarity_id_ultimate, "Pachycephalosaurus",35,20, 20.0f, 3, {{rr_petal_id_fossil,   0.1},{rr_petal_id_berry,      0.1},{rr_petal_id_web,       0.05}}},
    {rr_mob_id_ornithomimus,       rr_rarity_id_common, rr_rarity_id_ultimate, "Ornithomimus",     25, 10, 20.0f, 2,   {{rr_petal_id_feather,  0.1},{rr_petal_id_droplet,   0.05},{rr_petal_id_pellet,     0.1}}},
    {rr_mob_id_ankylosaurus,       rr_rarity_id_epic,   rr_rarity_id_ultimate, "Ankylosaurus",     50, 10, 30.0f, 4,   {{rr_petal_id_club,    0.15},{rr_petal_id_gravel,    0.05}}},
    {rr_mob_id_meteor,             rr_rarity_id_rare,   rr_rarity_id_ultimate, "Meteor",          100, 10, 32.0f, 1,   {{rr_petal_id_magnet,   0.5},{rr_petal_id_uranium,   0.05},{rr_petal_id_fireball,   1.0}}},
    {rr_mob_id_quetzalcoatlus,     rr_rarity_id_epic,   rr_rarity_id_ultimate, "Quetzalcoatlus",   65, 20, 28.0f, 5,   {{rr_petal_id_beak,    0.05},{rr_petal_id_fossil,     0.1},{rr_petal_id_lightning, 0.01}}},
    {rr_mob_id_edmontosaurus,      rr_rarity_id_epic,   rr_rarity_id_ultimate, "Edmontosaurus",    50, 15, 30.0f, 3,   {{rr_petal_id_bone,    0.01},{rr_petal_id_fossil,     0.1},{rr_petal_id_third_eye, 0.05}}},
    {rr_mob_id_pectinodon,         rr_rarity_id_rare,   rr_rarity_id_ultimate, "Pectinodon",       35, 10, 25.0f, 4,   {{rr_petal_id_stick,   0.01}, {rr_petal_id_feather, 0.01}, {rr_petal_id_wax, 0.01}}},

    // Garden mobs
    {rr_mob_id_ant,                rr_rarity_id_common, rr_rarity_id_ultimate, "Ant",              42, 12, 30.0f, 2,   {{rr_petal_id_pellet, 0.1}, {rr_petal_id_rice, 0.15}, {rr_petal_id_mandible, 0.01}}},
    {rr_mob_id_hornet,             rr_rarity_id_rare,   rr_rarity_id_ultimate, "Hornet",          40, 10, 30.0f, 4,   {{rr_petal_id_missile, 0.1},{rr_petal_id_crest, 0.1}}},
    {rr_mob_id_dragonfly,          rr_rarity_id_unusual,rr_rarity_id_ultimate, "Dragonfly",        50, 15, 27.0f, 2,   {{rr_petal_id_mint, 0.1},   {rr_petal_id_third_eye, 0.1}}},
    {rr_mob_id_honeybee,           rr_rarity_id_common, rr_rarity_id_ultimate, "Honeybee",         25, 20, 26.0f, 3,   {{rr_petal_id_wax, 0.1},    {rr_petal_id_stinger, 0.1}}},
    {rr_mob_id_beehive,            rr_rarity_id_rare,   rr_rarity_id_exotic,   "Beehive",         500,  0, 30.0f, 1.15,{}},
    {rr_mob_id_spider,             rr_rarity_id_rare,   rr_rarity_id_ultimate, "Spider",           25, 30, 28.0f, 4,   {{rr_petal_id_web, 0.1},    {rr_petal_id_pellet, 0.25}}},
    {rr_mob_id_house_centipede,    rr_rarity_id_common, rr_rarity_id_ultimate, "House Centipede",  50, 15, 35.0f, 2,   {{rr_petal_id_peas, 0.5},   {rr_petal_id_sand, 0.1}}},

    // Florr Garden Mobs. TODO: Not done
    {rr_mob_id_baby_ant,           rr_rarity_id_common, rr_rarity_id_ultimate, "Baby Ant",         42, 12, 30.0f, 1,   {}},
    {rr_mob_id_worker_ant,         rr_rarity_id_common, rr_rarity_id_ultimate, "Worker Ant",       42, 12, 30.0f, 1,   {}},
    {rr_mob_id_queen_ant,          rr_rarity_id_common, rr_rarity_id_ultimate, "Queen Ant",        42, 12, 30.0f, 1,   {}},
    {rr_mob_id_ladybug,            rr_rarity_id_common, rr_rarity_id_ultimate, "Ladybug",          42, 12, 30.0f, 1,   {}},
    {rr_mob_id_rock,               rr_rarity_id_common, rr_rarity_id_ultimate, "Rock",             42, 12, 30.0f, 1,   {}},

    // Ocean mobs
    {rr_mob_id_kelp,               rr_rarity_id_common, rr_rarity_id_ultimate, "Kelp",             50, 15, 30.0f, 1,   {}},
    {rr_mob_id_seagull,            rr_rarity_id_common, rr_rarity_id_ultimate, "Seagull",          50, 15, 30.0f, 1,   {}},
    {rr_mob_id_kingmackarel,       rr_rarity_id_common, rr_rarity_id_ultimate, "King Mackerel",    50, 15, 30.0f, 1,   {}},
    {rr_mob_id_seasnail,           rr_rarity_id_common, rr_rarity_id_ultimate, "Sea Snail",        50, 15, 30.0f, 1,   {}},

    // Florr Ocean Mobs. TODO: Some not done
    {rr_mob_id_jellyfish,          rr_rarity_id_common, rr_rarity_id_ultimate, "Jellyfish",        50, 15, 30.0f, 1,   {}},
    {rr_mob_id_shell,              rr_rarity_id_common, rr_rarity_id_ultimate, "Shell",            50, 15, 30.0f, 1,   {}},
    {rr_mob_id_crab,               rr_rarity_id_common, rr_rarity_id_ultimate, "Crab",             50, 15, 30.0f, 1,   {}},
    {rr_mob_id_sponge,             rr_rarity_id_common, rr_rarity_id_ultimate, "Sponge",           50, 15, 30.0f, 1,   {}},
    {rr_mob_id_starfish,           rr_rarity_id_common, rr_rarity_id_ultimate, "Starfish",         50, 15, 30.0f, 1,   {}},
    {rr_mob_id_bubble,             rr_rarity_id_common, rr_rarity_id_ultimate, "Bubble",           3,   0, 30.0f, 1,   {}},

    // Portals
    {rr_mob_id_portal_garden,      rr_rarity_id_common, rr_rarity_id_ultimate, "Portal Garden",    500, 0, 30.0f, 1,    {}},
    {rr_mob_id_portal_ocean,       rr_rarity_id_common, rr_rarity_id_ultimate, "Portal Ocean",     500, 0, 30.0f, 1,    {}},
    {rr_mob_id_portal_hell_creek,  rr_rarity_id_common, rr_rarity_id_ultimate, "Portal Hell Creek",500, 0, 30.0f, 1,  {}},

    // Desert
    {rr_mob_id_whirlpool,          rr_rarity_id_common, rr_rarity_id_ultimate, "whirlpool", 600, 10, 30.f, 1, {}}
};

double RR_HELL_CREEK_MOB_ID_RARITY_COEFFICIENTS[rr_mob_id_max] = {
    50,   //tric
    100,  //trex
    15,   //fern
    0.25, //tree
    75,   //pter
    50,   //dako
    25,   //pachy
    40,   //ornith
    25,   //anky
    1,    //meteor
    75,   //quetz
    25,   //edmo
};
double RR_GARDEN_MOB_ID_RARITY_COEFFICIENTS[rr_mob_id_max] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 10};

struct rr_petal_rarity_scale RR_PETAL_RARITY_SCALE[rr_rarity_id_max] = {
    {1,    240, 45,  0.1},
    {1.8,  120, 60,  0.2},
    {3.5,  60,  75,  0.3},
    {6.8,  30,  100, 0.4},
    {12.5, 15,  125, 0.5},
    {24.5, 7.5, 150, 0.6},
    {60,   2.5, 200, 0.7},
    {180,  0.5, 250, 0.8},
};

struct rr_mob_rarity_scale RR_MOB_RARITY_SCALING[rr_rarity_id_max] = {
    {1,      1,    1  },
    {2.4,    1.7,  1.2},
    {6,      2.9,  1.5},
    {14.4,   5,    2  },
    {40,     8.5,  2.8},
    {192,    14.5, 4  },
    {2560,   24.6, 5.5},
    {128000, 42,   7  },
};
// clang-format on
// Mixing up the colors so people don't even know :) I'll revert this soon
uint32_t RR_RARITY_COLORS[rr_rarity_id_max] = {
    0xff7eef6d, 0xffd1b654, 0xff7f4e9b, 0xff2c75ff,
    0xffe85f2a, 0xffb93daf, 0xfff7a629, 0xff48c941};

char const *RR_RARITY_NAMES[rr_rarity_id_max] = {
    "Common",    "Unusual", "Rare",   "Epic",
    "Legendary", "Mythic",  "Exotic", "Ancient"};

double RR_MOB_WAVE_RARITY_COEFFICIENTS[rr_rarity_id_max + 1] = {
    0, 1, 6, 10, 15, 25, 160, 1200, 250};

double RR_DROP_RARITY_COEFFICIENTS[rr_rarity_id_exotic + 2] = {
    0, 1, 8, 15, 40, 150, 500, 2500};
double RR_MOB_LOOT_RARITY_COEFFICIENTS[rr_rarity_id_max] = {2.5, 4,  6,   15,
                                                            35,  50, 125, 150};

static void init_game_coefficients()
{
    double sum = 1;
    double sum2 = 1;
    for (uint64_t a = 1; a < rr_rarity_id_max; ++a)
        RR_MOB_LOOT_RARITY_COEFFICIENTS[a] *=
            RR_MOB_LOOT_RARITY_COEFFICIENTS[a - 1];
    for (uint64_t a = 1; a <= rr_rarity_id_exotic; ++a)
    {
        sum += (RR_DROP_RARITY_COEFFICIENTS[a + 1] =
                    RR_DROP_RARITY_COEFFICIENTS[a] /
                    RR_DROP_RARITY_COEFFICIENTS[a + 1]);
    }
    for (uint64_t a = 1; a <= rr_rarity_id_exotic + 1; ++a)
    {
        RR_DROP_RARITY_COEFFICIENTS[a] = RR_DROP_RARITY_COEFFICIENTS[a] / sum +
                                         RR_DROP_RARITY_COEFFICIENTS[a - 1];
    }
    RR_DROP_RARITY_COEFFICIENTS[rr_rarity_id_exotic + 1] = 1;
    for (uint64_t a = 1; a <= rr_rarity_id_ultimate; ++a)
    {
        sum2 += (RR_MOB_WAVE_RARITY_COEFFICIENTS[a + 1] =
                     RR_MOB_WAVE_RARITY_COEFFICIENTS[a] /
                     RR_MOB_WAVE_RARITY_COEFFICIENTS[a + 1]);
    }
    for (uint64_t a = 1; a <= rr_rarity_id_ultimate + 1; ++a)
    {
        RR_MOB_WAVE_RARITY_COEFFICIENTS[a] =
            RR_MOB_WAVE_RARITY_COEFFICIENTS[a] / sum2 +
            RR_MOB_WAVE_RARITY_COEFFICIENTS[a - 1];
    }
    RR_MOB_WAVE_RARITY_COEFFICIENTS[rr_rarity_id_ultimate + 1] = 1;
    for (uint64_t mob = 1; mob < rr_mob_id_max; ++mob)
    {
        RR_HELL_CREEK_MOB_ID_RARITY_COEFFICIENTS[mob] +=
            RR_HELL_CREEK_MOB_ID_RARITY_COEFFICIENTS[mob - 1];
        RR_GARDEN_MOB_ID_RARITY_COEFFICIENTS[mob] +=
            RR_GARDEN_MOB_ID_RARITY_COEFFICIENTS[mob - 1];
    }
    for (uint64_t mob = 0; mob < rr_mob_id_max; ++mob)
    {
        RR_HELL_CREEK_MOB_ID_RARITY_COEFFICIENTS[mob] /=
            RR_HELL_CREEK_MOB_ID_RARITY_COEFFICIENTS[rr_mob_id_max - 1];
        RR_GARDEN_MOB_ID_RARITY_COEFFICIENTS[mob] /=
            RR_GARDEN_MOB_ID_RARITY_COEFFICIENTS[rr_mob_id_max - 1];
    }
}

#define offset(a, b)                                                           \
    ((x + a < 0 || y + b < 0 || x + a >= size / 2 || y + b >= size / 2)        \
         ? 0                                                                   \
         : template[(y + b) * size / 2 + x + a])
#define maze_grid(x, y) maze[(y)*size + (x)]

static void init_maze(uint32_t size, uint8_t *template,
                      struct rr_maze_grid *maze)
{
    for (int32_t y = 0; y < size / 2; ++y)
    {
        for (int32_t x = 0; x < size / 2; ++x)
        {
            uint8_t this_tile = offset(0, 0);
#ifdef RR_SERVER
            maze_grid(x * 2, y * 2).difficulty = this_tile;
            maze_grid(x * 2 + 1, y * 2).difficulty = this_tile;
            maze_grid(x * 2, y * 2 + 1).difficulty = this_tile;
            maze_grid(x * 2 + 1, y * 2 + 1).difficulty = this_tile;
#endif
            this_tile = this_tile != 0;
            // top left
            uint8_t top = offset(0, -1);
            uint8_t bottom = offset(0, 1);
            if (this_tile)
            {
                if (top == 0)
                {
                    if (offset(-1, 0) == 0)
                        maze_grid(x * 2, y * 2).value = 7;
                    else
                        maze_grid(x * 2, y * 2).value = this_tile;
                    if (offset(1, 0) == 0)
                        maze_grid(x * 2 + 1, y * 2).value = 5;
                    else
                        maze_grid(x * 2 + 1, y * 2).value = this_tile;
                }
                else
                {
                    maze_grid(x * 2, y * 2).value = this_tile;
                    maze_grid(x * 2 + 1, y * 2).value = this_tile;
                }
                if (bottom == 0)
                {
                    if (offset(-1, 0) == 0)
                        maze_grid(x * 2, y * 2 + 1).value = 6;
                    else
                        maze_grid(x * 2, y * 2 + 1).value = this_tile;
                    if (offset(1, 0) == 0)
                        maze_grid(x * 2 + 1, y * 2 + 1).value = 4;
                    else
                        maze_grid(x * 2 + 1, y * 2 + 1).value = this_tile;
                }
                else
                {
                    maze_grid(x * 2, y * 2 + 1).value = this_tile;
                    maze_grid(x * 2 + 1, y * 2 + 1).value = this_tile;
                }
            }
            else
            {
                if (top)
                {
                    if (offset(-1, 0) && offset(-1, -1))
                        maze_grid(x * 2, y * 2).value = 15;
                    else
                        maze_grid(x * 2, y * 2).value = this_tile;
                    if (offset(1, 0) && offset(1, -1))
                        maze_grid(x * 2 + 1, y * 2).value = 13;
                    else
                        maze_grid(x * 2 + 1, y * 2).value = this_tile;
                }
                else
                {
                    maze_grid(x * 2, y * 2).value = this_tile;
                    maze_grid(x * 2 + 1, y * 2).value = this_tile;
                }
                if (bottom)
                {
                    if (offset(-1, 0) && offset(-1, 1))
                        maze_grid(x * 2, y * 2 + 1).value = 14;
                    else
                        maze_grid(x * 2, y * 2 + 1).value = this_tile;
                    if (offset(1, 0) && offset(1, 1))
                        maze_grid(x * 2 + 1, y * 2 + 1).value = 12;
                    else
                        maze_grid(x * 2 + 1, y * 2 + 1).value = this_tile;
                }
                else
                {
                    maze_grid(x * 2, y * 2 + 1).value = this_tile;
                    maze_grid(x * 2 + 1, y * 2 + 1).value = this_tile;
                }
            }
        }
    }
}

static void print_chances(float difficulty)
{
    printf("-----Chances for %.0f-----\n", difficulty);
    uint32_t rarity_cap = rr_rarity_id_common + (difficulty + 7) / 8;
    if (rarity_cap > rr_rarity_id_ultimate)
        rarity_cap = rr_rarity_id_ultimate;
    uint32_t rarity = rarity_cap >= 2 ? rarity_cap - 2 : 0;
    for (; rarity <= rarity_cap; ++rarity)
    {
        float start =
            rarity == 0
                ? 0
                : pow(1 - (1 - RR_MOB_WAVE_RARITY_COEFFICIENTS[rarity]) * 0.3,
                      pow(1.5, difficulty));
        float end =
            rarity == rarity_cap
                ? 1
                : pow(1 - (1 - RR_MOB_WAVE_RARITY_COEFFICIENTS[rarity + 1]) *
                              0.3,
                      pow(1.5, difficulty));
        printf("%s: %.9f (1 per %.4f)\n", RR_RARITY_NAMES[rarity], end - start,
               1 / (end - start));
    }
}

double RR_BASE_CRAFT_CHANCES[rr_rarity_id_max - 1] = {0.5,  0.3,  0.15, 0.05,
                                                      0.03, 0.02, 0.015};
double RR_CRAFT_CHANCES[rr_rarity_id_max - 1];

static double from_prd_base(double C)
{
    double pProcOnN = 0;
    double pProcByN = 0;
    double sumNpProcOnN = 0;

    double maxFails = ceil(1 / C);
    for (uint32_t N = 1; N <= maxFails; ++N)
    {
        pProcOnN = fmin(1, N * C) * (1 - pProcByN);
        pProcByN += pProcOnN;
        sumNpProcOnN += N * pProcOnN;
    }
    return (1 / sumNpProcOnN);
}

static double get_prd_base(double p)
{
    if (p == 0)
        return 0;
    double Cupper = p;
    double Clower = 0;
    double Cmid = p / 2;
    double p1 = 0;
    double p2 = 1;
    while (1)
    {
        Cmid = (Cupper + Clower) / 2;
        p1 = from_prd_base(Cmid);
        if (fabs(p1 - p2) <= 0)
            break;

        if (p1 > p)
            Cupper = Cmid;
        else
            Clower = Cmid;
        p2 = p1;
    }
    return Cmid;
}

#define init(MAZE)                                                             \
    init_maze(sizeof(RR_MAZE_##MAZE[0]) / sizeof(struct rr_maze_grid),         \
              &RR_MAZE_TEMPLATE_##MAZE[0][0], &RR_MAZE_##MAZE[0][0]);

void rr_static_data_init()
{
    for (uint32_t r = 0; r < rr_rarity_id_max - 1; ++r)
        RR_CRAFT_CHANCES[r] = get_prd_base(RR_BASE_CRAFT_CHANCES[r]);
    init_game_coefficients();
    init(HELL_CREEK);
    init(BURROW);
#ifdef RR_SERVER
    print_chances(1);  // c
    print_chances(4);  // C
    print_chances(8);  // u
    print_chances(12); // U
    print_chances(16); // r
    print_chances(20); // R
    print_chances(24); // e
    print_chances(28); // E
    print_chances(32); // l
    print_chances(36); // L
    print_chances(40); // m
    print_chances(44); // M
    print_chances(48); // x
    print_chances(52); // X
    print_chances(56); // a
    print_chances(60); // A
#endif
}

double xp_to_reach_level(uint32_t level)
{
    if (level <= 60)
        return (level + 5) * pow(1.175, level);
    double base = (level + 5) * pow(1.175, 60);
    for (uint32_t i = 60; i < level; ++i)
        base *= rr_fclamp(1.18 - 0.0075 * (i - 60), 1.1, 1.18);
    return base;
}

uint32_t level_from_xp(double xp)
{
    uint32_t level = 1;
    while (xp >= xp_to_reach_level(level + 1))
        xp -= xp_to_reach_level(++level);
    return level;
}

#ifdef RR_SERVER
#define _ 0
#define c 1
#define C 4
#define u 8
#define U 12
#define r 16
#define R 20
#define e 24
#define E 28
#define l 32
#define L 36
#define m 40
#define M 44
#define x 48
#define X 52
#define a 56
#define A 60
#else
#define _ 0
#define c 1
#define C 1
#define u 1
#define U 1
#define r 1
#define R 1
#define e 1
#define E 1
#define l 1
#define L 1
#define m 1
#define M 1
#define x 1
#define X 1
#define a 1
#define A 1
#endif

#define RR_DEFINE_MAZE(name, size)                                             \
    struct rr_maze_grid RR_MAZE_##name[size][size];                            \
    uint8_t RR_MAZE_TEMPLATE_##name[size / 2][size / 2]
// clang-format off
RR_DEFINE_MAZE(HELL_CREEK, 80) = {
// 1 2 3 4 5 6 7 8 9 101112131415161718192021222324252627282930313233343536373839
{_,_,_,_,_,x,x,x,x,_,_,_,_,_,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,X,_,_,_,_,_},//0
{_,M,M,_,_,_,_,x,x,x,x,M,M,M,x,_,_,_,_,_,x,_,_,_,_,_,_,_,_,x,_,_,x,X,X,X,X,_,_,_},//1
{m,m,M,M,_,_,_,_,_,_,_,_,M,_,_,_,_,X,X,_,x,x,x,_,x,x,x,x,x,x,_,_,_,_,X,X,X,X,_,_},//2
{m,m,m,M,M,M,M,M,M,M,M,M,M,_,_,X,X,X,x,x,x,_,x,x,x,_,_,x,x,x,x,x,_,X,X,X,X,X,_,_},//3
{m,m,m,_,_,_,_,_,_,_,_,_,_,_,X,X,X,X,X,_,x,_,_,x,x,_,_,_,_,x,x,_,_,X,X,X,X,X,X,_},//4
{m,m,_,_,_,_,_,U,U,U,_,_,_,_,_,X,X,X,_,_,x,_,_,_,x,_,x,x,_,_,x,_,_,_,X,X,X,_,_,_},//5
{m,m,_,_,_,U,U,U,u,U,U,U,U,_,_,_,X,X,_,_,_,_,_,_,x,x,x,x,_,_,x,_,x,_,_,X,_,_,_,_},//6
{m,_,_,_,U,U,U,u,u,u,_,_,r,_,_,_,_,X,X,_,_,_,_,_,x,_,x,x,x,_,x,_,x,M,_,_,_,_,_,_},//7
{m,_,_,U,U,U,u,u,u,u,_,r,r,e,R,_,_,X,X,X,_,_,_,x,x,_,x,x,X,_,M,_,M,M,M,M,_,_,_,_},//8
{m,_,R,r,U,_,u,u,u,_,_,r,_,_,R,_,X,X,X,X,X,X,_,_,x,_,_,X,X,_,M,_,_,M,M,M,M,_,_,_},//9
{m,_,R,R,_,_,_,_,C,_,e,e,_,R,R,_,X,X,_,_,_,X,_,_,x,x,_,_,_,_,M,_,_,_,M,M,M,M,_,_},//10
{m,_,_,_,_,_,_,_,C,_,_,_,_,u,_,_,X,X,_,X,X,X,_,_,x,x,x,_,_,M,M,_,_,_,_,_,_,M,_,_},//11
{m,_,_,_,_,c,c,c,C,C,C,u,u,u,_,_,X,X,_,_,_,X,_,_,_,x,M,M,M,M,M,_,_,M,M,_,M,M,_,_},//12
{m,_,_,_,_,c,c,c,_,_,u,u,u,_,_,_,X,X,_,_,_,X,_,_,_,x,x,_,_,_,M,M,M,M,M,M,M,_,_,_},//13
{m,_,_,_,_,c,c,c,_,u,u,u,u,U,_,_,X,X,X,X,X,X,_,_,_,_,_,_,_,_,_,M,M,M,M,M,M,_,_,_},//14
{m,_,_,_,_,_,_,C,_,u,u,u,U,U,_,_,_,_,X,X,X,_,_,_,_,_,_,M,M,M,_,_,_,M,_,M,m,_,_,_},//15
{m,m,m,m,m,m,_,C,_,_,u,U,U,U,_,r,r,_,_,_,_,_,e,e,e,_,_,_,M,M,M,M,_,M,_,_,m,m,_,_},//16
{L,_,_,_,_,m,_,C,u,_,_,_,_,U,r,r,r,R,e,e,e,e,R,_,e,e,e,_,M,M,M,M,M,M,m,_,m,m,m,_},//17
{L,L,_,m,m,m,_,_,u,u,u,U,_,_,_,_,_,_,e,e,_,_,e,_,_,_,E,_,_,M,M,M,_,_,m,m,m,m,m,_},//18
{_,L,_,_,L,m,_,u,u,_,_,U,U,U,r,_,R,R,R,R,_,_,e,e,_,_,E,_,_,_,M,M,M,_,_,m,m,m,m,_},//19
{_,L,L,L,L,_,_,U,u,_,_,U,U,r,r,r,r,R,R,R,_,_,_,e,e,e,E,E,_,_,_,_,_,_,_,_,_,_,m,_},//20
{L,L,_,_,_,_,_,U,U,U,_,U,r,_,r,r,_,_,R,R,R,_,_,_,_,_,E,E,E,E,E,_,_,_,_,l,_,_,m,_},//21
{L,_,_,_,_,_,_,_,U,U,_,r,_,_,r,R,R,_,_,R,e,e,e,_,_,_,E,E,_,E,E,E,_,_,l,l,l,_,m,_},//22
{L,_,_,_,L,L,L,_,_,U,_,r,_,_,R,R,R,_,_,e,_,E,E,E,_,_,E,_,_,_,_,E,_,l,l,l,l,_,m,_},//23
{L,_,_,L,L,L,L,L,_,_,_,r,_,_,_,R,R,_,e,e,_,E,E,E,_,E,E,_,L,_,_,l,l,l,l,l,_,_,m,_},//24
{L,_,L,L,L,L,L,L,_,_,_,r,_,_,_,R,R,_,e,e,_,e,e,_,_,E,_,_,L,L,_,l,_,_,_,_,_,_,m,_},//25
{L,L,L,L,L,L,L,L,_,_,_,R,R,R,_,_,e,_,e,e,_,_,E,_,_,E,_,_,L,L,_,l,l,_,_,_,_,m,m,_},//26
{L,_,L,L,L,L,L,_,_,_,R,R,R,R,R,_,e,_,_,e,e,E,E,_,_,E,_,_,l,l,l,l,l,l,_,_,m,m,m,_},//27
{L,_,_,L,l,L,_,_,_,_,R,R,R,R,e,_,e,_,_,e,E,E,E,_,_,l,_,_,_,_,_,l,l,l,_,_,m,L,_,_},//28
{L,_,_,_,l,_,_,_,_,_,R,e,e,E,e,_,e,e,_,_,E,E,E,_,_,l,l,l,l,_,_,_,l,l,_,_,L,L,_,_},//29
{L,L,_,_,l,_,_,_,_,_,e,e,E,E,e,_,e,e,_,_,_,E,E,_,_,_,l,l,l,l,l,l,l,l,_,L,L,L,_,_},//30
{L,L,_,_,l,l,_,_,_,_,_,e,e,e,_,_,e,E,E,E,_,_,E,E,_,_,l,l,l,l,_,_,_,_,_,_,_,L,_,_},//31
{L,L,_,_,l,l,l,l,_,_,_,_,_,_,_,_,E,E,E,E,E,E,E,E,_,_,l,l,l,l,_,_,m,L,L,L,L,L,_,_},//32
{_,L,L,_,_,l,l,l,l,l,l,_,_,_,_,_,E,E,E,E,_,_,E,E,_,_,l,l,l,_,_,_,m,_,_,_,_,L,_,_},//33
{_,L,L,_,_,_,_,l,l,l,E,E,E,E,E,E,E,E,E,E,E,_,E,l,_,_,l,l,_,_,_,_,m,_,m,m,_,L,_,_},//34
{_,L,L,m,m,_,_,_,_,l,l,E,E,E,_,_,_,_,_,_,E,E,l,l,l,_,l,l,l,_,_,_,m,_,_,m,_,L,_,_},//35
{_,L,m,m,m,m,m,_,_,_,_,_,_,_,_,_,_,_,_,_,E,l,l,l,l,l,l,l,l,_,_,_,m,m,m,m,_,L,_,_},//36
{_,_,m,m,m,m,m,m,_,_,_,_,_,_,_,_,M,M,_,_,_,l,l,l,_,l,l,l,l,_,_,_,_,_,_,_,_,L,_,_},//37
{_,_,_,m,m,m,m,m,m,m,m,m,m,m,m,m,M,M,_,_,_,_,_,_,_,l,l,l,l,l,l,L,L,L,L,L,L,L,_,_},//38
{_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_},//39
};
// clang-format on
RR_DEFINE_MAZE(BURROW, 4) = {{1, 1}, {0, 1}};

#define MAZE_ENTRY(MAZE, GRID_SIZE)                                            \
    (sizeof(RR_MAZE_##MAZE[0]) / sizeof(struct rr_maze_grid)), GRID_SIZE,      \
        &RR_MAZE_##MAZE[0][0]

struct rr_maze_declaration RR_MAZES[rr_biome_id_max] = {
    {MAZE_ENTRY(HELL_CREEK, 1024), 5, {
        //x,y,w,h,spawn_x,spawn_y,min_level
        {0,   0,  3, 3, 6,  13, 1},  // 0
        {14, 17,  3, 3, 11, 15, 20}, // 1
        {18, 17,  5, 2, 16, 17, 40}, // 2
        {26, 20,  5, 5, 22, 23, 60}, // 3

        {32, 38, 3, 3, 29, 38, 80}, // 4
        {37, 29, 5, 2, 37, 33, 80}, // 5
    }},
    {MAZE_ENTRY(HELL_CREEK, 1024), 0},
    {MAZE_ENTRY(BURROW, 512), 0},
};

uint8_t RR_GLOBAL_BIOME = rr_biome_id_hell_creek;
#undef _
#undef c
#undef C
#undef u
#undef U
#undef r
#undef R
#undef e
#undef E
#undef l
#undef L
#undef m
#undef M
#undef x
#undef X
#undef a
#undef A
