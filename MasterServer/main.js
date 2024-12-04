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

import { MongoClient } from "mongodb";
import crypto from "crypto";

import { BinaryReader, BinaryWriter } from "./protocol.js";
import GameServer from "./gameserver.js";
import GameClient from "./client.js";

const port = 55554;
const namespace = "/api";
const dbName = "rrolf";
const PASSWORD_SALT = "aiapd8tfa3pd8tfn3pad8tap3d84t3q4pntardi4tad4otupadrtouad37q2aioymkznsxhmytcaoeyadou37wty3ou7qjoaud37tyadou37j4ywdou7wjytaousrt7jy3t";
const SERVER_SECRET = "ad904nf6adrgnariwpanyf3qaj8unri8t9b38jwna3g34ytgdr4bwtvd4u5";
const uri = process.env.MONGODB_URI;
// const allowedOrigins = [
//   'http://167.234.221.142:8000',
//   'http://localhost:8000',
//   "https://rysteria.pro", 
//   "https://dinorr.fun", 
//   "https://rwar.fun", 
//   "https://maxnest0x0.github.io/rysteria/"
// ];

const client = new MongoClient(uri);

const is_valid_uuid = (uuid) => uuid.length === 36 && uuid.match(/[0-9a-z]{8}-([0-9a-z]{4}-){3}[0-9a-z]{12}/) !== null;

const log = (type, args, color = 31) => {
    console.log(`\u001b[${color}[${new Date().toJSON()}::[${type}]:\t${args.join("\t")}\u001b[0m`);
};

const hash = (s) => crypto.createHash("sha512").update(s, "utf8").digest("hex");

const apply_missing_defaults = (account) => {
    const defaults = {
        password: "",
        username: "",
        xp: 0,
        petals: {"1:0": 5},
        failed_crafts: {},
        mob_gallery: {},
    };

    // Fill in any missing defaults
    for (let prop in defaults) {
        if (!account.hasOwnProperty(prop)) {
            account[prop] = defaults[prop];
        }
    }

    // Remove any extra properties
    for (let prop in account) {
        if (!defaults.hasOwnProperty(prop)) {
            delete account[prop];
        }
    }
    return account;
};

const write_db_entry = async (username, data) => {
    try {
        await client.db(dbName).collection("users").updateOne({ username }, { $set: data }, { upsert: true });
        console.log("Database entry written successfully.");
    } catch (error) {
        console.error("Error writing to database:", error);
    }
};

// TODO: Use this until we have a better way.
function randomName() {
    const adjectives = ['Swift', 'Brave', 'Mighty', 'Silent', 'Wise', 'Noble'];
    const nouns = ['Warrior', 'Hunter', 'Knight', 'Ranger', 'Guardian', 'Hero'];
    const adj = adjectives[Math.floor(Math.random() * adjectives.length)];
    const noun = nouns[Math.floor(Math.random() * nouns.length)];
    return `${adj}${noun}${Math.floor(Math.random() * 1000)}`;
}

const db_read_or_create_user = async (username, password) => {
    try {
        let user = await client.db(dbName).collection("users").findOne({ username });
        if (!user) {
            const newUser = apply_missing_defaults({ username });
            newUser.identity = {
                display_name: randomName(),
                is_game_linked: false,
            };
            newUser.password = hash(password + PASSWORD_SALT);
            console.log("Account create:", newUser);
            await client.db(dbName).collection("users").insertOne(newUser);
            return newUser;
        } else {
            apply_missing_defaults(user);
            return user;
        }
    } catch (error) {
        console.error("Error reading or creating user from database:", error);
        return null;
    }
};

const merge_petals = (obj1, obj2) => {
    if (obj1 == null) return obj2;
    if (obj2 == null) return obj1;
    const result = {};
    for (const key in obj1) {
        if (Object.prototype.hasOwnProperty.call(obj1, key)) {
            result[key] = obj1[key];
        }
    }
    for (const key in obj2) {
        if (Object.prototype.hasOwnProperty.call(obj2, key)) {
            if (result.hasOwnProperty(key)) {
                result[key] += obj2[key];
            } else {
                result[key] = obj2[key];
            }
        }
    }
    return result;
};

const handle_CORS = (req) => {
  const origin = req.headers.get("origin");
  // if (origin && allowedOrigins.includes(origin)) {
    return {
      "Access-Control-Allow-Origin": origin,
      "Access-Control-Allow-Methods": "GET, POST, PUT, DELETE, OPTIONS",
      "Access-Control-Allow-Headers": "Content-Type, Authorization",
      "Access-Control-Allow-Credentials": "true",
    };
  // }
  // return {};
};

const game_servers = {};
const connected_clients = {};

const server = Bun.serve({
  port,
  development: true,

  async fetch(req) {
    const url = new URL(req.url);
    const corsHeaders = handle_CORS(req);
    
    if (req.headers.get("upgrade") === "websocket") {
      return server.upgrade(req);
    }
    
    if (req.method === "OPTIONS") {
      return new Response(null, { 
        headers: corsHeaders,
        status: 204 
      });
    }

    const headers = {
      ...corsHeaders,
      "X-Powered-By": "custom rrolf http server written in c",
      "Content-Type": "application/json",
    };

    try {
      if (url.pathname === `${namespace}/account_link`) {
        const params = url.searchParams;
        const username = params.get("username");
        const access_token = params.get("access_token");
        
        if (!username || !access_token) {
          return Response.json(
            { error: "Username and access token are required." },
            { status: 400, headers }
          );
        }
        
        const discordResponse = await fetch("https://discord.com/api/users/@me", {
            headers: {
                authorization: `Bearer ${access_token}`,
            },
        });
        if (!discordResponse.ok) {
            console.error("Failed to fetch Discord user info:", discordResponse.statusText);
            return Response.json(
              { error: "Failed to fetch Discord user info." },
              { status: discordResponse.status, headers }
            );
        }
        const discordUserInfo = await discordResponse.json();
        console.log("Account Link:", discordUserInfo);
        const { id: discordId, username: discordUsername, avatar } = discordUserInfo;
        const avatar_url = `https://cdn.discordapp.com/avatars/${discordId}/${avatar}.jpg`;
        const existingDiscordUser = await client.db(dbName).collection("users").findOne({ discordId });
        if (existingDiscordUser) {
            if (existingDiscordUser.username === username) {
              return Response.json(
                { username: username },
                { status: 200, headers }
              );
            }
            if (!is_valid_uuid(existingDiscordUser.username) || !is_valid_uuid(username)) {
              return Response.json(
                { error: "invalid uuid" },
                { status: 400, headers }
              );
            }
            const current_session = await client.db(dbName).collection("users").findOne({ username });
            if (!existingDiscordUser) {
              return Response.json(
                { error: "failed" },
                { status: 400, headers }
              );
            }
            log("account_link", [existingDiscordUser.username, username]);
            if (current_session) {
              if (current_session.discordId || current_session.is_game_linked === true || current_session.identity.is_game_linked === true) {
                console.log("session already linked!", current_session);
                return Response.json(
                  { username: existingDiscordUser.username },
                  { status: 200, headers }
                );
              }
              const merged_progress = {
                xp: existingDiscordUser.xp + current_session.xp,
                petals: merge_petals(existingDiscordUser.petals, current_session.petals),
              };
              await client.db(dbName).collection("users").updateOne({ username: existingDiscordUser.username }, { $set: merged_progress });
              await client.db(dbName).collection("users").deleteOne({ username: username });
            }
            
            await disconnect_player(existingDiscordUser.username);
            await disconnect_player(username);
            
            log("account_link_response", [existingDiscordUser.username, current_session.username]);
            return Response.json(
              { username: existingDiscordUser.username },
              { status: 200, headers }
            );
        } else {
            const existingUser = await client.db(dbName).collection("users").findOne({ username });
            log("account_single_link", [existingUser.username, username]);
            if (existingUser) {
              await client.db(dbName).collection("users").updateOne({ username }, {
                $set: {
                  discordId,
                  "identity.display_name": discordUsername,
                  avatar_url: avatar_url,
                  is_game_linked: true,
                },
              });
              
              await disconnect_player(username);
              
              return Response.json(
                { username: existingUser.username },
                { status: 200, headers }
              );
            } else {
              return Response.json(
                { error: "Failed to link account. User unauthorized." },
                { status: 401, headers }
              );
            }
        }
      }

      if (url.pathname === `${namespace}/user_get_identity`) {
        const params = url.searchParams;
        const existing_identity_token = params.get("existing_identity_token");
        
        if (existing_identity_token) {
          console.log("Existing Token", existing_identity_token);
          let user = await client.db(dbName).collection("users").findOne({ username: existing_identity_token });
          if (!user) {
            user = await db_read_or_create_user(existing_identity_token, SERVER_SECRET);
          }
          const response = {
            identity_token: existing_identity_token,
            identity: user.identity,
            username: existing_identity_token,
          };
          if (!response.identity_token) response.identity_token = "token";
          if (!response.identity) response.identity = {};
          if (!response.identity.avatar_url) response.identity.avatar_url = "url";
          if (!response.identity.display_name) response.identity.display_name = "Guest";
          if (!response.identity.identity_id) response.identity.identity_id = "";
          if (!response.identity.account_number) response.identity.account_number = "0000";
          if (response.identity.is_game_linked === undefined) response.identity.is_game_linked = true;
          return Response.json(response, { status: 200, headers });
        } else {
          return Response.json(
            { error: "No existing token." },
            { status: 500, headers }
          );
        }
      }

      if (url.pathname.startsWith(`${namespace}/user_get_password/`)) {
        const token = url.pathname.split("/").pop();
        try {
          console.log(token);
          const user = await client.db(dbName).collection("users").findOne({ username: token });
          if (!user) throw new Error("User not found");
          return Response.json(
            { password: hash(user.identity.identity_id + PASSWORD_SALT) },
            { status: 200, headers }
          );
        } catch (error) {
          console.error("Error fetching user:", error);
          throw new Error("Internal Server Error");
        }
      }

      if (url.pathname.startsWith(`${namespace}/user_get_server_alias/`)) {
        const alias = url.pathname.split("/").pop();
        try {
          if (game_servers[alias]) return Response.json(
            { server_alias: game_servers[alias].rivet_server_id },
            { status: 200, headers }
          );
          else return Response.json(
            { server_alias: "" },
            { status: 200, headers }
          );
        } catch (error) {
          console.error("Error fetching server alias:", error);
          throw new Error("Internal Server Error");
        }
      }

      return Response.json(
        { error: "Not Found" },
        { status: 404, headers }
      );

    } catch (error) {
      console.error(error);
      return Response.json(
        { error: "Internal Server Error" },
        { status: 500, headers }
      );
    }
  },

  websocket: {
    open(ws) {
      const game_server = new GameServer();
      game_server[game_server.alias] = game_server;
      ws.data = { game_server };
    },
    message: async (ws, message) => {
      const data = new Uint8Array(message);
      const decoder = new BinaryReader(data);
      const game_server = ws.data.game_server;

      switch (decoder.ReadUint8()) {
        case 0: {
          const uuid = decoder.ReadStringNT();
          const pos = decoder.ReadUint8();
          log("attempt init", [uuid]);
          if (!is_valid_uuid(uuid) || connected_clients[uuid]) {
            log("player force disconnect", [uuid]);
            const encoder = new BinaryWriter();
            encoder.WriteUint8(2);
            encoder.WriteUint8(pos);
            encoder.WriteStringNT(uuid);
            ws.send(encoder.data.subarray(0, encoder.at));
            break;
          }
          try {
            const user = await db_read_or_create_user(uuid, SERVER_SECRET);
            connected_clients[uuid] = new GameClient(user, game_server.alias);
            game_server.clients[pos] = uuid;
            const encoder = new BinaryWriter();
            encoder.WriteUint8(1);
            encoder.WriteUint8(pos);
            connected_clients[uuid].write(encoder);
            ws.send(encoder.data.subarray(0, encoder.at));
          } catch (e) {
            console.error("Error in user initialization:", e);
          }
          break;
        }
        case 1: {
          const uuid = decoder.ReadStringNT();
          if (connected_clients[uuid] && connected_clients[uuid].server !== game_server.alias) break;
          const pos = game_server.clients.indexOf(uuid);
          if (pos !== -1) {
            log("client delete", [uuid]);
            const client = connected_clients[uuid];
            if (!client) break;
            if (client.user && client.user.username) {
                await write_db_entry(client.user.username, client.user);
            } else {
                console.error("Warning: Deleting client with invalid user data", uuid);
            }
            delete connected_clients[uuid];  
            game_server.clients[pos] = 0;
          }
          break;
        }
        case 2: {
          const uuid = decoder.ReadStringNT();
          if (!connected_clients[uuid]) break;
          if (connected_clients[uuid] && connected_clients[uuid].server !== game_server.alias) break;
          const user = connected_clients[uuid].user;
          user.xp = decoder.ReadFloat64();
          user.petals = {};
          user.failed_crafts = {};
          user.mob_gallery = {};
          let id = decoder.ReadUint8();
          while (id) {
            const rarity = decoder.ReadUint8();
            const count = decoder.ReadVarUint();
            user.petals[id + ':' + rarity] = count;
            id = decoder.ReadUint8();
          }
          id = decoder.ReadUint8();
          while (id) {
            const rarity = decoder.ReadUint8();
            const count = decoder.ReadVarUint();
            user.failed_crafts[id + ':' + rarity] = count;
            id = decoder.ReadUint8();
          }
          id = decoder.ReadUint8();
          while (id) {
            const rarity = decoder.ReadUint8();
            const count = decoder.ReadVarUint();
            user.mob_gallery[(id - 1) + ':' + rarity] = count;
            id = decoder.ReadUint8();
          }
          await write_db_entry(uuid, user);
          break;
        }
        case 3: {
          let petals = {};
          let id = decoder.ReadUint8();
          while (id) {
            let rarity = decoder.ReadUint8();
            petals[`${id}:${rarity}`] ||= 0;
            petals[`${id}:${rarity}`]++;
            id = decoder.ReadUint8();
          }
          break;
        }
        case 101:
          game_server.rivet_server_id = decoder.ReadStringNT();
          log("server id recv", [game_server.rivet_server_id]);
          break;
      }
    },
    close(ws) {
      for (const uuid of ws.data.game_server.clients) {
        if (connected_clients[uuid] && connected_clients[uuid].server === ws.data.game_server.alias) write_db_entry(uuid, connected_clients[uuid].user);
        delete connected_clients[uuid];
      }
      delete game_servers[ws.data.game_server.alias];
    },
  },
});

async function disconnect_player(uuid) {
  if (connected_clients[uuid]) {
    const gameServer = game_servers[connected_clients[uuid].server];
    if (gameServer && gameServer.ws) {
      const encoder = new BinaryWriter();
      encoder.WriteUint8(2);
      encoder.WriteUint8(gameServer.clients.indexOf(uuid));
      encoder.WriteStringNT(uuid);
      gameServer.ws.send(encoder.data.subarray(0, encoder.at));
    }
  }
}

setInterval(() => {
  if (Object.keys(connected_clients).length > 1) {
    log("player count", [Object.keys(connected_clients).length]);
  }
}, 15000);
