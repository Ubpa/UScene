Ubpa_GlobGroupSrcs(RST sources PATHS
	${CMAKE_CURRENT_SOURCE_DIR}
	"${PROJECT_SOURCE_DIR}/include/UScene/core"
)

Ubpa_AddTarget_GDR(MODE "LIB"
	SOURCES ${sources}
	INTERFACE_INC ON
	PUBLIC_GLIBS
		"Ubpa::UGM_core"
		"Ubpa::UECS_core"
		"Ubpa::UDP_core"
		"Ubpa::UTemplate_core"
)
