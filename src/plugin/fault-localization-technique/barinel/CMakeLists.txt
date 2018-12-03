project(barinel_fault_localization_technique_plugin)

include_directories(${barinel_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${barinel_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${barinel_fault_localization_technique_plugin_SOURCE_DIR} barinel_fault_localization_technique_src)

add_library(barinel_fault_localization_technique SHARED ${headers} ${barinel_fault_localization_technique_src})
target_link_libraries(barinel_fault_localization_technique SoDAEngine SoDA ${Boost_LIBRARIES})
