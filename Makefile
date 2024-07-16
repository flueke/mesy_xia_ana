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

# Remember to also set LD_LIBRARY_PATH so that ld can find the lib at runtime.
MESYTEC_MVLC := /usr/local/mesytec-mvlc

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

SOURCES     := $(wildcard *.$(SrcSuf)) MyMainFrameDict.$(SrcSuf)
OBJECTS     := $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
RUNGUI        = runGUI$(ExeSuf)

OBJS          = $(OBJECTS)

PROGRAMS      = $(RUNGUI)

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:            $(PROGRAMS)

$(RUNGUI):      $(OBJECTS)
		$(LD) $(LDFLAGS) $^ $(GLIBS) $(OutPutOpt)$@
		@echo "$@ done"

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

#MyMainFrameDict.$(SrcSuf): MyMainFrame.h CanvasFrame.h PictureFrame.h LinkDef.h
#		@echo "Generating dictionary $@..."
#		@rootcint -f $@ -c $^

# The modern version of rootcint. This is probably the way to go, instead of
# still using rootcint.
MyMainFrameDict.$(SrcSuf): MyMainFrame.h CanvasFrame.h PictureFrame.h LinkDef.h
	@echo "Generating dictionary $@..."
	rootcling -f $@ -rml $@.so -rmf $@.rootmap $^

clean:
		rm -f *.o *Dict.*

distclean:		clean
		rm -f $(PROGRAMS)
