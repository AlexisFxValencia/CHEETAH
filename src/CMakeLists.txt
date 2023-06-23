add_library(CHEETAH_lib "Neutron.cpp" "Neutron.h" "Vector3.cpp" "Vector3.h")
add_executable (CHEETAH "CHEETAH.cpp" "CHEETAH.h" )
target_link_libraries(CHEETAH PRIVATE CHEETAH_lib)

# MPI
find_package(MPI REQUIRED)
target_link_libraries(CHEETAH PUBLIC MPI::MPI_CXX)

#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET CHEETAH PROPERTY CXX_STANDARD 20)
#endif()

# TODO: Ajoutez des tests et installez des cibles si nécessaire.

