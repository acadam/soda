project(zoltar_fault_localization_technique_plugin)

include_directories(${zoltar_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${zoltar_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${zoltar_fault_localization_technique_plugin_SOURCE_DIR} zoltar_fault_localization_technique_src)

add_library(zoltar_fault_localization_technique SHARED ${headers} ${zoltar_fault_localization_technique_src})
target_link_libraries(zoltar_fault_localization_technique SoDAEngine SoDA ${Boost_LIBRARIES})
