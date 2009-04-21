# These should be here, but cause build failures on my machine.
#FIND_PACKAGE(JNI)
#INCLUDE_DIRECTORIES(${JAVA_INCLUDE_PATH})

SWIG_ADD_MODULE(cylon-java java ../cylon.i)
SWIG_LINK_LIBRARIES(cylon-java cylon)

INSTALL(TARGETS ${SWIG_MODULE_cylon-java_REAL_NAME} LIBRARY DESTINATION lib/java)
