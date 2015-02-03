set(cxx-sources
  aes256.cpp
  prove.cpp
  secrets.cpp
  start.cpp
  unlock.cpp
)

file(GLOB cxx-headers
     "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

include_directories(SYSTEM
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${libbitcoin_INCLUDE_DIRS}
)

link_directories(${libbitcoin_LIBRARY_DIRS})

add_library(darkleaks
  ${cxx-sources}
  ${asm-sources}
)

target_link_libraries(darkleaks PUBLIC ${libbitcoin_LIBRARIES})

if(NOT ANDROID)
  set_target_properties(darkleaks PROPERTIES VERSION ${VERSION} SOVERSION ${VERSION_MAJOR})
endif()

if(NOT WIN32)
  include(GNUInstallDirs)
  install(TARGETS darkleaks
          DESTINATION ${CMAKE_INSTALL_LIBDIR}
          COMPONENT main
  )
endif()
