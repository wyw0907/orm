

#include "orm/orm.h"

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
    std::optional<std::string> time;
};
REFLECTION(person, name, age, sex, time);

int main(int ac, char **av)
{
    std::string host = "127.0.0.1";
    std::string user = "wish";
    std::string passwd = "wish";
    std::string database = "wish";
    for (int i = 1; i < ac; i++)
    {
        if (!strcmp(av[i], "-h"))
        {
            host = av[i + 1];
        }
        if (!strcmp(av[i], "-u"))
        {
            user = av[i + 1];
        }
        if (!strcmp(av[i], "-p"))
        {
            passwd = av[i + 1];
        }
        if (!strcmp(av[i], "--database"))
        {
            database = av[i + 1];
        }
    }
    wish::orm::db<wish::orm::mysql> db;
    db.set_error_callback([](auto &&message) {
        std::cout << message << std::endl;
    });
    db.connect(host, user, passwd, database);
    db.execute("truncate person");
    person p{"wyw", 25, person::Boy, std::nullopt};
    db.insert(p);
    auto ret1 = db.query<person>().filter(FID(person::name) = "wyw").run();
    for (auto &r : ret1)
    {
        std::cout << "ret1" << r.name << std::endl;
    }
    auto ret2 = db.query<person>().filter(FID(person::name) = "wyw2").run();
    for (auto &r : ret2)
    {
        std::cout << "ret2" << r.name << std::endl;
    }
}