add_executable(test_reflect test_reflect.cpp)
target_link_libraries(test_reflect gtest pthread)
add_executable(test_orm test_orm.cpp)
target_link_libraries(test_orm mysqlclient gtest pthread)


add_executable(example example.cpp)
target_link_libraries(example mysqlclient)