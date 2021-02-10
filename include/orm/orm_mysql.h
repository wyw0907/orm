#ifndef _WISH_ORM_MYSQL_H
#define _WISH_ORM_MYSQL_H
#include "reflection.h"
#include "type_traits.h"
#include "mysql_type.h"
namespace wish
{
    namespace orm
    {
        class mysql
        {
        public:
            mysql() = default;
            ~mysql()
            {
                disconnect();
            }

            bool connect(const std::string &host, const std::string &user,
                         const std::string &passwd, const std::string &db = "",
                         uint16_t port = 3306, uint32_t timeout = 0,
                         const std::string &charset = "utf8")
            {
                if (m_conn != nullptr)
                {
                    mysql_close(m_conn);
                }
                m_conn = mysql_init(m_conn);
                if (m_conn == nullptr)
                {
                    m_last_error = "mysql init failed";
                    return false;
                }
                int reconnect = 1;
                mysql_options(m_conn, MYSQL_OPT_RECONNECT, &reconnect); // 什么情况下不需要重连？
                mysql_options(m_conn, MYSQL_SET_CHARSET_NAME, charset.c_str());
                if (timeout > 0)
                {
                    if (0 != mysql_options(m_conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout))
                    {
                        set_last_error(mysql_error(m_conn));
                        return false;
                    }
                }
                if (nullptr == mysql_real_connect(m_conn, host.c_str(), user.c_str(), passwd.c_str(),
                                                  db.c_str(), static_cast<unsigned int>(port), nullptr, 0))
                {
                    set_last_error(mysql_error(m_conn));
                    return false;
                }
                return true;
            }

            void disconnect()
            {
                if (m_conn != nullptr)
                {
                    mysql_close(m_conn);
                    m_conn = nullptr;
                }
            }

            void set_error_callback(std::function<void(const char *)> cb)
            {
                m_callback = cb;
            }

            std::string get_last_error()
            {
                return m_last_error;
            }

            bool execute(const std::string &sql)
            {
                if (mysql_real_query(m_conn, sql.c_str(), sql.size()))
                {
                    set_last_error(mysql_error(m_conn));
                    return false;
                }
                return true;
            }

            // insert语句
            template <typename T>
            std::enable_if_t<is_reflection_v<std::decay_t<T>>, bool> insert(T &&t)
            {
                return insert_impl(std::forward<T>(t), false);
            }

            // insert 一组数据， transaction表示是否需要使用事务
            template <typename T>
            std::enable_if_t<is_sequence_container<std::decay_t<T>>::value, bool> insert(T &&t, bool transaction = false)
            {
                return insert_impl(std::forward<T>(t), false, transaction);
            }

            // replace语句
            template <typename T>
            std::enable_if_t<is_reflection_v<std::decay_t<T>>, bool> update(T &&t)
            {
                return insert_impl(std::forward<T>(t), true);
            }

            // replace 一组数据， transaction表示是否需要使用事务
            template <typename T>
            std::enable_if_t<is_sequence_container<std::decay_t<T>>::value, bool> update(T &&t, bool transaction = false)
            {
                return insert_impl(std::forward<T>(t), true, transaction);
            }

            template <typename T, typename = typename std::enable_if_t<is_reflection_v<std::decay_t<T>>>>
            std::vector<T> query(const std::string &sql)
            {
                if (mysql_real_query(m_conn, sql.c_str(), sql.size()))
                {
                    set_last_error(mysql_error(m_conn));
                    return {};
                }
                MYSQL_RES *result = mysql_store_result(m_conn);
                if (result == nullptr)
                {
                    set_last_error(mysql_error(m_conn));
                    return {};
                }
                std::vector<T> tt;
                MYSQL_ROW row = mysql_fetch_row(result);
                if (row)
                {
                    assert(mysql_num_fields(result) == reflection::size<T>());
                    
                }
                while (row)
                {
                    T t{};
                    reflection::for_each_idx(t, [&row, this](size_t idx, auto &&item) {
                        this->set_value(row[idx], item);
                    });
                    tt.emplace_back(std::move(t));
                    row = mysql_fetch_row(result);
                }
                mysql_free_result(result);
                return tt;
            }

            bool begin()
            {
                if (mysql_query(m_conn, "BEGIN"))
                {
                    set_last_error(mysql_error(m_conn));
                    return false;
                }
                return true;
            }

            bool commit()
            {
                if (mysql_query(m_conn, "COMMIT"))
                {
                    set_last_error(mysql_error(m_conn));
                    return false;
                }
                return true;
            }

            bool rollback()
            {
                if (mysql_query(m_conn, "ROLLBACK"))
                {
                    set_last_error(mysql_error(m_conn));
                    return false;
                }
                return true;
            }

        private:
            struct guard_stmt
            {
                // default construction is just used for map
                guard_stmt()
                {
                }
                guard_stmt(MYSQL_STMT *m) : stmt(m)
                {
                    assert(stmt);
                }
                ~guard_stmt()
                {
                    mysql_stmt_close(stmt);
                }
                MYSQL_STMT *stmt = nullptr;
            };

            /*          这里为什么编译不了， 编译器版本gcc9.3.0, c++17
            template <typename T>
            bool insert_impl(T &&t, bool auto_update, bool transaction)
            {
                if constexpr (is_sequence_container<std::decay_t<T>>::value)
                {
                    ...
                }
                else if constexpr (is_reflection_v<std::decay_t<T>>)
                {
                    ...
                }
                else
                {
                    // 这么写为什么编译不了？
                    static_assert(false, "invalud type to insert");
                }
            }
            */

            template <typename T>
            std::enable_if_t<is_sequence_container<std::decay_t<T>>::value, bool> insert_impl(T &&t, bool auto_update, bool transaction)
            {
                using U = std::decay_t<T>;
                auto stmt = stmt_prepare<typename U::value_type>(auto_update);
                if (stmt == nullptr)
                {
                    return false;
                }
                if (transaction)
                {
                    begin();
                }
                for (auto &v : t)
                {
                    if (!stmt_execute(stmt, v))
                    {
                        if (transaction)
                        {
                            rollback();
                        }
                        return false;
                    }
                }
                if (transaction)
                {
                    commit();
                }
                return true;
            }

            template <typename T>
            std::enable_if_t<is_reflection_v<std::decay_t<T>>, bool> insert_impl(T &&t, bool auto_update)
            {
                auto stmt = stmt_prepare<T>(auto_update);
                if (stmt == nullptr)
                {
                    return false;
                }
                return stmt_execute(stmt, t);
            }

            template <typename T, typename = typename std::enable_if_t<is_reflection_v<T>>>
            MYSQL_STMT *stmt_prepare(bool auto_update)
            {
                constexpr auto table_name = reflection::name<T>();
                auto &stmts = auto_update ? m_update_stmts : m_insert_stmts;
                auto iter = stmts.find(table_name);
                if (iter == stmts.end())
                {
                    auto sql = generator_insert_sql<T>(auto_update);
                    MYSQL_STMT *stmt = mysql_stmt_init(m_conn);
                    if (stmt == nullptr)
                    {
                        set_last_error(mysql_error(m_conn));
                        return nullptr;
                    }
                    if (mysql_stmt_prepare(stmt, sql.c_str(), sql.size()))
                    {
                        set_last_error(mysql_stmt_error(stmt));
                        return nullptr;
                    }
                    stmts[table_name] = std::move(std::make_unique<guard_stmt>(stmt));
                    return stmt;
                }
                else
                {
                    return iter->second->stmt;
                }
            }

            template <typename T, typename = typename std::enable_if_t<is_reflection_v<T>>>
            bool stmt_execute(MYSQL_STMT *stmt, T &&t)
            {
                constexpr auto N = reflection::size<T>();
                //std::array<MYSQL_BIND, N> param_binds; 使用std::array执行sql时会崩溃？为什么
                std::vector<MYSQL_BIND> param_binds(N);
                //MYSQL_BIND param_binds[N] = {};  普通数组不会崩溃
                reflection::for_each_idx(t, [this, &param_binds](size_t idx, auto &&item) {
                    this->set_param_bind(param_binds[idx], item);
                });
                if (mysql_stmt_bind_param(stmt, &param_binds[0]))
                {
                    set_last_error(mysql_stmt_error(stmt));
                    return false;
                }
                if (mysql_stmt_execute(stmt))
                {
                    set_last_error(mysql_stmt_error(stmt));
                    return false;
                }
                /*if (0 == mysql_stmt_affected_rows(stmt))
                {
                    set_last_error(mysql_stmt_error(stmt));
                    return false;
                }*/
                return true;
            }

            template <typename T>
            constexpr std::enable_if_t<wish::is_optional<std::decay_t<T>>::value> set_param_bind(MYSQL_BIND &bind, const T &t)
            {
                if (!t.has_value())
                {
                    bind.buffer_type = MYSQL_TYPE_NULL;
                    //bind.is_null = const_cast<bool *>(&mysql::is_null);
                    bind.is_null_value = true;
                    return;
                }
                else
                {
                    return set_param_bind(bind, t.value());
                }
            }

            template <typename T>
            constexpr std::enable_if_t<!wish::is_optional<std::decay_t<T>>::value> set_param_bind(MYSQL_BIND &bind, const T &t)
            {
                if constexpr (wish::is_char_array_v<std::decay_t<T>>)
                {
                    // 字符数组
                    bind.buffer_type = MYSQL_TYPE_VARCHAR;
                    bind.buffer = static_cast<void *>(&t[0]);
                    bind.buffer_length = t.size();
                }
                else if constexpr (std::is_enum_v<std::decay_t<T>>)
                {
                    // 枚举类型转化成int类型
                    bind.buffer_type = MYSQL_TYPE_LONG;
                    bind.buffer = const_cast<void *>(static_cast<const void *>(&t));
                }
                else
                {
                    using MT = mysql_bind_type<std::decay_t<T>>;
                    static_assert(MT::value);
                    bind.buffer_type = (enum_field_types)MT::id;
                    if constexpr (MT::is_unsigned)
                    {
                        bind.is_unsigned = true;
                    }
                    if constexpr (std::is_same_v<std::string, std::decay_t<T>>)
                    {
                        bind.buffer = const_cast<void *>(static_cast<const void *>(t.c_str()));
                        bind.buffer_length = t.size();
                    }
                    else
                    {
                        bind.buffer = const_cast<void *>(static_cast<const void *>(&t));
                    }
                }
            }
            template <typename T>
            void set_value(const char *value, T &item)
            {
                using U = std::decay_t<T>;
                if (value == nullptr)
                {
                    return;
                }
                if constexpr (wish::is_char_array_v<U>)
                {
                    memcpy(&item[0], value, strlen(value));
                }
                if constexpr (std::is_same_v<U, std::string>)
                {
                    item = value;
                }
                if constexpr (std::is_enum_v<U>)
                {
                    item = static_cast<U>(std::stoi(value));
                }
                if constexpr (std::is_same_v<U, char>)
                {
                    item = *value;
                }
                if constexpr (std::is_same_v<U, int16_t>)
                {
                    item = static_cast<int16_t>(std::stoi(value));
                }
                if constexpr (std::is_same_v<U, uint16_t>)
                {
                    item = static_cast<uint16_t>(std::stoul(value));
                }
                if constexpr (std::is_same_v<U, int32_t>)
                {
                    item = std::stoi(value);
                }
                if constexpr (std::is_same_v<U, uint32_t>)
                {
                    item = static_cast<uint32_t>(std::stoul(value));
                }
                if constexpr (std::is_same_v<U, int64_t>)
                {
                    item = std::stoll(value);
                }
                if constexpr (std::is_same_v<U, uint64_t>)
                {
                    item = std::stoull(value);
                }
                if constexpr (std::is_same_v<U, float>)
                {
                    item = std::stof(value);
                }
                if constexpr (std::is_same_v<U, double>)
                {
                    item = std::stod(value);
                }
            }

            void set_last_error(const char *err)
            {
                m_last_error = err;
                if (m_callback)
                {
                    m_callback(m_last_error.c_str());
                }
            }

            MYSQL *m_conn = nullptr;
            std::string m_last_error;
            std::function<void(const char *)> m_callback;
            std::map<const char *, std::unique_ptr<guard_stmt>> m_insert_stmts;
            std::map<const char *, std::unique_ptr<guard_stmt>> m_update_stmts;
            constexpr static bool is_null = true;
        };

    } // namespace orm
} // namespace wish

#endif