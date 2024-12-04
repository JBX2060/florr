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

#include <Client/Ui/Ui.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Client/DOM.h>
#include <Client/Game.h>
#include <Client/InputData.h>
#include <Client/Renderer/Renderer.h>
#include <Client/Ui/Engine.h>

#include <Shared/Utilities.h>

static uint8_t account_container_should_show(struct rr_ui_element *this,
                                             struct rr_game *game)
{
    return game->menu_open == rr_game_menu_account && !game->simulation_ready;
}

static uint8_t account_button_should_show(struct rr_ui_element *this,
                                          struct rr_game *game)
{
    // if (game->account_linked == 0 && !game->simulation_ready)
    // {
    //     rr_ui_render_tooltip_right(this, game->link_reminder_tooltip, game);
    // }
    return !game->simulation_ready;
}

static void account_container_animate(struct rr_ui_element *this,
                                      struct rr_game *game)
{
    this->width = this->abs_width;
    this->height = this->abs_height;
    rr_renderer_translate(
        game->renderer, -(this->x + this->abs_width / 2) * 2 * this->animation,
        0);
}

static void account_toggle_button_on_render(struct rr_ui_element *this,
                                            struct rr_game *game)
{
    struct rr_renderer *renderer = game->renderer;
    if (game->focused == this)
        renderer->state.filter.amount = 0.2;
    rr_renderer_scale(renderer, renderer->scale);
    rr_renderer_set_fill(renderer, this->fill);
    renderer->state.filter.amount += 0.2;
    rr_renderer_begin_path(renderer);
    rr_renderer_round_rect(renderer, -this->abs_width / 2,
                           -this->abs_height / 2, this->abs_width,
                           this->abs_height, 6);
    rr_renderer_fill(renderer);
    rr_renderer_scale(renderer, 0.025);
    rr_renderer_set_fill(renderer, 0xffffffff);
    rr_renderer_begin_path(renderer);
    rr_renderer_move_to(renderer, 211.43, -3.40);
    rr_renderer_bezier_curve_to(renderer, 157.31, 44.35, 85.45, 73.66, 6.27,
                                73.66);
    rr_renderer_bezier_curve_to(renderer, -74.10, 73.66, -147.13, 43.40,
                                -201.50, -5.76);
    rr_renderer_bezier_curve_to(renderer, -327.95, 27.33, -406.19, 140.31,
                                -406.19, 275.28);
    rr_renderer_line_to(renderer, -406.19, 344.06);
    rr_renderer_bezier_curve_to(renderer, -406.19, 505.03, 406.20, 505.03,
                                406.20, 344.06);
    rr_renderer_line_to(renderer, 406.20, 275.28);
    rr_renderer_bezier_curve_to(renderer, 406.19, 143.86, 331.98, 32.77, 211.43,
                                -3.40);
    rr_renderer_move_to(renderer, 6.27, 24.73);
    rr_renderer_bezier_curve_to(renderer, 146.43, 24.73, 260.36, -84.71, 260.36,
                                -219.91);
    rr_renderer_bezier_curve_to(renderer, 260.36, -355.11, 146.67, -464.78,
                                6.27, -464.78);
    rr_renderer_bezier_curve_to(renderer, -133.66, -464.78, -247.59, -355.11,
                                -247.59, -219.91);
    rr_renderer_bezier_curve_to(renderer, -247.59, -84.70, -133.66, 24.73, 6.27,
                                24.73);
    rr_renderer_fill(renderer);
}

static void account_toggle_button_on_event(struct rr_ui_element *this,
                                           struct rr_game *game)
{
    if (game->input_data->mouse_buttons_up_this_tick & 1)
    {
        if (game->pressed != this)
            return;
        if (game->menu_open == rr_game_menu_account)
            game->menu_open = rr_game_menu_none;
        else
            game->menu_open = rr_game_menu_account;
    }
    rr_ui_render_tooltip_below(this, game->account_tooltip, game);
    game->cursor = rr_game_cursor_pointer;
}

struct rr_ui_element *rr_ui_account_toggle_button_init()
{
    struct rr_ui_element *this = rr_ui_element_init();
    rr_ui_set_background(this, 0x80888888);
    this->abs_width = this->abs_height = this->width = this->height = 40;
    this->on_event = account_toggle_button_on_event;
    this->on_render = account_toggle_button_on_render;
    this->should_show = account_button_should_show;
    return this;
}

static void link_rivet_account(struct rr_ui_element *this, struct rr_game *game)
{
    if (game->input_data->mouse_buttons_up_this_tick & 1)
    {
        // Redirect to Discord oauth
        EM_ASM({
            var currentUrl = window.location.href;
            var redirectUri = new URL(currentUrl).origin + new URL(currentUrl).pathname;
            window.location.href = "https://discord.com/oauth2/authorize?client_id=1310849698000539790&response_type=token&redirect_uri=" + encodeURIComponent(redirectUri) + "&scope=identify+guilds.join";
        });
    }
    else if (!(game->input_data->mouse_buttons & 1))
        rr_ui_render_tooltip_right(this, game->link_account_tooltip, game);
}


static uint8_t linked_account(struct rr_ui_element *this, struct rr_game *game)
{
    return !game->account_linked;
}

static void copy_uuid(struct rr_ui_element *this, struct rr_game *game)
{
    if (game->input_data->mouse_buttons_up_this_tick & 1)
        rr_copy_string(game->rivet_account.uuid);
}

// clang-format off
struct rr_ui_element *rr_ui_account_container_init(struct rr_game *game)
{
    struct rr_ui_element *uuid = rr_ui_text_init(game->rivet_account.uuid, 15, 0xffffffff);
    struct rr_ui_element *link_account =
        rr_ui_labeled_button_init("Login With Discord", 36, NULL);
    rr_ui_set_background(link_account, 0xff45a8a8);
    link_account->on_event = link_rivet_account;

    uuid->on_event = copy_uuid;
    struct rr_ui_element *this = rr_ui_pad(
        rr_ui_set_background(
            rr_ui_v_pad(
                rr_ui_set_justify(
                    rr_ui_v_container_init(
                        rr_ui_container_init(), 10, 5,
                        rr_ui_text_init("Account", 24, 0xffffffff),
                        rr_ui_static_space_init(10),
                        link_account,
                        rr_ui_static_space_init(10),
                        rr_ui_h_container_init(rr_ui_container_init(), 0, 0,
                            rr_ui_text_init("Account Name: ", 20, 0xffffffff),
                            rr_ui_text_init(game->rivet_account.name, 20, 0xffffffff),
                            NULL
                        ),
                        rr_ui_static_space_init(10),

                        rr_ui_v_container_init(rr_ui_container_init(), 0, 5,
                            rr_ui_text_init("You can link your account to", 15, 0xffffffff),
                            rr_ui_text_init("keep your progress across devices.", 15, 0xffffffff),
                            NULL
                        ),

                        NULL
                    ), -1, -1
                ), 50
            ), 0x40ffffff
        ), 10
    );
    this->animate = account_container_animate;
    this->should_show = account_container_should_show;
    return this;
}
// clang-format on
