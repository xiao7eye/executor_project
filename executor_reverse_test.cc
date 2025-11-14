#include "executor.h"        // 包含你的主头文件
#include "pose_test_utils.h" // 包含我们共享的 == 和 PrintTo
#include "gtest/gtest.h"

using adas::Executor;
using adas::Pose;

namespace adas
{
    // B: 倒车状态
    // M：后退1格
    // [cite: 285-286]
    TEST(ExecutorReverseTest, should_return_x_minus_1_given_status_is_reverse_command_is_M_and_facing_is_E)
    {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("BM"); // B(倒车), M(后退1)
        const Pose target{-1, 0, 'E'};
        ASSERT_EQ(target, executor->Query());
    }

    // L：右转90度
    // [cite: 301-302]
    TEST(ExecutorReverseTest, should_return_S_given_status_is_reverse_command_is_L_and_facing_is_E)
    {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("BL"); // B(倒车), L(右转)
        const Pose target{0, 0, 'S'};
        ASSERT_EQ(target, executor->Query());
    }

    // R：左转90度
    // [cite: 313-314]
    TEST(ExecutorReverseTest, should_return_N_given_status_is_reverse_command_is_R_and_facing_is_E)
    {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("BR"); // B(倒车), R(左转)
        const Pose target{0, 0, 'N'};
        ASSERT_EQ(target, executor->Query());
    }

    // BB: 状态取消
    // [cite: 293-295]
    TEST(ExecutorReverseTest, should_return_y_plus_1_given_command_is_BBM_and_facing_is_N)
    {
        std::unique_ptr<Executor> executor(Executor::NewExecutor()); // {0, 0, 'N'}
        executor->Execute("BBM");                                    // B(倒车), B(取消), M(正常前进1)
        const Pose target{0, 1, 'N'};
        ASSERT_EQ(target, executor->Query());
    }

    // --- 叠加状态测试 ---
    // [cite: 1446-1450]

    // B + F + M: 倒退2格
    // [cite: 290-292]
    TEST(ExecutorReverseTest, should_return_x_minus_2_given_status_is_reverse_and_fast_command_is_M_and_facing_is_E)
    {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        // B, F 顺序不重要
        executor->Execute("BFM"); // B(倒车), F(加速), M(倒退2)
        const Pose target{-2, 0, 'E'};
        ASSERT_EQ(target, executor->Query());
    }

    // B + F + L: 先倒退一格，然后右转90度
    // [cite: 307-308]
    TEST(ExecutorReverseTest, should_return_S_and_x_minus_1_given_status_is_reverse_and_fast_command_is_L_and_facing_is_E)
    {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("BFL"); // B(倒车), F(加速), L(倒退1, 右转)
        const Pose target{-1, 0, 'S'};
        ASSERT_EQ(target, executor->Query());
    }

    // B + F + R: 先倒退一格，然后左转90度
    // [cite: 319-320]
    TEST(ExecutorReverseTest, should_return_N_and_x_minus_1_given_status_is_reverse_and_fast_command_is_R_and_facing_is_E)
    {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("BFR"); // B(倒车), F(加速), R(倒退1, 左转)
        const Pose target{-1, 0, 'N'};
        ASSERT_EQ(target, executor->Query());
    }

} // namespace adas