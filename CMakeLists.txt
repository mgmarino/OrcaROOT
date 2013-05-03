cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(OrcaRoot)
set(ORCAROOT_MAJOR_VERSION 0)
set(ORCAROOT_MINOR_VERSION 1)
set(ORCAROOT_PATCH_VERSION 0)
set(ORCAROOT_VERSION ${ORCAROOT_MAJOR_VERSION}.${ORCAROOT_MINOR_VERSION}.${ORCAROOT_PATCH_VERSION})


include(cmake/FindROOT.cmake)

include_directories(${ROOT_INCLUDE_DIR})
link_directories(${ROOT_LIBRARY_DIR})
find_library(ROOT_XML_LIBRARY XMLParser ${ROOT_LIBRARY_DIR})

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  get_filename_component(BUILD_PARENT_DIR ${CMAKE_BINARY_DIR} PATH)
  set(CMAKE_INSTALL_PREFIX "${BUILD_PARENT_DIR}/install" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
endif()

include_directories(
	${PROJECT_SOURCE_DIR}/Decoders
	${PROJECT_SOURCE_DIR}/IO
	${PROJECT_SOURCE_DIR}/Management
	${PROJECT_SOURCE_DIR}/Processors
	${PROJECT_SOURCE_DIR}/Util
)

file(GLOB UTIL_SRC ${PROJECT_SOURCE_DIR}/Util/*.cc)
file(GLOB UTIL_HEADERS ${PROJECT_SOURCE_DIR}/Util/*.hh)
add_library(ORUtil SHARED ${UTIL_SRC} ${UTIL_HEADERS})
target_link_libraries(ORUtil ${ROOT_LIBRARIES}  ${ROOT_XML_LIBRARY})
install(TARGETS ORUtil LIBRARY DESTINATION lib)

file(GLOB DECODERS_SRC ${PROJECT_SOURCE_DIR}/Decoders/*.cc)
file(GLOB DECODERS_HEADERS ${PROJECT_SOURCE_DIR}/Decoders/*.hh)
add_library(ORDecoders SHARED ${DECODERS_SRC} ${DECODERS_HEADERS})
target_link_libraries(ORDecoders ${ROOT_LIBRARIES}  ${ROOT_XML_LIBRARY} ORUtil)
install(TARGETS ORDecoders LIBRARY DESTINATION lib)

file(GLOB IO_SRC ${PROJECT_SOURCE_DIR}/IO/*.cc)
file(GLOB IO_HEADERS ${PROJECT_SOURCE_DIR}/IO/*.hh)
add_library(ORIO SHARED ${IO_SRC} ${IO_HEADERS})
target_link_libraries(ORIO ${ROOT_LIBRARIES}  ${ROOT_XML_LIBRARY} ORUtil ORDecoders )
install(TARGETS ORIO LIBRARY DESTINATION lib)

file(GLOB PROCESSORS_SRC ${PROJECT_SOURCE_DIR}/Processors/*.cc)
file(GLOB PROCESSORS_HEADERS ${PROJECT_SOURCE_DIR}/Processors/*.hh)
add_library(ORProcessors SHARED ${PROCESSORS_SRC} ${PROCESSORS_HEADERS})
target_link_libraries(ORProcessors ${ROOT_LIBRARIES}  ${ROOT_XML_LIBRARY} ORDecoders ORIO)
install(TARGETS ORProcessors LIBRARY DESTINATION lib)


file(GLOB MANAGEMENT_SRC ${PROJECT_SOURCE_DIR}/Management/*.cc)
file(GLOB MANAGEMENT_HEADERS ${PROJECT_SOURCE_DIR}/Management/*.hh)
add_library(ORManagement SHARED ${MANAGEMENT_SRC} ${MANAGEMENT_HEADERS})
target_link_libraries(ORManagement ${ROOT_LIBRARIES}  ${ROOT_XML_LIBRARY} ORProcessors)
install(TARGETS ORManagement LIBRARY DESTINATION lib)

set(SOURCES
	${DECODERS_SRC}
	${IO_SRC}
	${MANAGEMENT_SRC}
	${PROCESSORS_SRC}
	${UTIL_SRC}	
)

set(HEADERS
	${DECODERS_HEADERS}
        ${IO_HEADERS}
        ${MANAGEMENT_HEADERS}
	${PROCESSORS_HEADERS}
	${UTIL_HEADERS}
)

install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

add_library(OrcaRoot SHARED ${SOURCES} ${HEADERS})
target_link_libraries(OrcaRoot ${ROOT_LIBRARIES}  ${ROOT_XML_LIBRARY})
install(TARGETS OrcaRoot LIBRARY DESTINATION lib)

add_executable(getHeaderInRootFile Applications/getHeaderInRootFile.cc)
target_link_libraries(getHeaderInRootFile OrcaRoot)

add_executable(orcaroot_exe Applications/orcaroot.cc)
target_link_libraries(orcaroot_exe OrcaRoot)

add_executable(orcarootIgor Applications/orcarootIgor.cc)
target_link_libraries(orcarootIgor OrcaRoot)

add_executable(orcaroot_KATRIN_Filter Applications/orcaroot_KATRIN_Filter.cc)
target_link_libraries(orcaroot_KATRIN_Filter OrcaRoot)

add_executable(orcaroot_KATRIN_PS Applications/orcaroot_KATRIN_PS.cc)
target_link_libraries(orcaroot_KATRIN_PS OrcaRoot)

add_executable(orcaroot_ipe_katrin Applications/orcaroot_ipe_katrin.cc)
target_link_libraries(orcaroot_ipe_katrin OrcaRoot)

add_executable(orcaroot_ipe_unc Applications/orcaroot_ipe_unc.cc)
target_link_libraries(orcaroot_ipe_unc OrcaRoot)

add_executable(orcaroot_minesh Applications/orcaroot_minesh.cc)
target_link_libraries(orcaroot_minesh OrcaRoot)

add_executable(orcaroot_vme_unc Applications/orcaroot_vme_unc.cc)
target_link_libraries(orcaroot_vme_unc OrcaRoot)

add_executable(orhexdump Applications/orhexdump.cc)
target_link_libraries(orhexdump OrcaRoot)

add_executable(testHeaderReadin Applications/testHeaderReadin.cc)
target_link_libraries(testHeaderReadin OrcaRoot)

add_executable(testStopper Applications/testStopper.cc)
target_link_libraries(testStopper OrcaRoot)

add_executable(testUtil Applications/testUtil.cc)
target_link_libraries(testUtil OrcaRoot)

add_executable(writeShaperTree Applications/writeShaperTree.cc)
target_link_libraries(writeShaperTree OrcaRoot)

install(TARGETS
	getHeaderInRootFile
	orcaroot_exe
	orcarootIgor
	orcaroot_KATRIN_Filter
	orcaroot_KATRIN_PS
	orcaroot_ipe_katrin
	orcaroot_ipe_unc
	orcaroot_minesh
	orcaroot_vme_unc
	orhexdump
	testHeaderReadin
	testStopper
	testUtil
	writeShaperTree
	DESTINATION bin)
configure_file(${CMAKE_INSTALL_PREFIX}/bin/orcaroot_exe ${CMAKE_INSTALL_PREFIX}/bin/orcaroot COPYONLY)


option(BUILD_TOOLKIT "Build Toolkit" ON)

if(BUILD_TOOLKIT)
	add_subdirectory(Toolkit)
endif()


#CMAKE Specifics
export(PACKAGE OrcaRoot)
configure_file(ModuleConfig.cmake.in "${PROJECT_BINARY_DIR}/OrcaRootConfig.cmake" @ONLY)
configure_file(ModuleConfigInstalled.cmake.in "${PROJECT_BINARY_DIR}/OrcaRootConfigInstalled.cmake" @ONLY)
configure_file(ModuleConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/OrcaRootConfigVersion.cmake" @ONLY)

#Now for the Environment Variables
configure_file(OrcaRootEnv.sh "${PROJECT_BINARY_DIR}/OrcaRootEnv.sh" @ONLY)

install(FILES
  "${PROJECT_BINARY_DIR}/OrcaRootConfig.cmake"
  "${PROJECT_BINARY_DIR}/OrcaRootConfigVersion.cmake"
  "${PROJECT_BINARY_DIR}/OrcaRootConfigInstalled.cmake"
  "${PROJECT_BINARY_DIR}/OrcaRootEnv.sh"	
  DESTINATION "${CMAKE_INSTALL_PREFIX}" COMPONENT dev)


