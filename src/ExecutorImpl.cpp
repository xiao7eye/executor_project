#include "CmderFactory.hpp"
#include "Executor.hpp"
#include "PoseHandler.hpp"
#include "Singleton.hpp"
#include <algorithm>

namespace adas
{
    // 1. 定义 Impl 基类
    struct Executor::Impl
    {
        PoseHandler poseHandler_;
        Impl(const Pose &pose) noexcept : poseHandler_(pose) {}
        virtual ~Impl() = default;
        virtual void Execute(const std::string &commands) = 0; // 纯虚函数
    };

    // 2. 定义具体类型的 Impl
    template <typename MapType>
    struct ConcreteImpl final : public Executor::Impl
    {
        ConcreteImpl(const Pose &pose) noexcept : Impl(pose) {}

        void Execute(const std::string &commands) override
        {
            // 使用对应的 MapType 工厂
            const auto cmders = Singleton<CmderFactory<MapType>>::Instance().GetCmders(commands);
            std::for_each(cmders.begin(), cmders.end(),
                          [this](const Cmder &cmder) noexcept
                          {
                              cmder(this->poseHandler_).DoOperate(this->poseHandler_);
                          });
        }
    };

    // 3. Executor 实现
    Executor::Executor(Impl *impl) noexcept : pImpl_(impl) {}
    Executor::~Executor() = default;

    // 默认工厂 (普通车)
    std::unique_ptr<Executor> Executor::NewExecutor(const Pose &pose) noexcept
    {
        return std::unique_ptr<Executor>(new Executor(new ConcreteImpl<NormalMap>(pose)));
    }

    // [新] 带类型的工厂
    std::unique_ptr<Executor> Executor::NewExecutor(const Pose &pose, int vehicleType) noexcept
    {
        switch (vehicleType)
        {
        case 1: // SportsCar
            return std::unique_ptr<Executor>(new Executor(new ConcreteImpl<SportsCarMap>(pose)));
        case 2: // Bus
            return std::unique_ptr<Executor>(new Executor(new ConcreteImpl<BusMap>(pose)));
        default: // Normal
            return std::unique_ptr<Executor>(new Executor(new ConcreteImpl<NormalMap>(pose)));
        }
    }

    void Executor::Execute(const std::string &commands) noexcept
    {
        pImpl_->Execute(commands);
    }

    Pose Executor::Query() const noexcept
    {
        return pImpl_->poseHandler_.Query();
    }

} // namespace adas