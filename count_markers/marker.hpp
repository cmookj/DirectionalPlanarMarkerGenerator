//
//  marker.hpp
//  count_markers
//
//  Created by Changmook Chun on 9/8/23.
//

#ifndef marker_hpp
#define marker_hpp

#include "color.hpp"

#include <vector>
#include <iostream>

using elem_t = std::vector<color_t>;

class marker_t {
public:
    marker_t () = delete;
    marker_t (const size_t);
    marker_t (const size_t, elem_t&&);
    
    virtual ~marker_t() = default;
    
    marker_t (const marker_t&) = default;
    marker_t (marker_t&&) = default;
    
    marker_t& operator= (const marker_t&) = default;
    marker_t& operator= (marker_t&&) = default;
    
    bool operator== (const marker_t&) const;
    bool operator!= (const marker_t&) const;
    
    size_t dim() const;
    color_t at(const size_t, const size_t) const;
    
    void print(std::ostream& strm = std::cout) const;
    
    void set(const unsigned, const unsigned);

    marker_t rotate_90() const;
    marker_t rotate_180() const;
    marker_t rotate_270() const;
    
    void save (size_t, const std::string&) const;

private:
    size_t _dim;
    elem_t _data;  // _dim by _dim sized matrix pattern
};

#endif /* marker_hpp */
