Ubpa_GetTargetName(tool "${PROJECT_SOURCE_DIR}/src/tool")
Ubpa_AddTarget(
  TEST
  MODE EXE
  LIB ${tool}
)
