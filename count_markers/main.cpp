//
//  main.cpp
//  count_markers
//
//  Created by Changmook Chun on 9/8/23.
//

#include "marker.hpp"

#include <iostream>

int main(int argc, const char * argv[]) {
    marker_t a {3};
    a.print();
    
    marker_t b {3};
    b.set(1, 1);
    b.set(2, 2);
    b.set(3, 3);
    b.print();
    
    if (a == b)
        std::cout << "a and b are the same.\n";
    else
        std::cout << "a and b are different.\n";
    
    a = b;
    if (a == b)
        std::cout << "a and b are the same.\n";
    else
        std::cout << "a and b are different.\n";
    
    marker_t original {5};
    original.set(1, 2);
    original.set(2, 4);
    original.set(3, 3);
    original.set(3, 5);
    original.set(4, 2);
    original.set(4, 4);
    original.set(5, 1);
    original.set(5, 3);
    original.print();
    
    marker_t rotated_90 = original.rotate_90();
    rotated_90.print();
    
    marker_t rotated_180 = original.rotate_180();
    rotated_180.print();
    
    marker_t rotated_270 = original.rotate_270();
    rotated_270.print();
    
    if (original == rotated_90.rotate_270())
        std::cout << "OK: 90 + 270 = 360.\n";
    
    if (original == rotated_180.rotate_180())
        std::cout << "OK: 180 + 180 = 360.\n";

    if (original == rotated_270.rotate_90())
        std::cout << "OK: 270 + 90 = 360.\n";

    auto dict_2x2 = create_dictionary(2*2);
    dict_2x2.print();

    auto dict_3x3 = create_dictionary(3*3);
    dict_3x3.print();
    dict_3x3.save(50, 15, 8, "/tmp/dict_3x3.pgm");

    dict_3x3.at(0).save(100, "/tmp/marker_3x3.pgm");
    
    return 0;
}
