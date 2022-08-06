add_library(DDiscord SHARED
   	discord-client.cpp
)

# set c++ standard 17
set_target_properties(DDiscord PROPERTIES
   	CXX_STANDARD 17
)

target_include_directories(DDiscord
	PRIVATE
		${PROJECT_SOURCE_DIR}/include/dpp-10.0
		${PROJECT_SOURCE_DIR}/bot
		${PROJECT_SOURCE_DIR}/bot/include
)

target_link_directories(DDiscord PRIVATE ${PROJECT_SOURCE_DIR}/lib/dpp-10.0)

if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
	target_link_libraries(DDiscord dpp.lib)
else()
	target_link_libraries(DDiscord dpp32.lib)
endif()