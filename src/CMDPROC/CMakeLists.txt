set(CMD_SRCS cmdproc.h cmdproc.c)

set(LIB_TYPE STATIC)

add_library (CMD_Lib ${LIB_TYPE} ${CMD_SRCS})

target_link_libraries(CMD_Lib m)
