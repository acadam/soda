project(basemodel_fault_localization_technique_plugin)

include_directories(${basemodel_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${basemodel_fault_localization_technique_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${basemodel_fault_localization_technique_plugin_SOURCE_DIR} basemodel_fault_localization_technique_src)

add_library(basemodel_fault_localization_technique SHARED ${headers} ${basemodel_fault_localization_technique_src})
target_link_libraries(basemodel_fault_localization_technique SoDAEngine SoDA ${Boost_LIBRARIES})
