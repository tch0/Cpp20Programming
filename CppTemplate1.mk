# https://github.com/tch0/MyConfigurations/blob/master/MakefileTemplate/CppTemplate1.mk

# Makefile template 1:
# For C++ sigle files, every C++ file will get a executable.

# make debug=yes to compile with -g
# make system=windows for windows system

.PHONY : all
.PHONY .IGNORE : clean

# add you own include path/library path/link library to CXXFLAGS
CXX = g++
CXXFLAGS += -std=c++20
RM = rm

# debug
ifeq ($(debug), yes)
CXXFLAGS += -g
else
CXXFLAGS += -O3
CXXFLAGS += -DNDEBUG
endif

# filenames and targets
all_source_files := $(wildcard *.cpp)
all_targets := $(basename $(all_source_files))

# all targets
all : $(all_targets)

# compile
% : %.cpp
	-$(CXX) $^ -o $@ $(CXXFLAGS)

# system: affect how to clean and executable file name
# value: windows/unix
system = unix
ifeq ($(system), windows)
all_targets := $(addsuffix .exe, $(all_targets))
RM := del
endif

# clean
clean :
	-$(RM) $(all_targets)
