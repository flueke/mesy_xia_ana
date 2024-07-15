#ifndef PictureFrame_h
#define PictureFrame_h
#include <set>
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TCut.h"
#include <TMath.h>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include "TGClient.h"
#include "TGButton.h"
#include "TRootEmbeddedCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TApplication.h"
#include "TGStatusBar.h"
#include "GuiTypes.h"
#include "TGFont.h"
#include "TGFileDialog.h"
#include "TObjString.h"
#include "TGFrame.h"
#include "TGTextView.h"
#include "TGTextEntry.h"
#include "TGComboBox.h"
using namespace std;
class PictureFrame : public TGMainFrame {
public:
	PictureFrame(const TGWindow* p, UInt_t w, UInt_t h, const char* file);
	virtual ~PictureFrame();
	void PrevBranch();
	void NextBranch();
	void PrevModule();
	void NextModule();
	void drawCanvas();
	void CloseWindow();
	void ExportPDF();
	string ModInfo(int Module);
private:
	TCanvas* canvas;
	TRootEmbeddedCanvas* embeddedCanvas;
	TGTextButton* prevBranchButton;
	TGTextButton* nextBranchButton;
	TGTextButton* prevModuleButton;
	TGTextButton* nextModuleButton;
	TGTextButton* ExportPDFButton;
	int fModuleIndex;
	int fCurrentBranchIndex;
	int fTotalBranches;
	TTree* fTree;
	set<int> uniqueMods;
	TString filename;
};
#endif
