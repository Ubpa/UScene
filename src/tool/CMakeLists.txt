Ubpa_GlobGroupSrcs(RST sources PATHS
	${CMAKE_CURRENT_SOURCE_DIR}
	"${PROJECT_SOURCE_DIR}/include/UScene/tool"
)

Ubpa_GetTargetName(core "${PROJECT_SOURCE_DIR}/src/core")

Ubpa_AddTarget_GDR(MODE "LIB"
	SOURCES ${sources}
	INTERFACE_INC ON
	PUBLIC_GLIBS "Ubpa::${core}"
)

Ubpa_GetTargetName(cur ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${cur} PRIVATE ${RapidJSON_INCLUDE_DIRS})
