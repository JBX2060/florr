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

#include <Client/Renderer/ComponentRender.h>

#include <math.h>

#include <Client/Game.h>
#include <Client/Renderer/Renderer.h>
#include <Client/Simulation.h>

#include <Client/Assets/RenderFunctions.h>

void rr_component_petal_render(EntityIdx entity, struct rr_game *game,
                               struct rr_simulation *simulation)
{
    struct rr_renderer *renderer = game->renderer;
    struct rr_component_physical *physical =
        rr_simulation_get_physical(simulation, entity);
    struct rr_component_petal *petal =
        rr_simulation_get_petal(simulation, entity);
    struct rr_component_health *health =
        rr_simulation_get_health(simulation, entity);
    if (petal->id == rr_petal_id_uranium)
    {
        rr_renderer_set_fill(renderer, 0x2063bf2e);
        rr_renderer_begin_path(renderer);
        if (physical->on_title_screen)
        {
            rr_renderer_arc(renderer, 0, 0,
                            (30 + 5 * sinf(physical->animation_timer * 3)) *
                                physical->radius / 15);
        }
        else
        {
            float radius = 200 * (petal->rarity + 1);
            rr_renderer_set_global_alpha(renderer,
                                         0.5 * health->uranium_animation);
            rr_renderer_arc(renderer, 0, 0,
                            radius * (1 - health->uranium_animation));
        }
        rr_renderer_fill(renderer);
    }
    rr_renderer_set_global_alpha(renderer, 1 - physical->deletion_animation);
    rr_renderer_scale(renderer, 1 + physical->deletion_animation * 0.5);
    if (petal->rarity >= rr_rarity_id_exotic)
    {
        // fixme: looks too different at 60 fps
        uint8_t exo = petal->rarity == rr_rarity_id_exotic;
        struct rr_simulation_animation *particle = rr_particle_alloc(
            physical->on_title_screen ? &game->title_screen_particle_manager
                                      : &game->particle_manager,
            rr_animation_type_default);
        float angle =
            rr_vector_theta(&physical->lerp_velocity) - 0.5 + rr_frand();
        rr_vector_from_polar(&particle->velocity,
                             (5 + rr_frand() * 3) * (exo ? 0.5 : 1), angle);
        particle->x = physical->lerp_x;
        particle->y = physical->lerp_y;
        if (physical->on_title_screen &&
            (petal->id == rr_petal_id_magnet || petal->id == rr_petal_id_crest))
        {
            float angle = rr_frand() * 2 * M_PI;
            float dist = rr_frand() * physical->radius;
            particle->x += cosf(angle) * dist;
            particle->y += sinf(angle) * dist;
        }
        particle->size = (3 + rr_frand() * 2) * (exo ? 0.5 : 1) *
                         (physical->on_title_screen ? physical->radius / 20 : 1);
        particle->opacity = (0.3 + rr_frand() * 0.2) * (exo ? 0.5 : 1);
        particle->color = 0xffffffff;
    }
    if (game->cache.tint_petals)
        rr_renderer_add_color_filter(renderer, RR_RARITY_COLORS[petal->rarity],
                                     0.4);
    rr_renderer_add_color_filter(renderer, 0xffff0000,
                                 0.5 * health->damage_animation);
    rr_renderer_rotate(renderer, physical->lerp_angle);

    rr_renderer_scale(renderer, physical->radius / 10);
    uint8_t use_cache =
        (((health->damage_animation < 0.1) | game->cache.low_performance_mode) &
         1) &
        (1 - game->cache.tint_petals);
    if (petal->id != rr_petal_id_peas || petal->detached == 1)
        rr_renderer_draw_petal(renderer, petal->id, use_cache);
    else
        rr_renderer_draw_static_petal(renderer, petal->id, petal->rarity,
                                      use_cache);
}