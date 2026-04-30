//
// Created by micha on 22.04.2026.
//

#pragma once

#include <stdexcept>
#include <juce_core/juce_core.h>

class InvalidTreeError : public std::runtime_error
{
public:
    using _Mybase = std::runtime_error;

    explicit InvalidTreeError(const juce::String& _Message) : _Mybase(_Message.getCharPointer()) {}
    explicit InvalidTreeError(const std::string& _Message) : _Mybase(_Message) {}
    explicit InvalidTreeError(const char* _Message) : _Mybase(_Message) {}
};

class LimitError : public std::runtime_error
{
public:
    using _Mybase = std::runtime_error;

    explicit LimitError(const juce::String& _Message) : _Mybase(_Message.getCharPointer()) {}
    explicit LimitError(const char* _Message) : _Mybase(_Message) {}
    explicit LimitError(const std::string& _Message) : _Mybase(_Message) {}
};

#define throwassert(condition, exc) \
JUCE_BLOCK_WITH_FORCED_SEMICOLON( \
    if(!(condition)) \
        throw exc;   \
)

#define throwassertfalse(exc) throwassert(false, exc)


