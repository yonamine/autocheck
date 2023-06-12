add_custom_target(AutocheckUnitTests)
set_target_properties(AutocheckUnitTests PROPERTIES FOLDER "Autocheck Tests")

function(add_autocheck_unittest test_dirname)
  add_unittest(AutocheckUnitTests ${test_dirname} ${ARGN})
endfunction()
