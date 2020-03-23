Ubpa_GetTargetName(tool "${PROJECT_SOURCE_DIR}/src/tool")
Ubpa_AddTarget(MODE "EXE" TEST ON LIBS "Ubpa::${tool}")
