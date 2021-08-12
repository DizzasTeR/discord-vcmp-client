add_library(DDiscord SHARED
   	sleepy-discord-client.cpp

)

if(BUILD_64)
	target_compile_options(DDiscord PRIVATE -D_SQ64)
else()
	set_target_properties(DDiscord PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
endif()

target_include_directories(DDiscord
	PRIVATE
		${PROJECT_SOURCE_DIR}/bot
		${PROJECT_SOURCE_DIR}/bot/include
)

target_link_libraries(DDiscord sleepy-discord)
