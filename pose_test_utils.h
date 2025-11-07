#pragma once
#include "executor.h"
#include <iostream> // 为了 std::ostream

namespace adas
{
    // 声明：告诉GTest如何打印Pose
    void PrintTo(const Pose &pose, std::ostream *os);

    // 声明：告诉GTest如何比较 (==) 两个Pose
    bool operator==(const Pose &lhs, const Pose &rhs);

} // namespace adas