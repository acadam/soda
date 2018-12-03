project(drt_fault_localization_technique_plugin)

include_directories(${drt_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${drt_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${drt_fault_localization_technique_plugin_SOURCE_DIR} drt_fault_localization_technique_src)

add_library(drt_fault_localization_technique SHARED ${headers} ${drt_fault_localization_technique_src})
target_link_libraries(drt_fault_localization_technique SoDAEngine SoDA ${Boost_LIBRARIES})
