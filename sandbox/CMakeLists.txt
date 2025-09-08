project("sandbox")

set(src
  "src/main.cpp"
  "src/camera_controller.cpp"
  )

add_executable(sandbox ${src})

target_include_directories(sandbox PUBLIC)

target_link_libraries(sandbox Noor)
