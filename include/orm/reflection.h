#ifndef _WISH_REFLECTION
#define _WISH_REFLECTION

#include "type_traits.h"

// 让被展开的宏再次展开，需要增加MACRO_EXPAND
#define MACRO_EXPAND(...) __VA_ARGS__

#define MAKE_ARG_LIST_1(op, arg, ...) op(arg)
#define MAKE_ARG_LIST_2(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_1(op, __VA_ARGS__))
#define MAKE_ARG_LIST_3(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_2(op, __VA_ARGS__))
#define MAKE_ARG_LIST_4(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_3(op, __VA_ARGS__))
#define MAKE_ARG_LIST_5(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_4(op, __VA_ARGS__))
#define MAKE_ARG_LIST_6(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_5(op, __VA_ARGS__))
#define MAKE_ARG_LIST_7(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_6(op, __VA_ARGS__))
#define MAKE_ARG_LIST_8(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_7(op, __VA_ARGS__))
#define MAKE_ARG_LIST_9(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_8(op, __VA_ARGS__))
#define MAKE_ARG_LIST_10(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_9(op, __VA_ARGS__))
#define MAKE_ARG_LIST_11(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_10(op, __VA_ARGS__))
#define MAKE_ARG_LIST_12(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_11(op, __VA_ARGS__))
#define MAKE_ARG_LIST_13(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_12(op, __VA_ARGS__))
#define MAKE_ARG_LIST_14(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_13(op, __VA_ARGS__))
#define MAKE_ARG_LIST_15(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_14(op, __VA_ARGS__))
#define MAKE_ARG_LIST_16(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_15(op, __VA_ARGS__))
#define MAKE_ARG_LIST_17(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_16(op, __VA_ARGS__))
#define MAKE_ARG_LIST_18(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_17(op, __VA_ARGS__))
#define MAKE_ARG_LIST_19(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_18(op, __VA_ARGS__))
#define MAKE_ARG_LIST_20(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_19(op, __VA_ARGS__))
#define MAKE_ARG_LIST_21(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_20(op, __VA_ARGS__))
#define MAKE_ARG_LIST_22(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_21(op, __VA_ARGS__))
#define MAKE_ARG_LIST_23(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_22(op, __VA_ARGS__))
#define MAKE_ARG_LIST_24(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_23(op, __VA_ARGS__))
#define MAKE_ARG_LIST_25(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_24(op, __VA_ARGS__))
#define MAKE_ARG_LIST_26(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_25(op, __VA_ARGS__))
#define MAKE_ARG_LIST_27(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_26(op, __VA_ARGS__))
#define MAKE_ARG_LIST_28(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_27(op, __VA_ARGS__))
#define MAKE_ARG_LIST_29(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_28(op, __VA_ARGS__))
#define MAKE_ARG_LIST_30(op, arg, ...) op(arg), MACRO_EXPAND(MAKE_ARG_LIST_29(op, __VA_ARGS__))

// -------------concat as a, b ,c ----------
#define SEPERATOR ,
#define CON_STR_1(element) #element
#define CON_STR_2(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_1(__VA_ARGS__))
#define CON_STR_3(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_2(__VA_ARGS__))
#define CON_STR_4(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_3(__VA_ARGS__))
#define CON_STR_5(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_4(__VA_ARGS__))
#define CON_STR_6(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_5(__VA_ARGS__))
#define CON_STR_7(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_6(__VA_ARGS__))
#define CON_STR_8(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_7(__VA_ARGS__))
#define CON_STR_9(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_8(__VA_ARGS__))
#define CON_STR_10(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_9(__VA_ARGS__))
#define CON_STR_11(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_10(__VA_ARGS__))
#define CON_STR_12(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_11(__VA_ARGS__))
#define CON_STR_13(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_12(__VA_ARGS__))
#define CON_STR_14(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_13(__VA_ARGS__))
#define CON_STR_15(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_14(__VA_ARGS__))
#define CON_STR_16(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_15(__VA_ARGS__))
#define CON_STR_17(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_16(__VA_ARGS__))
#define CON_STR_18(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_17(__VA_ARGS__))
#define CON_STR_19(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_18(__VA_ARGS__))
#define CON_STR_20(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_19(__VA_ARGS__))
#define CON_STR_21(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_20(__VA_ARGS__))
#define CON_STR_22(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_21(__VA_ARGS__))
#define CON_STR_23(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_22(__VA_ARGS__))
#define CON_STR_24(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_23(__VA_ARGS__))
#define CON_STR_25(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_24(__VA_ARGS__))
#define CON_STR_26(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_25(__VA_ARGS__))
#define CON_STR_27(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_26(__VA_ARGS__))
#define CON_STR_28(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_27(__VA_ARGS__))
#define CON_STR_29(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_28(__VA_ARGS__))
#define CON_STR_30(element, ...) #element SEPERATOR MACRO_EXPAND(CON_STR_29(__VA_ARGS__))

#define MACRO_CONCAT1(A, B) A##_##B
#define MACRO_CONCAT(A, B) MACRO_CONCAT1(A, B)

// -------------get_arg_count--------------
#define RSEQ_N() \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16,   \
              _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, N, ...) \
    N

#define GET_ARG_COUNT_INNER(...) MACRO_EXPAND(ARG_N(__VA_ARGS__))
#define GET_ARG_COUNT(...) GET_ARG_COUNT_INNER(__VA_ARGS__, RSEQ_N())

#define FIELD(t) t
#define MAKE_ARG_LIST(N, op, arg, ...) \
    MACRO_CONCAT(MAKE_ARG_LIST, N)     \
    (op, arg, __VA_ARGS__)

inline constexpr void reflect_members(...) {}

#define MAKE_META_DATA_IMPL(STRUCT_NAME, ...)                                             \
    inline constexpr auto reflect_members(STRUCT_NAME const &)                            \
    {                                                                                     \
        struct members                                                                    \
        {                                                                                 \
            using size_type = std::integral_constant<size_t, GET_ARG_COUNT(__VA_ARGS__)>; \
            constexpr static const char *name()                                           \
            {                                                                             \
                return #STRUCT_NAME;                                                      \
            }                                                                             \
            constexpr static auto size()                                                  \
            {                                                                             \
                return size_type::value;                                                  \
            }                                                                             \
            constexpr static auto arr()                                                   \
            {                                                                             \
                return arr_##STRUCT_NAME;                                                 \
            }                                                                             \
            constexpr static auto tuple()                                                 \
            {                                                                             \
                return std::make_tuple(__VA_ARGS__);                                      \
            }                                                                             \
        };                                                                                \
        return members{};                                                                 \
    }
/* target code 
 eg 
 struct T{
     A a,
     B b,
     C c,
 }
 constexpr std::arrray<const char *, 3> arr_T = {a, b, c};
 MAKE_META_DATA_IMPL(t, &T::a, &T::b, &T::c);
*/
#define MAKE_META_DATA(STRUCT_NAME, N, ...)                    \
    constexpr std::array<const char *, N> arr_##STRUCT_NAME =  \
        {MACRO_EXPAND(MACRO_CONCAT(CON_STR, N)(__VA_ARGS__))}; \
    MAKE_META_DATA_IMPL(STRUCT_NAME, MAKE_ARG_LIST(N, &STRUCT_NAME::FIELD, __VA_ARGS__))

#define REFLECTION(STRUCT_NAME, ...) MAKE_META_DATA(STRUCT_NAME, GET_ARG_COUNT(__VA_ARGS__), __VA_ARGS__)

namespace wish
{
    template <typename T>
    struct is_reflection
    {
        constexpr static bool value = !std::is_void_v<decltype(reflect_members(std::declval<T>()))>;
    };
    template <typename T>
    constexpr bool is_reflection_v = is_reflection<T>::value;

    namespace reflection
    {
        namespace
        {
            template <typename T, typename F, typename Tuple, size_t... Is>
            void apply_tuple_idx(T &&t, F &&f, Tuple &&tp, std::index_sequence<Is...>)
            {
                ((f(Is, t.*(std::get<Is>(tp)))), ...);
            }
        } // namespace

        template <typename T, typename F, typename = typename std::enable_if_t<is_reflection_v<T>>>
        void for_each(T &&t, F &&f)
        {
            using M = decltype(reflect_members(t));
            constexpr auto tp = M::tuple();
            std::apply([f = std::forward<F>(f), t = std::forward<T>(t)](auto &&... item) {
                ((f(t.*item)), ...);
            },
                       tp);
        }

        template <typename T, typename F, typename = typename std::enable_if_t<is_reflection_v<T>>>
        void for_each_idx(T &&t, F &&f)
        {
            using M = decltype(reflect_members(t));
            constexpr auto size = M::size();
            constexpr auto tp = M::tuple();
            apply_tuple_idx(std::forward<T>(t), std::forward<F>(f), std::move(tp), std::make_index_sequence<size>());
        }

        template <typename T, typename F, typename = typename std::enable_if_t<is_reflection_v<T>>>
        void for_each_pair(T &&t, F &&f)
        {
            using M = decltype(reflect_members(t));
            constexpr auto size = M::size();
            constexpr auto tp = M::tuple();
            constexpr auto arr = M::arr();
            apply_tuple_idx(
                std::forward<T>(t), [arr, f = std::forward<F>(f)](size_t idx, auto &&item) {
                    f(arr[idx], item);
                },
                std::move(tp), std::make_index_sequence<size>());
        }

        template <typename T, typename = typename std::enable_if_t<is_reflection_v<T>>>
        constexpr auto member_names()
        {
            using M = decltype(reflect_members(std::declval<T>()));
            constexpr auto arr = M::arr();
            return arr;
        }

        template <typename T, typename = typename std::enable_if_t<is_reflection_v<T>>>
        constexpr const char *name()
        {
            using M = decltype(reflect_members(std::declval<T>()));
            return M::name();
        }

        template <typename T, typename = typename std::enable_if_t<is_reflection_v<T>>>
        constexpr size_t size()
        {
            using M = decltype(reflect_members(std::declval<T>()));
            return M::size();
        }
    } // namespace reflection
} // namespace wish

#endif