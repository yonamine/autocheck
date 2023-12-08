add_subdirectory(driver)
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/vscode-autocheck/server)
  add_subdirectory(vscode-autocheck/server)
endif()
