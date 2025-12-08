//
// Created by moskw on 10.10.2025.
//

#pragma once

#ifndef KASYNO_ENUMS_H
#define KASYNO_ENUMS_H

enum class GameState {
    MAIN_MENU = 0,
    CASINO_MENU,
    GAME_MENU,
    EXIT,
};

enum class MainMenuOptions {
    MENU_CREATE_PLAYER = 0,
    MENU_CHECK_LEADERBOARD,
    MENU_EXIT,
};

enum class CreatePlayerResult {
    Ok = 0,
    Retry,
    Exit,
};

enum class CasinoOptions {
    CASINO_SELECT_GAME = 0,
    CASINO_CHECK_LEADERBOARD,
    CASINO_RETURN_TO_MAIN_MENU,
    CASINO_EXIT,
};

enum class GameMenuOptions {
    GAME_PLAY_SLOTS = 0,
    GAME_PLAY_ROULETTE,
    GAME_PLAY_BLACKJACK,
    GAME_RETURN_TO_CASINO_MENU,
    GAME_EXIT,
};

enum class BetOptions {
    BET_ALL_IN = 0,
    BET_HALF,
    BET_QUARTER,
    BET_CUSTOM,
};

enum class AfterGameOptions {
    AFTER_PLAY_ANOTHER_ROUND = 0,
    AFTER_CHANGE_BET_AND_PLAY_ANOTHER_ROUND,
    AFTER_PLAY_ANOTHER_GAME,
    AFTER_RETURN_TO_GAME_MENU,
    AFTER_RETURN_TO_CASINO_MENU,
    AFTER_EXIT,
};

enum class SlotsOptions {
    SPIN = 0,
    CHANGE_BET,
    VIEW_PAYOUTS,
    EXIT_TO_GAME_MENU,
    EXIT,
};

enum class SlotsBetOptions {
    BET_10 = 0,
    BET_20,
    BET_50,
    BET_100,
    BET_200,
    BET_500,
};

enum class SlotsIcon {
    CHERRY = 0,
    LEMON,
    BELL,
    STAR,
    CLOVER,
    SEVEN,
};

enum class RouletteOptions {
    SPIN = 0,
    CHANGE_BET,
    VIEW_PAYOUTS,
    EXIT_TO_GAME_MENU,
    EXIT,
};

enum class BlackjackOptions {
    PLAY_ROUND = 0,
    CHANGE_BET,
    VIEW_PAYOUTS,
    RULES,
    EXIT_TO_GAME_MENU,
    EXIT,
};

enum class BlackjackRoundOptions {
    HIT = 0,
    STAND,
    DOUBLE_DOWN,
    SPLIT,
    SURRENDER,
};

#endif //KASYNO_ENUMS_H