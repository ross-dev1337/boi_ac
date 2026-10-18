#include "dll_main.h"
 
structs::player* aimbot::closest_client() {
    const auto local = structs::local_player;
    if (local == nullptr || structs::ent == nullptr)
        return nullptr;

    structs::player* c_client = nullptr;

    const auto& local_origin = local->origin1;
    float best_dist = FLT_MAX;

    const auto max_players = static_cast<uint32_t>(std::size(structs::ent->entites));
    const auto player_count = (std::min)(*globals::game_vars::player_count, max_players);

    for (uint32_t i = 0; i < player_count; i++) {
        auto& enemy = structs::ent->entites[i];

        if (enemy == nullptr)
            continue;

        if (functions::client::is_me(enemy) || functions::client::is_alive(enemy) || functions::client::is_friend(enemy))
            continue;

        const float dist = local_origin.distance(enemy->origin1);
        if (dist < best_dist) {
            best_dist = dist;
            c_client = enemy;
        }
    }
    return c_client;
}

void aimbot::vec_to_angles(const math::vec3& vec, math::vec3& angles) {
    constexpr float rad_to_deg = 180.f / std::numbers::pi_v<float>;

    const float distance = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

    if (distance == 0.f) {
        angles = {};
        return;
    }

    angles.x = -std::atan2(vec.x, vec.y) * rad_to_deg + 180.f;
    const float pitch_input = std::clamp(vec.z / distance, -1.f, 1.f);
    angles.y = asinf(pitch_input) * rad_to_deg;
    angles.z = 0.f;
}

void aimbot::do_aimbot() {
    if (!globals::menu_options::aimbot::aimbot)
        return;

    const auto* target = aimbot::closest_client();
    if (target == nullptr)
        return;

    const auto local = structs::local_player;

    if (!is_visible(local, local->origin1, target->origin1))
        return;

    const math::vec3 direction = target->origin1 - local->origin1;
    math::vec3 angles{};
    vec_to_angles(direction, angles);

    local->view_angles = angles;
}

bool aimbot::is_visible(structs::player* local, const math::vec3& from, const math::vec3& to) {
    structs::traceresult_s tr{};
    const uint32_t fn = globals::game_vars::trace_line;
    const float fx = from.x, fy = from.y, fz = from.z;
    const float tx = to.x,   ty = to.y,   tz = to.z;

    __asm {
        push 0              // skip_tags
        lea  eax, tr
        push eax            // traceresult_s*
        push tz             // to (pushed right-to-left: z, y, x)
        push ty
        push tx
        push fz             // from
        push fy
        push fx
        mov  ecx, local     // tracer
        xor  edx, edx       // dl = check_players = false
        call fn
        add  esp, 0x20      // caller cleanup -- verify, see below
    }
    return !tr.collided;
}