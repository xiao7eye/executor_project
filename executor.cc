#include "executor.h"
#include <cassert>

namespace adas
{

    struct Executor::Impl
    {
        Pose pose{0, 0, 'N'};

        static bool IsValidHeading(char h) noexcept
        {
            return h == 'N' || h == 'S' || h == 'E' || h == 'W';
        }

        // 左转 90°
        static char TurnLeft(char h) noexcept
        {
            switch (h)
            {
            case 'N':
                return 'W';
            case 'W':
                return 'S';
            case 'S':
                return 'E';
            case 'E':
                return 'N';
            default:
                return h;
            }
        }

        // 右转 90°
        static char TurnRight(char h) noexcept
        {
            switch (h)
            {
            case 'N':
                return 'E';
            case 'E':
                return 'S';
            case 'S':
                return 'W';
            case 'W':
                return 'N';
            default:
                return h;
            }
        }

        // 前进 1 格：x 沿 EW 轴，y 沿 NS 轴
        void MoveForward() noexcept
        {
            switch (pose.heading)
            {
            case 'N':
                ++pose.y;
                break; // 北：y+1
            case 'S':
                --pose.y;
                break; // 南：y-1
            case 'E':
                ++pose.x;
                break; // 东：x+1
            case 'W':
                --pose.x;
                break; // 西：x-1
            default:
                break;
            }
        }
    };

    // Executor外部接口实现
    Executor::Executor() : pImpl_(new Impl) {}

    Executor::~Executor() = default;

    std::unique_ptr<Executor> Executor::NewExecutor(const Pose &pose) noexcept
    {
        std::unique_ptr<Executor> exec(new Executor);
        assert(Impl::IsValidHeading(pose.heading));
        exec->pImpl_->pose = pose;
        return exec;
    }

    void Executor::Execute(const std::string &commands) noexcept
    {
        for (char c : commands)
        {
            switch (c)
            {
            case 'M':
                pImpl_->MoveForward();
                break;
            case 'L':
                pImpl_->pose.heading = Impl::TurnLeft(pImpl_->pose.heading);
                break;
            case 'R':
                pImpl_->pose.heading = Impl::TurnRight(pImpl_->pose.heading);
                break;
            default:
                break;
            }
        }
    }

    Pose Executor::Query() const noexcept
    {
        return pImpl_->pose;
    }

}