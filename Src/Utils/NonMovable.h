#pragma once

struct NonCopyable;

struct NonMovable
{
    NonMovable() = default;
    NonMovable& operator = (NonCopyable&&) = delete;
    NonMovable(NonCopyable&&) = delete;
};