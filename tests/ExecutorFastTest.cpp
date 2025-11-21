#include "Executor.hpp"      // 包含你的主头文件
#include "PoseEq.hpp"        // 包含我们共享的 == 和 PrintTo
#include "gtest/gtest.h"

using adas::Executor;
using adas::Pose;

namespace adas
{
    // 测试 "FM" (加速状态下前进)
    // 对应PPT [cite: 520, 530-539]
    TEST(ExecutorFastTest, should_return_x_plus_2_given_status_is_fast_command_is_M_and_facing_is_E)
    {
        // given
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        // when
        executor->Execute("FM"); // "F" 进入加速, "M" 前进
        // then
        const Pose target{2, 0, 'E'}; // 期望：前进2格
        ASSERT_EQ(target, executor->Query());
    }

    // 测试 "FL" (加速状态下左转)
    // 对应PPT [cite: 521, 541-550]
    TEST(ExecutorFastTest, should_return_N_and_x_plus_1_given_status_is_fast_command_is_L_and_facing_is_E)
    {
        // given
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        // when
        executor->Execute("FL"); // "F" 进入加速, "L" 先前进1格再左转
        // then
        const Pose target{1, 0, 'N'}; // 期望：X+1, 朝向N
        ASSERT_EQ(target, executor->Query());
    }

    // 测试 "FR" (加速状态下右转)
    // 对应PPT [cite: 522, 551-560]
    TEST(ExecutorFastTest, should_return_S_and_x_plus_1_given_status_is_fast_given_command_is_R_and_facing_is_E)
    {
        // given
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        // when
        executor->Execute("FR"); // "F" 进入加速, "R" 先前进1格再右转
        // then
        const Pose target{1, 0, 'S'}; // 期望：X+1, 朝向S
        ASSERT_EQ(target, executor->Query());
    }

    // 测试 "FFM" (取消加速状态)
    // 对应PPT [cite: 523, 562-571]
    TEST(ExecutorFastTest, should_return_y_plus_1_given_command_is_FFM_and_facing_is_N)
    {
        // given
        std::unique_ptr<Executor> executor(Executor::NewExecutor()); // 默认 {0, 0, 'N'}
        // when
        executor->Execute("FFM"); // "F" 进入加速, "F" 取消加速, "M" 正常前进
        // then
        const Pose target{0, 1, 'N'}; // 期望：只前进1格
        ASSERT_EQ(target, executor->Query());
    }

} // namespace adas