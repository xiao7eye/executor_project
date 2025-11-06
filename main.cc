#include "executor.h"
#include <iostream>

int main()
{
    using adas::Executor;
    using adas::Pose;

    // 初始化
    auto exec1 = Executor::NewExecutor();
    exec1->Execute("MMR");
    Pose p1 = exec1->Query();
    std::cout << "Case1 -> x=" << p1.x << " y=" << p1.y << " h=" << p1.heading << "\n";

    // 显式初始化
    auto exec2 = Executor::NewExecutor({3, 4, 'W'});
    exec2->Execute("LMMRMLM");
    Pose p2 = exec2->Query();
    std::cout << "Case2 -> x=" << p2.x << " y=" << p2.y << " h=" << p2.heading << "\n";

    return 0;
}
