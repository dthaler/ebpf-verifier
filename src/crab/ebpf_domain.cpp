// Copyright (c) Prevail Verifier contributors.
// SPDX-License-Identifier: MIT
#pragma once

#include "crab_verifier_job.hpp"

using crab::domains::ebpf_domain_t;
using crab::variable_factory_t;

const program_info& ebpf_domain_t::get_program_info() const { return m_job->get_program_info(); }

variable_factory_t& ebpf_domain_t::get_variable_factory(crab_verifier_job_t* job) {
    return job->variable_factory();
}

variable_factory_t& ebpf_domain_t::variable_factory() { return m_job->variable_factory(); }
