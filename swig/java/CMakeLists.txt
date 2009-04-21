SWIG_ADD_MODULE(cylon-java java ../cylon.i)
SWIG_LINK_LIBRARIES(cylon-java cylon)

INSTALL(TARGETS ${SWIG_MODULE_cylon-java_REAL_NAME} LIBRARY DESTINATION lib/java)
