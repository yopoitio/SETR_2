set(SEENSORS_SRCS sensors.h sensors.c)

set(LIB_TYPE STATIC)

add_library (SENSORS_Lib ${LIB_TYPE} ${SENSORS_SRCS})