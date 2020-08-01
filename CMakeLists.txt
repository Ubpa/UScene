cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(UScene VERSION 0.6.0)
message(STATUS "[Project] ${PROJECT_NAME}")

include(cmake/InitUCMake.cmake)
Ubpa_InitUCMake()

Ubpa_InitProject()

Ubpa_AddDep(UECS   0.10.1)
Ubpa_AddDep(UGM    0.6.7 )
Ubpa_AddDep(UDP    0.7.0 )
Ubpa_AddDep(USRefl 0.5.1 )

Ubpa_AddSubDirsRec(include)
Ubpa_AddSubDirsRec(src)

Ubpa_Export(DIRECTORIES "include")
