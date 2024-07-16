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
#MESYTEC_MVLC := $(HOME)/local/mesytec-mvlc
MESYTEC_MVLC := $(HOME)/local/mvme
MESY_CFLAGS  := -I$(MESYTEC_MVLC)/include -I$(MESYTEC_MVLC)/include/mesytec-mvlc
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

MyMainFrameDict.$(SrcSuf): MyMainFrame.h CanvasFrame.h PictureFrame.h LinkDef.h
	@echo "Generating dictionary $@..."
	rootcling -f $@ -rml $@.so -rmf $@.rootmap $^

clean:
		rm -f *.o *Dict.*

distclean:		clean
		rm -f $(PROGRAMS)
