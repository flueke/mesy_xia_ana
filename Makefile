ROOTCFLAGS	=	$(shell root-config --cflags)
ROOTLIBS	=	$(shell root-config --libs)
ROOTVERSION =   $(shell root-config --version)
ROOTPREFIX  =   $(shell root-config --prefix)

# Set this to the installation path of mesytec-mvlc. Remember to also set
# LD_LIBRARY_PATH so that ld can find the lib at runtime.
MESYTEC_MVLC := /usr/local/mesytec-mvlc
#MESYTEC_MVLC := /home/florian/local/mvme
MESY_CFLAGS  := -I$(MESYTEC_MVLC)/include -I$(MESYTEC_MVLC)/include/mesytec-mvlc
MESY_LIBS    := -L$(MESYTEC_MVLC)/lib -lmesytec-mvlc

CXXFLAGS   += -Wall -fPIC -O3 -g
CXXFLAGS   += $(ROOTCFLAGS) $(MESY_CFLAGS)

# If -std=c++11/c++14 is set by root-config force c++17 here. Newer ROOT
# versions set c++14 or c++17 depending on how they were built.
ifneq "$(findstring -std=c++17, ${ROOTCFLAGS})" ""
CXXFLAGS += -std=c++17
endif

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(dir $(mkfile_path))

.PHONY: all clean print_root_version

all: print_root_version runCLI

EXP_HEADERS = MyExperiment.h MyExperiment_LinkDef.h
EXP_DEPS    = MyExperiment.cpp $(EXP_HEADERS)

MyExperiment_rdict.cxx: $(EXP_DEPS)
	rootcling -f $@ -rml libMyExperiment.so -rmf MyExperiment.rootmap $(EXP_HEADERS)

libMyExperiment.so: $(EXP_DEPS) MyExperiment_rdict.cxx
	$(CXX) $(ROOTCFLAGS) $(CXXFLAGS) $(ROOTLIBS) -shared -fPIC -o $@ $^ $(MESY_LIBS)

runCLI: runcli_main.cpp libMyExperiment.so
	$(CXX) $(LDFLAGS) $(ROOTCFLAGS) $(CXXFLAGS) $< '-Wl,-rpath,$$ORIGIN/:$$ORIGIN/../lib' libMyExperiment.so $(ROOTLIBS) -o $@

clean:
	-rm -f libMyExperiment.so MyExperiment_rdict.cxx *.pcm
	-rm -f runCLI runGUI MyExperiment.rootmap

print_root_version:
	@echo "Compiling against ROOT-${ROOTVERSION} found in ${ROOTPREFIX}"
















## Wildcard does not work anymore because of multiple main() functions. Manually
## specifying the files is better anyways.
#SOURCES     := CanvasFrame.cpp MyMainFrame.cpp PictureFrame.cpp MyMainFrameDict.$(SrcSuf) my_experiment.cpp MyExperimentDict.cpp
#OBJECTS     := $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
#RUNGUI        = runGUI$(ExeSuf)
#RUNCLI       = runCLI$(ExeSuf)
#
#OBJS          = $(OBJECTS)
#
#PROGRAMS      = $(RUNGUI) $(RUNCLI)
#
#.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)
#
#all:            $(PROGRAMS)
#
#$(RUNGUI): $(OBJECTS) test.o
#		$(LD) $(LDFLAGS) $^ $(GLIBS) $(OutPutOpt)$@
#		@echo "$@ done"
#
#$(RUNCLI):  $(OBJECTS) runcli_main.o
#		$(LD) $(LDFLAGS) $^ $(GLIBS) $(OutPutOpt)$@
#		@echo "$@ done"
#
#.$(SrcSuf).$(ObjSuf):
#	$(CXX) $(CXXFLAGS) -c $<
#
#MyMainFrameDict.$(SrcSuf): MyMainFrame.h my_experiment.h CanvasFrame.h PictureFrame.h LinkDef.h
#	@echo "Generating dictionary $@..."
#	rootcint -f $@ -c $^
#
##MyExperimentDict.$(SrcSuf): my_experiment.h MyMainFrame.h CanvasFrame.h PictureFrame.h my_experiment_LinkDef.h
##	@echo "Generating dictionary $@..."
##	rootcint -f $@ -c $^
#
#.PHONY: all clean distclean print_root_version
#
#clean:
#		rm -f *.o *Dict.* *.pcm
#
#distclean:		clean
#		rm -f $(PROGRAMS)
#
#print_root_version:
#	@echo "Compiling against ROOT-${ROOTVERSION} found in ${ROOTPREFIX}"
#
