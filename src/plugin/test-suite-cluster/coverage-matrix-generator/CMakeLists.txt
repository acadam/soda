project(coverage_matrix_generator_test_suite_cluster_plugin)

include_directories(${coverage_matrix_generator_test_suite_cluster_plugin_SOURCE_DIR}/../../../lib/SoDA/inc
                    ${coverage_matrix_generator_test_suite_cluster_plugin_SOURCE_DIR}/../../../lib/SoDAEngine/inc
                    ${RAPIDJSON_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE headers ./*.h)
aux_source_directory(${coverage_matrix_generator_test_suite_cluster_plugin_SOURCE_DIR} coverage_matrix_generator_test_suite_cluster_plugin_src)

add_library(coverage_matrix_generator_test_suite_cluster_plugin SHARED ${headers} ${coverage_matrix_generator_test_suite_cluster_plugin_src})
target_link_libraries(coverage_matrix_generator_test_suite_cluster_plugin SoDAEngine SoDA ${Boost_LIBRARIES})