#include "ActionGroup.hpp"
#include <algorithm>

namespace adas
{
    ActionGroup::ActionGroup(const std::list<ActionType> &actions) noexcept : actions(actions)
    {
    }

    void ActionGroup::PushAction(const ActionType actionType) noexcept
    {
        actions.push_back(actionType);
    }

    void ActionGroup::DoOperate(PoseHandler &poseHandler) const noexcept
    {
        // 定义原子操作的具体实现 (Lambda)
        static const std::vector<std::function<void(PoseHandler &)>> actionVec = {
            [](PoseHandler &ph)
            { ph.MoveForward(); }, // FORWARD_1_STEP_ACTION
            [](PoseHandler &ph)
            { ph.MoveReverse(); }, // BACKWARD_1_STEP_ACTION
            [](PoseHandler &ph)
            { ph.TurnLeft(); }, // TURNLEFT_ACTION
            [](PoseHandler &ph)
            { ph.TurnRight(); }, // REVERSE_TURNLEFT_ACTION (倒车时左转=右转)
            [](PoseHandler &ph)
            { ph.TurnRight(); }, // TURNRIGHT_ACTION
            [](PoseHandler &ph)
            { ph.TurnLeft(); }, // REVERSE_TURNRIGHT_ACTION (倒车时右转=左转)
            [](PoseHandler &ph)
            { ph.ToggleFast(); }, // BE_FAST_ACTION
            [](PoseHandler &ph)
            { ph.ToggleReverse(); } // BE_REVERSE_ACTION
        };

        // 批量执行
        std::for_each(actions.begin(), actions.end(), [&poseHandler](const ActionType actionType) mutable noexcept
                      { actionVec[static_cast<uint16_t>(actionType)](poseHandler); });
    }
}