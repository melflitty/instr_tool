cmake_minimum_required(VERSION 3.14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Set source files
set(SOURCES  "test_direct_ins.cpp")
set(PIN_TOOL MyFirstTool)



add_library (${PIN_TOOL} SHARED ${SOURCES})
set_property(TARGET ${PIN_TOOL} PROPERTY CXX_STANDARD 17)

target_link_libraries(${PIN_TOOL} ${PIN_LIBRARIES})
target_link_options(${PIN_TOOL} PUBLIC ${PIN_LDFLAGS}) 

install(TARGETS ${PIN_TOOL})
