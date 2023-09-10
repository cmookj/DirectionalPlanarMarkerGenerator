//
//  marker.hpp
//  count_markers
//
//  Created by Changmook Chun on 9/8/23.
//

#ifndef marker_hpp
#define marker_hpp

#include <vector>
#include <iostream>

enum class color_t {
    black, white
};

void invert(color_t&);

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

class dict_t {
public:
    dict_t () = default;
    virtual ~dict_t() = default;
    
    dict_t (const dict_t&) = default;
    dict_t (dict_t&&) = default;
    
    dict_t& operator= (const dict_t&) = default;
    dict_t& operator= (dict_t&&) = default;
        
    void append(const marker_t&);
    size_t count() const;
    
    const marker_t& at(const unsigned) const;
    
    void print(std::ostream& strm = std::cout) const;
    
    void save (const size_t, const size_t, const size_t, const std::string&, const size_t begin = 0) const;
    
private:
    std::vector<marker_t> _markers;
};

elem_t create_all_patterns(const unsigned);
dict_t create_dictionary(const unsigned);

#endif /* marker_hpp */
