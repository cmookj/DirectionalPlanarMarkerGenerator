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

void invert(color_t& color) {
    if (color == color_t::white) {
        color = color_t::black;
        return;
    }
    color = color_t::white;
}

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


#pragma mark - DICT

void dict_t::append(const marker_t& a_marker) {
    _markers.push_back(a_marker);
}

size_t dict_t::count() const {
    return _markers.size();
}

const marker_t& dict_t::at(const unsigned id) const {
    return _markers.at(id);
}

void dict_t::print(std::ostream& strm) const {
    strm << "The number of markers in dictionary = " << _markers.size() << '\n';
    std::for_each(std::cbegin(_markers),
                  std::cend(_markers),
                  [&strm](const auto& marker){ marker.print(strm); });
}

void dict_t::save (const size_t mkr_size, const size_t rows, const size_t cols, const std::string& filename, const size_t begin) const {
    if (_markers.size() == begin) return;
    
    // Set size
    const size_t gap = 5;
    const size_t width = mkr_size * cols + gap * (cols + 1);
    const size_t height = mkr_size * rows + gap * (rows + 1);
    
    // Prepare bitmap data
    auto data = std::make_unique<unsigned char[]>(width * height);
    memset (data.get(), 0xff, width * height); // Fill with white
    
    size_t I = 1, J = 1;
    // for (const auto& marker : _markers) {
    for_each (std::cbegin(_markers) + begin,
              std::cbegin(_markers) + begin + (rows * cols),
              [mkr_size, gap, width, cols, &data, &I, &J](const auto& marker) {
        
        const size_t dim = marker.dim();
        const size_t gap_cell = 1;
        const size_t cell_size = static_cast<size_t>(double(mkr_size - (dim + 1) * gap_cell) / double(dim + 2));
        for (size_t i = 0; i < dim; ++i) {
            for (size_t j = 0; j < dim; ++j) {
                if (marker.at(i, j) == color_t::black) {
                    for (size_t k = 0; k < cell_size; ++k)
                        memset (data.get()
                                + (I * gap + (I - 1) * mkr_size) * width
                                + ((i + 1)*cell_size + ((i + 1) * gap_cell) + k)*width
                                + (J * gap + (J - 1) * mkr_size)
                                + (j + 1)*(cell_size + gap_cell), 0, cell_size);
                }
                else {
                    for (size_t k = 0; k < cell_size; ++k)
                        memset (data.get()
                                + (I * gap + (I - 1) * mkr_size) * width
                                + ((i + 1)*cell_size + ((i + 1) * gap_cell) + k)*width
                                + (J * gap + (J - 1) * mkr_size)
                                + (j + 1)*(cell_size + gap_cell), 200, cell_size);
                }
            }
        }
        
        if (++J > cols) {
            I++;
            J = 1;
        }
    }
              );
    
    // Write file header with the bitmap data
    std::ofstream out {filename, std::ios::binary};
    
    out << "P5\n";
    out << width << ' ' << height << '\n';
    out << "255\n";
    
    out.write (
               reinterpret_cast<const char*> (data.get()),
               sizeof (unsigned char) * width * height
               );
}

#pragma mark - OTHER ROUTINES

void print_patterns(const unsigned n, const elem_t& patterns, std::ostream& strm = std::cout) {
    unsigned i = 0;
    std::for_each(std::cbegin(patterns), std::cend(patterns), [n, &i, &strm](const auto& d){
        if (i == n) {
            strm << '\n';
            i = 0;
        }
        strm << (d == color_t::black ? '*' : '_') << ' ';
        i++;
    });
    strm << "\n\n";
}

bool unique_all(const std::vector<marker_t>& markers) {
    size_t max = markers.size();
    
    for (size_t i = 0; i < max; ++i) {
        for (size_t j = 0; j < max; ++j) {
            if (i == j) continue;
            if (markers[i] == markers[j]) return false;
        }
    }
    return true;
}

bool exists_in_dict(const dict_t& dict, const std::vector<marker_t>& rotated_markers) {
    for (unsigned i = 0; i < dict.count(); ++i) {
        marker_t marker_in_dict = dict.at(i);
        
        // Check criterion #1
        auto found_it = std::find_if(std::cbegin(rotated_markers),
                                     std::cend(rotated_markers), [&marker_in_dict](const auto& marker){
            return marker == marker_in_dict;
        });
        
        if (found_it != std::cend(rotated_markers)) return true;
    }
    return false;
}

elem_t create_all_patterns(const unsigned n_bits) {
    // Create a data structure to represent all possible markers
    //
    // For example, when n_bits = 3
    //
    //         0  1  2
    //      0  *  *  *
    //      1  *  *  _
    //      2  *  _  *
    //      3  *  _  _
    //      4  _  *  *
    //      5  _  *  _
    //      6  _  _  *
    //      7  _  _  _
    //
    
    unsigned count_patterns = std::pow(2, n_bits);
    elem_t all_patterns (count_patterns * n_bits, color_t::white);
    unsigned group_len = std::pow(2, n_bits-1);
    
    for (unsigned col = 0; col < n_bits; ++col) {
        auto val = color_t::white;
        for (unsigned row = 0; row < count_patterns; ++row) {
            if (row % group_len == 0) invert(val);
            all_patterns[row * n_bits + col] = val;
        }
        group_len /= 2;
    }
    
    return elem_t {std::move(all_patterns)};
}

dict_t create_dictionary(const unsigned n_bits) {
    dict_t a_dictionary {};
    
    if (n_bits == 0) return a_dictionary;
    
    unsigned count_patterns = std::pow(2, n_bits);
    elem_t all_patterns = create_all_patterns(n_bits);
    unsigned dim = static_cast<unsigned>(std::sqrt(n_bits));
    
    // TEST
    print_patterns(n_bits, all_patterns);
    
    // Given a new current_pattern,
    //
    // 1. If any of the current_pattern's 0, 90, 180, and 270 degrees rotations
    //    have the same pattern, REJECT it.
    // 2. If any of the current_pattern's 90, 180, and 270 degrees rotations
    //    already exists in dictionary, REJECT it.
    // 3. Otherwise, add it to the dictionary.
    //
    for (unsigned pat_id = 0; pat_id < count_patterns; ++pat_id) {
        // Create a marker with current_pattern
        elem_t a_pattern (n_bits, color_t::white);
        std::copy(std::cbegin(all_patterns) + pat_id*n_bits,
                  std::cbegin(all_patterns) + (pat_id+1)*n_bits,
                  std::begin(a_pattern));
        marker_t a_marker {dim, std::move(a_pattern)};
        marker_t a_marker_rotated_90 = a_marker.rotate_90();
        marker_t a_marker_rotated_180 = a_marker.rotate_180();
        marker_t a_marker_rotated_270 = a_marker.rotate_270();
        
        std::vector<marker_t> rotated_markers {
            a_marker,
            a_marker_rotated_90,
            a_marker_rotated_180,
            a_marker_rotated_270
        };
        
        // TEST
        // a_marker.print();
        
        // Check criterion #1
        if (unique_all(rotated_markers)) {
            if (a_dictionary.count() == 0)
                a_dictionary.append(a_marker);
            else
                // Check criterion #2
                if (!exists_in_dict(a_dictionary, rotated_markers))
                    a_dictionary.append(a_marker);
        }
    }
    
    return a_dictionary;
}
