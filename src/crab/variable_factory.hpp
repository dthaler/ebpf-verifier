// Copyright (c) Prevail Verifier contributors.
// SPDX-License-Identifier: MIT
#pragma once
#include <string>
#include <vector>

namespace crab {

enum class data_kind_t { types, values, offsets };

// This factory is eBPF-specific.
class variable_factory_t final {
    using index_t = uint64_t;

    friend class variable_t;

  private:
    std::vector<std::string> _variable_names;

    class variable_t make(const std::string& name);

    std::string get_variable_name(index_t id) const { return _variable_names.at(id); }

  public:
    variable_factory_t();
    ~variable_factory_t() {}

    variable_t reg(data_kind_t, int);
    variable_t cell_var(data_kind_t array, index_t offset, unsigned size);
    variable_t map_value_size();
    variable_t map_key_size();
    variable_t meta_offset();
    variable_t packet_size();
    variable_t instruction_count();
};

} // namespace crab
