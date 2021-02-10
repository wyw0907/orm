#ifndef _WISH_ORM_H
#define _WISH_ORM_H
#include <iostream>
#include <string.h>
#include <assert.h>
#include "reflection.h"
#include "type_traits.h"
#include "generator.h"
#include "orm_mysql.h"
namespace wish
{
    namespace orm
    {
        template <typename DB>
        class db
        {
        public:
            template <typename... Args>
            bool connect(Args&&... args)
            {
                return m_db.template connect(std::forward<Args>(args)...);
            }

            void disconnect()
            {
                return m_db.template disconnect();
            }

            void set_error_callback(std::function<void(const char *)> cb)
            {
                return m_db.template set_error_callback(cb);
            }

            std::string get_last_error()
            {
                return m_db.template get_last_error();
            }

            bool execute(const std::string &sql)
            {
                return m_db.template execute(sql);
            }

            // insert语句
            template <typename T>
            std::enable_if_t<is_reflection_v<std::decay_t<T>>, bool> insert(T &&t)
            {
                return m_db.template insert(std::forward<T>(t));
            }

            // insert 一组数据， transaction表示是否需要使用事务
            template <typename T>
            std::enable_if_t<is_sequence_container<std::decay_t<T>>::value, bool> insert(T &&t, bool transaction = false)
            {
                return m_db.template insert(std::forward<T>(t), transaction);
            }

            // replace语句
            template <typename T>
            std::enable_if_t<is_reflection_v<std::decay_t<T>>, bool> update(T &&t)
            {
                return m_db.template update(std::forward<T>(t));
            }

            // replace 一组数据， transaction表示是否需要使用事务
            template <typename T>
            std::enable_if_t<is_sequence_container<std::decay_t<T>>::value, bool> update(T &&t, bool transaction = false)
            {
                return m_db.template update(std::forward<T>(t), transaction);
            }

            template <typename T, typename = typename std::enable_if_t<is_reflection_v<std::decay_t<T>>>>
            std::vector<T> query_explicit(const std::string &sql)
            {
                return m_db.template query<T>(sql);
            }

            template <typename T, typename = typename std::enable_if_t<is_reflection_v<std::decay_t<T>>>>
            decltype(auto) query()
            {
                return generator_query_sql<T>([this](const std::string &sql) -> std::vector<T> {
                    return this->m_db.template query<T>(sql);
                });
            }

            bool begin()
            {
                return m_db.begin();
            }

            bool commit()
            {
                return m_db.begin();
            }

            bool rollback()
            {
                return m_db.begin();
            }

        private:
            DB m_db;
        };
    } // namespace orm
} // namespace wish

#endif