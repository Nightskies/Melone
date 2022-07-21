#pragma once

#include <memory>
#include <variant>

#include "PlatformDetection.h"

template<class I>
using SPtr = std::shared_ptr<I>;

template<class I>
using UPtr = std::unique_ptr<I>;

template<class...Functors>
struct Overloaded : Functors... 
{
	using Functors::operator()...;
};

template<class...Functors>
Overloaded(Functors...)->Overloaded<Functors...>;

template<typename V, typename T, size_t I = 0>
constexpr size_t GetVariantIndex()
{
	if constexpr (I >= std::variant_size_v<V>)
	{
		return (std::variant_size_v<V>);
	}
	else
	{
		if constexpr (std::is_same_v<std::variant_alternative_t<I, V>, T>)
		{
			return (I);
		}
		else
		{
			return (GetVariantIndex<V, T, I + 1>());
		}
	}
}

template <typename E>
constexpr auto GetEnumIndex(E e) noexcept 
{
	return static_cast<std::underlying_type_t<E>>(e);
}

template <class T, class U> struct is_variant_type;

template <class T, class... Ts>
struct is_variant_type<T, std::variant<Ts...>>
	: std::bool_constant<(std::is_same_v<T, Ts> || ...)>
{ };

#ifdef MELONE_DEBUG
	#define MELONE_ENABLE_ASSERTS
#endif

#ifdef MELONE_ENABLE_ASSERTS
	#define MELONE_ASSERT(x, ...) { if(!(x)) { MELONE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MELONE_CORE_ASSERT(x, ...) { if(!(x)) { MELONE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define MELONE_ASSERT(x, ...)
	#define MELONE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)