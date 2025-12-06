//
// Created by moskw on 06.12.2025.
//

#ifndef KASYNO_ROULETTETYPES_H
#define KASYNO_ROULETTETYPES_H

#pragma once

enum class RouletteTileType {
    BLACK = 0,
    RED,
    GREEN,
};

struct RouletteTile {
    RouletteTileType color;
    int number;
};

#endif //KASYNO_ROULETTETYPES_H