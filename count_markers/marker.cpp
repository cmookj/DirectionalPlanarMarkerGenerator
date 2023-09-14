//
//  marker.cpp
//  count_markers
//
//  Created by Changmook Chun on 9/8/23.
//

#include "marker.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>

#pragma mark - MARKER

marker_t::marker_t (const size_t n)
: _dim {n}
, _data {elem_t (n * n, color_t::white)} {}

marker_t::marker_t (const size_t n, elem_t&& new_elem)
: _dim {n}
, _data {new_elem}
{}

bool marker_t::operator== (const marker_t& rhs) const {
    return _dim == rhs._dim && std::equal(std::cbegin(_data), std::cend(_data), std::cbegin(rhs._data));
}

bool marker_t::operator!= (const marker_t& rhs) const {
    return !(*this == rhs);
}

size_t marker_t::dim() const {
    return _dim;
}

color_t marker_t::at(const size_t i, const size_t j) const {
    return _data[i * _dim + j];
}

void marker_t::print(std::ostream& strm) const {
    const size_t n = _dim;
    unsigned i = 0;
    std::for_each(std::cbegin(_data), std::cend(_data), [n, &i, &strm](const auto& d){
        if (i == n) {
            strm << '\n';
            i = 0;
        }
        strm << (d == color_t::black ? '*' : '_') << ' ';
        i++;
    });
    strm << "\n\n";
}

void marker_t::set(const unsigned i, const unsigned j) {
    if (i > _dim || j > _dim) return;
    
    _data[(i - 1)*_dim + (j - 1)] = color_t::black;
}

marker_t marker_t::rotate_90() const {
    elem_t new_data (_dim * _dim, color_t::white);
    
    // The index of the elements in the original pattern data is as follows:
    //
    //         0      1       ...    n-1
    //         n     n+1      ...   2n-1
    //        ...    ...      ...    ...
    //      n(n-1) n(n-1)+1   ...  n*n-1
    //
    // The elements in the rotated (by 90 degrees) pattern data is as follows:
    //
    //      n(n-1)    ...       n      0
    //     n(n-1)+1   ...      n+1     1
    //       ...      ...      ...    ...
    //      n*n-1     ...      2n-1   n-1
    //
    
    // The first row is copied to the last column, i.e., from (n-1)th element
    // for n times with stride of n.
    // The second row is copied to the second to last column, i.e., from (n-2)th
    // element for n times with stride of n.
    // The code below implements the algorithm.
    
    size_t top_row = _dim - 1; // Initial index of the top row.
    size_t row = top_row;
    size_t stride = _dim;
    size_t count = 0;
    
    std::for_each (std::cbegin(_data), std::cend(_data), [&new_data, &row, &count, &top_row, stride](const auto& e){
        if (e == color_t::black) {
            new_data[row] = color_t::black;
        }
        row += stride;
        count++;
        
        if (count == stride) {
            row = --top_row;
            count = 0;
        }
    });
    
    return marker_t {_dim, std::move(new_data)};
}

marker_t marker_t::rotate_180() const {
    marker_t rotated_by_90 = this -> rotate_90();
    return rotated_by_90.rotate_90();
}

marker_t marker_t::rotate_270() const {
    marker_t rotated_by_180 = this -> rotate_180();
    return rotated_by_180.rotate_90();
}

void marker_t::save (size_t size, const std::string& filename) const {
    // Set size (minimum width of a cell = 3 pixels)
    const size_t gap = 1;
    const size_t min_size = 3 * _dim + (_dim + gap);
    if (size < min_size) size = min_size;
    
    // Prepare bitmap data
    auto data = std::make_unique<unsigned char[]>(size * size);
    memset (data.get(), 0xff, size * size); // Fill with white
    
    const size_t cell_size = static_cast<size_t>(double(size - (_dim + 1) * gap) / double(_dim + 2));
    for (size_t i = 0; i < _dim; ++i) {
        for (size_t j = 0; j < _dim; ++j) {
            if (_data[i * _dim + j] == color_t::black) {
                for (size_t k = 0; k < cell_size; ++k)
                    memset (data.get()
                            + ((i + 1)*cell_size + ((i + 1) * gap) + k)*size
                            + (j + 1)*(cell_size + gap), 0, cell_size);
            }
            else {
                for (size_t k = 0; k < cell_size; ++k)
                    memset (data.get()
                            + ((i + 1)*cell_size + ((i + 1) * gap) + k)*size
                            + (j + 1)*(cell_size + gap), 200, cell_size);
            }
        }
    }
    
    // Write file header with the bitmap data
    std::ofstream out {filename, std::ios::binary};
    
    out << "P5\n";
    out << size << ' ' << size << '\n';
    out << "255\n";
    
    out.write (
               reinterpret_cast<const char*> (data.get()),
               sizeof (unsigned char) * size * size
               );
}
