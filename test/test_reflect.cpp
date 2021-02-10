#include "orm/reflection.h"
#include "orm/type_traits.h"
#include <iostream>
#include <gtest/gtest.h>
struct Person
{
    enum SexType
    {
        Boy,
        Gril,
    };
    std::string name;
    int age;
    SexType sex;
    std::optional<std::string> remark;
};

REFLECTION(Person, name, age, sex, remark);

class test_reflect : public testing::Test
{
public:
    void SetUp() override
    {
        p = Person{"wyw", 25, Person::Boy, std::nullopt};
    }
    void TearDown() override {}

    Person p;
};

TEST_F(test_reflect, test_name)
{
    ASSERT_EQ(wish::reflection::name<Person>(), "Person");
}

TEST_F(test_reflect, test_size)
{
    ASSERT_EQ(wish::reflection::size<Person>(), 4);
}

TEST_F(test_reflect, test_for_each)
{
    wish::reflection::for_each(p, [](auto &&item) {
        if constexpr (wish::is_optional<std::decay_t<decltype(item)>>::value)
        {
            if (item.has_value())
            {
                std::cout << item.value() << std::endl;
            }
            else
            {
                std::cout << item.value_or("null") << std::endl;
            }
        }
        else
        {
            std::cout << item << std::endl;
        }
    });
    ASSERT_TRUE(true);
}

TEST_F(test_reflect, test_member_names)
{
    std::vector<std::string> names;
    for (auto &v : wish::reflection::member_names<Person>())
    {
        names.push_back(v);
    }
    ASSERT_EQ(names.size(), 4);
    ASSERT_EQ(names[0], "name");
    ASSERT_EQ(names[1], "age");
    ASSERT_EQ(names[2], "sex");
    ASSERT_EQ(names[3], "remark");
}

TEST_F(test_reflect, test_for_each_idx)
{
    wish::reflection::for_each_idx(p, [](size_t idx, auto &&item) {
        if constexpr (wish::is_optional<std::decay_t<decltype(item)>>::value)
        {
            if (item.has_value())
            {
                std::cout << "idx: " << idx << " : " << item.value() << std::endl;
            }
            else
            {
                std::cout << "idx: " << idx << " : " << item.value_or("null") << std::endl;
            }
        }
        else
        {
            std::cout << "idx: " << idx << " : " << item << std::endl;
        }
    });
    ASSERT_TRUE(true);
}

TEST_F(test_reflect, test_for_each_pair)
{
    p.name = "yueyue";
    p.age = 28;
    p.sex = Person::Gril;
    p.remark = "remark test";
    wish::reflection::for_each_pair(p, [](const char *name, auto &&item) {
        if constexpr (wish::is_optional<std::decay_t<decltype(item)>>::value)
        {
            if (item.has_value())
            {
                std::cout << "name: " << name << " : " << item.value() << std::endl;
            }
            else
            {
                std::cout << "name: " << name << " : " << item.value_or("null") << std::endl;
            }
        }
        else
        {
            std::cout << "name: " << name << " : " << item << std::endl;
        }
    });
    ASSERT_TRUE(true);
}

int main(int ac, char **av)
{
    testing::InitGoogleTest(&ac, av);
    return RUN_ALL_TESTS();
}