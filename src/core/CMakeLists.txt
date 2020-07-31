set(components
  #LocalToParent
  LocalToWorld
  #NonUniformScale
  #Parent
  Rotation
  #RotationEuler
  Scale
  Translation
  WorldToLocal
)

set(refls "")

foreach(cmpt ${components})
  set(dst "${PROJECT_SOURCE_DIR}/include/UScene/Components/details/${cmpt}_AutoRefl.inl")
  Ubpa_USReflAutoRefl(
    "${PROJECT_SOURCE_DIR}/include/UScene/Components/${cmpt}.h"
    ${dst}
  )
  list(APPEND refls ${dst})
endforeach()

Ubpa_AddTarget(
  MODE STATIC
  SOURCE
    "${PROJECT_SOURCE_DIR}/include/UScene"
    ${refls}
  INC
    "${PROJECT_SOURCE_DIR}/include"
  LIB
    Ubpa::UGM_core
    Ubpa::UECS_core
    Ubpa::UDP_core
    Ubpa::USRefl_core
)
