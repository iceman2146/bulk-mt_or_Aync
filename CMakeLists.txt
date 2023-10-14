cmake_minimum_required(VERSION 3.26)
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})


project(async VERSION ${PROJECT_VESRION})
find_package(Threads)
configure_file(version.h.in version.h) 

set(CMAKE_CXX_STANDARD 14)

set(EXEC_NAME ${CMAKE_PROJECT_NAME}_cli)
set(LIB_NAME ${CMAKE_PROJECT_NAME})

include_directories(include include/base include/base/interface  include/otus)

add_library(${LIB_NAME} SHARED
 src/otus/async.cpp
 src/commander.cpp
 src/execute.cpp
 src/log.cpp
 src/context.cpp
)

add_executable(${EXEC_NAME} 
src/main.cpp
)
target_link_libraries(${LIB_NAME}
  ${CMAKE_THREAD_LIBS_INIT}
)
set_target_properties(${LIB_NAME} PROPERTIES PUBLIC_HEADER async.h)
target_link_libraries(${EXEC_NAME} PRIVATE
  ${LIB_NAME} 
)
  target_compile_options(${EXEC_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
  target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
include(GNUInstallDirs)
install(TARGETS ${LIB_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )



target_link_libraries(${EXEC_NAME} PRIVATE ${LIB_NAME})
install(TARGETS ${EXEC_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)
include (CPack)
