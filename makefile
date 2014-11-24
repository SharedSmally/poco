

#added Poco libraries:
#POCO_{BASE|ZIP|NET|CRYPTO|ODBC|SQLITE|XML}_LIBS
include makefile_poco

#common flags:
#add include paths: -I${include_dirs}
#CPPFLAGS += 
#extra flags for c compile ${CC}
#CFLAGS +=
#extra flags for cxx compile ${CXX}
#CXXFLAGS += std=c++0x
#add library paths: -L${library_dirs}
#LDFLAGS +=
#add libraries: -l${library} 
LDLIBS += 

