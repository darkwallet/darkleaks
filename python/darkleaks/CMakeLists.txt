find_package(Boost COMPONENTS python REQUIRED)
find_package(PythonLibs REQUIRED)

include_directories(SYSTEM
  ${CMAKE_CURRENT_SOURCE_DIR}/../../lib
  ${Boost_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIRS}
)

PYTHON_ADD_MODULE(_darkleaks darkleaks.cpp)
target_link_libraries(_darkleaks PUBLIC darkleaks ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

set_target_properties(_darkleaks PROPERTIES PREFIX "")

add_custom_command(TARGET _darkleaks POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   $<TARGET_FILE:_darkleaks> ${CMAKE_CURRENT_BINARY_DIR})
file(COPY __init__.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(NOT WIN32)
    include(GNUInstallDirs)
    string(REGEX REPLACE "\\.[0-9]+$" "" PYTHON_VERSION "${PYTHONLIBS_VERSION_STRING}")
    install(TARGETS _darkleaks
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/python${PYTHON_VERSION}/site-packages
        COMPONENT python)
endif()
