//
// Created by jeremiah on 7/27/25.
//

#include "Innkeeper.h"

Innkeeper::Innkeeper(int _sprite, int _x, int _y, Direction _dir, int _check_x, int _check_y, int _price,
                     std::vector<std::string> _greeting) :
                     sprite(_sprite), x(_x), y(_y), dir(_dir),
                     check_x(_check_x), check_y(_check_y),
                     price(_price), greeting(_greeting) {

}
