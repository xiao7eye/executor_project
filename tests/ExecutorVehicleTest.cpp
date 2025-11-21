#include "Executor.hpp"
#include "PoseEq.hpp"
#include "gtest/gtest.h"

namespace adas
{
    // 跑车测试
    TEST(SportsCarTest, should_move_2_steps_when_command_M)
    {
        // vehicleType = 1 (SportsCar)
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}, 1));
        executor->Execute("M");
        const Pose target{2, 0, 'E'};
        ASSERT_EQ(target, executor->Query());
    }

    TEST(SportsCarTest, should_turn_left_then_move_1_step)
    {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}, 1));
        executor->Execute("L");
        const Pose target{0, 1, 'N'}; // E->N (Left) -> (0,1) (Move)
        ASSERT_EQ(target, executor->Query());
    }

    // 公交车测试
    TEST(BusTest, should_move_1_step_then_turn_left)
    {
        // vehicleType = 2 (Bus)
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}, 2));
        executor->Execute("L");
        const Pose target{1, 0, 'N'}; // (1,0) (Move) -> N (Left)
        ASSERT_EQ(target, executor->Query());
    }
}