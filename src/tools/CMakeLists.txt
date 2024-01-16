add_subdirectory(driver)
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/autocheck-plugins/server)
  add_subdirectory(autocheck-plugins/server)
endif()
