Ubpa_DownloadZip_Pro(
	https://cdn.jsdelivr.net/gh/Ubpa/UData@master/UScene/include/_deps/rapidjson.zip
	rapidjson.zip
	${CMAKE_CURRENT_SOURCE_DIR}/_deps
	SHA256 EB266C625973E952AD88500C45331A8DB5F79B2FEB276371DE67A7131B8785F3
)

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
