macro(add_app exec_name)
  add_executable("${exec_name}" "${exec_name}.c")
  target_link_libraries("${exec_name}" src)
  target_link_libraries("${exec_name}" util)
endmacro(add_app)

add_app(main)
#add_app(tester)
