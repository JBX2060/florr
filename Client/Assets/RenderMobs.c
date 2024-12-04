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

#include <Client/Assets/RenderFunctions.h>

#include <math.h>
#include <stdlib.h>

#include <Client/Assets/Render.h>
#include <Client/Renderer/Renderer.h>
#include <Shared/StaticData.h>

// head, body, legs, tail, IN THAT ORDER

struct rr_renderer_spritesheet mob_sprites[rr_mob_id_max];
struct rr_renderer_spritesheet friendly_trex_spritesheet;
void render_sprite(struct rr_renderer *renderer, uint8_t id, uint32_t pos,
                   uint8_t flags)
{
    if (flags & 1)
        render_sprite_from_cache(renderer,
                                 (flags & 2) && id == rr_mob_id_trex
                                     ? &friendly_trex_spritesheet
                                     : &mob_sprites[id],
                                 pos);
    else
        mob_sprites[id].sprites[pos].render(renderer);
}

void rr_renderer_draw_mob(struct rr_renderer *renderer, uint8_t id,
                          float raw_animation_tick, float turning_value,
                          uint8_t flags)
{
    float animation_tick = sinf(raw_animation_tick);
    struct rr_renderer_context_state original_state;
    struct rr_renderer_context_state state;

    rr_renderer_context_state_init(renderer, &original_state);
    // rr_renderer_rotate(renderer, M_PI / 2);
    /*
    function change(line) {
    const split = line.split("renderer,");
    if (split.length === 1)
        return line;
    let part = split[1].split(',').map(x => parseFloat(x));
    let final = [];
    for (let n = 0; n < part.length; n += 2)
        final.push(-part[n+1],part[n]);
    return split[0] + "renderer," + final.map(x=>x.toFixed(2)).join(',') + ");";
    }
    function convert(file) {
    console.log(file.split(";\n").map(x =>
    change(x)).join(";\n").replaceAll(";;",";"))}
    */
    switch (id)
    {
    case rr_mob_id_pachycephalosaurus:
        rr_renderer_scale(renderer, 0.75);
    case rr_mob_id_triceratops:
    case rr_mob_id_trex:
    case rr_mob_id_edmontosaurus:
        rr_renderer_scale(renderer, 0.2f);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_translate(renderer, animation_tick * 10.0f, 0);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_context_state_free(renderer, &state);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_translate(renderer, animation_tick * -10.0f, 0);
        render_sprite(renderer, id, 3, flags);
        rr_renderer_context_state_free(renderer, &state);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_translate(renderer, -75, 0);
        rr_renderer_rotate(renderer, turning_value);
        rr_renderer_translate(renderer, -80, 0);
        render_sprite(renderer, id, 4, flags);
        rr_renderer_context_state_free(renderer, &state);

        render_sprite(renderer, id, 1, flags);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_translate(renderer, 145, 0);
        render_sprite(renderer, id, 0, flags);
        rr_renderer_context_state_free(renderer, &state);
        break;
    case rr_mob_id_kelp:
        rr_renderer_scale(renderer, 3.0f);
    case rr_mob_id_fern:
    // simple for now
    case rr_mob_id_queen_ant:
        rr_renderer_scale(renderer, 0.3f);
    case rr_mob_id_tree:
    case rr_mob_id_meteor:
    case rr_mob_id_beehive:
        rr_renderer_scale(renderer, 0.4f);
        render_sprite(renderer, id, 0, flags);
        break;
    case rr_mob_id_pteranodon:
        rr_renderer_scale(renderer, 0.15f);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * 0.1f);
        rr_renderer_translate(renderer, 0, 160);
        render_sprite(renderer, id, 1, flags);
        rr_renderer_context_state_free(renderer, &state);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * -0.1f);
        rr_renderer_translate(renderer, 0, -160);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_context_state_free(renderer, &state);
        render_sprite(renderer, id, 0, flags);
        break;
    case rr_mob_id_dakotaraptor:
    case rr_mob_id_pectinodon:
    case rr_mob_id_ornithomimus:
        rr_renderer_scale(renderer, 0.16f);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_translate(renderer, 0, -65);
        rr_renderer_rotate(renderer, animation_tick * 0.1f);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_context_state_free(renderer, &state);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_translate(renderer, 0, 65);
        rr_renderer_rotate(renderer, animation_tick * -0.1f);
        render_sprite(renderer, id, 3, flags);
        rr_renderer_context_state_free(renderer, &state);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_translate(renderer, -120, 0);
        rr_renderer_rotate(renderer, turning_value);
        rr_renderer_translate(renderer, -75, 0);
        render_sprite(renderer, id, 4, flags);
        rr_renderer_context_state_free(renderer, &state);

        render_sprite(renderer, id, 1, flags);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_translate(renderer,
                              id == rr_mob_id_ornithomimus ? 175 : 125, 0);

        render_sprite(renderer, id, 0, flags);
        rr_renderer_context_state_free(renderer, &state);
        break;
    case rr_mob_id_ankylosaurus:
        rr_renderer_scale(renderer, 0.2f);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_translate(renderer, -155, 0);
        rr_renderer_rotate(renderer, turning_value);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_context_state_free(renderer, &state);

        render_sprite(renderer, id, 1, flags);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_translate(renderer, 145, 0);
        render_sprite(renderer, id, 0, flags);
        rr_renderer_context_state_free(renderer, &state);
        break;
    case rr_mob_id_quetzalcoatlus:
        rr_renderer_scale(renderer, 0.2f);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * 0.1f);
        rr_renderer_translate(renderer, -75, 125);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_context_state_free(renderer, &state);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * -0.1f);
        rr_renderer_translate(renderer, -75, -125);
        render_sprite(renderer, id, 3, flags);
        rr_renderer_context_state_free(renderer, &state);
        render_sprite(renderer, id, 1, flags);

        rr_renderer_translate(renderer, 165, 0);
        render_sprite(renderer, id, 0, flags);
        break;
     case rr_mob_id_portal_garden:
        rr_renderer_scale(renderer, 0.5f);
        rr_renderer_set_line_join(renderer, 1); 
        rr_renderer_save(renderer);

        rr_renderer_save(renderer);
        for (int i = 0; i < 8; i++) {
            float scale = 1.0f - i / 8.0f;
            float size = scale * 80;
            rr_renderer_rotate(renderer, animation_tick * -0.2f - 2 * M_PI / 3);
            rr_renderer_set_stroke(renderer, 0x26FFFFFF); 
            rr_renderer_begin_path(renderer);
            rr_renderer_rect(renderer, -size / 2, -size / 2, size, size);
            rr_renderer_set_line_width(renderer, 40.0f);
            rr_renderer_stroke(renderer);
            rr_renderer_set_line_width(renderer, 20.0f);
            rr_renderer_stroke(renderer);
        }
        rr_renderer_restore(renderer);
        break;
    case rr_mob_id_portal_ocean:
        rr_renderer_scale(renderer, 0.5f);
        rr_renderer_set_line_join(renderer, 1); 
        rr_renderer_save(renderer);

        rr_renderer_save(renderer);
        for (int i = 0; i < 16; i++) {
            float scale = 1.0f - i / 8.0f;
            float size = scale * 80;
            rr_renderer_rotate(renderer, animation_tick * -0.2f - 2 * M_PI / 3);
            rr_renderer_set_stroke(renderer, 0x26FFFFFF); 
            rr_renderer_begin_path(renderer);
            rr_renderer_rect(renderer, -size / 2, -size / 2, size, size);
            rr_renderer_set_line_width(renderer, 40.0f);
            rr_renderer_stroke(renderer);
            rr_renderer_set_line_width(renderer, 20.0f);
            rr_renderer_stroke(renderer);
        }
        rr_renderer_restore(renderer);
        break;

    case rr_mob_id_portal_hell_creek:
        rr_renderer_scale(renderer, 0.5f);
        rr_renderer_set_line_join(renderer, 1); 
        rr_renderer_save(renderer);

        rr_renderer_save(renderer);
        for (int i = 0; i < 8; i++) {
            float scale = 1.0f - i / 8.0f;
            float size = scale * 80;
            rr_renderer_rotate(renderer, animation_tick * -0.2f - 2 * M_PI / 3);
            rr_renderer_set_stroke(renderer, 0x26FFFFFF); 
            rr_renderer_begin_path(renderer);
            rr_renderer_rect(renderer, -size / 2, -size / 2, size, size);
            rr_renderer_set_line_width(renderer, 40.0f);
            rr_renderer_stroke(renderer);
            rr_renderer_set_line_width(renderer, 20.0f);
            rr_renderer_stroke(renderer);
        }
        rr_renderer_restore(renderer);
        break;
    case rr_mob_id_ant:
        rr_renderer_scale(renderer, 0.35f);
        rr_renderer_translate(renderer, -35, 0);
        render_sprite(renderer, id, 1, flags);
        rr_renderer_translate(renderer, 70, 0);
        render_sprite(renderer, id, 0, flags);
        break;
    case rr_mob_id_hornet:
    case rr_mob_id_honeybee:
        rr_renderer_scale(renderer, 0.2f);
        /*
        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * 0.1f - M_PI / 6);
        rr_renderer_translate(renderer, 0, 75);
        rr_renderer_rotate(renderer, M_PI / 2);
        render_sprite(renderer, id, 3, flags);
        rr_renderer_context_state_free(renderer, &state);
        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * -0.1f);
        rr_renderer_translate(renderer, 0, 75);
        rr_renderer_rotate(renderer, M_PI / 2);
        render_sprite(renderer, id, 3, flags);
        rr_renderer_context_state_free(renderer, &state);
        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * 0.1f + M_PI / 6);
        rr_renderer_translate(renderer, 0, 75);
        rr_renderer_rotate(renderer, M_PI / 2);
        render_sprite(renderer, id, 3, flags);
        rr_renderer_context_state_free(renderer, &state);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * 0.1f - M_PI / 6 + M_PI);
        rr_renderer_translate(renderer, 0, 75);
        rr_renderer_rotate(renderer, M_PI / 2);
        render_sprite(renderer, id, 3, flags);
        rr_renderer_context_state_free(renderer, &state);
        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * -0.1f + M_PI);
        rr_renderer_translate(renderer, 0, 75);
        rr_renderer_rotate(renderer, M_PI / 2);
        render_sprite(renderer, id, 3, flags);
        rr_renderer_context_state_free(renderer, &state);
        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * 0.1f + M_PI / 6 + M_PI);
        rr_renderer_translate(renderer, 0, 75);
        rr_renderer_rotate(renderer, M_PI / 2);
        render_sprite(renderer, id, 3, flags);
        rr_renderer_context_state_free(renderer, &state);
        */

        rr_renderer_translate(renderer, -90, 0);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_translate(renderer, 90, 0);
        render_sprite(renderer, id, 1, flags);
        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * 0.2f + 2 * M_PI / 3);
        rr_renderer_translate(renderer, 100, 0);
        render_sprite(renderer, id, 4, flags);
        rr_renderer_context_state_free(renderer, &state);
        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * -0.2f - 2 * M_PI / 3);
        rr_renderer_translate(renderer, 100, 0);
        render_sprite(renderer, id, 4, flags);
        rr_renderer_context_state_free(renderer, &state);
        rr_renderer_translate(renderer, 80, 0);
        render_sprite(renderer, id, 0, flags);
        break;
    case rr_mob_id_dragonfly:
        rr_renderer_scale(renderer, 0.2);
        rr_renderer_translate(renderer, -90, 0);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_translate(renderer, 90, 0);
        render_sprite(renderer, id, 1, flags);
        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * 0.2f);
        rr_renderer_translate(renderer, 0, -100);
        render_sprite(renderer, id, 3, flags);
        rr_renderer_context_state_free(renderer, &state);
        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_scale2(renderer, 1, -1);
        rr_renderer_rotate(renderer, animation_tick * -0.2f);
        rr_renderer_translate(renderer, 0, -100);
        render_sprite(renderer, id, 3, flags);
        rr_renderer_context_state_free(renderer, &state);
        rr_renderer_translate(renderer, 90, 0);
        render_sprite(renderer, id, 0, flags);
        break;
    case rr_mob_id_spider:
        rr_renderer_scale(renderer, 0.2);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * 0.05f + M_PI / 4);
        rr_renderer_translate(renderer, 100, 0);
        rr_renderer_scale2(renderer, 1, -1);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_context_state_free(renderer, &state);
        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * -0.05f + 5 * M_PI / 12);
        rr_renderer_translate(renderer, 100, 0);
        rr_renderer_scale2(renderer, 1, -1);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_context_state_free(renderer, &state);
        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * 0.05f + 7 * M_PI / 12);
        rr_renderer_translate(renderer, 100, 0);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_context_state_free(renderer, &state);
        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * -0.05f + 3 * M_PI / 4);
        rr_renderer_translate(renderer, 100, 0);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_context_state_free(renderer, &state);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * 0.05f - M_PI / 4);
        rr_renderer_translate(renderer, 100, 0);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_context_state_free(renderer, &state);
        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * -0.05f - 5 * M_PI / 12);
        rr_renderer_translate(renderer, 100, 0);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_context_state_free(renderer, &state);
        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * 0.05f - 7 * M_PI / 12);
        rr_renderer_translate(renderer, 100, 0);
        rr_renderer_scale2(renderer, 1, -1);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_context_state_free(renderer, &state);
        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * -0.05f - 3 * M_PI / 4);
        rr_renderer_translate(renderer, 100, 0);
        rr_renderer_scale2(renderer, 1, -1);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_context_state_free(renderer, &state);

        // rr_renderer_translate(renderer, -120, 0);
        // render_sprite(renderer, id, 1, flags);
        rr_renderer_translate(renderer, 10, 0);
        render_sprite(renderer, id, 0, flags);
        break;
    case rr_mob_id_house_centipede:
        if (!(flags & 4))
        {
            rr_renderer_translate(renderer, 58.7, 0);
            render_sprite(renderer, id, 0, flags);
        }
        else
        {
            rr_renderer_context_state_init(renderer, &state);
            rr_renderer_rotate(renderer, animation_tick * 0.1f + M_PI / 2);
            rr_renderer_translate(renderer, 55, 0);
            render_sprite(renderer, id, 2, flags);
            rr_renderer_context_state_free(renderer, &state);
            rr_renderer_rotate(renderer, animation_tick * 0.1f - M_PI / 2);
            rr_renderer_translate(renderer, 55, 0);
            rr_renderer_scale2(renderer, 1, -1);
            render_sprite(renderer, id, 2, flags);
            rr_renderer_context_state_free(renderer, &state);
            render_sprite(renderer, id, 1, flags);
        }
        break;
    case rr_mob_id_jellyfish:
        rr_renderer_scale(renderer, 2.0f); 
        rr_renderer_context_state_init(renderer, &state);

        rr_renderer_set_stroke(renderer, 0xFFFFFFFF);
        rr_renderer_set_fill(renderer, 0xFFFFFFFF);
        rr_renderer_set_global_alpha(renderer, 0.6f);

        rr_renderer_begin_path(renderer);
        for (int tX = 0; tX < 10; tX++) {
            float tY = tX / 10.0f * 2 * M_PI;
            rr_renderer_save(renderer);
            rr_renderer_rotate(renderer, tY);
            rr_renderer_move_to(renderer, 17.5f, 0); 
             float tZ = sin(tY + (animation_tick * 0.05f * M_PI / 3));
            rr_renderer_rotate(renderer, tZ * 0.5f);
            rr_renderer_quadratic_curve_to(renderer, 4 + 17.5f, tZ * -2, 14 + 17.5f, 0);
            rr_renderer_restore(renderer);
        }

        rr_renderer_set_line_cap(renderer, 1);
        rr_renderer_set_line_width(renderer, 2.3f);
        rr_renderer_stroke(renderer);

        rr_renderer_begin_path(renderer);
        rr_renderer_partial_arc(renderer, 0, 0, 20, 0, 2 * M_PI, 0); 
        rr_renderer_set_global_alpha(renderer, 0.5f);
        rr_renderer_fill(renderer);
        rr_renderer_clip(renderer);
        rr_renderer_set_line_width(renderer, 3.0f);
        rr_renderer_stroke(renderer);

        rr_renderer_set_line_width(renderer, 1.2f);
        rr_renderer_set_global_alpha(renderer, 0.6f);
        rr_renderer_begin_path(renderer);
        rr_renderer_set_line_cap(renderer, 1); 

        // for (int u0 = 0; u0 < 4; u0++) {
        //     rr_renderer_save(renderer);
        //     rr_renderer_rotate(renderer, u0 / 4.0f * 2 * M_PI);
        //     rr_renderer_translate(renderer, 4, 0);
        //     rr_renderer_move_to(renderer, 0, -2);
        //     rr_renderer_bezier_curve_to(renderer, 6.5f, -8, 6.5f, 8, 0, 2);
        //     rr_renderer_restore(renderer);
        // }

        rr_renderer_stroke(renderer);
        rr_renderer_context_state_free(renderer, &state);
        break;
    case rr_mob_id_starfish:
        rr_renderer_scale(renderer, 0.6f);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, emscripten_get_now() / 1000.0f);

        const int rJ = 5; // Number of limbs
        static float legD[rJ] = {100, 100, 100, 100, 100};
        float *rK = legD;

        rr_renderer_begin_path(renderer);
        for (int tc = 0; tc < rJ; tc++) {
            float td = (tc + 0.5f) / rJ * M_PI * 2;
            float te = (tc + 1.0f) / rJ * M_PI * 2;
            rK[tc] += (60.0f - rK[tc]) * 0.2f;
            float tf = rK[tc];
            if (tc == 0) {
                rr_renderer_move_to(renderer, tf, 0);
            }
            rr_renderer_quadratic_curve_to(renderer, cos(td) * 5.0f, sin(td) * 5.0f, cos(te) * tf, sin(te) * tf);
        }

        rr_renderer_set_line_cap(renderer, 1); 
        rr_renderer_set_line_join(renderer, 1);
        rr_renderer_set_line_width(renderer, 38.0f);
        rr_renderer_set_stroke(renderer, 0xFFa9413d);
        rr_renderer_stroke(renderer);

        rr_renderer_set_line_width(renderer, 26.0f);
        rr_renderer_set_stroke(renderer, 0xFFD14F4D); 
        rr_renderer_set_fill(renderer, 0xFFD14F4D); 
        rr_renderer_fill(renderer);
        rr_renderer_stroke(renderer);

        for (int tg = 0; tg < rJ; tg++) {
            float th = tg / (float)rJ * M_PI * 2;
            rr_renderer_save(renderer); 
            rr_renderer_rotate(renderer, th);
            float ti = rK[tg] / 100.0f;
            float tj = 30.0f;
            const int num_dots = 3;
            for (int dot = 0; dot < num_dots; dot++) {
                float dot_size = 5.0f;
                rr_renderer_move_to(renderer, tj, 0);
                rr_renderer_partial_arc(renderer, tj, 0, dot_size, 0, M_PI * 2, 0);
                tj += 20.0f;
            }
            rr_renderer_restore(renderer);
        }
        
        rr_renderer_set_fill(renderer, 0xFFd4766c);
        rr_renderer_fill(renderer);
        rr_renderer_context_state_free(renderer, &state);
        break;
    case rr_mob_id_shell:
        rr_renderer_scale(renderer, 1.0f);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_translate(renderer, 5.0f, 0.0f);
        rr_renderer_set_line_width(renderer, 5.0f);
        rr_renderer_set_line_cap(renderer, 1);
        rr_renderer_set_line_join(renderer, 1); 
        rr_renderer_set_stroke(renderer, 0xC9B46EFF); 
        rr_renderer_set_fill(renderer, 0xC9B46EFF); 

        rr_renderer_save(renderer);
        rr_renderer_translate(renderer, 3.0f, 0.0f);
        rr_renderer_begin_path(renderer);
        rr_renderer_move_to(renderer, -10.0f, 0.0f);
        rr_renderer_line_to(renderer, -40.0f, -15.0f);
        rr_renderer_quadratic_curve_to(renderer, -33.0f, 0.0f, -40.0f, 15.0f);
        rr_renderer_restore(renderer);
        rr_renderer_stroke(renderer);
        rr_renderer_fill(renderer);

        rr_renderer_begin_path(renderer);
        rr_renderer_move_to(renderer, 0.0f, 30.0f);

        int rt = 28;
        int ru = 36;
        int rv = 5;
        rr_renderer_move_to(renderer, 0.0f, rt);
        for (int rw = 0; rw < rv; rw++) {
            float rx = ((rw + 0.5f) / rv * 2 - 1) * M_PI / 2;
            float ry = ((rw + 1.0f) / rv * 2 - 1) * M_PI / 2;
            rr_renderer_quadratic_curve_to(renderer, cos(rx) * ru * 0.85f, -sin(rx) * ru, cos(ry) * rt * 0.7f, -sin(ry) * rt);
        }
        rr_renderer_line_to(renderer, -28.0f, -9.0f);
        rr_renderer_quadratic_curve_to(renderer, -38.0f, 0.0f, -28.0f, 9.0f);
        rr_renderer_line_to(renderer, 0.0f, rt);

        rr_renderer_set_fill(renderer, 0xFCDD86FF);
        rr_renderer_fill(renderer);
        rr_renderer_stroke(renderer);

        rr_renderer_begin_path(renderer);
        for (int rz = 0; rz < 4; rz++) {
            float rA = (rz / 3.0f * 2 - 1) * M_PI / 7;
            float rB = -30 + cos(rA) * 13;
            float rC = sin(rA) * 11;
            rr_renderer_move_to(renderer, rB, rC);
            rr_renderer_line_to(renderer, rB + cos(rA) * 27, rC + sin(rA) * 27);
        }
        rr_renderer_set_line_width(renderer, 4.0f);
        rr_renderer_stroke(renderer);
        rr_renderer_context_state_free(renderer, &state);
        break;
    case rr_mob_id_sponge:
        rr_renderer_scale(renderer, 0.4f);
        rr_renderer_context_state_init(renderer, &state);

        float rr2 = 100.0f;
        float rs2 = 80.0f;
        int rt2 = 18;
        float ru2 = 8.0f;
        rr_renderer_begin_path(renderer);
        float rv2 = 1.0f / rt2 * M_PI * 2;
        rr_renderer_move_to(renderer, rs2, 0);
        for (int rw2 = 0; rw2 < rt2; rw2++) {
            float rx2 = rw2 * rv2;
            float ry2 = (rw2 + 1) * rv2;
            rr_renderer_bezier_curve_to(renderer,
                                        cos(rx2) * rr2, sin(rx2) * rr2,
                                        cos(ry2) * rr2, sin(ry2) * rr2,
                                        cos(ry2) * rs2, sin(ry2) * rs2);
        }
        rr_renderer_set_fill(renderer, 0xEFC99BFF);
        rr_renderer_fill(renderer);
        rr_renderer_set_line_width(renderer, ru2);
        rr_renderer_set_line_cap(renderer, 1); 
        rr_renderer_set_line_join(renderer, 1); 
        rr_renderer_set_stroke(renderer, 0xC1A37DFF); 
        rr_renderer_stroke(renderer);

        rr_renderer_set_fill(renderer, 0xC1A37DFF);

        int rF2 = 6;
        int rG2 = 3;
        rr_renderer_begin_path(renderer);
        for (int sY = 0; sY < rF2; sY++) {
            float sZ = sY / (float)rF2 * M_PI * 2;
            rr_renderer_save(renderer);
            rr_renderer_rotate(renderer, sZ);
            for (int t0 = 0; t0 < rG2; t0++) {
                float t1 = t0 / (float)rG2;
                float t2 = 18 + t1 * 68;
                float t3 = 7 + t1 * 6;
                rr_renderer_move_to(renderer, t2, 0);
                rr_renderer_partial_arc(renderer, t2, 0, t3, 0, M_PI * 2, 0);
            }
            rr_renderer_restore(renderer);
        }
        rr_renderer_fill(renderer);
        rr_renderer_context_state_free(renderer, &state);
        break;
    case rr_mob_id_bubble:
        rr_renderer_scale(renderer, 2.0f);
        rr_renderer_context_state_init(renderer, &state);

        rr_renderer_set_stroke(renderer, 0xFFFFFFFF);
        rr_renderer_set_fill(renderer, 0xFFFFFFFF);
        rr_renderer_set_global_alpha(renderer, 1.0f * 0.4f);
        rr_renderer_save(renderer);

        rr_renderer_begin_path(renderer);
        rr_renderer_partial_arc(renderer, 0, 0, 20, 0, 2 * M_PI, 0);
        rr_renderer_fill(renderer);
        rr_renderer_clip(renderer);

        rr_renderer_set_global_alpha(renderer, 1.0f * 0.5f);
        rr_renderer_set_line_width(renderer, 3.0f);
        rr_renderer_stroke(renderer);

        rr_renderer_save(renderer);

        rr_renderer_begin_path(renderer);
        rr_renderer_move_to(renderer, 0, 0);
        rr_renderer_arc(renderer, 0, 0, 2); 
        rr_renderer_set_line_width(renderer, 8.0f);
        rr_renderer_set_line_join(renderer, 1); 
        rr_renderer_set_line_cap(renderer, 1); 
        rr_renderer_stroke(renderer);
        rr_renderer_restore(renderer); 

        rr_renderer_context_state_free(renderer, &state);
        break;
    case rr_mob_id_kingmackarel:
        rr_renderer_scale(renderer, 0.2f);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_translate(renderer,  0, 100);
        rr_renderer_rotate(renderer, animation_tick * 0.1f);
        rr_renderer_translate(renderer,  0, -100 + 180);
        render_sprite(renderer, id, 4, flags);
        rr_renderer_context_state_free(renderer, &state);

        render_sprite(renderer, id, 1, flags);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_translate(renderer, -30, -100);
        rr_renderer_rotate(renderer, animation_tick * 0.1f);
        rr_renderer_translate(renderer, -30, 30);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_context_state_free(renderer, &state);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_translate(renderer, 30, -100);
        rr_renderer_rotate(renderer, animation_tick * -0.1f);
        rr_renderer_translate(renderer, 30, 30);
        render_sprite(renderer, id, 3, flags);
        rr_renderer_context_state_free(renderer, &state);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_translate(renderer,  0, -75);
        rr_renderer_translate(renderer,  0, 75 - 145);
        render_sprite(renderer, id, 0, flags);
        rr_renderer_context_state_free(renderer, &state);
        break;
    case rr_mob_id_seasnail:
        rr_renderer_scale(renderer, 0.2f);
        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_translate(renderer,  0, -125 + animation_tick * 30);
        render_sprite(renderer, id, 0, flags);
        rr_renderer_context_state_free(renderer, &state);
        render_sprite(renderer, id, 1, flags);
        break;
    case rr_mob_id_seagull:
        rr_renderer_scale(renderer, 0.15f);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * 0.1f);
        rr_renderer_translate(renderer, 125, 15);
        render_sprite(renderer, id, 1, flags);
        rr_renderer_context_state_free(renderer, &state);

        rr_renderer_context_state_init(renderer, &state);
        rr_renderer_rotate(renderer, animation_tick * -0.1f);
        rr_renderer_translate(renderer, -125, 15);
        render_sprite(renderer, id, 2, flags);
        rr_renderer_context_state_free(renderer, &state);
        render_sprite(renderer, id, 0, flags);
        break;
    }
    rr_renderer_context_state_free(renderer, &original_state);
}

static void friendly_mask(struct rr_renderer *renderer)
{
    rr_renderer_add_color_filter(renderer, 0xffffff45, 0.3);
}

void rr_renderer_mob_cache_init()
{
    rr_renderer_spritesheet_init(
        &mob_sprites[0], NULL, 336, 192, rr_triceratops_head_draw, 336, 192,
        rr_triceratops_body_draw, 240, 240, rr_triceratops_leg1_draw, 240, 240,
        rr_triceratops_leg2_draw, 336, 192, rr_triceratops_tail_draw, 0);

    rr_renderer_spritesheet_init(
        &mob_sprites[1], NULL, 240, 144, rr_t_rex_head_draw, 336, 192,
        rr_t_rex_body_draw, 240, 240, rr_t_rex_leg1_draw, 240, 240,
        rr_t_rex_leg2_draw, 336, 192, rr_t_rex_tail_draw, 0);

    rr_renderer_spritesheet_init(
        &friendly_trex_spritesheet, friendly_mask, 240, 144, rr_t_rex_head_draw,
        336, 192, rr_t_rex_body_draw, 240, 240, rr_t_rex_leg1_draw, 240, 240,
        rr_t_rex_leg2_draw, 336, 192, rr_t_rex_tail_draw, 0);

    rr_renderer_spritesheet_init(&mob_sprites[2], NULL, 672, 672, rr_fern_draw,
                                 0);

    rr_renderer_spritesheet_init(&mob_sprites[3], NULL, 384, 384, rr_tree_draw,
                                 0);

    rr_renderer_spritesheet_init(
        &mob_sprites[4], NULL, 336, 192, rr_pteranodon_body_draw, 288, 432,
        rr_pteranodon_wing1_draw, 288, 432, rr_pteranodon_wing2_draw, 0);

    rr_renderer_spritesheet_init(
        &mob_sprites[5], NULL, 240, 144, rr_dakotaraptor_head_draw, 336, 192,
        rr_dakotaraptor_body_draw, 240, 144, rr_dakotaraptor_wing1_draw, 240,
        144, rr_dakotaraptor_wing2_draw, 336, 192, rr_dakotaraptor_tail_draw,
        0);

    rr_renderer_spritesheet_init(&mob_sprites[6], NULL, 240, 144,
                                 rr_pachycephalosaurus_head_draw, 336, 192,
                                 rr_pachycephalosaurus_body_draw, 240, 240,
                                 rr_pachycephalosaurus_leg1_draw, 240, 240,
                                 rr_pachycephalosaurus_leg2_draw, 336, 192,
                                 rr_pachycephalosaurus_tail_draw, 0);

    rr_renderer_spritesheet_init(
        &mob_sprites[7], NULL, 240, 144, rr_ornithomimus_head_draw, 336, 192,
        rr_ornithomimus_body_draw, 240, 144, rr_ornithomimus_wing1_draw, 240,
        144, rr_ornithomimus_wing2_draw, 336, 192, rr_ornithomimus_tail_draw,
        0);

    rr_renderer_spritesheet_init(
        &mob_sprites[8], NULL, 144, 144, rr_ankylosaurus_head_draw, 336, 192,
        rr_ankylosaurus_body_draw, 336, 192, rr_ankylosaurus_tail_draw, 0);

    rr_renderer_spritesheet_init(&mob_sprites[9], NULL, 240, 144,
                                 rr_meteor_draw, 0);

    rr_renderer_spritesheet_init(
        &mob_sprites[10], NULL, 336, 192, rr_quetzalcoatlus_head_draw, 336, 192,
        rr_quetzalcoatlus_body_draw, 336, 192, rr_quetzalcoatlus_wing1_draw,
        336, 192, rr_quetzalcoatlus_wing2_draw, 0);

    rr_renderer_spritesheet_init(
        &mob_sprites[11], NULL, 240, 144, rr_edmontosaurus_head_draw, 339, 192,
        rr_edmontosaurus_body_draw, 240, 240, rr_edmontosaurus_leg1_draw, 240,
        240, rr_edmontosaurus_leg2_draw, 336, 192, rr_edmontosaurus_tail_draw,
        0);

    rr_renderer_spritesheet_init(
        &mob_sprites[12], NULL, 240, 144, rr_pectinodon_head_draw, 336, 192,
        rr_pectinodon_body_draw, 240, 144, rr_pectinodon_wing1_draw, 240, 144,
        rr_pectinodon_wing2_draw, 336, 192, rr_pectinodon_tail_draw, 0);
    
    // garden
    rr_renderer_spritesheet_init(&mob_sprites[13], NULL, 192, 192, rr_ant_head_draw, 
    192, 192, rr_ant_thorax_draw, 192, 192, rr_ant_abdomen_draw, 192, 192, rr_ant_leg_draw, 0);

    rr_renderer_spritesheet_init(&mob_sprites[14], NULL, 192, 192, rr_hornet_head_draw, 
    192, 192, rr_hornet_thorax_draw, 192, 192, rr_hornet_abdomen_draw, 192, 192, rr_hornet_leg_draw,
    192, 192, rr_hornet_wing_draw, 0);

    rr_renderer_spritesheet_init(&mob_sprites[15], NULL, 192, 192, rr_dragonfly_head_draw, 
    192, 192, rr_dragonfly_thorax_draw, 192, 192, rr_dragonfly_abdomen_draw, 192, 192, rr_dragonfly_wing_draw, 0);

    rr_renderer_spritesheet_init(&mob_sprites[16], NULL, 192, 192, rr_honeybee_head_draw, 
    192, 192, rr_honeybee_thorax_draw, 192, 192, rr_honeybee_abdomen_draw, 192, 192, rr_honeybee_leg_draw,
    192, 192, rr_honeybee_wing_draw, 0);

    rr_renderer_spritesheet_init(&mob_sprites[17], NULL, 384, 384, rr_beehive_draw, 0);

    rr_renderer_spritesheet_init(&mob_sprites[18], NULL, 240, 240, rr_spider_head_draw, 
    240, 240, rr_spider_abdomen_draw, 240, 240, rr_spider_leg_draw, 0);

    rr_renderer_spritesheet_init(&mob_sprites[19], NULL, 2048, 2048, rr_house_centipede_head_draw, 
    2048, 2048, rr_house_centipede_body_draw, 2048, 2048, rr_house_centipede_leg_draw, 0);

    // ocean
    rr_renderer_spritesheet_init(&mob_sprites[26], NULL, 336, 336, rr_seagull_body_draw, 
    432, 432, rr_seagull_wing1_draw, 432, 432, rr_seagull_wing2_draw, 0);

    rr_renderer_spritesheet_init(&mob_sprites[22], NULL, 1024, 1024,
                                 rr_queen_ant_draw, 0);       

    rr_renderer_spritesheet_init(&mob_sprites[27], NULL, 144, 240, rr_king_mackarel_head_draw, 
    192, 336, rr_king_mackarel_body_draw, 144, 144, rr_king_mackarel_fin1_draw, 144, 144, 
    rr_king_mackarel_fin2_draw, 240, 240, rr_king_mackarel_tail_draw, 0);

    rr_renderer_spritesheet_init(&mob_sprites[25], NULL, 384, 384, rr_kelp_draw, 0);
    rr_renderer_spritesheet_init(&mob_sprites[28], NULL, 240, 240, rr_sea_snail_head_draw, 240, 240, rr_sea_snail_body_draw, 0);
}