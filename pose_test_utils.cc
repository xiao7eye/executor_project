#include "pose_test_utils.h"
#include <tuple> // 为了 std::tie

namespace adas
{
    // 定义：如何打印
    void PrintTo(const Pose &pose, std::ostream *os)
    {
        *os << "{ x: " << pose.x
            << ", y: " << pose.y
            << ", heading: '" << pose.heading << "' }";
    }

    // 定义：如何比较
    bool operator==(const Pose &lhs, const Pose &rhs)
    {
        // std::tie 会自动比较元组中的所有成员
        return std::tie(lhs.x, lhs.y, lhs.heading) ==
               std::tie(rhs.x, rhs.y, rhs.heading);
    }

} // namespace adas