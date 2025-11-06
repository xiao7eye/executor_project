#include "executor.h"
#include "gtest/gtest.h"

using adas::Executor;
using adas::Pose;

namespace adas
{

    // 辅助函数1：如何打印 Pose
    void PrintTo(const Pose &pose, std::ostream *os)
    {
        *os << "{ x: " << pose.x
            << ", y: " << pose.y
            << ", heading: '" << pose.heading << "' }";
    }

    // 辅助函数2：如何比较两个Pose
    bool operator==(const Pose &lhs, const Pose &rhs)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.heading == rhs.heading;
    }

} 
TEST(ExecutorTest, should_return_default_pose_when_without_init_and_command)
{
    //准备
    auto exec = Executor::NewExecutor();
    //执行
    //断言
    Pose expected = {0, 0, 'N'};
    EXPECT_EQ(exec->Query(), expected);
}

TEST(ExecutorTest, should_return_init_pose_when_without_command)
{
    //准备
    Pose initPose = {3, 4, 'W'};
    auto exec = Executor::NewExecutor(initPose);
    //执行
    //断言
    EXPECT_EQ(exec->Query(), initPose);
}

// M指令
TEST(ExecutorTest, should_move_to_x_plus_1_when_facing_E_and_command_M)
{
    auto exec = Executor::NewExecutor({0, 0, 'E'});
    exec->Execute("M");
    EXPECT_EQ(exec->Query(), Pose({1, 0, 'E'}));
}

TEST(ExecutorTest, should_move_to_x_minus_1_when_facing_W_and_command_M)
{
    auto exec = Executor::NewExecutor({0, 0, 'W'});
    exec->Execute("M");
    EXPECT_EQ(exec->Query(), Pose({-1, 0, 'W'}));
}

TEST(ExecutorTest, should_move_to_y_plus_1_when_facing_N_and_command_M)
{
    auto exec = Executor::NewExecutor({0, 0, 'N'});
    exec->Execute("M");
    EXPECT_EQ(exec->Query(), Pose({0, 1, 'N'}));
}

TEST(ExecutorTest, should_move_to_y_minus_1_when_facing_S_and_command_M)
{
    auto exec = Executor::NewExecutor({0, 0, 'S'});
    exec->Execute("M");
    EXPECT_EQ(exec->Query(), Pose({0, -1, 'S'}));
}

//L指令
TEST(ExecutorTest, should_turn_to_N_when_facing_E_and_command_L)
{
    auto exec = Executor::NewExecutor({0, 0, 'E'});
    exec->Execute("L");
    EXPECT_EQ(exec->Query(), Pose({0, 0, 'N'}));
}

TEST(ExecutorTest, should_turn_to_S_when_facing_W_and_command_L)
{
    auto exec = Executor::NewExecutor({0, 0, 'W'});
    exec->Execute("L");
    EXPECT_EQ(exec->Query(), Pose({0, 0, 'S'}));
}

TEST(ExecutorTest, should_turn_to_W_when_facing_N_and_command_L)
{
    auto exec = Executor::NewExecutor({0, 0, 'N'});
    exec->Execute("L");
    EXPECT_EQ(exec->Query(), Pose({0, 0, 'W'}));
}

TEST(ExecutorTest, should_turn_to_E_when_facing_S_and_command_L)
{
    auto exec = Executor::NewExecutor({0, 0, 'S'});
    exec->Execute("L");
    EXPECT_EQ(exec->Query(), Pose({0, 0, 'E'}));
}

//R指令
TEST(ExecutorTest, should_turn_to_S_when_facing_E_and_command_R)
{
    auto exec = Executor::NewExecutor({0, 0, 'E'});
    exec->Execute("R");
    EXPECT_EQ(exec->Query(), Pose({0, 0, 'S'}));
}

TEST(ExecutorTest, should_turn_to_N_when_facing_W_and_command_R)
{
    auto exec = Executor::NewExecutor({0, 0, 'W'});
    exec->Execute("R");
    EXPECT_EQ(exec->Query(), Pose({0, 0, 'N'}));
}

TEST(ExecutorTest, should_turn_to_E_when_facing_N_and_command_R)
{
    auto exec = Executor::NewExecutor({0, 0, 'N'});
    exec->Execute("R");
    EXPECT_EQ(exec->Query(), Pose({0, 0, 'E'}));
}

TEST(ExecutorTest, should_turn_to_W_when_facing_S_and_command_R)
{
    auto exec = Executor::NewExecutor({0, 0, 'S'});
    exec->Execute("R");
    EXPECT_EQ(exec->Query(), Pose({0, 0, 'W'}));
}
// 额外的测试：序列测试
TEST(ExecutorTest, should_handle_sequence_MMR_from_default)
{
    auto exec = Executor::NewExecutor(); // {0, 0, 'N'}
    exec->Execute("MMR");
    // N -> M -> {0, 1, 'N'}
    // N -> M -> {0, 2, 'N'}
    // N -> R -> {0, 2, 'E'}
    EXPECT_EQ(exec->Query(), Pose({0, 2, 'E'}));
}

TEST(ExecutorTest, should_handle_sequence_LMMRMLM_from_custom_pose)
{
    // 初始: {3, 4, 'W'}
    // L -> {3, 4, 'S'}
    // M -> {3, 3, 'S'}
    // M -> {3, 2, 'S'}
    // R -> {3, 2, 'W'}
    // M -> {2, 2, 'W'}
    // L -> {2, 2, 'S'}
    // M -> {2, 1, 'S'}
    auto exec = Executor::NewExecutor({3, 4, 'W'});
    exec->Execute("LMMRMLM");
    EXPECT_EQ(exec->Query(), Pose({2, 1, 'S'}));
}

TEST(ExecutorTest, should_ignore_invalid_commands)
{
    auto exec = Executor::NewExecutor({0, 0, 'N'});
    exec->Execute("MLRxLyMz");
    // N -> M -> {0, 1, 'N'}
    // N -> L -> {0, 1, 'W'}
    // W -> R -> {0, 1, 'N'}
    // N -> M -> {0, 2, 'N'}
    EXPECT_EQ(exec->Query(), Pose({-1, 1, 'W'}));
}