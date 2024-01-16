function(copy_dependency_dll)
  set(oneValueArgs TARGET DEPENDENCY)
  cmake_parse_arguments(COPY_DEPENDENCY_DLL "" "${oneValueArgs}" "" ${ARGN})

  get_target_property(full_path ${COPY_DEPENDENCY_DLL_DEPENDENCY} IMPORTED_LOCATION)
  get_filename_component(lib_name "${full_path}" NAME)

  add_custom_command(
      TARGET ${COPY_DEPENDENCY_DLL_TARGET}
      POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy "${full_path}" "${CMAKE_BINARY_DIR}/${lib_name}"
      COMMENT "Copying ${COPY_DEPENDENCY_DLL_DEPENDENCY}..."
  )
endfunction()
