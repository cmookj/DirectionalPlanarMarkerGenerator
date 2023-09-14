//
//  dictionary.hpp
//  count_markers
//
//  Created by Changmook Chun on 9/8/23.
//

#ifndef dictionary_hpp
#define dictionary_hpp

#include "marker.hpp"

#include <vector>
#include <iostream>

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

#endif /* dictionary_hpp */
