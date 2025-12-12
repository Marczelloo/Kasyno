/**
 * @file Enums.h
 * @brief Enumeration definitions for application states and options
 * @author Marczelloo
 * @date 2025-12-11
 */

//
// Created by moskw on 10.10.2025.
//

#pragma once

#ifndef KASYNO_ENUMS_H
#define KASYNO_ENUMS_H

/**
 * @enum GameState
 * @brief Application state machine states
 */
enum class GameState {
    MAIN_MENU = 0,    ///< Main menu state
    CASINO_MENU,      ///< Casino menu state
    GAME_MENU,        ///< Game selection menu state
    EXIT,             ///< Exit application state
};

/**
 * @enum MainMenuOptions
 * @brief Options available in the main menu
 */
enum class MainMenuOptions {
    MENU_CREATE_PLAYER = 0,      ///< Create new player
    MENU_CHECK_LEADERBOARD,      ///< View leaderboard
    MENU_EXIT,                   ///< Exit application
};

/**
 * @enum CreatePlayerResult
 * @brief Result of player creation attempt
 */
enum class CreatePlayerResult {
    Ok = 0,    ///< Player created successfully
    Retry,     ///< Retry player creation
    Exit,      ///< Exit to main menu
};

/**
 * @enum CasinoOptions
 * @brief Options available in the casino menu
 */
enum class CasinoOptions {
    CASINO_SELECT_GAME = 0,           ///< Select a game
    CASINO_CHECK_LEADERBOARD,         ///< View leaderboard
    CASINO_RETURN_TO_MAIN_MENU,       ///< Return to main menu
    CASINO_EXIT,                      ///< Exit application
};

/**
 * @enum GameMenuOptions
 * @brief Options available in the game selection menu
 */
enum class GameMenuOptions {
    GAME_PLAY_SLOTS = 0,              ///< Play slots
    GAME_PLAY_ROULETTE,               ///< Play roulette
    GAME_PLAY_BLACKJACK,              ///< Play blackjack
    GAME_RETURN_TO_CASINO_MENU,       ///< Return to casino menu
    GAME_EXIT,                        ///< Exit application
};

/**
 * @enum BetOptions
 * @brief Quick bet amount options
 */
enum class BetOptions {
    BET_ALL_IN = 0,   ///< Bet all balance
    BET_HALF,         ///< Bet half of balance
    BET_QUARTER,      ///< Bet quarter of balance
    BET_CUSTOM,       ///< Enter custom bet amount
};

/**
 * @enum AfterGameOptions
 * @brief Options available after a game round
 */
enum class AfterGameOptions {
    AFTER_PLAY_ANOTHER_ROUND = 0,                ///< Play another round with same bet
    AFTER_CHANGE_BET_AND_PLAY_ANOTHER_ROUND,    ///< Change bet and play another round
    AFTER_PLAY_ANOTHER_GAME,                     ///< Play a different game
    AFTER_RETURN_TO_GAME_MENU,                   ///< Return to game menu
    AFTER_RETURN_TO_CASINO_MENU,                 ///< Return to casino menu
    AFTER_EXIT,                                  ///< Exit application
};

/**
 * @enum SlotsOptions
 * @brief Options available in slots game
 */
enum class SlotsOptions {
    SPIN = 0,              ///< Spin the reels
    CHANGE_BET,            ///< Change bet amount
    VIEW_PAYOUTS,          ///< View payout table
    EXIT_TO_GAME_MENU,     ///< Exit to game menu
    EXIT,                  ///< Exit application
};

/**
 * @enum SlotsBetOptions
 * @brief Quick bet options for slots
 */
enum class SlotsBetOptions {
    BET_10 = 0,    ///< Bet 10
    BET_20,        ///< Bet 20
    BET_50,        ///< Bet 50
    BET_100,       ///< Bet 100
    BET_200,       ///< Bet 200
    BET_500,       ///< Bet 500
};

/**
 * @enum SlotsIcon
 * @brief Symbols available on slot machine reels
 */
enum class SlotsIcon {
    CHERRY = 0,    ///< Cherry symbol
    LEMON,         ///< Lemon symbol
    BELL,          ///< Bell symbol
    STAR,          ///< Star symbol
    CLOVER,        ///< Clover symbol
    SEVEN,         ///< Seven symbol
};

/**
 * @enum RouletteOptions
 * @brief Options available in roulette game
 */
enum class RouletteOptions {
    SPIN = 0,              ///< Spin the wheel
    CHANGE_BET,            ///< Change bet amount/type
    VIEW_PAYOUTS,          ///< View payout table
    EXIT_TO_GAME_MENU,     ///< Exit to game menu
    EXIT,                  ///< Exit application
};

/**
 * @enum BlackjackOptions
 * @brief Options available in blackjack game
 */
enum class BlackjackOptions {
    PLAY_ROUND = 0,        ///< Play a round
    CHANGE_BET,            ///< Change bet amount
    VIEW_PAYOUTS,          ///< View payout table
    RULES,                 ///< View game rules
    EXIT_TO_GAME_MENU,     ///< Exit to game menu
    EXIT,                  ///< Exit application
};

enum class BlackjackRoundOptions {
    HIT = 0,               ///< Take another card
    STAND,                 ///< End turn
    DOUBLE_DOWN,          ///< Double bet and take one card
    SPLIT,                 ///< Split hand into two
    SURRENDER,            ///< Forfeit half bet and end round
};



#endif //KASYNO_ENUMS_H