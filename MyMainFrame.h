#ifndef MyMainFrame_h
#define MyMainFrame_h
#include <map>
#include <TMath.h>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>

#include "TCut.h"
#include "TGClient.h"
#include "TGButton.h"
#include "TFile.h"
#include "TTree.h"
#include "TApplication.h"
#include "TGFont.h"
#include "TGFileDialog.h"
#include "TObjString.h"
#include "TGFrame.h"
#include "TGTextView.h"
#include "CanvasFrame.h"
using namespace std;
class MyMainFrame : public TGMainFrame{
public:
	MyMainFrame(const TGWindow* p, UInt_t w, UInt_t h);
	virtual ~MyMainFrame();
	void ExportFile();
	bool restore_ROOT_v0(string name);
	bool restore_ROOT(string name);
	bool ClearFile();
	void DrawFile();
	void CanvasClosed();
	ClassDef(MyMainFrame, 1)
private:
	CanvasFrame* fCanvasFrame;
	TGTextView* fFilename1;
	TGTextView* fFilename2;
	TGTextView* fFilename3;
	TGTextButton* fExportButton;
	TGTextButton* fDrawButton;
	TGTextButton* fClearButton;
	TGTextButton* fExitButton;
	TString file;
};

#endif
