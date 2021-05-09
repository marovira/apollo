set(APOLLO_INCLUDE_ROOT_LIST
    ${APOLLO_SOURCE_ROOT}/apollo.hpp
    )
configure_file(${APOLLO_SOURCE_ROOT}/apollo.hpp.in ${APOLLO_INCLUDE_ROOT_LIST})

# Add the lower directories
add_subdirectory(${APOLLO_SOURCE_ROOT}/core)

# Wrap each list for the source groups above.
set(APOLLO_INCLUDE_ROOT_GROUP ${APOLLO_INCLUDE_ROOT_LIST} PARENT_SCOPE)
set(APOLLO_INCLUDE_CORE_GROUP ${APOLLO_INCLUDE_CORE_LIST} PARENT_SCOPE)

set(APOLLO_SOURCE_CORE_GROUP ${APOLLO_SOURCE_CORE_LIST} PARENT_SCOPE)

# Now compile the full list of files so we can format everything at once.
set(APOLLO_HEADER_FILES
    ${APOLLO_INCUDE_ROOT_LIST}
    PARENT_SCOPE)

set(APOLLO_SOURCE_FILES
    ${APOLLO_SOURCE_CORE_LIST}
    PARENT_SCOPE)
