set(APOLLO_TEST_CORE_ROOT ${APOLLO_TEST_ROOT}/core)
set(APOLLO_CORE_TESTS
    ${APOLLO_TEST_CORE_ROOT}/core_main.cpp
    ${APOLLO_TEST_CORE_ROOT}/vector_test.cpp
    ${APOLLO_TEST_CORE_ROOT}/vector_specialisation_test.cpp
    ${APOLLO_TEST_CORE_ROOT}/utils_test.cpp
    ${APOLLO_TEST_CORE_ROOT}/ray_test.cpp
    ${APOLLO_TEST_CORE_ROOT}/matrix_test.cpp
    PARENT_SCOPE)

