//
//  color.cpp
//  count_markers
//
//  Created by Changmook Chun on 9/8/23.
//

#include "color.hpp"

void invert(color_t& color) {
    if (color == color_t::white) {
        color = color_t::black;
        return;
    }
    color = color_t::white;
}
