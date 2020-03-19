Ubpa_GlobGroupSrcs(RST sources PATHS
	${CMAKE_CURRENT_SOURCE_DIR}
	"${PROJECT_SOURCE_DIR}/include/UScene/core"
)

Ubpa_ToPackageName(ugm_package_name UGM ${UGM_VERSION})
Ubpa_ToPackageName(uecs_package_name UECS ${UECS_VERSION})
Ubpa_ToPackageName(udp_package_name UDP ${UDP_VERSION})
Ubpa_ToPackageName(utemplate_package_name UTemplate ${UTemplate_VERSION})

Ubpa_AddTarget(MODE "LIB"
	SOURCES ${sources}
	INTERFACE_INC ON
	LIBS
		"Ubpa::${ugm_package_name}_core"
		"Ubpa::${uecs_package_name}_core"
		"Ubpa::${uecs_package_name}_cmpt"
		"Ubpa::${udp_package_name}_core"
		"Ubpa::${utemplate_package_name}_core"
)
