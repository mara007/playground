#pragma once

#include <iostream>
#include <ostream>
#include <map>
#include <string>

#define TRACE() std::cout << "[" << __FILE__ << ":" << __LINE__ << "] "

//! arbitrary multipurpose properties
using tags_t = std::map<std::string, std::string>;

static std::ostream& operator<<(std::ostream& ostr, const tags_t& tags) {
    ostr << "[";
    for (auto& [k,v] : tags)
        ostr << "{" << k << ", " << v << "},";
    ostr << "]\n";
    return ostr;
}

