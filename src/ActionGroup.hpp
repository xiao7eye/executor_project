#pragma once
#include "PoseHandler.hpp"
#include <functional>
#include <list>
#include <vector>

namespace adas
{
    // 定义所有原子操作类型
    enum class ActionType : uint16_t
    {
        FORWARD_1_STEP_ACTION = 0,
        BACKWARD_1_STEP_ACTION,
        TURNLEFT_ACTION,
        REVERSE_TURNLEFT_ACTION,
        TURNRIGHT_ACTION,
        REVERSE_TURNRIGHT_ACTION,
        BE_FAST_ACTION,
        BE_REVERSE_ACTION,
    };

    class ActionGroup final
    {
    public:
        ActionGroup(void) = default;
        // 允许用列表初始化：ActionGroup({ActionType::...})
        explicit ActionGroup(const std::list<ActionType> &actions) noexcept;
        ~ActionGroup() = default;

        void PushAction(const ActionType actionType) noexcept;
        void DoOperate(PoseHandler &poseHandler) const noexcept;

    private:
        std::list<ActionType> actions;
    };
}