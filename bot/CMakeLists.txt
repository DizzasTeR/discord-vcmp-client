add_library(DDiscord SHARED
   	discord-client.cpp
)

# set c++ standard 17
set_target_properties(DDiscord PROPERTIES
   	CXX_STANDARD 17
)

if(BUILD_64)
	target_compile_options(DDiscord PRIVATE -D_SQ64)
else()
	set_target_properties(DDiscord PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
endif()

target_include_directories(DDiscord
	PRIVATE
		${PROJECT_SOURCE_DIR}/include/dpp-10.0
		${PROJECT_SOURCE_DIR}/bot
		${PROJECT_SOURCE_DIR}/bot/include
)


target_link_directories(DDiscord PRIVATE ${PROJECT_SOURCE_DIR}/lib/dpp-10.0)
target_link_libraries(DDiscord dpp.lib)

# Link with dpp.lib
# target_link_libraries(DDiscord
# 	PRIVATE
# 		${PROJECT_SOURCE_DIR}/lib/dpp-10.0/dpp.lib
# )