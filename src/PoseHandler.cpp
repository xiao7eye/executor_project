#include "PoseHandler.hpp"

namespace adas
{
    PoseHandler::PoseHandler(const Pose &pose) noexcept : pose_(pose)
    {
    }

    // (MoveForward, TurnLeft, TurnRight 保持不变)
    void PoseHandler::MoveForward() noexcept
    {
        switch (pose_.heading)
        {
        case 'N':
            ++pose_.y;
            break;
        case 'S':
            --pose_.y;
            break;
        case 'E':
            ++pose_.x;
            break;
        case 'W':
            --pose_.x;
            break;
        default:
            break;
        }
    }

    void PoseHandler::TurnLeft() noexcept
    {
        switch (pose_.heading)
        {
        case 'N':
            pose_.heading = 'W';
            break;
        case 'W':
            pose_.heading = 'S';
            break;
        case 'S':
            pose_.heading = 'E';
            break;
        case 'E':
            pose_.heading = 'N';
            break;
        default:
            break;
        }
    }

    void PoseHandler::TurnRight() noexcept
    {
        switch (pose_.heading)
        {
        case 'N':
            pose_.heading = 'E';
            break;
        case 'E':
            pose_.heading = 'S';
            break;
        case 'S':
            pose_.heading = 'W';
            break;
        case 'W':
            pose_.heading = 'N';
            break;
        default:
            break;
        }
    }

    // (ToggleFast, IsFast, Query 保持不变)
    void PoseHandler::ToggleFast() noexcept
    {
        isFast_ = !isFast_;
    }

    bool PoseHandler::IsFast() const noexcept
    {
        return isFast_;
    }

    Pose PoseHandler::Query() const noexcept
    {
        return pose_;
    }

    // --- [新] 'B' 指令的实现 ---

    // [新] M(倒车)的逻辑: 后退一格
    // [对应需求3.md]
    void PoseHandler::MoveReverse() noexcept
    {
        switch (pose_.heading)
        {
        case 'N':
            --pose_.y;
            break; // 北：y-1
        case 'S':
            ++pose_.y;
            break; // 南：y+1
        case 'E':
            --pose_.x;
            break; // 东：x-1
        case 'W':
            ++pose_.x;
            break; // 西：x+1
        default:
            break;
        }
    }

    // [新] 切换 'B' 状态
    void PoseHandler::ToggleReverse() noexcept
    {
        isReverse_ = !isReverse_;
    }

    // [新] 查询 'B' 状态
    bool PoseHandler::IsReverse() const noexcept
    {
        return isReverse_;
    }

} // namespace adas