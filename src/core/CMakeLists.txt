Ubpa_DownloadFile(
	https://cdn.jsdelivr.net/gh/Ubpa/UData@master/common/include/stb_image.h
	${CMAKE_CURRENT_SOURCE_DIR}/_deps/stb_image.h
	SHA256 73B55B976E61DEBBB7BC10D5B31C8F043AB5D966E67F75258E0D3F57F2A0321E
)

Ubpa_DownloadFile(
	https://cdn.jsdelivr.net/gh/Ubpa/UData@master/common/include/stb_image_write.h
	${CMAKE_CURRENT_SOURCE_DIR}/_deps/stb_image_write.h
	SHA256 89e77920c2399cd6c638000855abcda1ea8951bb8fdd31b27b953dbb4d55bc46
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

if(false)
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
