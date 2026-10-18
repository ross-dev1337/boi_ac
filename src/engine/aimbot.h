#pragma once 

namespace aimbot {
    structs::player* closest_client(); 
    void vec_to_angles(const math::vec3& vec, math::vec3& angles);
    void do_aimbot(); 
    bool is_visible(structs::player* local, const math::vec3& from, const math::vec3& to); 
}