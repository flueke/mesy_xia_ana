#include <TMath.h>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <array>
#include <memory>
#include <cstdio>
#include <regex>
#include <stdexcept>

#include "my_experiment.h"

#include "TH2D.h"
#include "TCut.h"
#include "TGClient.h"
#include "TGButton.h"
#include "TRootEmbeddedCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TGFont.h"
#include "TGFileDialog.h"
#include "TObjString.h"
#include "TGFrame.h"
#include "MyMainFrame.h"
#include "TGTextView.h"
#include "TGMsgBox.h"
#include "TSystem.h"
#include "TKey.h"
#include "TClass.h"
using namespace std;

MyMainFrame::MyMainFrame(const TGWindow* p, UInt_t w, UInt_t h)
	: TGMainFrame(p, w, h),fCanvasFrame(nullptr) {

	TGHorizontalFrame* fHorFrame = new TGHorizontalFrame(this, w, h);
	TGCompositeFrame* cframe2 = new TGCompositeFrame(this, w*0.4, h, kVerticalFrame | kFixedSize);
	TGCompositeFrame* cframe3 = new TGCompositeFrame(this, w*0.5, h, kVerticalFrame | kFixedSize);

	fExportButton = new TGTextButton(cframe2, "&Export");
	cframe2->AddFrame(fExportButton, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsCenterX | kLHintsCenterY, 10, 10, 5, 2));
	fExportButton->Connect("Clicked()", "MyMainFrame", this, "ExportFile()");

	fDrawButton = new TGTextButton(cframe2, "&Draw");
	cframe2->AddFrame(fDrawButton, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsCenterX | kLHintsCenterY, 10, 10, 2, 2));
	fDrawButton->Connect("Clicked()", "MyMainFrame", this, "DrawFile()");

	fExitButton = new TGTextButton(cframe2, "&Exit", "gApplication->Terminate(0)");
	cframe2->AddFrame(fExitButton, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kLHintsCenterX | kLHintsCenterY, 10, 10, 2, 5));

	TGTextView* fText1 = new TGTextView(cframe3, 250, 35, "2024");
	fText1->SetBackgroundColor(0xcccccc);
	cframe3->AddFrame(fText1, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 10, 10, 15, 5));

	TGTextView* fText2 = new TGTextView(cframe3, 250, 35, "The First File:");
	fText2->SetBackgroundColor(0xcccccc);
	cframe3->AddFrame(fText2, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 10, 10, 5, 5));

	fFilename1 = new TGTextView(cframe3, 250, 35, "None");
	cframe3->AddFrame(fFilename1, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 10, 10, 5, 5));

	TGTextView* fText3 = new TGTextView(cframe3, 250, 35, "The Second File:");
	fText3->SetBackgroundColor(0xcccccc);
	cframe3->AddFrame(fText3, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 10, 10, 5, 5));

	fFilename2 = new TGTextView(cframe3, 250, 35, "None");
	cframe3->AddFrame(fFilename2, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 10, 10, 5, 5));

	TGTextView* fText4 = new TGTextView(cframe3, 250, 35, "Current File:");
	fText4->SetBackgroundColor(0xcccccc);
	cframe3->AddFrame(fText4, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 10, 10, 5, 5));

	fFilename3 = new TGTextView(cframe3, 250, 35, "None");
	cframe3->AddFrame(fFilename3, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 10, 10, 5, 5));

	fHorFrame->AddFrame(cframe2, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 20, 20, 20, 20));
	fHorFrame->AddFrame(cframe3, new TGLayoutHints(kLHintsRight | kLHintsCenterY, 20, 20, 20, 20));
	this->AddFrame(fHorFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kFixedSize));

	TGFontPool* pool = gClient->GetFontPool();
	TGFont* font = pool->GetFont("helvetica", 15, kFontWeightNormal, kFontSlantRoman);
	font->Print();
	FontStruct_t ft = font->GetFontStruct();

	fExportButton->SetFont(ft);
	fDrawButton->SetFont(ft);
	fExitButton->SetFont(ft);

	this->MapSubwindows();
	this->SetWMSizeHints(w, h, w, h, 1, 1);
	this->SetWindowName("Experiment Analysis Software Created by CGY");
	this->Resize(this->GetDefaultSize());
	this->MapWindow();
}

MyMainFrame::~MyMainFrame() {
	Cleanup();
}

void MyMainFrame::ExportFile() {
	TGFileInfo fi;
	const char *fileType[] = {"mvme/mvlc listfile archives","*.zip",0,0};
	fi.fFileTypes = fileType;
	new TGFileDialog(gClient->GetDefaultRoot(), this, kFDOpen, &fi);
	if (fi.fMultipleSelection && fi.fFileNamesList) {
		TObjString* el;
		int filecount = 0;
		TIter next(fi.fFileNamesList);
		TString s[100];
		while ((el = (TObjString*)next())) {
			s[filecount] = el->GetString();
			filecount++;
		}
		bool success = true;
		for(int i=0;i<filecount;i++){
			if(!restore_ROOT(string(s[i].Data()))){
				new TGMsgBox(gClient->GetDefaultRoot(),this,"Error", s[i], kMBIconExclamation, kMBOk);
				success = false;
			}
		}
		if(success){
			new TGMsgBox(gClient->GetDefaultRoot(),this,"Success","Extraction complete!", kMBIconExclamation, kMBOk);
		}
	}
	else if (fi.fFilename) {
		if(restore_ROOT(string(fi.fFilename))){
			new TGMsgBox(gClient->GetDefaultRoot(),this,"Success","Extraction complete!", kMBIconExclamation, kMBOk);
		}else{
			new TGMsgBox(gClient->GetDefaultRoot(),this,"Error","Failed to open!", kMBIconExclamation, kMBOk);
			return;
		}
	}
	return;
}

bool MyMainFrame::restore_ROOT(string inputFilename)
{
	return process_one_listfile(inputFilename);
}

void MyMainFrame::DrawFile() {
	if (!fCanvasFrame) {
		cout << "!" << endl;//gClient->GetDefaultRoot()
		fCanvasFrame = new CanvasFrame(gClient->GetDefaultRoot(), 1600, 1000);
		fCanvasFrame->Connect("CloseWindow()", "MyMainFrame", this, "CanvasClosed()");
	}
	else {
		fCanvasFrame->RaiseWindow();
	}
	return;
}

void MyMainFrame::CanvasClosed(){
	if(fCanvasFrame){
		fCanvasFrame->Disconnect("CloseWindow()", "MyMainFrame", this, "CanvasClosed()");
		fCanvasFrame = nullptr;

	}
	return;
}

bool MyMainFrame::ClearFile(){
	//cout<<"Click!"<<endl;
	TString filename = "Modified_RIBLL.root";
	gSystem->Exec(TString::Format("rm -f %s",filename.Data()));
	return true;
}
ClassImp(MyMainFrame)
