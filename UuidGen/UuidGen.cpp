#include <cassert>
#include <iostream>
#include <string>

#include <fmt/core.h>

#define UUID_SYSTEM_GENERATOR
#include <stduuid/uuid.h>

int main() {
    std::random_device rd;
    auto               seed_data = std::array<int, std::mt19937::state_size>{};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq                seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937                 generator(seq);
    uuids::uuid_random_generator gen{generator};

    uuids::uuid const id = gen();
    assert(!id.is_nil());
    assert(id.as_bytes().size() == 16);
    assert(id.version() == uuids::uuid_version::random_number_based);
    assert(id.variant() == uuids::uuid_variant::rfc);

    fmt::println("flag{{{}}}", uuids::to_string(id));
}