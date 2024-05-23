build_lib(
  LIBNAME emtest
  SOURCE_FILES
    helper/emtest-helper.cc
    model/emtest-channel.cc
    model/emtest-net-device.cc
  HEADER_FILES
    helper/emtest-helper.h
    model/emtest-channel.h
    model/emtest-net-device.h
  LIBRARIES_TO_LINK ${libnetwork}
)
