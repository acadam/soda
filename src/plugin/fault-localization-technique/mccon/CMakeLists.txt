project(mccon_fault_localization_technique_plugin)

include_directories(${mccon_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${mccon_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${mccon_fault_localization_technique_plugin_SOURCE_DIR} mccon_fault_localization_technique_src)

add_library(mccon_fault_localization_technique SHARED ${headers} ${mccon_fault_localization_technique_src})
target_link_libraries(mccon_fault_localization_technique SoDAEngine SoDA ${Boost_LIBRARIES})
