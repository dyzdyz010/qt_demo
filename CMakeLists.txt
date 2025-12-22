cmake_minimum_required(VERSION 3.16)

project(qt_demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
    add_compile_options("/Zc:__cplusplus" "/permissive-" "/utf-8")
endif()

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appqt_demo
    main.cpp
    formcontroller.cpp
    formcontroller.h
    formmodel.cpp
    formmodel.h
    rulemodel.cpp
    rulemodel.h
    ruleengine.cpp
    ruleengine.h
    rulecontroller.cpp
    rulecontroller.h
    conditionmodel.cpp
    conditionmodel.h
    ruleeditorcontroller.cpp
    ruleeditorcontroller.h
    listvaluemodel.cpp
    listvaluemodel.h
)

qt_add_qml_module(appqt_demo
    URI qt_demo
    QML_FILES
        Main.qml
        FormPage.qml
        RuleConfig.qml
        RuleListPanel.qml
        RuleEditorPanel.qml
        ConditionRow.qml
        RulePreviewDialog.qml
        Theme.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appqt_demo PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appqt_demo
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appqt_demo
    PRIVATE Qt6::Quick
    PRIVATE Qt6::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS appqt_demo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
