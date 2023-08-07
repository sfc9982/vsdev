/*
    This is a simple example in C of using the rich presence API asynchronously.
*/

#define _CRT_SECURE_NO_WARNINGS /* thanks Microsoft */

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iostream>

#include <fmt/core.h>

#include "discord_rpc.h"
#pragma comment(lib, "discord-rpc.lib")

static const char *APPLICATION_ID   = "345229890980937739";
static int         FrustrationLevel = 0;
static int64_t     StartTime;
static int         SendPresence = 1;

static int prompt(std::string &line) {
    fmt::print("> ");
    int res = std::getline(std::cin, line) ? 1 : 0;
    return res;
}

static void updateDiscordPresence() {
    if (SendPresence) {
        char                buffer[256];
        DiscordRichPresence discordPresence;
        memset(&discordPresence, 0, sizeof(discordPresence));
        discordPresence.state = "West of House";
        sprintf(buffer, "Frustration level: %d", FrustrationLevel);
        discordPresence.details        = buffer;
        discordPresence.startTimestamp = StartTime;
        discordPresence.endTimestamp   = time(nullptr) + 5 * 60;
        discordPresence.largeImageKey  = "canary-large";
        discordPresence.smallImageKey  = "ptb-small";
        discordPresence.partyId        = "party1234";
        discordPresence.partySize      = 1;
        discordPresence.partyMax       = 6;
        discordPresence.partyPrivacy   = DISCORD_PARTY_PUBLIC;
        discordPresence.matchSecret    = "xyzzy";
        discordPresence.joinSecret     = "join";
        discordPresence.spectateSecret = "look";
        discordPresence.instance       = 0;
        Discord_UpdatePresence(&discordPresence);
    } else {
        Discord_ClearPresence();
    }
}

static void handleDiscordReady(const DiscordUser *connectedUser) {
    fmt::println("\nDiscord: connected to user {}#{} - {}",
                 connectedUser->username,
                 connectedUser->discriminator,
                 connectedUser->userId);
}

static void handleDiscordDisconnected(int errcode, const char *message) {
    fmt::println("\nDiscord: disconnected ({}: {})", errcode, message);
}

static void handleDiscordError(int errcode, const char *message) {
    fmt::println("\nDiscord: error ({}: {})", errcode, message);
}

static void handleDiscordJoin(const char *secret) {
    fmt::println("\nDiscord: join ({})", secret);
}

static void handleDiscordSpectate(const char *secret) {
    fmt::println("\nDiscord: spectate ({})", secret);
}

static void handleDiscordJoinRequest(const DiscordUser *request) {
    int         response = -1;
    std::string yn;
    fmt::println("\nDiscord: join request from {}#{} - {}",
                 request->username,
                 request->discriminator,
                 request->userId);
    do {
        fmt::print("Accept? (y/n)");
        if (!prompt(yn)) {
            break;
        }

        if (!yn[0]) {
            continue;
        }

        if (yn[0] == 'y') {
            response = DISCORD_REPLY_YES;
            break;
        }

        if (yn[0] == 'n') {
            response = DISCORD_REPLY_NO;
            break;
        }
    } while (true);
    if (response != -1) {
        Discord_Respond(request->userId, response);
    }
}

static void discordInit() {
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready        = handleDiscordReady;
    handlers.disconnected = handleDiscordDisconnected;
    handlers.errored      = handleDiscordError;
    handlers.joinGame     = handleDiscordJoin;
    handlers.spectateGame = handleDiscordSpectate;
    handlers.joinRequest  = handleDiscordJoinRequest;
    Discord_Initialize(APPLICATION_ID, &handlers, 1, nullptr);
}

static void gameLoop() {
    std::string line;

    StartTime = time(nullptr);

    fmt::println("You are standing in an open field west of a white house.");
    while (prompt(line)) {
        auto &seq = line[0];
        if (seq) {
            if (seq == 'q') {
                fmt::println("Now leaving...");
                break;
            }

            if (seq == 't') {
                fmt::println("Shutting off Discord.");
                Discord_Shutdown();
                continue;
            }

            if (seq == 'c') {
                if (SendPresence) {
                    fmt::println("Clearing presence information.");
                    SendPresence = 0;
                } else {
                    fmt::println("Restoring presence information.");
                    SendPresence = 1;
                }
                updateDiscordPresence();
                continue;
            }

            if (seq == 'y') {
                fmt::println("Re-init Discord.");
                discordInit();
                continue;
            }

            if (time(nullptr) & 1) {
                fmt::println("I don't understand that.");
            } else {
                std::size_t pos = line.find(' ');
                if (pos != std::string::npos) {
                    line = line.substr(0, pos);
                }
                fmt::println("I don't know the word \"{}\".", line);
            }

            ++FrustrationLevel;

            updateDiscordPresence();
        }

#ifdef DISCORD_DISABLE_IO_THREAD
        Discord_UpdateConnection();
#endif
        Discord_RunCallbacks();
    }
}

int main(int argc, char *argv[]) {
    discordInit();
    gameLoop();
    Discord_Shutdown();

    return 0;
}
