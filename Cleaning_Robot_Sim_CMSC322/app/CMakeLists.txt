add_executable(user_interface
  User_Interface.cpp
)

target_link_libraries(user_interface
  PRIVATE
  yaml-cpp
  fmt::fmt
  #include our other libraries created in the home directory cmake file
)

target_compile_features(user_interface PRIVATE cxx_std_20)
