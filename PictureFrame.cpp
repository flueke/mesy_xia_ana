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
#include "MyMainFrame.h"
#include "TGTextView.h"
#include "TGMsgBox.h"
#include "TSystem.h"
#include "TKey.h"
#include "TClass.h"
#include "TCanvas.h"
#include "TGTextEntry.h"
#include "TText.h"
using namespace std;
PictureFrame::PictureFrame(const TGWindow* p, UInt_t w, UInt_t h, const char* file) : TGMainFrame(p, w, h) {

	TGVerticalFrame* fVerFrame = new TGVerticalFrame(this, w, h);
	TGCompositeFrame* fHorFrame1 = new TGCompositeFrame(fVerFrame, w*0.4, h*0.04, kHorizontalFrame | kFixedSize);
	TGCompositeFrame* fHorFrame2 = new TGCompositeFrame(fVerFrame, w, h*0.96, kVerticalFrame | kFixedSize);


	prevBranchButton = new TGTextButton(fHorFrame1, "&Prev Branch");
	fHorFrame1->AddFrame(prevBranchButton, new TGLayoutHints( kLHintsExpandX | kLHintsExpandY | kLHintsCenterX | kLHintsCenterY, 5, 5, 5, 5));
	prevBranchButton->Connect("Clicked()", "PictureFrame", this, "PrevBranch()");

	nextBranchButton = new TGTextButton(fHorFrame1, "&Next Branch");
	fHorFrame1->AddFrame(nextBranchButton, new TGLayoutHints( kLHintsExpandX | kLHintsExpandY | kLHintsCenterX | kLHintsCenterY, 5, 5, 5, 5));
	nextBranchButton->Connect("Clicked()", "PictureFrame", this, "NextBranch()");
	
	prevModuleButton = new TGTextButton(fHorFrame1, "&Prev Module");
	fHorFrame1->AddFrame(prevModuleButton, new TGLayoutHints( kLHintsExpandX | kLHintsExpandY | kLHintsCenterX | kLHintsCenterY, 5, 5, 5, 5));
	prevModuleButton->Connect("Clicked()", "PictureFrame", this, "PrevModule()");

	nextModuleButton = new TGTextButton(fHorFrame1, "&Next Module");
	fHorFrame1->AddFrame(nextModuleButton, new TGLayoutHints( kLHintsExpandX | kLHintsExpandY | kLHintsCenterX | kLHintsCenterY, 5, 5, 5, 5));
	nextModuleButton->Connect("Clicked()", "PictureFrame", this, "NextModule()");
	
	ExportPDFButton = new TGTextButton(fHorFrame1, "&Export PDF");
	fHorFrame1->AddFrame(ExportPDFButton, new TGLayoutHints( kLHintsExpandX | kLHintsExpandY | kLHintsCenterX | kLHintsCenterY, 5, 5, 5, 5));
	ExportPDFButton->Connect("Clicked()", "PictureFrame", this, "ExportPDF()");

	embeddedCanvas = new TRootEmbeddedCanvas("EmbeddedCanvas", fHorFrame2, w * 0.95, h * 0.95);
	fHorFrame2->AddFrame(embeddedCanvas, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5, 5, 5, 5));
	//canvas = embeddedCanvas->GetCanvas();
	fVerFrame->AddFrame(fHorFrame1, new TGLayoutHints(kLHintsCenterX | kLHintsTop, 2, 2, 2, 2));
	fVerFrame->AddFrame(fHorFrame2, new TGLayoutHints(kLHintsCenterY | kLHintsBottom, 2, 2, 2, 2));
	this->AddFrame(fVerFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kFixedSize));
	
	TGFontPool* pool = gClient->GetFontPool();
	TGFont* font = pool->GetFont("helvetica", 10, kFontWeightNormal, kFontSlantRoman);
	font->Print();
	FontStruct_t ft = font->GetFontStruct();
	
	prevBranchButton->SetFont(ft);
	nextBranchButton->SetFont(ft);
	prevModuleButton->SetFont(ft);
	nextModuleButton->SetFont(ft);
	ExportPDFButton->SetFont(ft);	
	cout<<"Picture Frame: File"<<file<<endl;
	//cout<<"!!!!!"<<endl;
	//canvas->Clear();
	TFile* source = new TFile(file, "READ");
	cout<<"Picture Frame:"<<file<<endl;
	filename = file;
	fTree = (TTree*)source->Get("Data");
	fTotalBranches = fTree->GetListOfBranches()->GetEntries();
	cout<<"fTotalBranches"<<fTotalBranches<<endl;
	fCurrentBranchIndex = 0;
	fModuleIndex = 0;
	int Value;
	TBranch *modBranch = fTree->GetBranch("Mod");
	modBranch->SetAddress(&Value);
    	for (int i = 0; i < fTotalBranches; ++i) {
        	modBranch->GetEntry(i);
		uniqueMods.insert(Value);
	}
	drawCanvas();
	this->SetCleanup(kDeepCleanup);
	this->MapSubwindows();
	this->SetWMSizeHints(w, h, w, h, 1, 1);
	this->SetWindowName("Histo Window");
	this->Resize(this->GetDefaultSize());
	this->MapWindow();
}

string PictureFrame::ModInfo(int Module) {
	if(Module == 0){
		return "TOF";
	}
	else if(Module == 240){
		return "Si0";
	}
	else if(Module == 80){
		return "Si1_Big";
	}
	else if(Module ==16){
		return "Si1_Small";
	}
	else if(Module == 96){
		return "Si2_Big";
	}
	else if(Module == 32){
		return "Si2_Small";
	}
	else if(Module == 112){
		return "Si3_Big";
	}
	else if(Module == 48){
		return "Si3_Small";
	}
	else if(Module == 128){
		return "Si4_Big";
	}
	else if(Module == 64){
		return "Si4_Small";
	}
	else if(Module == 144){
		return "Si5";
	}
	else if(Module == 160){
		return "Si6";
	}
	else if(Module == 176){
		return "Si7";
	}
	else if(Module == 5){
		return "CsI";
	}
	else if(Module == 192){
		return "HPGe";
	}
	return "Null";
}

void PictureFrame::drawCanvas(){
	canvas = embeddedCanvas->GetCanvas();
	//cout<<"!"<<endl;
	canvas->Clear();
	//cout<<"!"<<endl;
	auto mod = uniqueMods.begin();
	//cout<<"!"<<endl;
	advance(mod, fModuleIndex);
	//cout<<"mod:"<<*mod<<endl;
	//cout<<"!"<<endl;
	TBranch* branch = dynamic_cast<TBranch*>(fTree->GetListOfBranches()->At(fCurrentBranchIndex));
	const char* branchName = branch->GetName();
	//cout<<"!"<<endl;
	
	canvas->Divide(8, 4);
	
	for (int i = 1; i <= 32; i++) {
		canvas->cd(i);
		if(std::string(branchName) == "Mod"){
			string condition = "Ch == " + to_string(i-1);
			fTree->Draw((std::string(branchName) + ">>" + std::string(branchName) + std::to_string(i-1)).c_str(), condition.c_str(), "goff");
			TH1F* hist = (TH1F*)gDirectory->Get((std::string(branchName) + std::to_string(i-1)).c_str());
			if (hist->GetEntries() == 0) {
				TText *text = new TText(0.5, 0.5,"Empty");
				text->SetTextSize(0.1);
				text->Draw();
			}
			else{
				fTree->Draw((std::string(branchName) + ">>" + std::string(branchName) + std::to_string(i-1)).c_str(), condition.c_str(), "");
			}
		}else if(std::string(branchName) == "Ch"){
			string condition = "Mod == "+ to_string(*mod);
			fTree->Draw((std::string(branchName) + ">>" + std::string(branchName) + " " + ModInfo(fModuleIndex)).c_str(), "", "goff");
			TH1F* hist = (TH1F*)gDirectory->Get((std::string(branchName) + " " + ModInfo(fModuleIndex)).c_str());
			if (hist->GetEntries() == 0) {
				TText *text = new TText(0.5, 0.5,"Empty");
				text->SetTextSize(0.1);
				text->Draw();
			}
			else{
				fTree->Draw((std::string(branchName) + ">>" + std::string(branchName) + " " + ModInfo(fModuleIndex)).c_str(), condition.c_str(), "");
			}
			break;
		}
		else {
			string condition = "Ch == " + to_string(i-1) + " && Mod == " + to_string(*mod);
 			fTree->Draw((std::string(branchName) + ">>" + std::string(branchName) + std::to_string(fModuleIndex) + std::to_string(i-1) + " " + ModInfo(fModuleIndex)).c_str(), condition.c_str(), "goff");
			TH1F* hist = (TH1F*)gDirectory->Get((std::string(branchName) + std::to_string(fModuleIndex) + std::to_string(i-1) + " " + ModInfo(fModuleIndex)).c_str());
			if (hist->GetEntries() == 0) {
				TText *text = new TText(0.5, 0.5,"Empty");
				text->SetTextSize(0.1);
				text->Draw();
			}
			else{
				if(std::string(branchName) != "TDC"){
					fTree->Draw((std::string(branchName) + ">>" + std::string(branchName) + std::to_string(fModuleIndex) + std::to_string(i-1) + " " + ModInfo(fModuleIndex)).c_str(), condition.c_str(), "");
				}
				else{
					fTree->Draw(("("+std::string(branchName) +" * 62.5 + CFD * 0.024)" + ">>" + std::string(branchName) + std::to_string(fModuleIndex) + std::to_string(i-1) + " " + ModInfo(fModuleIndex)).c_str(), condition.c_str(), "");
				}
			}
		}
		//canvas->Draw();
	}

	canvas->Update();
	return;
}

void PictureFrame::PrevBranch() {
	fCurrentBranchIndex = (fCurrentBranchIndex - 1 + fTotalBranches) % fTotalBranches;
	drawCanvas();
	return;
}

void PictureFrame::NextBranch() {
	fCurrentBranchIndex = (fCurrentBranchIndex + 1) % fTotalBranches;
	drawCanvas();
	return;
}

void PictureFrame::PrevModule() {
	fModuleIndex = (fModuleIndex -1 + uniqueMods.size()) % uniqueMods.size();
	drawCanvas();
	return;
}

void PictureFrame::NextModule() {
	fModuleIndex = (fModuleIndex + 1) % uniqueMods.size();
	drawCanvas();
	return;
}

void PictureFrame::ExportPDF(){
	int k=0;
	fCurrentBranchIndex = 0;
	
	string root_name = filename.Data();
	size_t lastSlashPos = root_name.find_last_of("/\\");
	size_t lastDotPos = root_name.find_last_of('.');
	root_name = root_name.substr(lastSlashPos + 1,lastDotPos-lastSlashPos-1);
	
	while(fCurrentBranchIndex<fTotalBranches){
		fModuleIndex = 0;
		cout<<"uniqueMods.size()"<<uniqueMods.size()<<endl;
		while(fModuleIndex<(int)uniqueMods.size()){
			drawCanvas();
			cout<<"k"<<k<<endl;
			if(k==0){
				canvas->Print((root_name+"_32.pdf(").data(), ("Title:"+std::to_string(k)).c_str());
				cout<<"Done1"<<endl;
			}else if(k == (int)(fTotalBranches* uniqueMods.size()-1)){
				canvas->Print((root_name+"_32.pdf)").data(), ("Title:"+std::to_string(k)).c_str());
				cout<<"Done"<<endl;
			}else{
				canvas->Print((root_name+"_32.pdf").data(), ("Title:"+std::to_string(k)).c_str());
				cout<<"Done2"<<endl;
			}
			k++;
			fModuleIndex++;
		}
		fCurrentBranchIndex++;
	}
	fCurrentBranchIndex = 0;
	fModuleIndex = 0;
	drawCanvas();
	return;
}

PictureFrame::~PictureFrame() {
	cout << "Close!!!" << endl;
	Cleanup();
}

void PictureFrame::CloseWindow() {
	Cleanup();
	DeleteWindow();
}

ClassImp(PictureFrame)
