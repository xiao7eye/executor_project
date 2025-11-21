#include "Executor.hpp"
#include "PoseEq.hpp"
#include "gtest/gtest.h"

namespace adas
{
    // TR: 左转 -> 前进 -> 左转
    TEST(ExecutorTurnRoundTest, should_normal_tr_build_left_forward_left)
    {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("TR");
        const Pose target{0, 1, 'W'}; // E->N(转) -> N(进) -> W(转)
        ASSERT_EQ(target, executor->Query());
    }

    // FTR: 前进 -> 左转 -> 前进 -> 左转
    TEST(ExecutorTurnRoundTest, should_fast_tr_build_forward_left_forward_left)
    {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("FTR");
        const Pose target{1, 1, 'W'};
        ASSERT_EQ(target, executor->Query());
    }

    // BTR: 倒车状态下忽略 TR
    TEST(ExecutorTurnRoundTest, in_the_B_state_the_reverse_command_will_be_ignored)
    {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("BTR");
        const Pose target{0, 0, 'E'};
        ASSERT_EQ(target, executor->Query());
    }
}