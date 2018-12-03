project(kulczynksi2_fault_localization_technique_plugin)

include_directories(${kulczynksi2_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${kulczynksi2_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${kulczynksi2_fault_localization_technique_plugin_SOURCE_DIR} kulczynksi2_fault_localization_technique_src)

add_library(kulczynksi2_fault_localization_technique SHARED ${headers} ${kulczynksi2_fault_localization_technique_src})
target_link_libraries(kulczynksi2_fault_localization_technique SoDAEngine SoDA ${Boost_LIBRARIES})
