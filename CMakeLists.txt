CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(bpio)

SET(CMAKE_INSTALL_PREFIX "/")
SET(SF_ROOT /opt/SoftwareFactory/)
SET(SF_INSTALL_ASSETS_DIR ${SF_ROOT}/assets)
SET(SF_INSTALL_CONF_DIR ${SF_ROOT}/conf)
SET(SF_INSTALL_LIB_DIR ${SF_ROOT}/lib)
SET(SF_INSTALL_BIN_DIR ${SF_ROOT}/bin)
SET(SF_INSTALL_INC_DIR ${SF_ROOT}/inc)

FIND_PACKAGE(jsoncpp REQUIRED)
FIND_PACKAGE(gflags REQUIRED)
FIND_PACKAGE(Protobuf REQUIRED)
FIND_PACKAGE(GTest REQUIRED)

INCLUDE_DIRECTORIES(${SF_ROOT}/inc)
INCLUDE_DIRECTORIES(com)

LINK_DIRECTORIES(${SF_ROOT}/lib)

LINK_LIBRARIES(glog gflags jsoncpp dl pthread protobuf)

ADD_LIBRARY(${PROJECT_NAME} SHARED
    com/${PROJECT_NAME}.cpp
)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    bppb
)

INSTALL(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${SF_INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${SF_INSTALL_LIB_DIR}
    RUNTIME DESTINATION ${SF_INSTALL_BIN_DIR}
)

FILE(GLOB conf_files "conf/*.json")
INSTALL(FILES 
    ${conf_files}
    PERMISSIONS
    OWNER_READ
    GROUP_READ
    WORLD_READ
    DESTINATION ${SF_INSTALL_CONF_DIR}
)

AUX_SOURCE_DIRECTORY(test bpio_files)
foreach(ut_file ${bpio_files})
    GET_FILENAME_COMPONENT(ut_file_target ${ut_file} NAME_WE)
    ADD_EXECUTABLE(${ut_file_target} ${ut_file})
    TARGET_LINK_LIBRARIES(${ut_file_target}
        gtest_main    
        gtest
        bppb
        pthread
        bpcommon
        jsoncpp
    )
endforeach(ut_file ${bpio_files})