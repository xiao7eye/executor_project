#pragma once
#include "Command.hpp"
#include <functional>
#include <list>
#include <string>
#include <string_view>
#include <unordered_map>

namespace adas
{
    using Cmder = std::function<ActionGroup(PoseHandler &poseHandler)>;
    using CmderList = std::list<Cmder>;

    // 1. 普通车配置
    class NormalMap
    {
    public:
        static const std::unordered_map<char, Cmder> &Get() noexcept
        {
            static const std::unordered_map<char, Cmder> cmderMap{
                {'M', MoveCommand()},
                {'L', TurnLeftCommand()},
                {'R', TurnRightCommand()},
                {'F', FastCommand()},
                {'B', ReverseCommand()},
                {'Z', TurnRoundCommand()},
            };
            return cmderMap;
        }
    };

    // 2. 跑车配置
    class SportsCarMap
    {
    public:
        static const std::unordered_map<char, Cmder> &Get() noexcept
        {
            static const std::unordered_map<char, Cmder> cmderMap{
                {'M', [](PoseHandler &ph)
                 {
                     return ActionGroup({ActionType::FORWARD_1_STEP_ACTION, ActionType::FORWARD_1_STEP_ACTION});
                 }},
                {'L', [](PoseHandler &ph)
                 {
                     return ActionGroup({ActionType::TURNLEFT_ACTION, ActionType::FORWARD_1_STEP_ACTION});
                 }},
                {'R', [](PoseHandler &ph)
                 {
                     return ActionGroup({ActionType::TURNRIGHT_ACTION, ActionType::FORWARD_1_STEP_ACTION});
                 }},
                {'F', FastCommand()},
                {'B', ReverseCommand()},
                {'Z', TurnRoundCommand()},
            };
            return cmderMap;
        }
    };

    // 3. 公交车配置
    class BusMap
    {
    public:
        static const std::unordered_map<char, Cmder> &Get() noexcept
        {
            static const std::unordered_map<char, Cmder> cmderMap{
                {'M', MoveCommand()},
                {'L', [](PoseHandler &ph)
                 {
                     return ActionGroup({ActionType::FORWARD_1_STEP_ACTION, ActionType::TURNLEFT_ACTION});
                 }},
                {'R', [](PoseHandler &ph)
                 {
                     return ActionGroup({ActionType::FORWARD_1_STEP_ACTION, ActionType::TURNRIGHT_ACTION});
                 }},
                {'F', FastCommand()},
                {'B', ReverseCommand()},
                {'Z', TurnRoundCommand()},
            };
            return cmderMap;
        }
    };

    // 4. 模板工厂
    template <typename MapType = NormalMap>
    class CmderFactory final
    {
    public:
        CmderList GetCmders(const std::string &commands) const noexcept
        {
            CmderList cmders;
            for (const auto cmd : ParseCommandString(commands))
            {
                const auto &map = MapType::Get();
                const auto it = map.find(cmd);
                if (it != map.end())
                {
                    cmders.push_back(it->second);
                }
            }
            return cmders;
        }

    private:
        std::string ParseCommandString(std::string_view commands) const noexcept
        {
            std::string result(commands);
            ReplaceAll(result, "TR", "Z");
            return result;
        }

        void ReplaceAll(std::string &inout, std::string_view what, std::string_view with) const noexcept
        {
            for (std::string::size_type pos{};
                 inout.npos != (pos = inout.find(what.data(), pos, what.length()));
                 pos += with.length())
            {
                inout.replace(pos, what.length(), with.data(), with.length());
            }
        }
    };
}