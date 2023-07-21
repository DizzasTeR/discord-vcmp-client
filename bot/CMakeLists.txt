set(OPTION_ARCHITECTURE "x64" CACHE STRING "Architecture")

add_library(DDiscord SHARED
   	discord-client.cpp
)

# set c++ standard 17
set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)

add_compile_options(-fpermissive -fPIC)

add_subdirectory(${PROJECT_SOURCE_DIR}/DPP dpp)
target_link_libraries(DDiscord PUBLIC dpp)

target_include_directories(DDiscord
	PRIVATE
	${PROJECT_SOURCE_DIR}/bot
	${PROJECT_SOURCE_DIR}/bot/include
	${PROJECT_SOURCE_DIR}/DPP/include
)