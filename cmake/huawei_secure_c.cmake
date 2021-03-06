# old cmake does not have ExternalProject file
IF(CMAKE_VERSION VERSION_LESS "2.8.6")
  MACRO (CHECK_LIB_SECURE_C)
  ENDMACRO()
  RETURN()
ENDIF()

INCLUDE(ExternalProject)

MACRO (USE_BUNDLED_LIB_SECURE_C)
  SET(LIBSECUREC "libsecurec")

  SET(SECURE_C_DEFINES "-DSECURE_C")

  SET(SEUCRE_C_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extra/huawei_secure_c/include)
  SET(SECURE_C_SOURCE_DIR "${CMAKE_SOURCE_DIR}/extra/huawei_secure_c")
  SET(SECURE_C_BINARY_DIR "${CMAKE_BINARY_DIR}/extra/huawei_secure_c/build")

  ExternalProject_Add(secure_c
    PREFIX extra/huawei_secure_c
    SOURCE_DIR ${SECURE_C_SOURCE_DIR}
    BINARY_DIR ${SECURE_C_BINARY_DIR}
    STAMP_DIR  ${SECURE_C_BINARY_DIR}
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -E copy_directory ${SECURE_C_SOURCE_DIR} ${SECURE_C_BINARY_DIR}
    BUILD_COMMAND cd src && make
    INSTALL_COMMAND ""
  )

  ADD_LIBRARY(libsecurec STATIC IMPORTED GLOBAL)
  SET_TARGET_PROPERTIES(libsecurec PROPERTIES IMPORTED_LOCATION "${SECURE_C_BINARY_DIR}/src/libsecurec.a")
  ADD_DEPENDENCIES(libsecurec secure_c)
ENDMACRO()

MACRO (CHECK_LIB_SECURE_C)
  USE_BUNDLED_LIB_SECURE_C()
  SET(SECURE_C_LIBRARY "bundled Huawei Secure C")
ENDMACRO()
