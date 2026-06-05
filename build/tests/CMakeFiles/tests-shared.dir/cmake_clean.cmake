file(REMOVE_RECURSE
  "../bin/tests-shared"
  "../bin/tests-shared.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/tests-shared.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
