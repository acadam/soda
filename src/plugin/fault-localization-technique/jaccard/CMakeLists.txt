project(jaccard_fault_localization_technique_plugin)

include_directories(${jaccard_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${jaccard_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${jaccard_fault_localization_technique_plugin_SOURCE_DIR} jaccard_fault_localization_technique_src)

add_library(jaccard_fault_localization_technique SHARED ${headers} ${jaccard_fault_localization_technique_src})
target_link_libraries(jaccard_fault_localization_technique SoDAEngine SoDA ${Boost_LIBRARIES})
