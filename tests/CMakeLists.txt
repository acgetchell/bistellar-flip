add_executable(bistellar_tests ${PROJECT_SOURCE_DIR}/tests/main.cpp
                               bistellar_flip_test.cpp)
target_compile_features(bistellar_tests PRIVATE cxx_std_20)
target_link_libraries(bistellar_tests PRIVATE project_warnings fmt::fmt
                                              TBB::tbb CGAL::CGAL)

# Run the tests
add_test(NAME bistellar-unit-tests COMMAND $<TARGET_FILE:bistellar_tests>)
