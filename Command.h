#pragma once
#include "PoseHandler.h"
#include <memory>

namespace adas
{
    // [新] 倒车指令
    class ReverseCommand final
    {
    public:
        void operator()(PoseHandler &poseHandler) const noexcept
        {
            poseHandler.ToggleReverse();
        }
    };

    // [修改] MoveCommand 支持 B 和 B+F 状态
    class MoveCommand final
    {
    public:
        void operator()(PoseHandler &poseHandler) const noexcept
        {
            // 检查倒车状态
            bool isReverse = poseHandler.IsReverse();
            // B+F 叠加: 倒退2格 [cite: 1447]
            if (isReverse && poseHandler.IsFast())
            {
                poseHandler.MoveReverse();
            }
            // B 状态: 倒退1格 [cite: 1443]
            // F 状态: 前进2格
            else if (poseHandler.IsFast())
            {
                poseHandler.MoveForward();
            }

            // B 状态: 调用倒退
            if (isReverse)
            {
                poseHandler.MoveReverse();
            }
            // 正常状态: 调用前进
            else
            {
                poseHandler.MoveForward();
            }
        }
    };

    // [修改] TurnLeftCommand 支持 B 和 B+F 状态
    class TurnLeftCommand final
    {
    public:
        void operator()(PoseHandler &poseHandler) const noexcept
        {
            bool isReverse = poseHandler.IsReverse();
            bool isFast = poseHandler.IsFast();

            // B+F 叠加: 先倒退一格 [cite: 1448]
            if (isReverse && isFast)
            {
                poseHandler.MoveReverse();
            }
            // F 状态: 先前进一格
            else if (isFast)
            {
                poseHandler.MoveForward();
            }

            // B 状态: L指令执行右转 [cite: 1444]
            if (isReverse)
            {
                poseHandler.TurnRight();
            }
            // 正常状态: L指令执行左转
            else
            {
                poseHandler.TurnLeft();
            }
        }
    };

    // [修改] TurnRightCommand 支持 B 和 B+F 状态
    class TurnRightCommand final
    {
    public:
        void operator()(PoseHandler &poseHandler) const noexcept
        {
            bool isReverse = poseHandler.IsReverse();
            bool isFast = poseHandler.IsFast();

            // B+F 叠加: 先倒退一格 [cite: 1449]
            if (isReverse && isFast)
            {
                poseHandler.MoveReverse();
            }
            // F 状态: 先前进一格
            else if (isFast)
            {
                poseHandler.MoveForward();
            }

            // B 状态: R指令执行左转 [cite: 1445]
            if (isReverse)
            {
                poseHandler.TurnLeft();
            }
            // 正常状态: R指令执行右转
            else
            {
                poseHandler.TurnRight();
            }
        }
    };

    // (FastCommand 保持不变)
    class FastCommand final
    {
    public:
        void operator()(PoseHandler &poseHandler) const noexcept
        {
            poseHandler.ToggleFast();
        }
    };

} // namespace adas