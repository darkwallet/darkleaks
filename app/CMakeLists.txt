file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.py")
file(COPY ${files} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(NOT WIN32)
    install(FILES
            ${files}
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT app
    )
    install(DIRECTORY assets
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT app
    )
endif()
