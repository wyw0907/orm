# orm
orm by c++17

example
```
    wish::orm::db<wish::orm::mysql> db;
    db.connect("127.0.0.1", "wish", "wish", "wish");
    // 直接执行sql
    db.execute("truncate person");
    person p{"wyw", 25, person::Boy, std::nullopt};
    // 插入
    db.insert(p);
    // 查询
    auto ret1 = db.query<person>().filter(FID(person::name) = "wyw").run();
    for (auto& r : ret1)
    {
        ...
    }

```
