ROOTCFLAGS	=	$(shell root-config --cflags)
ROOTLIBS	=	$(shell root-config --libs)
ROOTVERSION =   $(shell root-config --version)
ROOTPREFIX  =   $(shell root-config --prefix)

# Set this to the installation path of mesytec-mvlc. Remember to also set
# LD_LIBRARY_PATH so that ld can find the lib at runtime.
#MESYTEC_MVLC := /usr/local/mesytec-mvlc
# If mvme is installed its path can also be used to find the mesytec-mvlc lib.
MESYTEC_MVLC := /home/florian/local/mvme

MESY_CFLAGS  := -I$(MESYTEC_MVLC)/include -I$(MESYTEC_MVLC)/include/mesytec-mvlc
MESY_LIBS    := -L$(MESYTEC_MVLC)/lib -lmesytec-mvlc

CXXFLAGS   += -Wall -fPIC -O3 -g
CXXFLAGS   += $(ROOTCFLAGS) $(MESY_CFLAGS)

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(dir $(mkfile_path))
RPATH       = '-Wl,-rpath,$$ORIGIN/:$(MESYTEC_MVLC)/lib:$(current_dir)'

# If -std=c++11/c++14 is set by root-config force c++17 here. Newer ROOT
# versions set c++14 or c++17 depending on how they were built.
ifneq "$(findstring -std=c++17, ${ROOTCFLAGS})" ""
CXXFLAGS += -std=c++17
endif

.PHONY: all clean print_root_version

all: print_root_version runCLI

EXP_HEADERS = MyExperiment.h MyExperiment_LinkDef.h
EXP_IMPLS   = MyExperiment.cpp MyExperiment_rdict.cxx

# Dictionary code generation. rootcling is the modern variant of rootcint.
MyExperiment_rdict.cxx: $(EXP_HEADERS)
	rootcling -f $@ -rml libMyExperiment.so -rmf MyExperiment.rootmap $(EXP_HEADERS)

# Create a shared library which inclues the generated ROOT dictionary code. This
# way we have to generated code in one place and not linked into multiple binaries.
libMyExperiment.so: $(EXP_HEADERS) $(EXP_IMPLS) MyExperiment_rdict.cxx
	$(CXX) $(ROOTCFLAGS) $(CXXFLAGS) $(ROOTLIBS) -shared -fPIC $(EXP_IMPLS) $(MESY_LIBS) $(RPATH) -o $@

# Build the command line tool. rpath is used to help ld find the libs at runtime
# without having to modify the system ld.so.conf file. Check `ldd runCLI` to see
# the paths. This allows you to execute runCLI from any directory.
runCLI: runcli_main.cpp libMyExperiment.so
	$(CXX) $(LDFLAGS) $(ROOTCFLAGS) $(CXXFLAGS) $< libMyExperiment.so $(ROOTLIBS) $(RPATH) -o $@

clean:
	-rm -f libMyExperiment.so MyExperiment_rdict.cxx *.pcm
	-rm -f runCLI runGUI MyExperiment.rootmap

print_root_version:
	@echo "Compiling against ROOT-${ROOTVERSION} found in ${ROOTPREFIX}"
