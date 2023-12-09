add_executable(run_simulation
  User_Interface.cpp
)

target_link_libraries(run_simulation
  PRIVATE
  yaml-cpp
  fmt::fmt
  #include our other libraries created in the home directory cmake file
)

target_compile_features(run_simulation PRIVATE cxx_std_20)
