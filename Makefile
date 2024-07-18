CXX           =
ObjSuf        = o
SrcSuf        = cpp
ExeSuf        =
DllSuf        = so
OutPutOpt     = -o # keep whitespace after "-o"

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLDFLAGS  := $(shell root-config --ldflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
HASTHREAD    := $(shell root-config --has-thread)
ROOTVERSION =   $(shell root-config --version)
ROOTPREFIX  =   $(shell root-config --prefix)

# Remember to also set LD_LIBRARY_PATH so that ld can find the lib at runtime.
MESYTEC_MVLC := /usr/local/mesytec-mvlc
MESYTEC_MVLC := /home/florian/local/mvme

# Force c++17 for now. The ROOT binaries for ubuntu-20.04 will complain because
# they are built with c++14 support only. No issues so far, the output file
# looks good.  Longer term solution: upgrade you OS, compiler and ROOT! ROOT
# for ubuntu-22 is built with c++17 support!
MESY_CFLAGS  := -I$(MESYTEC_MVLC)/include -I$(MESYTEC_MVLC)/include/mesytec-mvlc -std=c++17
MESY_LIBS    := -L$(MESYTEC_MVLC)/lib -lmesytec-mvlc

CXX           = g++
CXXFLAGS      = -O -Wall -fPIC -g
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS) $(MESY_CFLAGS)
LDFLAGS      += $(ROOTLDFLAGS)
LIBS          = $(ROOTLIBS) $(MESY_LIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(MESY_LIBS) $(SYSLIBS)

#------------------------------------------------------------------------------

# Wildcard does not work anymore because of multiple main() functions. Manually
# specifying the files is better anyways.
SOURCES     := CanvasFrame.cpp MyMainFrame.cpp PictureFrame.cpp MyMainFrameDict.$(SrcSuf) my_experiment.cpp MyExperimentDict.cpp
OBJECTS     := $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
RUNGUI        = runGUI$(ExeSuf)
RUNCLI       = runCLI$(ExeSuf)

OBJS          = $(OBJECTS)

PROGRAMS      = $(RUNGUI) $(RUNCLI)

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:            $(PROGRAMS)

$(RUNGUI): $(OBJECTS) test.o
		$(LD) $(LDFLAGS) $^ $(GLIBS) $(OutPutOpt)$@
		@echo "$@ done"

$(RUNCLI):  $(OBJECTS) runcli_main.o
		$(LD) $(LDFLAGS) $^ $(GLIBS) $(OutPutOpt)$@
		@echo "$@ done"

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

MyMainFrameDict.$(SrcSuf): MyMainFrame.h my_experiment.h CanvasFrame.h PictureFrame.h LinkDef.h
	@echo "Generating dictionary $@..."
	rootcint -f $@ -c $^

#MyExperimentDict.$(SrcSuf): my_experiment.h MyMainFrame.h CanvasFrame.h PictureFrame.h my_experiment_LinkDef.h
#	@echo "Generating dictionary $@..."
#	rootcint -f $@ -c $^

.PHONY: all clean distclean print_root_version

clean:
		rm -f *.o *Dict.* *.pcm

distclean:		clean
		rm -f $(PROGRAMS)

print_root_version:
	@echo "Compiling against ROOT-${ROOTVERSION} found in ${ROOTPREFIX}"

#mvme_root_event_rdict.cxx: $(EVENT_OBJECTS_DEPS)
#	rootcling -f $@ -rml libmvme_root_event.so -rmf mvme_root_event.rootmap $(EVENT_OBJECTS_HEADERS)
#
#libmvme_root_event.so: $(EVENT_OBJECTS_DEPS) mvme_root_event_rdict.cxx
#	$(CXX) $(ROOTCFLAGS) $(CXXFLAGS) $(ROOTLIBS) -shared -fPIC -o $@ $< mvme_root_event_rdict.cxx
#
#mvme_root_client: $(current_dir)mvme_root_client.cc libmvme_root_event.so $(TEMPLATE_FILES)
#	$(CXX) $(LDFLAGS) $(ROOTCFLAGS) $(CXXFLAGS) $< '-Wl,-rpath,$$ORIGIN/:$$ORIGIN/../lib' libmvme_root_event.so $(ROOTLIBS) -o $@
#
#clean:
#	-rm -f libmvme_root_event.so mvme_root_event_rdict.cxx mvme_root_event_rdict_rdict.pcm
#	-rm -f mvme_root_client mvme_root_event.rootmap
#
#install: all
#	mkdir -p $(MVME)/bin $(MVME)/lib $(MVME)/include
#	install -m 755 mvme_root_client $(MVME)/bin/mvme_root_client
#	install -m 755 libmvme_root_event.so $(MVME)/lib/libmvme_root_event.so
#	install -m 644 mvme_root_event_rdict_rdict.pcm $(MVME)/lib/mvme_root_event_rdict_rdict.pcm
#	install -m 644 $(EVENT_OBJECTS_HEADERS) $(MVME)/include/mvme
#
