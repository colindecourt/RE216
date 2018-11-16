cmake_minimum_required(VERSION 2.8.3)

project(RE216_JALON05)
add_subdirectory(include)


#add here source files you need to compile and link to the client app
SET(SRC_CLIENT
	client.c
	${client_tools_files}
    	include/common_tools.h
    	include/common_tools.c
    	include/client_tools.h
    	include/client_tools.c
		include/lines.h
		include/lines.c
	)
add_executable(RE216_JALON05_CLIENT ${SRC_CLIENT})


#add here source files you need to compile and link to the server app
SET(SRC_SERVER
	server.c
	${server_tools_files}
    include/common_tools.h
    include/common_tools.c
    include/server_tools.h
    include/server_tools.c
	include/lines.h
	include/lines.c
	)
add_executable(RE216_JALON05_SERVER  ${SRC_SERVER})
