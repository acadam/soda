project(wong3_fault_localization_technique_plugin)

include_directories(${wong3_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${wong3_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${wong3_fault_localization_technique_plugin_SOURCE_DIR} wong3_fault_localization_technique_src)

add_library(wong3_fault_localization_technique SHARED ${headers} ${wong3_fault_localization_technique_src})
target_link_libraries(wong3_fault_localization_technique SoDAEngine SoDA ${Boost_LIBRARIES})
