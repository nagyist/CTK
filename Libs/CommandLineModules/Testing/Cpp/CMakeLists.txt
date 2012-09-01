set(KIT CTKCommandLineModules)
set(LIBRARY_NAME ${KIT})

set(_test_srcs)
set(_test_mocs)

if(CTK_LIB_CommandLineModules/Frontend/QtGui)
  set(QT_USE_QTUITOOLS 1)
  include(${QT_USE_FILE})

  if(CTK_LIB_CommandLineModules/Backend/LocalProcess)
    list(APPEND _test_srcs ctkCmdLineModuleFutureTest.cpp)
    list(APPEND _test_mocs ctkCmdLineModuleFutureTest.cpp)
  endif()
  if(CTK_LIB_CommandLineModules/Backend/FunctionPointer)
    list(APPEND _test_srcs ctkCmdLineModuleQtCustomizationTest.cpp)
    list(APPEND _test_mocs ctkCmdLineModuleQtCustomizationTest.cpp)
  endif()
endif()

create_test_sourcelist(Tests ${KIT}CppTests.cpp ${_test_srcs})

set(TestsToRun ${Tests})
remove(TestsToRun ${KIT}CppTests.cpp)

set(Tests_SRCS ${Tests_SRCS}
  ctkCmdLineModuleSignalTester.cpp
)
set(Tests_MOC_SRCS ${Tests_MOC_SRCS}
  ctkCmdLineModuleSignalTester.h
)
set(Tests_RESOURCES
  ctkCmdLineModuleTestResources.qrc
)

set(_base_src_include_dir ${CMAKE_SOURCE_DIR}/Libs/CommandLineModules)
set(_base_bin_include_dir ${CMAKE_BINARY_DIR}/Libs/CommandLineModules)

include_directories(
  ${CMAKE_SOURCE_DIR}/Libs/Testing
  ${CMAKE_CURRENT_BINARY_DIR}
  ${_base_src_include_dir}/Core
  ${_base_bin_include_dir}/Core
  )

set(_additional_link_libraries)

if(CTK_LIB_CommandLineModules/Backend/LocalProcess)
  include_directories(${_base_src_include_dir}/Backend/LocalProcess
                      ${_base_bin_include_dir}/Backend/LocalProcess)
  list(APPEND _additional_link_libraries CTKCommandLineModulesBackendLocalProcess)
endif()

if(CTK_LIB_CommandLineModules/Backend/FunctionPointer)
  include_directories(${_base_src_include_dir}/Backend/FunctionPointer
                      ${_base_bin_include_dir}/Backend/FunctionPointer)
  list(APPEND _additional_link_libraries CTKCommandLineModulesBackendFunctionPointer)
endif()

if(CTK_LIB_CommandLineModules/Frontend/QtGui)
  include_directories(${_base_src_include_dir}/Frontend/QtGui
                      ${_base_bin_include_dir}/Frontend/QtGui)
  list(APPEND _additional_link_libraries CTKCommandLineModulesFrontendQtGui)
endif()

set(Tests_MOC_CPP)
QT4_WRAP_CPP(Tests_MOC_CPP ${Tests_MOC_SRCS})

if(_test_mocs)
  QT4_GENERATE_MOCS(${_test_mocs})
endif()

set(Tests_UI_CPP)
if(TEST_UI_FORMS)
  QT4_WRAP_UI(Tests_UI_CPP ${Tests_UI_FORMS})
endif()
set(Tests_RESOURCES_SRCS)
QT4_ADD_RESOURCES(Tests_RESOURCES_SRCS ${Tests_RESOURCES})

add_executable(${KIT}CppTests ${Tests} ${Tests_SRCS} ${Tests_MOC_CPP} ${Tests_UI_CPP} ${Tests_RESOURCES_SRCS})
target_link_libraries(${KIT}CppTests ${_additional_link_libraries})
add_dependencies(${KIT}CppTests ctkCmdLineTestModules)

if(TARGET CTKCommandLineModulesCoreCppTests)
  add_dependencies(${KIT}CppTests CTKCommandLineModulesCoreCppTests)
endif()
if(TARGET CTKCommandLineModulesFrontendQtGuiCppTests)
  add_dependencies(${KIT}CppTests CTKCommandLineModulesFrontendQtGuiCppTests)
endif()

#
# Add Tests
#

foreach(_src ${_test_srcs})
  get_filename_component(_test_name ${_src} NAME_WE)
  SIMPLE_TEST(${_test_name})
endforeach()