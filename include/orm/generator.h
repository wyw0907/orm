#ifndef _WISH_ORM_FILTER_H
#define _WISH_ORM_FILTER_H
#include "reflection.h"
namespace wish
{
    namespace orm
    {
        template <typename T,
                  typename V,
                  typename = typename std::enable_if_t<wish::is_char_array_v<std::decay_t<T>> || std::is_same_v<std::string, std::decay_t<T>> || std::is_arithmetic_v<std::decay_t<V>>>>
        std::string concat(const std::string &field, const char *op, const V &value)
        {
            using UL = std::decay_t<T>;
            using UR = std::decay_t<V>;
            if constexpr (std::is_same_v<std::string, UL> || wish::is_char_array_v<UL>)
            {
                if constexpr (std::is_arithmetic_v<UR>)
                {
                    return field + op + "'" + std::to_string(value) + "'";
                }
                else
                {
                    return field + op + "'" + value + "'";
                }
            }
            else if constexpr (std::is_arithmetic_v<UL>)
            {
                if constexpr (std::is_arithmetic_v<UR>)
                {
                    return field + op + std::to_string(value);
                }
                else
                {
                    return field + op + value;
                }
            }
        }

        template <typename T>
        struct filter
        {
            filter(std::string_view f) : field(f.data(), f.size())
            {
            }
            template <typename V>
            std::string operator=(const V &value)
            {
                return concat<T, V>(field, " = ", value);
            }
            template <typename V>
            std::string operator>(const V &value)
            {
                return concat<T, V>(field, " > ", value);
            }
            template <typename V>
            std::string operator<(const V &value)
            {
                return concat<T, V>(field, " < ", value);
            }
            template <typename V>
            std::string operator!=(const V &value)
            {
                return concat<T, V>(field, " != ", value);
            }
            std::string field;
        };

        template <typename U>
        constexpr std::string_view get_field_name(std::string_view fullname)
        {
            using T = typename field_attribute<U>::type;
            constexpr const char *name = reflection::name<T>();
            return fullname.substr(strlen(name) + 2, fullname.size());
        }
// eg FID(person::name) -> name
#define FID(field) wish::orm::filter<decltype(field)>(wish::orm::get_field_name<decltype(&field)>(std::string_view(#field)))

        template <typename T, typename F, typename = typename std::enable_if_t<is_reflection_v<T>>>
        class generator
        {
        public:
            generator(F &&f) : m_closure(std::forward<F>(f)) {}
            ~generator() = default;
            generator(const generator &) = delete;
            generator &operator=(const generator &) = delete;
            generator(generator &&) = default;
            generator &operator=(generator &&) = default;

            generator &set_sql(const std::string &sql)
            {
                m_sql = sql;
                return *this;
            }

            generator &set_sql(std::string &&sql)
            {
                m_sql = std::move(sql);
                return *this;
            }

            // eg filter(FID(person::name) = "test");
            generator &filter(const std::string &expression)
            {
                m_filters.emplace_back(expression);
                return *this;
            }

            generator &limit(size_t n)
            {
                m_limit = n;
                return *this;
            }

            std::vector<T> run()
            {
                std::string sql = m_sql;
                if (!m_filters.empty())
                {
                    sql += " where " + m_filters[0];
                    for (size_t idx = 1; idx != m_filters.size(); ++idx)
                    {
                        sql += " and " + m_filters[idx];
                    }
                }
                if (m_limit != 0)
                {
                    sql += " limit " + std::to_string(m_limit);
                }
                return m_closure(sql);
            }

            void reset()
            {
                m_filters.clear();
                m_limit = 0;
            }

        private:
            std::string m_sql; //sql
            std::vector<std::string> m_filters;
            size_t m_limit = 0;
            F m_closure;
        };

        template <typename T, typename = typename std::enable_if_t<is_reflection_v<T>>>
        std::string generator_insert_sql(bool auto_update = false)
        {
            auto table_name = wish::reflection::name<T>();
            /*
                    insert into table (...) values(...); 
                */
            std::string sql = auto_update ? "replace into " : "insert into ";
            sql += table_name;
            std::string sql_field;
            std::string sql_value;
            constexpr auto member_names = reflection::member_names<T>();
            for (int i = 0; i < member_names.size(); ++i)
            {
                if (i == member_names.size() - 1)
                {
                    sql_value += "?";
                    sql_field += member_names[i];
                }
                else
                {
                    sql_value += "?, ";
                    sql_field += std::string(member_names[i]) + ", ";
                }
            }
            sql += "(" + sql_field + ") values(" + sql_value + ");";
            return sql;
        }

        template <typename T, typename F, typename = typename std::enable_if_t<is_reflection_v<T>>>
        generator<T, F> generator_query_sql(F &&f)
        {
            auto table_name = wish::reflection::name<T>();
            /*
                select ... from table_name;
            */
            std::string sql = "select ";
            std::string sql_field;
            constexpr auto member_names = reflection::member_names<T>();
            for (int i = 0; i < member_names.size(); ++i)
            {
                if (i == member_names.size() - 1)
                {
                    sql_field += member_names[i];
                }
                else
                {
                    sql_field += std::string(member_names[i]) + ", ";
                }
            }
            sql += sql_field + " from " + table_name;
            generator<T, F> g{std::forward<F>(f)};
            return std::move(g.set_sql(std::move(sql)));
        }

    } // namespace orm
} // namespace wish

#endif