#pragma once

#include <cstdarg>
#include <functional>
#include <log/assert.hpp>
#include <utility>
#include <variant>
namespace utils
{
template <typename... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};


template <typename T, typename E>
class Result
{
    union Value
    {
        Value(T value) : ok{value} {}
        Value(E value) : err{value} {}

        T ok;
        E err;
    } const value;

    enum class State
    {
        Ok,
        Err,
    } const state;

public:
    Result(T ok) : value{ok}, state{State::Ok} {}
    Result(E err) : value{err}, state{State::Err} {}

    [[nodiscard]] operator std::variant<T, E>() const noexcept
    {
        switch (state)
        {
        case State::Ok:
            return std::variant{value.ok};

        case State::Err:
            return std::variant{value.err};

        default:
            std::unreachable();
        }
        std::unreachable();
    }

    [[nodiscard]]
    inline T Get() const noexcept
    {
        ASSERT(state == State::Ok);
        return value.ok;
    }

    [[nodiscard]]
    inline E Err() const noexcept
    {
        ASSERT(state == State::Err);
        return value.err;
    }

    [[nodiscard]]
    inline T GetOr(T other) const noexcept
    {
        switch (state)
        {
        case State::Ok:
            return value.ok;

        case State::Err:
            return other;

        default:
            std::unreachable();
        }
        std::unreachable();
    }

    inline bool IsOk() const noexcept { return state == State::Ok; }

    inline bool IsErr() const noexcept { return state == State::Err; }

    inline void IfOk(std::function<void(T)> func) const noexcept
    {
        Visit(func, [](E) {});
    }

    inline void IfErr(std::function<void(T)> func) const noexcept
    {
        Visit([](T) {}, func);
    }

    inline void Visit(std::function<void(T)> okFunc, std::function<void(E)> errFunc) const noexcept
    {
        switch (state)
        {
        case State::Ok:
            okFunc(value.ok);
            return;

        case State::Err:
            errFunc(value.err);
            return;

        default:
            std::unreachable();
        }
        std::unreachable();
    }
};
} // namespace utils
