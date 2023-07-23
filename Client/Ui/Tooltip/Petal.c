#include <Client/Ui/Ui.h>

#include <stdio.h>
#include <stdlib.h>

#include <Client/Ui/Engine.h>

#include <Shared/StaticData.h>

struct rr_ui_element *rr_ui_petal_tooltip_init(uint8_t id, uint8_t rarity)
{
    char *cd = malloc((sizeof *cd) * 16);
    if (RR_PETAL_DATA[id].secondary_cooldown > 0)
        cd[sprintf(cd, "↻ %.1f + %.1fs",
                   (RR_PETAL_DATA[id].cooldown * 2 / 5) * 0.1, (RR_PETAL_DATA[id].secondary_cooldown * 2 / 5) * 0.1)] = 0;
    else
        cd[sprintf(cd, "↻ %.1fs", (RR_PETAL_DATA[id].cooldown * 2 / 5) * 0.1)] = 0;
    char *hp = malloc((sizeof *hp) * 16);
    hp[sprintf(hp, "%.1f",
               RR_PETAL_DATA[id].health * RR_PETAL_RARITY_SCALE[rarity].health)] = 0;
    char *dmg = malloc((sizeof *dmg) * 16);
    dmg[sprintf(dmg, "%.1f",
                RR_PETAL_DATA[id].damage * RR_PETAL_RARITY_SCALE[rarity].damage /
                    RR_PETAL_DATA[id].count[rarity])] = 0;
    
    struct rr_ui_element *this = rr_ui_set_background(
        rr_ui_v_container_init(
            rr_ui_container_init(), 10, 5, 6,
            rr_ui_h_container_init(
                rr_ui_container_init(), 0, 20, 2,
                rr_ui_set_justify(
                    rr_ui_text_init(RR_PETAL_NAMES[id], 24, 0xffffffff), -1, 0),
                rr_ui_set_justify(rr_ui_text_init(cd, 16, 0xffffffff), 1, 0)),
            rr_ui_set_justify(rr_ui_text_init(RR_RARITY_NAMES[rarity], 16,
                                              RR_RARITY_COLORS[rarity]),
                              -1, 0),
            rr_ui_static_space_init(10),
            rr_ui_set_justify(
                rr_ui_text_init(RR_PETAL_DESCRIPTIONS[id], 16, 0xffffffff), -1,
                0),
            rr_ui_set_justify(rr_ui_h_container_init(
                                  rr_ui_container_init(), 0, 0, 2,
                                  rr_ui_text_init("Health: ", 12, 0xff44ff44),
                                  rr_ui_text_init(hp, 12, 0xff44ff44)),
                              -1, 0),
            rr_ui_set_justify(rr_ui_h_container_init(
                                  rr_ui_container_init(), 0, 0, 2,
                                  rr_ui_text_init("Damage: ", 12, 0xffff4444),
                                  rr_ui_text_init(dmg, 12, 0xffff4444)),
                              -1, 0)),
        0x80000000);
    if (id == rr_petal_id_magnet)
    {
        char *extra = malloc((sizeof *extra) * 8);
        extra[sprintf(extra, "+%d", -25 + 25 * rarity)] = 0;
        rr_ui_container_add_element(this, rr_ui_set_justify(rr_ui_h_container_init(
                rr_ui_container_init(), 0, 0, 2,
                rr_ui_text_init("Pickup range: ", 12, 0xff44ffdd),
                rr_ui_text_init(extra, 12, 0xff44ffdd)),
            -1, 0)
        );
    }
    else if (id == rr_petal_id_leaf)
    {
        char *extra = malloc((sizeof *extra) * 8);
        extra[sprintf(extra, "%.1f/s", 25 * 0.025 * RR_PETAL_RARITY_SCALE[rarity].damage)] = 0;
        rr_ui_container_add_element(this, rr_ui_set_justify(rr_ui_h_container_init(
                rr_ui_container_init(), 0, 0, 2,
                rr_ui_text_init("Heal: ", 12, 0xffffff44),
                rr_ui_text_init(extra, 12, 0xffffff44)),
            -1, 0)
        );
    }
    else if (id == rr_petal_id_egg)
    {
        rr_ui_container_add_element(this, rr_ui_set_justify(rr_ui_h_container_init(
                rr_ui_container_init(), 0, 0, 2,
                rr_ui_text_init("Spawns: ", 12, 0xffffff44),
                rr_ui_text_init(RR_RARITY_NAMES[rarity == 0 ? 0 : rarity - 1], 12, RR_RARITY_COLORS[rarity == 0 ? 0 : rarity - 1]),
                rr_ui_text_init(" T-Rex", 12, 0xffffff44)),
            -1, 0)
        );
    }
    else if (id == rr_petal_id_faster)
    {
        char *extra = malloc((sizeof *extra) * 16);
        extra[sprintf(extra, "%.1f rad/s", (0.008 + 0.004 * rarity) * 25)] = 0;
        rr_ui_container_add_element(this, rr_ui_set_justify(rr_ui_h_container_init(
                rr_ui_container_init(), 0, 0, 2,
                rr_ui_text_init("Petal rotation: ", 12, 0xffffff44),
                rr_ui_text_init(extra, 12, 0xffffff44)),
            -1, 0)
        );
    }
    else if (id == rr_petal_id_uranium)
    {
        char *extra = malloc((sizeof *extra) * 16);
        extra[sprintf(extra, "%.1f hp/s", RR_PETAL_DATA[id].damage * RR_PETAL_RARITY_SCALE[rarity].damage * 2.5)] = 0;
        rr_ui_container_add_element(this, rr_ui_set_justify(rr_ui_h_container_init(
                rr_ui_container_init(), 0, 0, 2,
                rr_ui_text_init("Self poison: ", 12, 0xffab2aad),
                rr_ui_text_init(extra, 12, 0xffab2aad)),
            -1, 0)
        );
    }
    rr_ui_link_toggle(rr_ui_set_justify(this, -1, -1), rr_ui_never_show);
    this->poll_events = rr_ui_no_focus;
    return this;
}