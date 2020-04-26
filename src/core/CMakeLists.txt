UBPA_DOWNLOADZIP_PRO(
	https://cdn.jsdelivr.net/gh/Ubpa/UData@master/UScene/tiny_obj_loader.zip
	tiny_obj_loader.zip
	${CMAKE_CURRENT_SOURCE_DIR}/Primitive/_deps
	SHA256 5084FC75CD1783A68F6D9C136256C05919846DC6BB3075F55CF134A522ADB641
)

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

Ubpa_GetTargetName(core ${PROJECT_SOURCE_DIR}/src/core)
Ubpa_GetTargetName(parser ${PROJECT_SOURCE_DIR}/src/parser)

function(GenRefl dir)
	file(GLOB_RECURSE headers LIST_DIRECTORIES false
		"${PROJECT_SOURCE_DIR}/include/UScene/core/${dir}/*.h")

	add_custom_command(
	  TARGET ${core}
	  PRE_BUILD
	  COMMAND ${parser} ARGS "${PROJECT_SOURCE_DIR}/src/core/${dir}/detail/dynamic_reflection/" ${headers}
	  DEPENDS ${parser}
	)
endfunction()

if(true)
GenRefl(Cmpt)
GenRefl(Material)
GenRefl(Light)
GenRefl(Primitive)

add_custom_command(
  TARGET ${core}
  PRE_BUILD
  COMMAND ${parser} ARGS "${PROJECT_SOURCE_DIR}/src/core/detail/dynamic_reflection/"
	"${PROJECT_SOURCE_DIR}/include/UScene/core/SObj.h"
	"${PROJECT_SOURCE_DIR}/include/UScene/core/Scene.h"
  DEPENDS ${parser}
)
endif()
