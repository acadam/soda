project(minus_fault_localization_technique_plugin)

include_directories(${minus_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${minus_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${minus_fault_localization_technique_plugin_SOURCE_DIR} minus_fault_localization_technique_src)

add_library(minus_fault_localization_technique SHARED ${headers} ${minus_fault_localization_technique_src})
target_link_libraries(minus_fault_localization_technique SoDAEngine SoDA ${Boost_LIBRARIES})
