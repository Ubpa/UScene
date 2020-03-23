Ubpa_GlobGroupSrcs(RST sources PATHS
	${CMAKE_CURRENT_SOURCE_DIR}
	"${PROJECT_SOURCE_DIR}/include/UScene/tool"
)

Ubpa_GetTargetName(core "${PROJECT_SOURCE_DIR}/src/core")

Ubpa_AddTarget(MODE "LIB"
	SOURCES ${sources}
	INTERFACE_INC ON
	LIBS "Ubpa::${core}"
)

Ubpa_GetTargetName(cur ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${cur} PUBLIC ${RapidJSON_INCLUDE_DIRS})
