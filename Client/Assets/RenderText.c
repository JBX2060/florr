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

#include <Client/Renderer/Renderer.h>
#include <Shared/StaticData.h>

#define TEXT_RESOLUTION (20.0f)
#define TEXT_PADDING (26.0f)
struct rr_renderer text_cache;

float PETAL_TEXT_LENGTHS[rr_petal_id_max];
float RARITY_TEXT_LENGTHS[rr_rarity_id_max];
float MOB_TEXT_LENGTHS[rr_mob_id_max];

void rr_renderer_draw_petal_name(struct rr_renderer *renderer, uint8_t id, float size, int8_t h, int8_t v)
{
    rr_renderer_scale(renderer, size / TEXT_RESOLUTION);
    float width = PETAL_TEXT_LENGTHS[id];
    rr_renderer_draw_clipped_image(renderer, &text_cache, 2 + width / 2, id * TEXT_PADDING, width + 4, TEXT_PADDING, h * width / 2, v * TEXT_RESOLUTION / 2);
    rr_renderer_scale(renderer, TEXT_RESOLUTION / size);
}

void rr_renderer_draw_mob_name(struct rr_renderer *renderer, uint8_t id, float size, int8_t h, int8_t v)
{
    rr_renderer_scale(renderer, size / TEXT_RESOLUTION);
    float width = MOB_TEXT_LENGTHS[id];
    rr_renderer_draw_clipped_image(renderer, &text_cache, 2 + width / 2, (id + rr_petal_id_max) * TEXT_PADDING, width + 4, TEXT_RESOLUTION, h * width / 2, v * TEXT_RESOLUTION / 2);
    rr_renderer_scale(renderer, TEXT_RESOLUTION / size);
}

void rr_renderer_draw_rarity_name(struct rr_renderer *renderer, uint8_t id, float size, int8_t h, int8_t v)
{
    rr_renderer_scale(renderer, size / TEXT_RESOLUTION);
    float width = RARITY_TEXT_LENGTHS[id];
    rr_renderer_draw_clipped_image(renderer, &text_cache, 2 + width / 2, (id + rr_petal_id_max + rr_mob_id_max) * TEXT_PADDING, width + 4, TEXT_RESOLUTION, h * width / 2, v * TEXT_RESOLUTION / 2);
    rr_renderer_scale(renderer, TEXT_RESOLUTION / size);
}

static void rr_renderer_text_cache_draw()
{
    rr_renderer_set_transform(&text_cache, 1, 0, 2, 0, 1, 2);
    rr_renderer_set_fill(&text_cache, 0xffffffff);
    rr_renderer_set_stroke(&text_cache, 0xff222222);
    rr_renderer_set_text_size(&text_cache, TEXT_RESOLUTION);
    rr_renderer_set_line_width(&text_cache, 1.2);
    for (uint8_t i = 0; i < rr_petal_id_max; ++i)
    {
        PETAL_TEXT_LENGTHS[i] = rr_renderer_get_text_size(RR_PETAL_DATA[i].name) * TEXT_RESOLUTION;
        rr_renderer_stroke_text(&text_cache, RR_PETAL_DATA[i].name, 0, 0);
        rr_renderer_fill_text(&text_cache, RR_PETAL_DATA[i].name, 0, 0);
        rr_renderer_translate(&text_cache, 0, TEXT_PADDING);
    }
    for (uint8_t i = 0; i < rr_mob_id_max; ++i)
    {
        MOB_TEXT_LENGTHS[i] = rr_renderer_get_text_size(RR_MOB_DATA[i].name) * TEXT_RESOLUTION;
        rr_renderer_stroke_text(&text_cache, RR_MOB_DATA[i].name, 0, 0);
        rr_renderer_fill_text(&text_cache, RR_MOB_DATA[i].name, 0, 0);
        rr_renderer_translate(&text_cache, 0, TEXT_PADDING);
    }
    for (uint8_t i = 0; i < rr_rarity_id_max; ++i)
    {
        rr_renderer_set_fill(&text_cache, RR_RARITY_COLORS[i]);
        RARITY_TEXT_LENGTHS[i] = rr_renderer_get_text_size(RR_RARITY_NAMES[i]) * TEXT_RESOLUTION;
        rr_renderer_stroke_text(&text_cache, RR_RARITY_NAMES[i], 0, 0);
        rr_renderer_fill_text(&text_cache, RR_RARITY_NAMES[i], 0, 0);
        rr_renderer_translate(&text_cache, 0, TEXT_PADDING);
    }
}

static void rr_renderer_text_cache_redraw(void *captures)
{
    rr_renderer_text_cache_draw();
}

void rr_renderer_text_cache_init()
{
    rr_renderer_init(&text_cache);
    rr_renderer_set_dimensions(
        &text_cache, 256,
        TEXT_PADDING * (rr_petal_id_max + rr_mob_id_max + rr_rarity_id_max));
    text_cache.on_context_restore = rr_renderer_text_cache_redraw;
    rr_renderer_text_cache_draw();
}
