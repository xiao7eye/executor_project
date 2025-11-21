#pragma once
#include "Executor.hpp"

namespace adas
{
    class PoseHandler final
    {
    public:
        PoseHandler(const Pose &pose) noexcept;

        PoseHandler(const PoseHandler &) = delete;
        PoseHandler &operator=(const PoseHandler &) = delete;
        PoseHandler(PoseHandler &&) noexcept = default;
        PoseHandler &operator=(PoseHandler &&) noexcept = default;
        ~PoseHandler() = default;

        // --- 公共动作接口 ---
        void MoveForward() noexcept;
        void TurnLeft() noexcept;
        void TurnRight() noexcept;
        void ToggleFast() noexcept;
        bool IsFast() const noexcept;
        Pose Query() const noexcept;

        // [新] 'B' 指令所需的新接口
        void MoveReverse() noexcept;     // 新增：后退
        void ToggleReverse() noexcept;   // 新增：切换倒车
        bool IsReverse() const noexcept; // 新增：查询倒车

    private:
        Pose pose_;
        bool isFast_{false};
        bool isReverse_{false}; // [新] B指令的状态
    };

} // namespace adas