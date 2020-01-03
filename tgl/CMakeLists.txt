project(tgl)

set(INC ${PROJECT_SOURCE_DIR})

set(TGL_HEADERS
	init.hpp
	strings.hpp
	image.hpp
	texture.hpp
	mesh.hpp
	shader.hpp
	blending.hpp
)
PREPEND(TGL_HEADERS "${INC}/tgl" ${TGL_HEADERS})

set(HEADERS
	${INC}/tgl.hpp
	${TGL_HEADERS}
)

set(SOURCES
	init.cpp
	image.cpp
	texture.cpp
	render_target.cpp
	shader.cpp
	mesh.cpp
	blending.cpp
)
PREPEND(SOURCES "${PROJECT_SOURCE_DIR}/tgl" ${SOURCES})

add_library(tgl STATIC
	${HEADERS}
	${SOURCES}
)

target_include_directories(tgl PUBLIC  ${INC})

target_link_libraries(tgl
	tl
	glm
	glad
	stb
)

#set_target_properties(tgl PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
#cotire(tgl)
