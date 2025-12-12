/**
 * @file RouletteTypes.h
 * @brief Type definitions for roulette game
 * @author Marczelloo
 * @date 2025-12-11
 */

//
// Created by moskw on 06.12.2025.
//

#ifndef KASYNO_ROULETTETYPES_H
#define KASYNO_ROULETTETYPES_H

#pragma once

/**
 * @enum RouletteTileType
 * @brief Colors of roulette tiles
 */
enum class RouletteTileType {
    BLACK = 0,   ///< Black tile
    RED,         ///< Red tile
    GREEN,       ///< Green tile (zero)
};

/**
 * @struct RouletteTile
 * @brief Represents a single tile on the roulette wheel
 */
struct RouletteTile {
    RouletteTileType color;  ///< Tile color
    int number;              ///< Number on the tile (0-36)
};

#endif //KASYNO_ROULETTETYPES_H