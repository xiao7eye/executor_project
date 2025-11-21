#pragma once
#include "ActionGroup.hpp"

namespace adas
{
    class MoveCommand final
    {
    public:
        ActionGroup operator()(PoseHandler &poseHandler) const noexcept
        {
            ActionGroup actionGroup;
            // 确定是前进还是后退
            const auto action = poseHandler.IsReverse() ? ActionType::BACKWARD_1_STEP_ACTION : ActionType::FORWARD_1_STEP_ACTION;

            if (poseHandler.IsFast())
                actionGroup.PushAction(action); // 加速：多走一步

            actionGroup.PushAction(action); // 基础的一步
            return actionGroup;
        }
    };

    class TurnLeftCommand final
    {
    public:
        ActionGroup operator()(PoseHandler &poseHandler) const noexcept
        {
            ActionGroup actionGroup;
            if (poseHandler.IsFast())
            {
                const auto moveAction = poseHandler.IsReverse() ? ActionType::BACKWARD_1_STEP_ACTION : ActionType::FORWARD_1_STEP_ACTION;
                actionGroup.PushAction(moveAction);
            }

            // 确定转向方向 (倒车时反向)
            const auto turnAction = poseHandler.IsReverse() ? ActionType::REVERSE_TURNLEFT_ACTION : ActionType::TURNLEFT_ACTION;
            actionGroup.PushAction(turnAction);
            return actionGroup;
        }
    };

    class TurnRightCommand final
    {
    public:
        ActionGroup operator()(PoseHandler &poseHandler) const noexcept
        {
            ActionGroup actionGroup;
            if (poseHandler.IsFast())
            {
                const auto moveAction = poseHandler.IsReverse() ? ActionType::BACKWARD_1_STEP_ACTION : ActionType::FORWARD_1_STEP_ACTION;
                actionGroup.PushAction(moveAction);
            }

            const auto turnAction = poseHandler.IsReverse() ? ActionType::REVERSE_TURNRIGHT_ACTION : ActionType::TURNRIGHT_ACTION;
            actionGroup.PushAction(turnAction);
            return actionGroup;
        }
    };

    class FastCommand final
    {
    public:
        ActionGroup operator()(PoseHandler &poseHandler) const noexcept
        {
            return ActionGroup({ActionType::BE_FAST_ACTION});
        }
    };

    class ReverseCommand final
    {
    public:
        ActionGroup operator()(PoseHandler &poseHandler) const noexcept
        {
            return ActionGroup({ActionType::BE_REVERSE_ACTION});
        }
    };

    // [新] 掉头指令 (TR)
    class TurnRoundCommand final
    {
    public:
        ActionGroup operator()(PoseHandler &poseHandler) const noexcept
        {
            // TR 不受 B 影响 (BTR = TR), 但受 F 影响
            if (poseHandler.IsReverse())
                return ActionGroup(); // B 状态下忽略 TR (PPT测试用例要求)

            if (poseHandler.IsFast())
            {
                // FTR: 前进1 -> 左转 -> 前进1 -> 左转
                return ActionGroup({ActionType::FORWARD_1_STEP_ACTION, ActionType::TURNLEFT_ACTION,
                                    ActionType::FORWARD_1_STEP_ACTION, ActionType::TURNLEFT_ACTION});
            }

            // TR: 左转 -> 前进1 -> 左转
            return ActionGroup({ActionType::TURNLEFT_ACTION, ActionType::FORWARD_1_STEP_ACTION, ActionType::TURNLEFT_ACTION});
        }
    };
}