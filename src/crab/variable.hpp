// Copyright (c) Prevail Verifier contributors.
// SPDX-License-Identifier: MIT
#pragma once

#include <iosfwd>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <vector>

#include "crab_utils/bignums.hpp"
#include "crab_utils/debug.hpp"
#include "radix_tree/radix_tree.hpp"
#include "crab/variable_factory.hpp"

using index_t = uint64_t;

/* Basic type definitions */

namespace crab {

// XXX: this is eBPF-specific.
std::ostream& operator<<(std::ostream& o, const data_kind_t& s);

// Wrapper for typed variables used by the crab abstract domains and linear_constraints.
// Being a class (instead of a type alias) enables overloading in dsl_syntax
class variable_t final {
    friend variable_factory_t;

    index_t _id;
    variable_factory_t& _factory;

    explicit variable_t(index_t id, variable_factory_t& factory) : _id(id), _factory(factory) {}

  public:
    variable_t& operator=(variable_t&& other) {
        if (this != &other) {
            _id = other._id;
            _factory = std::move(other._factory);
        }
        return *this;
    }

    variable_t& operator=(const variable_t& other) {
        _id = other._id;
        _factory = other._factory;
        return *this;
    }

    variable_t(const crab::variable_t& other) : _id(other._id), _factory(other._factory) {}

    [[nodiscard]] std::size_t hash() const { return (size_t)_id; }

    bool operator==(variable_t o) const { return _id == o._id; }

    bool operator!=(variable_t o) const { return (!(operator==(o))); }

    // for flat_map
    bool operator<(variable_t o) const { return _id < o._id; }


    [[nodiscard]] std::string name() const { return _factory.get_variable_name(_id); }

    bool is_type() { return name().find(".type") != std::string::npos; }

    friend std::ostream& operator<<(std::ostream& o, variable_t v)  { return o << v.name(); }
}; // class variable_t

inline size_t hash_value(variable_t v) { return v.hash(); }

} // namespace crab
