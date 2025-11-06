#pragma once
#include <cstdint>
#include <memory> 
#include <string> 

namespace adas
{

    // 位置与朝向（x、y 为 int32；heading 取 'N' 'S' 'E' 'W'）
    struct Pose
    {
        int32_t x;
        int32_t y;
        char heading;
    };

    class Executor final
    {
    public:
        // 若未显式初始化，则采用 (0,0,'N') 的默认值
        static std::unique_ptr<Executor> NewExecutor(const Pose &pose = {0, 0, 'N'}) noexcept;

        void Execute(const std::string &commands) noexcept;

        // 查询当前坐标与朝向
        [[nodiscard]] Pose Query() const noexcept;

        // 禁止拷贝/赋值，允许移动
        Executor(const Executor &) = delete;
        Executor &operator=(const Executor &) = delete;
        Executor(Executor &&) noexcept = default;
        Executor &operator=(Executor &&) noexcept = default;

        ~Executor();

    private:
        Executor();

        // 前向的实现体
        struct Impl;
        std::unique_ptr<Impl> pImpl_;
    };

}
