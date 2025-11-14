#include "executor.h"
#include "Command.h"
#include "PoseHandler.h"
#include <cassert>
#include <functional>
#include <memory>
#include <unordered_map>

namespace adas
{
    struct Executor::Impl
    {
        PoseHandler poseHandler_;

        using CommandFunc = std::function<void(PoseHandler &)>;
        const std::unordered_map<char, CommandFunc> commandMap_;

        // [修改] 在 map 中注册 'B' 指令
        Impl(const Pose &pose) noexcept
            : poseHandler_(pose),
              commandMap_{
                  {'M', MoveCommand()},
                  {'L', TurnLeftCommand()},
                  {'R', TurnRightCommand()},
                  {'F', FastCommand()},
                  {'B', ReverseCommand()} // [新] 添加 'B' 指令
              }
        {
        }

        static bool IsValidHeading(char h) noexcept
        {
            return h == 'N' || h == 'S' || h == 'E' || h == 'W';
        }
    };

    // ======== Executor 外部接口实现 (无任何变化) ========

    Executor::Executor(const Pose &pose) noexcept
        : pImpl_(std::make_unique<Impl>(pose))
    {
    }

    Executor::~Executor() = default;

    std::unique_ptr<Executor> Executor::NewExecutor(const Pose &pose) noexcept
    {
        if (!Impl::IsValidHeading(pose.heading))
        {
            return std::unique_ptr<Executor>(new Executor(Pose{0, 0, 'N'}));
        }
        return std::unique_ptr<Executor>(new Executor(pose));
    }

    // [无变化] Execute 函数完美地支持了 'B' 指令，无需修改
    void Executor::Execute(const std::string &commands) noexcept
    {
        for (char c : commands)
        {
            const auto it = pImpl_->commandMap_.find(c);

            if (it != pImpl_->commandMap_.end())
            {
                it->second(pImpl_->poseHandler_);
            }
        }
    }

    Pose Executor::Query() const noexcept
    {
        return pImpl_->poseHandler_.Query();
    }

} // namespace adas