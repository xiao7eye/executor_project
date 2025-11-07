#include "executor.h"
#include <cassert>
#include <memory>

namespace adas
{

    // ======== 内部实现体 ========
    struct Executor::Impl
    {
        // 1. 车辆状态 (添加 fast_ 状态)
        // [对应PPT cite: 635]
        Pose pose{0, 0, 'N'};
        bool isFast_{false}; // [新] F指令的状态

        // --- 2. 抽象命令接口 (无变化) ---
        class ICommand
        {
        public:
            virtual ~ICommand() = default;
            virtual void DoOperate(Executor::Impl &executor) const noexcept = 0;
        };

        // --- 3. 辅助函数 (添加 IsFast/ToggleFast) ---

        // [新] F指令的状态切换函数 [cite: 631, 637-640]
        void ToggleFast() noexcept
        {
            isFast_ = !isFast_;
        }

        // [新] F指令的状态查询函数 [cite: 632, 641-644]
        bool IsFast() const noexcept
        {
            return isFast_;
        }

        // (原有的辅助函数)
        static bool IsValidHeading(char h) noexcept
        {
            return h == 'N' || h == 'S' || h == 'E' || h == 'W';
        }

        void TurnLeft() noexcept
        {
            switch (pose.heading)
            {
            case 'N':
                pose.heading = 'W';
                break;
            case 'W':
                pose.heading = 'S';
                break;
            case 'S':
                pose.heading = 'E';
                break;
            case 'E':
                pose.heading = 'N';
                break;
            default:
                break;
            }
        }

        void TurnRight() noexcept
        {
            switch (pose.heading)
            {
            case 'N':
                pose.heading = 'E';
                break;
            case 'E':
                pose.heading = 'S';
                break;
            case 'S':
                pose.heading = 'W';
                break;
            case 'W':
                pose.heading = 'N';
                break;
            default:
                break;
            }
        }

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

        // --- 4. 具体命令 (添加 FastCommand, 修改 M/L/R) ---

        // [新] FastCommand: 切换 'F' 状态
        // [对应PPT cite: 646-653]
        class FastCommand final : public ICommand
        {
        public:
            void DoOperate(Executor::Impl &executor) const noexcept override
            {
                executor.ToggleFast();
            }
        };

        // [修改] MoveCommand: 支持 'F' 状态
        // [对应PPT cite: 672-683]
        class MoveCommand final : public ICommand
        {
        public:
            void DoOperate(Executor::Impl &executor) const noexcept override
            {
                // 加速状态下 (isFast) [cite: 148]
                if (executor.IsFast())
                {
                    executor.MoveForward(); // 多移动一次
                }
                executor.MoveForward(); // 基础移动 (总共1次或2次)
            }
        };

        // [修改] TurnLeftCommand: 支持 'F' 状态
        // [对应PPT cite: 149]
        class TurnLeftCommand final : public ICommand
        {
        public:
            void DoOperate(Executor::Impl &executor) const noexcept override
            {
                // 加速状态下 (isFast) [cite: 149]
                if (executor.IsFast())
                {
                    executor.MoveForward(); // 先前进1格
                }
                executor.TurnLeft(); // 再左转
            }
        };

        // [修改] TurnRightCommand: 支持 'F' 状态
        // [对应PPT cite: 150]
        class TurnRightCommand final : public ICommand
        {
        public:
            void DoOperate(Executor::Impl &executor) const noexcept override
            {
                // 加速状态下 (isFast) [cite: 150]
                if (executor.IsFast())
                {
                    executor.MoveForward(); // 先前进1格
                }
                executor.TurnRight(); // 再右转
            }
        };
    };

    // ======== Executor 外部接口实现 ========
    Executor::Executor() : pImpl_(new Impl) {}

    Executor::~Executor() = default;

    std::unique_ptr<Executor> Executor::NewExecutor(const Pose &pose) noexcept
    {
        std::unique_ptr<Executor> exec(new Executor);
        assert(Impl::IsValidHeading(pose.heading));
        exec->pImpl_->pose = pose;
        return exec;
    }

    // [修改] Execute: 添加 'F' 到命令工厂
    // [对应PPT cite: 654-671]
    void Executor::Execute(const std::string &commands) noexcept
    {
        for (char c : commands)
        {
            std::unique_ptr<Impl::ICommand> cmder;

            switch (c)
            {
            case 'M':
                cmder = std::make_unique<Impl::MoveCommand>();
                break;
            case 'L':
                cmder = std::make_unique<Impl::TurnLeftCommand>();
                break;
            case 'R':
                cmder = std::make_unique<Impl::TurnRightCommand>();
                break;
            case 'F': // [新] 添加 'F' 指令
                cmder = std::make_unique<Impl::FastCommand>();
                break;
            default:
                break;
            }

            if (cmder)
            {
                cmder->DoOperate(*pImpl_);
            }
        }
    }

    Pose Executor::Query() const noexcept
    {
        return pImpl_->pose;
    }

} // namespace adas