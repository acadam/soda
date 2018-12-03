project(op2_fault_localization_technique_plugin)

include_directories(${op2_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${op2_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${op2_fault_localization_technique_plugin_SOURCE_DIR} op2_fault_localization_technique_src)

add_library(op2_fault_localization_technique SHARED ${headers} ${op2_fault_localization_technique_src})
target_link_libraries(op2_fault_localization_technique SoDAEngine SoDA ${Boost_LIBRARIES})
