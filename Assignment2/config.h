#pragma once

// SPEED
#define shell_speed 2

// SPAWN DIRECTIONS
#define spawn_directions std::vector({ \
    Direction::LEFT,      \
    Direction::RIGHT,     \
    Direction::LEFT,      \
    Direction::RIGHT,     \
    Direction::LEFT,      \
    Direction::RIGHT,     \
    Direction::LEFT,      \
    Direction::RIGHT,     \
    Direction::LEFT,      \
})

// HP
#define wall_hp 2
#define tank_hp 1

// DAMAGE
#define shell_damage 1
#define mine_damage 1
#define collision_damage 1

// steps after shell send
#define steps_after_shells_end 40

// wait and cooldown
#define shoot_cooldown_steps 4
#define backward_wait_steps 2