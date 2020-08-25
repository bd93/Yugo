#pragma once

#include <memory>

template<typename T>
using sPtr = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr sPtr<T> sPtrCreate(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using uPtr = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr uPtr<T> uPtrCreate(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

#define ARRAY_SIZE(array) sizeof(array)/sizeof(array[0])