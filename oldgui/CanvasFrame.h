#ifndef CanvasFrame_h
#define CanvasFrame_h
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
#include "PictureFrame.h"
#include "TCutG.h"
#include "TLine.h"
#include "TGStatusBar.h"

using namespace std;
class CanvasFrame : public TGMainFrame {
public:
	CanvasFrame(const TGWindow* p, UInt_t w, UInt_t h);
	virtual ~CanvasFrame();
	void SelectFile();
	void DrawHisto1D();
	void NextBranch();
	void PrevBranch();
	void Update();
	void Reset();
	void DrawHisto2D();
	void changeMode();
	bool IsInteger(const char* text);
	bool IsNonNegativeInteger(const char* text);
	bool IsNonNegativeDouble(const char* text);
	void CloseWindow();
	void SelectItemX();
	void SelectItemY();
	void UpdateTGComboBox(TTree* fTree);
	void OnReturnProcessed();
	void HistoEventHandle();
	void PrevModule();
	void NextModule();
	void PrevChannel();
	void NextChannel();
	void PDF();
	void CanvasClosed();
	void DrawFile();
	void SetEnable(bool temp);
	void Temp();
	void CutCanvas();
	void DrawCut(Int_t event, Int_t x, Int_t y, TObject *obj);
	void OpenTheWindow(TCutG* cutG);
	void DrawWindow();
	void Si_Channel();
	void UpdateStatusBar(Int_t event, Int_t px, Int_t py, TObject *selected);
	string ModInfo(int Module);
private:
	TGStatusBar* statusBar;
	PictureFrame* fPictureFrame;
	TGTextView* fText2;
	TGTextView* fText3;
	TGTextView* fText5;
	TGTextView* fText6;
	TGTextView* fText7;
	TGTextView* fText8;
	TGTextView* fText9;
	TGTextView* fText10;
	TGTextView* fText11;
	TGTextView* fText12;
	TGTextButton* fSelectButton;
	TGTextButton* fPrevBranchButton;
	TGTextButton* fNextBranchButton;
	TGTextButton* fResetButton;
	TGTextButton* fModeButton;
	TGTextButton* fUpdateButton;
	TGTextButton* fDrawHisto1DButton;
	TGTextButton* fDrawHisto2DButton;
	TGTextButton* fPrevChannelButton;
	TGTextButton* fNextChannelButton;
	TGTextButton* fPrevModuleButton;
	TGTextButton* fNextModuleButton;
	TGTextButton* fPDFButton;
	TGTextButton* fDrawHistoButton;
	TGTextButton* fTempButton;
	TGTextView* fFilename1;
	TGTextView* fFilename2;
	TRootEmbeddedCanvas* embeddedCanvas;
	TCanvas* canvas;
	int fTotalBranches;
	int fCurrentBranchIndex;
	const char* file;
	const char* x_name;
	const char* y_name;
	int prev_x;
	int prev_y;
	TGTextEntry* fTextEntry1;
	TGTextEntry* fTextEntry2;
	TGTextEntry* fTextEntry3;
	TGTextEntry* fTextEntry4;
	TGTextEntry* fTextEntry5;
	TGTextEntry* fTextEntry6;
	TGTextEntry* fTextEntry7;
	TGTextEntry* fTextEntry8;
	TGComboBox* histo_x;
	TGComboBox* histo_y;
	int mod;
	int x_bin;
	Long64_t x_min;
	Long64_t x_max;
	int y_bin;
	Long64_t y_min;
	Long64_t y_max;
	int Module;
	int Channel;
	bool select;
	bool cut_flag;
	TCutG* cutG;
	int cut_count;
	vector<int> x_point;
	vector<int> y_point;
	TLine* line;
	bool drawLine;
	bool CutDone;
};
#endif
