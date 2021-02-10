#include "orm/orm.h"
#include <gtest/gtest.h>
#include <numeric>
#include <chrono>
#include <iostream>
struct person
{
    enum SexType
    {
        Boy,
        Gril,
    };
    std::string name;
    int age = 0;
    SexType sex = SexType::Boy;
    uint64_t sequence = 0;
    int32_t score = -1;
    std::string remark;
    std::optional<std::string> time;
};

/*
    CREATE TABLE IF NOT EXISTS `person` (
        `id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT,
        `name` varchar(64) NOT NULL,
        `age` INT(11) UNSIGNED DEFAULT 0, 
        `sex` INT NOT NULL,
        `sequence` BIGINT(20) UNSIGNED DEFAULT 0,
        `score` INT(11) SIGNED DEFAULT -1,
        `remark` TEXT DEFAULT NULL,
        `time` timestamp DEFAULT NULL,
        PRIMARY KEY(`id`),
        INDEX name(`name`)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;
*/

REFLECTION(person, name, age, sex, sequence, score, remark, time);

class test_orm : public testing::Test
{
public:
    void SetUp() override
    {
        m_db.set_error_callback([](auto &&message) {
            std::cout << message << std::endl;
        });
        assert(m_db.connect("127.0.0.1", "wish", "wish", "wish"));
        assert(m_db.execute("truncate person"));
        for (int i : std::array<int, 1000>())
        {
            pp.push_back({"xiao gang", 12, person::Boy, 1111, -99, "test insert cycle", "2020-12-12 12:12:12"});
        }
    }
    void TearDown() override
    {
    }

    std::vector<person> pp;
    wish::orm::db<wish::orm::mysql> m_db;
};

TEST_F(test_orm, mysql_bind_type)
{
    using mysql_type_person = wish::orm::mysql_bind_type<person>;
    ASSERT_FALSE(mysql_type_person::value);

    using mysql_type_char = wish::orm::mysql_bind_type<char>;
    ASSERT_TRUE(mysql_type_char::value);
    ASSERT_EQ(mysql_type_char::name, "TINYINT");
    ASSERT_EQ(mysql_type_char::id, MYSQL_TYPE_TINY);
    ASSERT_FALSE(mysql_type_char::is_unsigned);

    using mysql_type_int32_t = wish::orm::mysql_bind_type<int32_t>;
    ASSERT_TRUE(mysql_type_int32_t::value);
    ASSERT_EQ(mysql_type_int32_t::name, "INT");
    ASSERT_EQ(mysql_type_int32_t::id, MYSQL_TYPE_LONG);
    ASSERT_FALSE(mysql_type_int32_t::is_unsigned);

    using mysql_type_uint32_t = wish::orm::mysql_bind_type<uint32_t>;
    ASSERT_TRUE(mysql_type_uint32_t::value);
    ASSERT_EQ(mysql_type_uint32_t::name, "INT");
    ASSERT_EQ(mysql_type_uint32_t::id, MYSQL_TYPE_LONG);
    ASSERT_TRUE(mysql_type_uint32_t::is_unsigned);
}

TEST_F(test_orm, mysql_execute_insert)
{
    std::string sql = "insert into person (`name`, `age`, `sex`, `sequence`, `score`, `remark`, `time`)"
                      " values('xiao wang', 11, 1, 1111, -99, 'test execute', '2020-12-12 12:12:12');";
    ASSERT_TRUE(m_db.execute(sql));
}

TEST_F(test_orm, mysql_insert_one)
{
    person p{"wyw", 25, person::Boy, UINT64_MAX, -5, "", std::nullopt};
    ASSERT_TRUE(m_db.insert(p));
}

TEST_F(test_orm, mysql_replace_one)
{
    person p{"wyw", 25, person::Boy, UINT64_MAX, -5, "", std::nullopt};
    ASSERT_TRUE(m_db.update(p));
}

TEST_F(test_orm, mysql_execute_cycle)
{
    std::string sql = "insert into person (`name`, `age`, `sex`, `sequence`, `score`, `remark`, `time`)"
                      " values('xiao wang', 11, 1, 1111, -99, 'test execute cycle', '2020-12-12 12:12:12');";
    for (int i : std::array<int, 1000>())
    {
        ASSERT_TRUE(m_db.execute(sql));
    }
}
// 使用stmt提升性能微乎其微，如果每次只插入1条sql，不如直接mysql_query
TEST_F(test_orm, mysql_insert_one_cycle)
{
    person p{"xiao gang", 12, person::Boy, 1111, -99, "test insert cycle", "2020-12-12 12:12:12"};
    for (int i : std::array<int, 1000>())
    {
        ASSERT_TRUE(m_db.insert(p));
    }
}

TEST_F(test_orm, mysql_insert_multi)
{
    ASSERT_TRUE(m_db.insert(pp));
}

TEST_F(test_orm, mysql_query_explicit)
{
    std::vector<person> ppt;
    std::array<uint64_t, 1000> arr;
    std::iota(std::begin(arr), std::end(arr), 0);
    for (auto i : arr)
    {
        std::chrono::system_clock clk;
        auto tt = std::chrono::system_clock::to_time_t(clk.now());
        //auto xx = std::put_time(std::localtime(&tt), "%Y%m%d %H%M%S");
        //std::cout << xx << std::endl;
        char buf[64] = {0};
        strftime(buf, 64, "%Y:%m:%d %H-%M-%S", std::localtime(&tt));
        ppt.emplace_back(person{
            "xiao lv",
            10,
            person::Gril,
            i,
            -1,
            "test query explicit",
            buf});
    }
    ASSERT_TRUE(m_db.insert(ppt));

    std::string sql = "select name, age, sex, sequence, score, remark, time from person where name = 'xiao lv' order by sequence";
    auto ret = m_db.query_explicit<person>(sql);
    int ret_i = 0;
    for (auto &r : ret)
    {
        ASSERT_EQ(r.age, 10);
        ASSERT_EQ(r.sex, person::Gril);
        ASSERT_EQ(r.name, "xiao lv");
        ASSERT_EQ(r.score, -1);
        ASSERT_EQ(r.sequence, ret_i);
        ret_i++;
    }
}

TEST_F(test_orm, mysql_query_filter0)
{
    std::vector<person> ppt;
    std::array<uint64_t, 10> arr;
    std::iota(std::begin(arr), std::end(arr), 0);
    for (auto i : arr)
    {
        ppt.emplace_back(person{
            "xiao hong",
            9,
            person::Gril,
            i,
            99,
            "test query filter",
            std::nullopt});
    }
    ASSERT_TRUE(m_db.insert(ppt));

    auto ret = m_db.query<person>().filter(FID(person::name) = "xiao hong").limit(3).run();
    ASSERT_EQ(ret.size(), 3);
    for (auto &r : ret)
    {
        ASSERT_EQ(r.age, 9);
        ASSERT_EQ(r.sex, person::Gril);
        ASSERT_EQ(r.name, "xiao hong");
        ASSERT_EQ(r.score, 99);
    }

    auto ret2 = m_db.query<person>().filter(FID(person::name) = "xiao hong").filter(FID(person::sequence) < 5).run();
    ASSERT_EQ(ret2.size(), 5);
    for (auto &r : ret2)
    {
        ASSERT_EQ(r.age, 9);
        ASSERT_EQ(r.sex, person::Gril);
        ASSERT_EQ(r.name, "xiao hong");
        ASSERT_EQ(r.score, 99);
    }
}

int main(int ac, char **av)
{
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}