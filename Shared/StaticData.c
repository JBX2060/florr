#include <Shared/StaticData.h>

#include <stdio.h>
#include <math.h>

struct rr_petal_data RR_PETAL_DATA[rr_petal_id_max] = {
    {rr_petal_id_no_petal, 0.0f,   0.0f,  0.0f,  0,    {0, 0, 0, 0, 0, 0, 0}},
    {rr_petal_id_basic,    10.0f,  10.0f, 0.0f,  37,   {1, 1, 1, 1, 1, 1, 1}},
    {rr_petal_id_light,    7.0f,   5.0f,  0.0f,  12,   {1, 2, 2, 3, 3, 5, 5}},
    {rr_petal_id_rock,     5.0f,   80.0f, 0.0f,  100,  {1, 1, 1, 1, 1, 1, 1}},
    {rr_petal_id_stinger,  35.0f,  2.0f,  10.0f, 113,  {1, 1, 1, 1, 1, 3, 5}},
    {rr_petal_id_faster,   7.0f,   5.0f,  15.0f, 20,   {1, 1, 1, 1, 1, 2, 2}},
    {rr_petal_id_missile,  20.0f,  10.0f, 15.0f, 75,   {1, 1, 1, 1, 1, 1, 1}},
    {rr_petal_id_peas,     10.0f,  5.0f,  8.0f,  25,   {4, 4, 4, 4, 4, 4, 5}},
    {rr_petal_id_leaf,     7.0f,   7.0f,  8.0f,  25,   {1, 1, 1, 1, 1, 1, 2}},
};

struct rr_mob_data RR_MOB_DATA[rr_mob_id_max] = {
    {rr_mob_id_baby_ant,       25,   10, 16.0f, {}},
    {rr_mob_id_worker_ant,     62.5, 10, 16.0f, {}},
    {rr_mob_id_rock,           100,  10, 26.0f, {}},
    {rr_mob_id_centipede_head, 25,   10, 36.0f, {}},
    {rr_mob_id_centipede_body, 25,   10, 36.0f, {}},
    {rr_mob_id_hornet,         37.5, 50, 26.0f, {}}
};

float RR_PETAL_RARITY_SCALE[rr_rarity_id_max] = {
    1,
    1.5,
    2.5,
    4,
    6,
    9,
    14
};

struct rr_mob_rarity_scale RR_MOB_RARITY_SCALING[rr_rarity_id_max] = {
    {1.0, 1.0, 1.0},
    {2.5, 1.6, 1.1},
    {4.5, 2.6, 1.3},
    {9.0, 4.5, 1.6},
    {15,  8.0, 2.5},
    {25,  13,  4.0},
    {50,  25,  6.0}
};

uint32_t RR_RARITY_COLORS[rr_rarity_id_max] = {0xff7eef6d, 0xffffe65d, 0xff4d52e3, 0xff861fde, 0xffde1f1f, 0xff1fdbde, 0xffff2b75};
char const *RR_RARITY_NAMES[rr_rarity_id_max] = {"Common", "Unusual", "Rare", "Epic", "Legendary", "Mythic", "Ultra"};

double RR_DROP_RARITY_COEFFICIENTS[rr_rarity_id_max + 1] = {0, 1, 8, 10, 10, 20, 5, 5};
uint32_t RR_MOB_RARITY_COEFFICIENTS[rr_rarity_id_max] = {5, 5, 5, 5, 5, 5, 5};

static void init_rarity_coefficients()
{
    double sum = 1;
    for (uint64_t a = 1; a < rr_rarity_id_max; ++a)
    {
        sum += (RR_DROP_RARITY_COEFFICIENTS[a + 1] = RR_DROP_RARITY_COEFFICIENTS[a] / RR_DROP_RARITY_COEFFICIENTS[a + 1]);
        RR_MOB_RARITY_COEFFICIENTS[a] *= RR_MOB_RARITY_COEFFICIENTS[a - 1];
    }
    for (uint64_t a = 1; a <= rr_rarity_id_max; ++a)
        RR_DROP_RARITY_COEFFICIENTS[a] = RR_DROP_RARITY_COEFFICIENTS[a] / sum + RR_DROP_RARITY_COEFFICIENTS[a - 1];
    RR_DROP_RARITY_COEFFICIENTS[rr_rarity_id_max] = 1;
}

static void init_loot_table(struct rr_loot_data *data, uint8_t id, float seed)
{
    printf("loot table with seed %f\n", seed);
    data->id = id;
    for (uint64_t mob = 0; mob < rr_rarity_id_max; ++mob)
    {
        uint64_t cap = mob != 0 ? mob : 1;
        data->loot_table[mob][0] = pow(1 - seed, RR_MOB_RARITY_COEFFICIENTS[mob]);
        for (uint64_t drop = 0; drop <= cap; ++drop)
        {
            float start = RR_DROP_RARITY_COEFFICIENTS[drop];
            float end = drop == cap ? 1 : RR_DROP_RARITY_COEFFICIENTS[drop + 1];
            data->loot_table[mob][drop + 1] = pow(1 - (1 - end) * seed, RR_MOB_RARITY_COEFFICIENTS[mob]) - pow(1 - (1 - start) * seed, RR_MOB_RARITY_COEFFICIENTS[mob]);
            printf("%f ", data->loot_table[mob][drop + 1]);
        }
        puts("");
    }
    printf("------------------------------------------\n");
}

static void init_loot_tables()
{
    init_loot_table(&RR_MOB_DATA[rr_mob_id_baby_ant].loot[0], rr_petal_id_light, 0.25);
    init_loot_table(&RR_MOB_DATA[rr_mob_id_baby_ant].loot[1], rr_petal_id_leaf, 0.25);

    init_loot_table(&RR_MOB_DATA[rr_mob_id_worker_ant].loot[0], rr_petal_id_light, 0.25);

    init_loot_table(&RR_MOB_DATA[rr_mob_id_rock].loot[0], rr_petal_id_rock, 0.25);

    init_loot_table(&RR_MOB_DATA[rr_mob_id_centipede_head].loot[0], rr_petal_id_peas, 0.1);
    init_loot_table(&RR_MOB_DATA[rr_mob_id_centipede_head].loot[1], rr_petal_id_light, 0.05);
    init_loot_table(&RR_MOB_DATA[rr_mob_id_centipede_head].loot[2], rr_petal_id_leaf, 0.05);

    init_loot_table(&RR_MOB_DATA[rr_mob_id_centipede_body].loot[0], rr_petal_id_peas, 0.1);
    init_loot_table(&RR_MOB_DATA[rr_mob_id_centipede_body].loot[1], rr_petal_id_leaf, 0.05);

    init_loot_table(&RR_MOB_DATA[rr_mob_id_hornet].loot[0], rr_petal_id_missile, 0.25);
}

void rr_static_data_init()
{
    init_rarity_coefficients();
    init_loot_tables();
}
 