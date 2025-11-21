#pragma once
#include <cstdint>
#include <memory>
#include <string>

namespace adas
{
    struct Pose
    {
        int32_t x;
        int32_t y;
        char heading;
    };

    class Executor final
    {
    public:
        // [修复] 将 Impl 的前向声明移到 public
        // 这样 src/ExecutorImpl.cpp 中的 ConcreteImpl 才能继承它
        struct Impl;

        static std::unique_ptr<Executor> NewExecutor(const Pose &pose = {0, 0, 'N'}) noexcept;
        // 支持指定车型 (0: Normal, 1: SportsCar, 2: Bus)
        static std::unique_ptr<Executor> NewExecutor(const Pose &pose, int vehicleType) noexcept;

        void Execute(const std::string &commands) noexcept;
        [[nodiscard]] Pose Query() const noexcept;

        Executor(const Executor &) = delete;
        Executor &operator=(const Executor &) = delete;
        Executor(Executor &&) noexcept = default;
        Executor &operator=(Executor &&) noexcept = default;
        ~Executor();

    private:
        // 私有构造函数接收 Impl 指针
        Executor(Impl *impl) noexcept;

        std::unique_ptr<Impl> pImpl_;
    };
}