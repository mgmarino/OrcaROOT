set(TOOLKIT_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/OrcaRootHeader/OrcaRootHeader.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OrcaRootHeader/Plist.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OrcaRootHeader/PlistNode.cc
	${CMAKE_CURRENT_SOURCE_DIR}/OrcaRootHeader/PlistParser.cc
)

set(TOOLKIT_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/OrcaRootHeader/OrcaRootHeader.hh
	${CMAKE_CURRENT_SOURCE_DIR}/OrcaRootHeader/Plist.hh
	${CMAKE_CURRENT_SOURCE_DIR}/OrcaRootHeader/PlistNode.hh
	${CMAKE_CURRENT_SOURCE_DIR}/OrcaRootHeader/PlistParser.hh
)


add_library(ORToolKit SHARED ${TOOLKIT_SRC} ${TOOLKIT_HEADERS})
target_link_libraries(ORToolKit ${ROOT_LIBRARIES}  ${ROOT_XML_LIBRARY} OrcaRoot)
install(TARGETS ORToolKit LIBRARY DESTINATION lib)


add_executable(orcaroot-header-dump OrcaRootHeader/orcaroot-header-dump.cc)
target_link_libraries(orcaroot-header-dump ORToolKit)

add_executable(orcaroot-header-error-sample OrcaRootHeader/orcaroot-header-error-sample.cc)
target_link_libraries(orcaroot-header-error-sample ORToolKit)

add_executable(orcaroot-header-sample OrcaRootHeader/orcaroot-header-sample.cc)
target_link_libraries(orcaroot-header-sample ORToolKit)

add_executable(orcaroot-header-xml-dump OrcaRootHeader/orcaroot-header-xml-dump.cc)
target_link_libraries(orcaroot-header-xml-dump ORToolKit)