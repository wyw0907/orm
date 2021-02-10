#ifndef _WISH_TYPE_TRAITS_H
#define _WISH_TYPE_TRAITS_H
#include <string>
#include <memory>
#include <vector>
#include <tuple>
#include <array>
#include <type_traits>
#include <optional>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <functional>

namespace wish
{
    template <template <typename...> class U, typename T>
    struct is_template_instant_of : std::false_type
    {
    };

    template <template <typename...> class U, typename... Args>
    struct is_template_instant_of<U, U<Args...>> : std::true_type
    {
    };

    template <typename T>
    struct is_optional : is_template_instant_of<std::optional, T>
    {
    };

    template <typename T>
    struct is_sequence_container : std::bool_constant<is_template_instant_of<std::vector, T>::value || is_template_instant_of<std::deque, T>::value || is_template_instant_of<std::list, T>::value>
    {
    };

    template <typename T>
    struct is_associat_container : std::bool_constant<is_template_instant_of<std::map, T>::value || is_template_instant_of<std::set, T>::value>
    {
    };

    // like char [N], char *
    template <class T>
    constexpr bool is_char_array_v = std::is_array_v<T> && std::is_same_v<char, std::remove_pointer_t<T>>;

    template <typename T>
    struct type_to_nullable
    {
        using type = std::optional<T>;
    };

    template <typename T>
    struct type_to_nullable<std::optional<T>>
    {
        using type = std::optional<T>;
    };

    template <typename... Args>
    auto tuple_to_nullvalue(std::tuple<Args...> &&t)
    {
        return std::tuple<typename type_to_nullable<Args>::type...>(std::move(t));
    }

    template <typename... Args>
    auto tuple_to_nullvalue(const std::tuple<Args...> &t)
    {
        return std::tuple<typename type_to_nullable<Args>::type...>(t);
    }

    template <typename T>
    struct field_attribute
    {
    };

    template <typename T, typename U>
    struct field_attribute<U T::*>
    {
        using type = T;
        using field_type = U;
    };
} // namespace wish
#endif