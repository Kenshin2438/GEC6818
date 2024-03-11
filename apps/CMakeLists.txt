macro(add_app exec_name)
  add_executable("${exec_name}" "${exec_name}.c")
  target_link_libraries("${exec_name}" util)
endmacro(add_app)

# add_app(practice_lcd_draw)

add_app(show_bmp)
add_app(csust)
add_app(album)
