file(GLOB cxx-headers
     "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

include_directories(SYSTEM
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../lib
  ${libbitcoin_INCLUDE_DIRS}
)

add_executable(dl_prove dl_prove.cpp)
add_executable(dl_secrets dl_secrets.cpp)
add_executable(dl_start dl_start.cpp)
add_executable(dl_unlock dl_unlock.cpp)
#add_executable(dl_check_addr dl_check_addr.cpp) needs libbitcoin-client

target_link_libraries(dl_prove darkleaks)
target_link_libraries(dl_secrets darkleaks)
target_link_libraries(dl_start darkleaks)
target_link_libraries(dl_unlock darkleaks)
#target_link_libraries(dl_check_addr darkleaks)

if(NOT WIN32)
    include(GNUInstallDirs)
    install(TARGETS dl_prove dl_secrets dl_start dl_unlock #dl_check_addr
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT tools)
endif()
