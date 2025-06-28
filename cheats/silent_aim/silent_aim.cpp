#include <Windows.h>
#include <thread>
#include <random>
#include <vector>
#include <immintrin.h>
#include <cmath>
#include <future>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")


#include "../features.h"
#include "../../misc/globals/globals.hpp"
#define M_PI 3.14159265358979323846

RBX::Instance getFreeAimClosestPart(RBX::PlayerInstance& player, const POINT& cursor_point) {
    std::vector<RBX::Instance> parts = {
        player.head, player.rootPart, player.upperTorso, player.lowerTorso,
        player.leftUpperLeg, player.leftFoot, player.rightFoot,
        player.leftUpperArm, player.leftHand, player.rightUpperArm, player.rightHand,
    };

    RBX::Vector2 dimensions = globals::visualengine.GetDimensions();
    RBX::Matrix4x4 view_matrix = globals::visualengine.GetViewMatrix();
    RBX::Vector2 cursor = { static_cast<float>(cursor_point.x), static_cast<float>(cursor_point.y) };
    float min_distance = FLT_MAX;
    RBX::Instance closest_part;

    for (size_t i = 0; i < parts.size(); ++i) {
        if (!parts[i].address) continue;

        RBX::Vector3 part_position = parts[i].GetPosition();
        RBX::Vector2 part_screen_position = RBX::WorldToScreen(part_position, dimensions, view_matrix);
        float distance = (part_screen_position - cursor).getMagnitude();

        if (distance < min_distance) {
            min_distance = distance;
            closest_part = parts[i];
        }
    }

    return closest_part;
}

static RBX::Vector3 Recalculate_Velocity(RBX::PlayerInstance player)
{
    RBX::Vector3 old_Position = player.rootPart.GetPosition();
    std::this_thread::sleep_for(std::chrono::milliseconds(115));
    return (player.rootPart.GetPosition() - old_Position) / 0.115;
}

static RBX::PlayerInstance getClosestPlayerFromCursor() {
    POINT cursor_point;
    GetCursorPos(&cursor_point);
    ScreenToClient(FindWindowA(0, ("Roblox")), &cursor_point);

    RBX::Vector2 cursor = { static_cast<float>(cursor_point.x), static_cast<float>(cursor_point.y) };
    std::vector<RBX::PlayerInstance>& cached_players = globals::cached_players;
    RBX::PlayerInstance closestPlayer{};
    int shortestDistance = 9e9;

    RBX::PlayerInstance localPlayer = globals::localplayer;
    RBX::Instance localPlayerTeam = localPlayer.team;

    RBX::Vector2 dimensions = globals::visualengine.GetDimensions();
    RBX::Matrix4x4 viewmatrix = globals::visualengine.GetViewMatrix();

    for (RBX::PlayerInstance& player : cached_players) {
        if (player.address == localPlayer.address || !player.character.address || !player.humanoid.address)
            continue;

        bool knockedCheck = globals::silent_aim_checks[0];
        bool deadCheck = globals::silent_aim_checks[1];
        bool grabbedCheck = globals::silent_aim_checks[2];
        bool teamCheck = globals::silent_aim_checks[3];

        if (knockedCheck && player.knockedOut.getBoolFromValue())
            continue;

        if (deadCheck && player.humanoid.GetHealth() <= 0)
            continue;

        if (grabbedCheck && player.ifGrabbed.address != 0)
            continue;

        if (teamCheck && player.team.address == localPlayerTeam.address)
            continue;

        RBX::Instance part = player.rootPart;
        RBX::Vector3 partPosition = part.GetPosition();
        RBX::Vector2 partPositionOnScreen = RBX::WorldToScreen(partPosition, dimensions, viewmatrix);

        float distance_from_cursor = (partPositionOnScreen - cursor).getMagnitude();
        if (shortestDistance > distance_from_cursor) {
            closestPlayer = player;
            shortestDistance = distance_from_cursor;
        }
    }

    return closestPlayer;
}

static bool isWithinFOV(const RBX::Vector3& hit_position_3D) {
    POINT cursor_point;
    GetCursorPos(&cursor_point);
    ScreenToClient(FindWindowA(0, ("Roblox")), &cursor_point);

    auto cursor_pos_x = cursor_point.x;
    auto cursor_pos_y = cursor_point.y;

    RBX::Instance visualengine = globals::visualengine;
    RBX::Vector2 screen_dimensions = visualengine.GetDimensions();
    RBX::Vector2 hit_position_2D = RBX::WorldToScreen(hit_position_3D, screen_dimensions, visualengine.GetViewMatrix());

    float magnitude = (hit_position_2D - RBX::Vector2{ static_cast<float>(cursor_pos_x), static_cast<float>(cursor_pos_y) }).getMagnitude();
    return (magnitude <= globals::free_aim_fov);
}

static void run(RBX::PlayerInstance player, POINT cursor_point)
{
    RBX::Vector3 hit_position_3D{};

    RBX::Instance character = player.character;
    RBX::Instance hitbox{};
    static POINT game_mouse_pos;

    GetCursorPos(&game_mouse_pos);
    ScreenToClient(FindWindowA(0, ("Roblox")), &game_mouse_pos);

    RBX::PlayerInstance localPlayer = globals::localplayer;

    bool knockedCheck = globals::silent_aim_checks[0];
    bool grabbedCheck = globals::silent_aim_checks[1];
    bool teamCheck = globals::silent_aim_checks[2];

    if (knockedCheck == true && player.knockedOut.getBoolFromValue() == true)
        return;

    if (grabbedCheck && player.ifGrabbed.address != 0)
        return;

    if (teamCheck == true && player.team.address == localPlayer.team.address)
        return;

    if (globals::free_aim_closest_part) {
        hitbox = getFreeAimClosestPart(player, cursor_point);
    }
    else {
        switch (globals::free_aim_part) {
        case 0: hitbox = player.head; break;
        case 1: hitbox = player.rootPart; break;
        case 2: hitbox = player.upperTorso; break;
        case 3: hitbox = player.lowerTorso; break;
        case 4: hitbox = player.leftHand; break;
        case 5: hitbox = player.rightHand; break;
        case 6: hitbox = player.leftUpperArm; break;
        case 7: hitbox = player.rightUpperArm; break;
        case 8: hitbox = player.leftUpperLeg; break;
        case 9: hitbox = player.rightUpperLeg; break;
        case 10: hitbox = player.leftFoot; break;
        case 11: hitbox = player.rightFoot; break;
        }
    }

    hit_position_3D = hitbox.GetPosition();

    GetCursorPos(&cursor_point);
    ScreenToClient(FindWindowA(0, ("Roblox")), &cursor_point);

    RBX::Instance visualengine = globals::visualengine;
    RBX::Vector2 dimensions = visualengine.GetDimensions();
    RBX::Matrix4x4 view_matrix = visualengine.GetViewMatrix();

    if (globals::free_aim_prediction) {
        RBX::Vector3 velocity_vec = hitbox.GetVelocity();
        velocity_vec = velocity_vec / RBX::Vector3{ globals::free_aim_prediction_x, globals::free_aim_prediction_y, globals::free_aim_prediction_x };
        hit_position_3D = hitbox.GetPosition() + velocity_vec;
    }

    RBX::Vector2 hit_position_2D = RBX::WorldToScreen(hit_position_3D, dimensions, view_matrix);

    if (hit_position_2D.x == -1 || hit_position_2D.y == -1)
        return;

    float screen_height = dimensions.y;

    float magnitude = (hit_position_2D - RBX::Vector2{ static_cast<float>(cursor_point.x), static_cast<float>(cursor_point.y) }).getMagnitude();

    if (globals::free_aim_is_in_fov && magnitude > globals::free_aim_fov)
        return;

    POINT mousePos;
    GetCursorPos(&mousePos);

    RBX::Vector2 current_position = { static_cast<float>(cursor_point.x), static_cast<float>(cursor_point.y) };
    uint64_t new_position_x = static_cast<uint64_t>(cursor_point.x);
    uint64_t new_position_y = static_cast<uint64_t>(screen_height - std::abs(dimensions.y - (cursor_point.y)) - 58);

    RBX::Instance dahood_aim_instance = globals::localplayer.aim;
    RBX::Instance hc_aim_instance = globals::localplayer.hc_aim;

    RBX::Vector2 move_delta = hit_position_2D - current_position;

   
        hc_aim_instance.setFramePosX(new_position_x);
        hc_aim_instance.setFramePosY(new_position_y);
        dahood_aim_instance.setFramePosX(new_position_x);
       dahood_aim_instance.setFramePosY(new_position_y);

   
    if (globals::mouse_service) {
        RBX::Instance::initialize_mouse_service(globals::mouse_service);
        RBX::Instance{}.write_mouse_position(globals::mouse_service, hit_position_2D.x, hit_position_2D.y);
    }

    return;
}

RBX::PlayerInstance aimbot_target2;

void RBX::initsilent() {
    ::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
    RBX::PlayerInstance savedPlayer{};
    bool isAimboting = false;
    RBX::Vector3 calculatedVelocity{};
    RBX::PlayerInstance currentPlayer;
    RBX::PlayerInstance localplayer = globals::localplayer;

    HWND rblx = FindWindowA(0, "Roblox");
    while (true) {
    ///    
        ::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
        POINT cursor_point;
        GetCursorPos(&cursor_point);

        if (GetForegroundWindow() != rblx) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        globals::free_aim_bind.update();

        bool is_right_click_held = globals::free_aim_bind.enabled;


        if (!is_right_click_held || !globals::free_aim) {
            isAimboting = false;
            aimbot_target2.address = 0;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
      //  std::this_thread::sleep_for(std::chrono::milliseconds(50));
        currentPlayer = (globals::free_aim_sticky && isAimboting && savedPlayer.address != 0) ? savedPlayer : getClosestPlayerFromCursor();
        if (currentPlayer.address == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            isAimboting = false;
            aimbot_target2.address = 0;
            continue;
        }

        aimbot_target2 = currentPlayer;

        run(currentPlayer, cursor_point);
        savedPlayer = currentPlayer;
        isAimboting = true;
    }
 //   std::this_thread::sleep_for(std::chrono::milliseconds(50));
}