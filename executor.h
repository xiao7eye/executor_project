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
        // [修改] 构造函数现在是创建 Impl 的唯一方式
        static std::unique_ptr<Executor> NewExecutor(const Pose &pose = {0, 0, 'N'}) noexcept;

        void Execute(const std::string &commands) noexcept;
        [[nodiscard]] Pose Query() const noexcept;

        Executor(const Executor &) = delete;
        Executor &operator=(const Executor &) = delete;
        Executor(Executor &&) noexcept = default;
        Executor &operator=(Executor &&) noexcept = default;

        ~Executor();

    private:
        // [修改] 私有构造函数现在需要一个 Pose
        Executor(const Pose &pose) noexcept;

        struct Impl;
        std::unique_ptr<Impl> pImpl_;
    };

}