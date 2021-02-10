#ifndef _WISH_ORM_MYSQL_TYPE_H
#define _WISH_ORM_MYSQL_TYPE_H
#include "type_traits.h"
#include <mysql/mysql.h>

namespace wish
{
    namespace orm
    {
        constexpr static std::array<const char *, 256> s_mysql_types = {
            /*MYSQL_TYPE_DECIMAL*/ "DECIMAL",
            /*MYSQL_TYPE_TINY*/ "TINYINT",
            /*MYSQL_TYPE_SHORT*/ "SMALLINT",
            /*MYSQL_TYPE_LONG*/ "INT",
            /*MYSQL_TYPE_FLOAT*/ "FLOAT",
            /*MYSQL_TYPE_DOUBLE*/ "DOUBLE",
            /*MYSQL_TYPE_NULL*/ "NULL",
            /*MYSQL_TYPE_TIMESTAMP*/ "TIMESTAMP",
            /*MYSQL_TYPE_LONGLONG*/ "BIGINT",
            /*MYSQL_TYPE_INT24*/ "",
            /*MYSQL_TYPE_DATE*/ "DATE",
            /*MYSQL_TYPE_TIME*/ "TIME",
            /*MYSQL_TYPE_DATETIME*/ "DATETIME",
            /*MYSQL_TYPE_YEAR*/ "",
            /*MYSQL_TYPE_NEWDATE*/ "",
            /*MYSQL_TYPE_VARCHAR*/ "VARCHAR",
            /*MYSQL_TYPE_BIT*/ "",
            /*MYSQL_TYPE_TIMESTAMP2*/ "",
            /*MYSQL_TYPE_DATETIME2*/ "",
            /*MYSQL_TYPE_TIME2*/ "",
            /*MYSQL_TYPE_TYPED_ARRAY*/ "",
            "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
            "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
            "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
            "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
            "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
            "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
            "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
            "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
            "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
            /*MYSQL_TYPE_INVALID = 243*/ "",
            /*MYSQL_TYPE_BOOL = 244*/ "",
            /*MYSQL_TYPE_JSON = 245*/ "",
            /*MYSQL_TYPE_NEWDECIMAL = 246*/ "",
            /*MYSQL_TYPE_ENUM = 247*/ "",
            /*MYSQL_TYPE_SET = 248*/ "",
            /*MYSQL_TYPE_TINY_BLOB = 249*/ "",
            /*MYSQL_TYPE_MEDIUM_BLOB = 250*/ "",
            /*MYSQL_TYPE_LONG_BLOB = 251*/ "",
            /*MYSQL_TYPE_BLOB = 252*/ "BLOB",
            /*MYSQL_TYPE_VAR_STRING = 253*/ "TEXT",
            /*MYSQL_TYPE_STRING = 254*/ "TEXT",
            /*MYSQL_TYPE_GEOMETRY = 255*/ ""};

        template <typename T>
        struct mysql_bind_type
        {
            using type = T;
            constexpr static bool value = false;
            constexpr static int id = 256;
            constexpr static const char *name = "";
            constexpr static bool is_unsigned = false;
        };

#define REGISTER_TYPE(TYPE, TYPE_ID)                                                               \
    template <>                                                                                    \
    struct mysql_bind_type<TYPE>                                                                   \
    {                                                                                              \
        using type = TYPE;                                                                         \
        constexpr static bool value = true;                                                        \
        constexpr static int id = TYPE_ID;                                                         \
        constexpr static const char *name = s_mysql_types[TYPE_ID];                                \
        constexpr static bool is_unsigned = std::is_arithmetic_v<TYPE> && !std::is_signed_v<TYPE>; \
    };

        //using mysql_blob = std::vector<uint8_t>;
        REGISTER_TYPE(char, MYSQL_TYPE_TINY);
        REGISTER_TYPE(int8_t, MYSQL_TYPE_TINY);
        REGISTER_TYPE(uint8_t, MYSQL_TYPE_TINY);
        REGISTER_TYPE(int16_t, MYSQL_TYPE_SHORT);
        REGISTER_TYPE(uint16_t, MYSQL_TYPE_SHORT);
        REGISTER_TYPE(int32_t, MYSQL_TYPE_LONG);
        REGISTER_TYPE(uint32_t, MYSQL_TYPE_LONG);
        REGISTER_TYPE(float, MYSQL_TYPE_FLOAT);
        REGISTER_TYPE(double, MYSQL_TYPE_DOUBLE);
        REGISTER_TYPE(int64_t, MYSQL_TYPE_LONGLONG);
        REGISTER_TYPE(uint64_t, MYSQL_TYPE_LONGLONG);
        REGISTER_TYPE(std::string, MYSQL_TYPE_STRING);
        REGISTER_TYPE(MYSQL_TIME, MYSQL_TYPE_TIMESTAMP);
        //REGISTER_TYPE(mysql_blob, MYSQL_TYPE_BLOB);
        //REGISTER_TYPE(char *, MYSQL_TYPE_VARCHAR);
    } // namespace orm
} // namespace wish

#endif