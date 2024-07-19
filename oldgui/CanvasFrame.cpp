#include "TH2D.h"
#include "TCut.h"
#include <TMath.h>
#include <fstream>
#include <map>
#include <set>
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
#include "TROOT.h"
#include "TPolyLine.h"
#include "TPoint.h"
#include "TCutG.h"
#include "TLine.h"
#include "TLeaf.h"
#include "TGStatusBar.h"
using namespace std;
CanvasFrame::CanvasFrame(const TGWindow* p, UInt_t w, UInt_t h)
	: TGMainFrame(p, w, h), fPictureFrame(nullptr), fTotalBranches(0), fCurrentBranchIndex(0), file(nullptr), x_name(nullptr), y_name(nullptr), 
	prev_x(1), prev_y(1), mod(0), x_bin(100), x_min(0), x_max(100), y_bin(0), y_min(100), y_max(100), Module(256), Channel(32), select(false),cut_flag(false),drawLine(false){
		
	TGHorizontalFrame* fHorFrame = new TGHorizontalFrame(this, w, h);
	TGCompositeFrame* cframe2 = new TGCompositeFrame(fHorFrame, w*0.2, h, kVerticalFrame | kFixedSize);
	TGCompositeFrame* cframe3 = new TGCompositeFrame(fHorFrame, w * 0.78, h, kVerticalFrame | kFixedSize);
	
	embeddedCanvas = new TRootEmbeddedCanvas("EmbeddedCanvas", cframe3,w*0.75,h*0.9);
	statusBar = new TGStatusBar(cframe3, w*0.75, h*0.08);
	Int_t parts[] = {100, 100, 100};
    	statusBar->SetParts(parts, 3);
	//statusBar->SetText("Status Bar", 0);
	cframe3->AddFrame(embeddedCanvas, new TGLayoutHints( kLHintsCenterX | kLHintsCenterY, 2, 2, 2, 2));
	cframe3->AddFrame(statusBar, new TGLayoutHints( kLHintsCenterX | kLHintsCenterY, 2, 2, 2, 2));
	
	canvas = embeddedCanvas->GetCanvas();
	canvas->Connect("RangeChanged()", "CanvasFrame", this, "HistoEventHandle()");
	canvas->Connect("ProcessedEvent(Int_t, Int_t, Int_t, TObject*)", "CanvasFrame", this, "DrawCut(Int_t, Int_t, Int_t, TObject*)");
	canvas->Connect("ProcessedEvent(Int_t, Int_t, Int_t, TObject*)", "CanvasFrame", this, "UpdateStatusBar(Int_t, Int_t, Int_t, TObject*)");

	
	fText2 = new TGTextView(cframe2, 250, 25, "Current File:");
	fText2->SetBackgroundColor(0xcccccc);
	cframe2->AddFrame(fText2, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 5, 5));
	
	fFilename1 = new TGTextView(cframe2, 250, 35, "None");
	cframe2->AddFrame(fFilename1, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 5, 5));
	
	fText3 = new TGTextView(cframe2, 250, 25, "Current Mode:");
	fText3->SetBackgroundColor(0xcccccc);
	cframe2->AddFrame(fText3, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 5, 5));
	
	fFilename2 = new TGTextView(cframe2, 250, 25, "Histo 1D");
	cframe2->AddFrame(fFilename2, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 5, 5));

	fModeButton = new TGTextButton(cframe2, "&Change MODE");
	cframe2->AddFrame(fModeButton, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 5, 5));
	fModeButton->Connect("Clicked()", "CanvasFrame", this, "changeMode()");
	
	fSelectButton = new TGTextButton(cframe2, "&Select");
	cframe2->AddFrame(fSelectButton, new TGLayoutHints(kLHintsExpandX  | kLHintsCenterX, 2, 2, 5, 5));
	fSelectButton->Connect("Clicked()", "CanvasFrame", this, "SelectFile()");
	
	TGHorizontalFrame* histo_chart = new TGHorizontalFrame(cframe2, 250, 30);
	histo_x = new TGComboBox(histo_chart);
	histo_x->Resize(120,30);
	histo_x->Connect("Selected(Int_t)", "CanvasFrame", this, "SelectItemX()");
	histo_y = new TGComboBox(histo_chart);
	histo_y->Resize(120,30);
	histo_y->Connect("Selected(Int_t)", "CanvasFrame", this, "SelectItemY()");
	histo_chart->AddFrame(histo_x, new TGLayoutHints(kLHintsExpandX  |kLHintsCenterX, 2, 2, 5, 5));
	histo_chart->AddFrame(histo_y, new TGLayoutHints(kLHintsExpandX  |kLHintsCenterX, 2, 2, 5, 5));
	cframe2->AddFrame(histo_chart, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 5, 5));
	
	TGHorizontalFrame* fHorFrame1 = new TGHorizontalFrame(cframe2, 250, 25);
	fText5 = new TGTextView(fHorFrame1, 100, 25, "X| Bin:");
	fHorFrame1->AddFrame(fText5, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

	fTextEntry1 = new TGTextEntry(fHorFrame1);
	fTextEntry1->SetDefaultSize(200, 25);
	fHorFrame1->AddFrame(fTextEntry1, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));
	fTextEntry1->SetText("100");
	fTextEntry1->Connect("ReturnPressed()", "CanvasFrame", this, "OnReturnProcessed()");
	
	TGHorizontalFrame* fHorFrame2 = new TGHorizontalFrame(cframe2, 250, 25);
	fText6 = new TGTextView(fHorFrame2, 100, 25, "X| Min:");
	fHorFrame2->AddFrame(fText6, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

	fTextEntry2 = new TGTextEntry(fHorFrame2);
	fTextEntry2->SetDefaultSize(200, 25);
	fHorFrame2->AddFrame(fTextEntry2, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));
	fTextEntry2->SetText("0");
	fTextEntry2->Connect("ReturnPressed()", "CanvasFrame", this, "OnReturnProcessed()");
	
	TGHorizontalFrame* fHorFrame3 = new TGHorizontalFrame(cframe2, 250, 25);
	fText7 = new TGTextView(fHorFrame3, 100, 25, "X| Max:");
	fHorFrame3->AddFrame(fText7, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

	fTextEntry3 = new TGTextEntry(fHorFrame3);
	fTextEntry3->SetDefaultSize(200, 25);
	fHorFrame3->AddFrame(fTextEntry3, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));
	fTextEntry3->SetText("100");
	fTextEntry3->Connect("ReturnPressed()", "CanvasFrame", this, "OnReturnProcessed()");
	
	cframe2->AddFrame(fHorFrame1, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 2, 2));
	cframe2->AddFrame(fHorFrame2, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 2, 2));
	cframe2->AddFrame(fHorFrame3, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 2, 2));

	TGHorizontalFrame* fHorFrame4 = new TGHorizontalFrame(cframe2, 250, 25);
	fText8 = new TGTextView(fHorFrame4, 100, 25, "Y| Bin:");
	fHorFrame4->AddFrame(fText8, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

	fTextEntry4 = new TGTextEntry(fHorFrame4);
	fTextEntry4->SetDefaultSize(200, 25);
	fHorFrame4->AddFrame(fTextEntry4, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));
	fTextEntry4->SetText("100");
	fTextEntry4->Connect("ReturnPressed()", "CanvasFrame", this, "OnReturnProcessed()");
	
	TGHorizontalFrame* fHorFrame5 = new TGHorizontalFrame(cframe2, 250, 25);
	fText9 = new TGTextView(fHorFrame5, 100, 30, "Y| Min:");
	fHorFrame5->AddFrame(fText9, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

	fTextEntry5 = new TGTextEntry(fHorFrame5);
	fTextEntry5->SetDefaultSize(200, 25);
	fHorFrame5->AddFrame(fTextEntry5, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));
	fTextEntry5->SetText("0");
	fTextEntry5->Connect("ReturnPressed()", "CanvasFrame", this, "OnReturnProcessed()");
	
	TGHorizontalFrame* fHorFrame6 = new TGHorizontalFrame(cframe2, 250, 25);
	fText10 = new TGTextView(fHorFrame6, 100, 25, "Y| Max:");
	fHorFrame6->AddFrame(fText10, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

	fTextEntry6 = new TGTextEntry(fHorFrame6);
	fTextEntry6->SetDefaultSize(200, 25);
	fHorFrame6->AddFrame(fTextEntry6, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));
	fTextEntry6->SetText("100");
	fTextEntry6->Connect("ReturnPressed()", "CanvasFrame", this, "OnReturnProcessed()");

	cframe2->AddFrame(fHorFrame4, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 2, 2));
	cframe2->AddFrame(fHorFrame5, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 2, 2));
	cframe2->AddFrame(fHorFrame6, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 2, 2));
	
	TGHorizontalFrame* fHorFrame7 = new TGHorizontalFrame(cframe2, 250, 25);
	fText11 = new TGTextView(fHorFrame7, 100, 25, "Module:");
	fHorFrame7->AddFrame(fText11, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

	fTextEntry7 = new TGTextEntry(fHorFrame7);
	fTextEntry7->SetDefaultSize(200, 25);
	fHorFrame7->AddFrame(fTextEntry7, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));
	fTextEntry7->SetText("256");
	fTextEntry7->Connect("ReturnPressed()", "CanvasFrame", this, "OnReturnProcessed()");
	
	TGHorizontalFrame* fHorFrame8 = new TGHorizontalFrame(cframe2, 250, 25);
	fText12 = new TGTextView(fHorFrame8, 100, 25, "Channel:");
	fHorFrame8->AddFrame(fText12, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));

	fTextEntry8 = new TGTextEntry(fHorFrame8);
	fTextEntry8->SetDefaultSize(200, 25);
	fHorFrame8->AddFrame(fTextEntry8, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));
	fTextEntry8->SetText("32");
	fTextEntry8->Connect("ReturnPressed()", "CanvasFrame", this, "OnReturnProcessed()");
	
	cframe2->AddFrame(fHorFrame7, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 2, 2));
	cframe2->AddFrame(fHorFrame8, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 2, 2));
	
	fPrevBranchButton = new TGTextButton(cframe2, "&Prev Branch");
	cframe2->AddFrame(fPrevBranchButton, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 4, 4));
	fPrevBranchButton->Connect("Clicked()", "CanvasFrame", this, "PrevBranch()");
	
	fNextBranchButton = new TGTextButton(cframe2, "&Next Branch");
	cframe2->AddFrame(fNextBranchButton, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 4, 4));
	fNextBranchButton->Connect("Clicked()", "CanvasFrame", this, "NextBranch()");
	
	fPrevModuleButton = new TGTextButton(cframe2, "&Prev Module");
	cframe2->AddFrame(fPrevModuleButton, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 4, 4));
	fPrevModuleButton->Connect("Clicked()", "CanvasFrame", this, "PrevModule()");
	
	fNextModuleButton = new TGTextButton(cframe2, "&Next Module");
	cframe2->AddFrame(fNextModuleButton, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 4, 4));
	fNextModuleButton->Connect("Clicked()", "CanvasFrame", this, "NextModule()");
	
	fPrevChannelButton = new TGTextButton(cframe2, "&Prev Channel");
	cframe2->AddFrame(fPrevChannelButton, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 4, 4));
	fPrevChannelButton->Connect("Clicked()", "CanvasFrame", this, "PrevChannel()");
	
	fNextChannelButton = new TGTextButton(cframe2, "&Next Channel");
	cframe2->AddFrame(fNextChannelButton, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 4, 4));
	fNextChannelButton->Connect("Clicked()", "CanvasFrame", this, "NextChannel()");
	
	fDrawHisto1DButton = new TGTextButton(cframe2, "&Draw1DHisto");
	cframe2->AddFrame(fDrawHisto1DButton, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 4, 4));
	fDrawHisto1DButton->Connect("Clicked()", "CanvasFrame", this, "DrawHisto1D()");
	
	fDrawHisto2DButton = new TGTextButton(cframe2, "&Draw2DHisto");
	cframe2->AddFrame(fDrawHisto2DButton, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 4, 4));
	fDrawHisto2DButton->Connect("Clicked()", "CanvasFrame", this, "DrawHisto2D()");
	
	fTempButton = new TGTextButton(cframe2, "&Graphical Cut");
	cframe2->AddFrame(fTempButton, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 4, 4));
	fTempButton->Connect("Clicked()", "CanvasFrame", this, "CutCanvas()");
	
	
	fDrawHistoButton = new TGTextButton(cframe2, "&HistoOverview");
	cframe2->AddFrame(fDrawHistoButton, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 4, 4));
	fDrawHistoButton->Connect("Clicked()", "CanvasFrame", this, "DrawFile()");
	
	fPDFButton = new TGTextButton(cframe2, "&PDF");
	cframe2->AddFrame(fPDFButton, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 4, 4));
	fPDFButton->Connect("Clicked()", "CanvasFrame", this, "PDF()");

	fResetButton = new TGTextButton(cframe2, "&Reset");
	cframe2->AddFrame(fResetButton, new TGLayoutHints(kLHintsExpandX | kLHintsCenterX, 2, 2, 4, 4));
	fResetButton->Connect("Clicked()", "CanvasFrame", this, "Reset()");

	fHorFrame->AddFrame(cframe2, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 5, 20, 20));
	fHorFrame->AddFrame(cframe3, new TGLayoutHints(kLHintsRight | kLHintsCenterY, 20, 20, 20, 20));
	this->AddFrame(fHorFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY | kFixedSize));
	
	Reset();
	
	TGFontPool* pool = gClient->GetFontPool();
	TGFont* font = pool->GetFont("helvetica", 10, kFontWeightNormal, kFontSlantRoman);
	font->Print();
	FontStruct_t ft = font->GetFontStruct();
	
	fSelectButton->SetFont(ft);
	fPrevBranchButton->SetFont(ft);
	fNextBranchButton->SetFont(ft);
	fResetButton->SetFont(ft);
	fModeButton->SetFont(ft);
	fDrawHisto1DButton->SetFont(ft);
	fDrawHisto2DButton->SetFont(ft);
	fPrevChannelButton->SetFont(ft);
	fNextChannelButton->SetFont(ft);
	fPrevModuleButton->SetFont(ft);
	fNextModuleButton->SetFont(ft);
	fPDFButton->SetFont(ft);
	fDrawHistoButton->SetFont(ft);
	fTempButton->SetFont(ft);
	
	this->SetCleanup(kDeepCleanup);
	this->MapSubwindows();
	this->SetWMSizeHints(w, h, w, h, 1, 1);
	this->SetWindowName("Histo Window");
	this->Resize(this->GetDefaultSize());
	this->MapWindow();
}

void CanvasFrame::UpdateStatusBar(Int_t event, Int_t x, Int_t y, TObject *selected) {
    TH1F *histo1 = nullptr;
    TH2F *histo2 = nullptr;
    TList *listOfPrimitives = canvas->GetListOfPrimitives();
    TObject *obj = nullptr;
    TIter next(listOfPrimitives);
    while ((obj = next())) {
        if (obj->InheritsFrom(TH1F::Class())) {
        	histo1 = dynamic_cast<TH1F*>(obj);
        	break;
        } 
        else if (obj->InheritsFrom(TH2F::Class())) {
        	histo2 = dynamic_cast<TH2F*>(obj);
        	break;
        }
    }
    if (event == kMouseMotion) {
        statusBar->SetText(Form("%d, %d", x, y), 0);
        if(mod == 0 && cut_flag == false && select == true){
        	//cout<<"!!!!"<<endl;
        	if(histo1){
        		Double_t x_axis = canvas->AbsPixeltoX(x);
        		Int_t bin = histo1->GetXaxis()->FindBin(canvas->AbsPixeltoX(x));
       		Double_t content = histo1->GetBinContent(bin);
       		statusBar->SetText(Form("%d, %d, x = %.4f, count_x = %.4f, x_bin = %d", x, y, x_axis, content, bin), 0);
        	}
        }else if (mod == 1 && cut_flag == false && select == true){
        	if(histo2){
        		Double_t x_axis = canvas->AbsPixeltoX(x);
        		Double_t y_axis = canvas->AbsPixeltoY(y);
        		Int_t bin_x = histo2->GetXaxis()->FindBin(canvas->AbsPixeltoX(x));
        		Int_t bin_y = histo2->GetYaxis()->FindBin(canvas->AbsPixeltoY(y));
       		Double_t content = histo2->GetBinContent(bin_x, bin_y);
       		statusBar->SetText(Form("%d, %d, x = %.4f, y = %.4f, count = %.4f, x_bin = %d, y_bin = %d", x, y, x_axis, y_axis, content, bin_x, bin_y), 0);
        	}
        }
    } else {
        statusBar->SetText("", 0);
    }
    return;
}



CanvasFrame::~CanvasFrame() {
	cout<<"Close!!!"<<endl;
	Cleanup();
}

void CanvasFrame::CloseWindow() {
	Cleanup();
	//this->CloseWindow();
	DeleteWindow();
}


void CanvasFrame::CutCanvas(){
	cut_flag = true;
	drawLine = false;
	CutDone = false;
	cut_count = 0;
	x_point.clear();
	y_point.clear();
}

void CanvasFrame::DrawCut(Int_t event, Int_t x, Int_t y, TObject *obj){
	if(cut_flag){
		if(event == kButton1Down){
			if(cut_count == 0){
				cutG = new TCutG("cutG");
				cutG->SetLineColor(kRed);
				cutG->SetFillStyle(3001);
				line = nullptr;
				drawLine = true;
				line = new TLine();
			}
			cout<<"Int_t x "<<x<<" Int_t y "<<y<<endl;
			cout<<"canvas->AbsPixeltoX(x) "<<canvas->AbsPixeltoX(x)<<endl;
			cout<<"canvas->AbsPixeltoY(y) "<<canvas->AbsPixeltoY(y)<<endl;
			cutG->SetPoint(cutG->GetN(), canvas->AbsPixeltoX(x), canvas->AbsPixeltoY(y));
			cutG->Draw("same");
			x_point.push_back(x);
			y_point.push_back(y);
			cut_count++;
			//cout<<"Click"<<endl;
			//cutG = new TCutG("cutG",cut_count);
		}else if (event == kButton1Double){
			cut_flag = false;
			drawLine = false;
			//cutG->SetPoint(cutG->GetN(), cutG->GetX()[cutG->GetN()-1], cutG->GetY()[cutG->GetN()-1]);
			cutG->SetPoint(cutG->GetN(), cutG->GetX()[0], cutG->GetY()[0]);
			cutG->Draw("same");
			delete line;
			line = nullptr;
			cut_count = 0;
			x_point.clear();
			y_point.clear();
			cut_count = 0;
			CutDone = true;
			cout<<"TCut Done"<<endl;
			canvas->Modified();
			canvas->Update();
			OpenTheWindow(cutG);
		}else if(event == kMouseMotion && drawLine){
			//cout<<"!!!!!!!!!!!!!!!"<<endl;
			line->SetLineColor(kRed);
			//line->SetX1(0);
			//line->SetY1(0);
			line->SetX1(canvas->AbsPixeltoX(x_point[cut_count-1]));
			line->SetY1(canvas->AbsPixeltoY(y_point[cut_count-1]));
			line->SetX2(canvas->AbsPixeltoX(x));
			line->SetY2(canvas->AbsPixeltoY(y));
			line->Draw();
			canvas->Modified();
			canvas->Update();
		}
	}
	return;
}

void CanvasFrame::OpenTheWindow(TCutG* cutG){
	cutG->Print();
	TTree *outputTree = new TTree("Data","Particle data");
	cout<<"io"<<endl;
	TGText* FileName1 = fFilename1->GetText();
	TString textContent = FileName1->AsString();
	textContent.Strip(TString::kBoth);
	cout<<"Begin to transform:"<<textContent.Data()<<endl;
	Double_t x_value, y_value;
	UInt_t x_value_Int, y_value_Int;
	ULong64_t x_value_Long, y_value_Long;
	TFile* source = new TFile(textContent.Data(),"READ");
	TTree* tree = (TTree*)source->Get("Data");
	cout<<"io"<<endl;
	TBranch* branchX = tree->GetBranch(x_name);
	TBranch* branchY = tree->GetBranch(y_name);
	cout<<"io"<<endl;
	cout<<"io"<<endl;
	cout<<x_name<<" "<<y_name<<endl;
	if(branchX->GetLeaf(branchX->GetListOfLeaves()->At(0)->GetName())){
		cout<<"Not Null"<<endl;
	}
	cout<<branchX->GetListOfLeaves()->At(0)->GetName()<<endl;
	cout<<branchX->GetLeaf(branchX->GetListOfLeaves()->At(0)->GetName())->GetTypeName()<<endl;
	//cout<<leafX->GetTypeName()<<endl;
	int Type1 = 0;
	int Type2 = 0;
	if(branchX->GetLeaf(branchX->GetListOfLeaves()->At(0)->GetName())->GetTypeName()==TString("Double_t")){
		tree->SetBranchAddress(x_name, &x_value);
	} else if(branchX->GetLeaf(branchX->GetListOfLeaves()->At(0)->GetName())->GetTypeName()==TString("UInt_t")){
		tree->SetBranchAddress(x_name, &x_value_Int);
		//x_value = static_cast<Double_t>(x_value_Int);
		Type1 = 1;
	} else if(branchX->GetLeaf(branchX->GetListOfLeaves()->At(0)->GetName())->GetTypeName()==TString("ULong64_t")){
		tree->SetBranchAddress(x_name, &x_value_Long);
		//x_value = static_cast<Double_t>(x_value_Long);
		Type1 = 2;
	}
	cout<<"io"<<endl;
	string x = x_name;
	string y = y_name;
	if(x != y){
		cout<<branchY->GetListOfLeaves()->At(0)->GetName()<<endl;
		cout<<branchY->GetLeaf(branchY->GetListOfLeaves()->At(0)->GetName())->GetTypeName()<<endl;
		if(branchY->GetLeaf(branchY->GetListOfLeaves()->At(0)->GetName())->GetTypeName()==TString("Double_t")){
			tree->SetBranchAddress(y_name, &y_value);
		} else if(branchY->GetLeaf(branchY->GetListOfLeaves()->At(0)->GetName())->GetTypeName()==TString("UInt_t")){
			tree->SetBranchAddress(y_name, &y_value_Int);
			//y_value = static_cast<Double_t>(y_value_Int);
			Type2 = 1;
		} else if(branchY->GetLeaf(branchY->GetListOfLeaves()->At(0)->GetName())->GetTypeName()==TString("ULong64_t")){
			tree->SetBranchAddress(y_name, &y_value_Long);
			//y_value = static_cast<Double_t>(y_value_Long);
			Type2 = 2;
		}
	}
	cout<<"io"<<endl;
	TObjArray *branchlist = tree->GetListOfBranches();
	TIter next(branchlist);
	while(TObject *obj = next()){
		TBranch *branch = (TBranch*) obj;
		outputTree->Branch(branch->GetName(),branch->GetAddress(),branch->GetTitle());
	}
	cout<<"io"<<endl;
	ULong64_t Number = tree->GetEntries();
	for(ULong64_t i = 0; i < Number; i++){
		tree->GetEntry(i);
		if(Type1 ==1){
			x_value = static_cast<Double_t>(x_value_Int);
		}else if(Type1 ==2){
			x_value = static_cast<Double_t>(x_value_Long);
		}
		if(x == y){
			cout<<"!!"<<endl;
			y_value = x_value;
		}else{
			if(Type2 == 1){
				y_value = static_cast<Double_t>(y_value_Int);
			}else if(Type2 ==2){
				y_value = static_cast<Double_t>(y_value_Long);
			}
		}
		//cout<<x_value_Int<<" "<<y_value_Int<<endl;
		//cout<<x_value<<" "<<y_value<<endl;
		if(cutG->IsInside(y_value, x_value)){
		//if(cutG->IsInside(x_value, y_value)){
			cout<<"!"<<endl;
			cout<<x_value<<" "<<y_value<<endl;
			outputTree->Fill();
		}
	}
	TFile* output_TCut_file = new TFile("temp_TCut.root","recreate");
	outputTree->SetDirectory(output_TCut_file);
	output_TCut_file->cd();
	outputTree->Write();
	output_TCut_file->Write();
	output_TCut_file->Close();
	return;
}

bool CanvasFrame::IsInteger(const char* text) {
    if (text == nullptr) {
        return false;
    }
    size_t length = strlen(text);
    if (length == 0) {
        return false;
    }
    size_t i = 0;
    if (text[0] == '-') {
        if (length == 1) {
            return false;
        }
        i = 1;
    }
    for (; i < length; ++i) {
        if (text[i] < '0' || text[i] > '9') {
            return false;
        }
    }
    TString textString = text;
    Long64_t number = textString.Atoll();
    return true;
}

bool CanvasFrame::IsNonNegativeInteger(const char* text) {
	if (text == nullptr) {
		return false;
	}
	size_t length = strlen(text);
	if (length == 0) {
		return false;
	}
	for (size_t i = 0; i < length; ++i) {
		if (text[i] < '0' || text[i] > '9') {
			return false;
		}
	}
	TString textString = text;
	Long64_t number = textString.Atoll();
	return number >= 0;
}

bool CanvasFrame::IsNonNegativeDouble(const char* text) {
	if (text == nullptr) {
		return false;
	}
	double number = strtod(text, nullptr);
	return number >= 0;
}

void CanvasFrame::SelectFile() {
	TGFileInfo fi;
	const char* fileType[] = { "ROOT files","*.root",0,0 };
	fi.fFileTypes = fileType;
	new TGFileDialog(gClient->GetDefaultRoot(), this, kFDOpen, &fi);
	if (fi.fMultipleSelection && fi.fFileNamesList) {
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Please Select one files!", kMBIconExclamation, kMBOk);
		return;
	}
	else if (fi.fFilename) {
		file = fi.fFilename;
		TGText* FileName1 = fFilename1->GetText();
		FileName1->Clear();
		TGLongPosition* pos = new TGLongPosition(0, 0);
		FileName1->InsText(*pos, file);
		fFilename1->Update();
		if(mod == 1){
			TFile* source = new TFile(file, "READ");
			TTree* fTree = (TTree*)source->Get("Data");
			fTotalBranches = fTree->GetListOfBranches()->GetEntries();
			UpdateTGComboBox(fTree);
			fTextEntry4->SetEnabled(true);
			fTextEntry5->SetEnabled(true);
			fTextEntry6->SetEnabled(true);
			fTextEntry7->SetEnabled(true);
			fTextEntry8->SetEnabled(true);
			fSelectButton->SetEnabled(false);
			fModeButton->SetEnabled(false);
			fPDFButton->SetEnabled(false);
			fResetButton->SetEnabled(true);
			fDrawHisto1DButton->SetEnabled(false);
			fDrawHisto2DButton->SetEnabled(true);
			histo_x->SetEnabled(true);
			histo_y->SetEnabled(true);		
		}else{
			fTextEntry4->SetEnabled(false);
			fTextEntry5->SetEnabled(false);
			fTextEntry6->SetEnabled(false);
			fTextEntry7->SetEnabled(true);
			fTextEntry8->SetEnabled(true);
			fSelectButton->SetEnabled(false);
			fModeButton->SetEnabled(false);
			fPDFButton->SetEnabled(true);
			fResetButton->SetEnabled(true);
			fDrawHisto1DButton->SetEnabled(true);
			fDrawHisto2DButton->SetEnabled(false);
			histo_x->SetEnabled(false);
			histo_y->SetEnabled(false);
		}
		fDrawHistoButton->SetEnabled(true);
		fTextEntry1->SetEnabled(true);
		fTextEntry2->SetEnabled(true);
		fTextEntry3->SetEnabled(true);
		fPrevBranchButton->SetEnabled(false);
		fNextBranchButton->SetEnabled(false);
		fPrevChannelButton->SetEnabled(true);
		fNextChannelButton->SetEnabled(true);
		fPrevModuleButton->SetEnabled(true);
		fNextModuleButton->SetEnabled(true);
		select = true;
	}
	return;
}

string CanvasFrame::ModInfo(int Module) {
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

void CanvasFrame::DrawHisto1D() {
	const size_t max_int_length = std::to_string(std::numeric_limits<int>::max()).size();
	const size_t max_Long64_t_length = std::to_string(std::numeric_limits<Long64_t>::max()).size();
	TString xmin = fTextEntry2->GetText();
	TString xmax = fTextEntry3->GetText();
	if(strlen(fTextEntry1->GetText()) > max_int_length || strlen(xmin) > max_Long64_t_length || strlen(xmax) > max_Long64_t_length
	|| strlen(fTextEntry7->GetText()) > 3 || strlen(fTextEntry8->GetText()) > 2){
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Invalid Input!", kMBIconExclamation, kMBOk);
		return;
	}
	if ((fTextEntry1->GetText() >= std::to_string(std::numeric_limits<int>::max()) && strlen(fTextEntry1->GetText()) == max_int_length )
	|| (xmin >= std::to_string(std::numeric_limits<Long64_t>::max()) && strlen(xmin) == max_Long64_t_length )
	|| (xmax >= std::to_string(std::numeric_limits<Long64_t>::max()) && strlen(xmax) == max_Long64_t_length )
	|| xmin.Atoll() > xmax.Atoll()){
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Invalid Input!", kMBIconExclamation, kMBOk);
		return;
	}
	if (!(IsNonNegativeInteger(fTextEntry1->GetText()) && IsInteger(fTextEntry2->GetText()) && IsInteger(fTextEntry3->GetText())
	   && IsNonNegativeInteger(fTextEntry7->GetText()) && IsNonNegativeInteger(fTextEntry8->GetText()))) {
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Invalid Input!", kMBIconExclamation, kMBOk);
		return;
	}
	if (mod == 1) {
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Only For 1-D Histo!", kMBIconExclamation, kMBOk);
		return;
	}			
	if (select) {
		Module = std::atoi(fTextEntry7->GetText());
		Channel = std::atoi(fTextEntry8->GetText());
		canvas->Clear();
		TGText* FileName1 = fFilename1->GetText();
		TString textContent = FileName1->AsString();
		//file = textContent.Data();
		textContent.Strip(TString::kBoth);
		TFile* source = new TFile(textContent.Data(), "READ");
		//fCurrentBranchIndex = 0;
		TTree* fTree = (TTree*)source->Get("Data");
		fTotalBranches = fTree->GetListOfBranches()->GetEntries();
		//cout<<"Success!"<<endl;
		if (fCurrentBranchIndex >= 0 && fCurrentBranchIndex < fTotalBranches) {
			TBranch* branch = dynamic_cast<TBranch*>(fTree->GetListOfBranches()->At(fCurrentBranchIndex));
			const char* branchName = branch->GetName();
			TH1F* histogram;
			if (Module == 256 && Channel == 32) {
				histogram = new TH1F(Form("hist_%s", branchName), Form("Histogram of %s", branchName), std::atoi(fTextEntry1->GetText()), 
				xmin.Atoll(), xmax.Atoll());
				if(std::string(branchName) == "TDC"){
					//TString expression = Form("(%s * 62.5 + CFD * 0.024) / 500000", branchName);
					TString expression = Form("%s * 62.5 + CFD * 0.024", branchName);
					fTree->Project(histogram->GetName(), expression.Data());
				}else{
					fTree->Project(histogram->GetName(), branchName);
				}
			}else if (Module != 256 && Channel == 32) {
				if(std::string(branchName) == "Mod") {
					histogram = new TH1F(Form("hist_%s", branchName), Form("Histogram of %s", branchName), std::atoi(fTextEntry1->GetText()), 
					xmin.Atoll(), xmax.Atoll());
					fTree->Project(histogram->GetName(), branchName);
				}else{
					histogram = new TH1F(Form("hist_%s", branchName), Form(("Histogram of %s (Mod==%d) "+ModInfo(Module)).c_str(), branchName, Module), std::atoi(fTextEntry1->GetText()), 
					//histogram = new TH1F(Form("hist_%s", branchName), Form("Histogram of %s (Mod==%d)", branchName, Module), std::atoi(fTextEntry1->GetText()), 
					xmin.Atoll(), xmax.Atoll());
					TString condition = Form("Mod==%d", Module);
					if(std::string(branchName) == "TDC"){
						//TString expression = Form("(%s * 62.5 + CFD * 0.024) / 500000", branchName);
						TString expression = Form("%s * 62.5 + CFD * 0.024", branchName);
						fTree->Project(histogram->GetName(), expression.Data(), condition.Data());
					}else{
						fTree->Project(histogram->GetName(), branchName, condition.Data());
					}
					
				}
			}else if (Module == 256 && Channel != 32) {
				if(std::string(branchName) == "Ch") {
					histogram = new TH1F(Form("hist_%s", branchName), Form("Histogram of %s", branchName), std::atoi(fTextEntry1->GetText()), 
					xmin.Atoll(), xmax.Atoll());
					fTree->Project(histogram->GetName(), branchName);
				}else{
					histogram = new TH1F(Form("hist_%s", branchName), Form("Histogram of %s (Channel==%d)", branchName, Channel), std::atoi(fTextEntry1->GetText()), 
					xmin.Atoll(), xmax.Atoll());
					TString condition = Form("Ch==%d", Channel);
					if(std::string(branchName) == "TDC"){
						//TString expression = Form("(%s * 62.5 + CFD * 0.024) / 500000", branchName);
						TString expression = Form("%s * 62.5 + CFD * 0.024", branchName);
						fTree->Project(histogram->GetName(), expression.Data(), condition.Data());
					}else{
						fTree->Project(histogram->GetName(), branchName, condition.Data());
					}
				}
			}else{
				if(std::string(branchName) == "Ch") {
					histogram = new TH1F(Form("hist_%s", branchName), Form(("Histogram of %s (Mod==%d) "+ModInfo(Module)).c_str(), branchName, Module), std::atoi(fTextEntry1->GetText()),
					//histogram = new TH1F(Form("hist_%s", branchName), Form("Histogram of %s (Mod==%d)", branchName, Module), std::atoi(fTextEntry1->GetText()), 
					xmin.Atoll(), xmax.Atoll());
					TString condition = Form("Mod==%d", Module);
					fTree->Project(histogram->GetName(), branchName, condition.Data());
				}else if (std::string(branchName) == "Mod"){
					histogram = new TH1F(Form("hist_%s", branchName), Form("Histogram of %s (Channel==%d)", branchName, Channel), std::atoi(fTextEntry1->GetText()), 
					xmin.Atoll(), xmax.Atoll());
					TString condition = Form("Ch==%d", Channel);
					fTree->Project(histogram->GetName(), branchName, condition.Data());
				}else{
					histogram = new TH1F(Form("hist_%s", branchName), Form(("Histogram of %s (Mod==%d Channel==%d) "+ModInfo(Module)).c_str(), branchName, Module, Channel),
					//histogram = new TH1F(Form("hist_%s", branchName), Form("Histogram of %s (Mod==%d Channel==%d)", branchName, Module, Channel), 
					std::atoi(fTextEntry1->GetText()), xmin.Atoll(), xmax.Atoll());
					TString condition = Form("Mod==%d && Ch==%d", Module, Channel);
					if(std::string(branchName) == "TDC"){
						//TString expression = Form("(%s * 62.5 + CFD * 0.024) / 500000", branchName);
						TString expression = Form("%s * 62.5 + CFD * 0.024", branchName);
						fTree->Project(histogram->GetName(), expression.Data(), condition.Data());
					}else{
						fTree->Project(histogram->GetName(), branchName, condition.Data());
					}
				}
			}
			canvas->cd();
			histogram->Draw();
			canvas->Update();
			fDrawHisto1DButton->SetEnabled(false);
			fPrevBranchButton->SetEnabled(true);
			fNextBranchButton->SetEnabled(true);
		}
		else {
			std::cout << "Error: Invalid branch index!" << std::endl;
			return;
		}
	}
	else {
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Select File First!", kMBIconExclamation, kMBOk);
		return;
	}
	return;
}

void CanvasFrame::DrawHisto2D() {
	const size_t max_int_length = std::to_string(std::numeric_limits<int>::max()).size();
	const size_t max_Long64_t_length = std::to_string(std::numeric_limits<Long64_t>::max()).size();
	TString xmin = fTextEntry2->GetText();
	TString xmax = fTextEntry3->GetText();
	TString ymin = fTextEntry5->GetText();
	TString ymax = fTextEntry6->GetText();
	if(strlen(fTextEntry1->GetText()) > max_int_length || strlen(fTextEntry2->GetText()) > max_Long64_t_length || strlen(fTextEntry3->GetText()) > max_Long64_t_length
	|| strlen(fTextEntry4->GetText()) > max_int_length || strlen(fTextEntry5->GetText()) > max_Long64_t_length || strlen(fTextEntry6->GetText()) > max_Long64_t_length
	|| strlen(fTextEntry7->GetText()) > 3 || strlen(fTextEntry8->GetText()) > 2){
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Invalid Input!", kMBIconExclamation, kMBOk);
		return;
	}
	if ((fTextEntry1->GetText() >= std::to_string(std::numeric_limits<int>::max()) && strlen(fTextEntry1->GetText()) == max_int_length )
	|| (fTextEntry2->GetText() >= std::to_string(std::numeric_limits<Long64_t>::max()) && strlen(fTextEntry2->GetText()) == max_Long64_t_length )
	|| (fTextEntry3->GetText() >= std::to_string(std::numeric_limits<Long64_t>::max()) && strlen(fTextEntry3->GetText()) == max_Long64_t_length )){
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Invalid Input!", kMBIconExclamation, kMBOk);
		return;
	}
	if ((fTextEntry1->GetText() >= std::to_string(std::numeric_limits<int>::max()) && strlen(fTextEntry1->GetText()) == max_int_length )
	|| (xmin >= std::to_string(std::numeric_limits<Long64_t>::max()) && strlen(xmin) == max_Long64_t_length )
	|| (xmax >= std::to_string(std::numeric_limits<Long64_t>::max()) && strlen(xmax) == max_Long64_t_length )
	|| (fTextEntry4->GetText() >= std::to_string(std::numeric_limits<int>::max()) && strlen(fTextEntry4->GetText()) == max_int_length )
	|| (ymin >= std::to_string(std::numeric_limits<Long64_t>::max()) && strlen(ymin) == max_Long64_t_length )
	|| (ymax >= std::to_string(std::numeric_limits<Long64_t>::max()) && strlen(ymax) == max_Long64_t_length )
	|| xmin.Atoll() > xmax.Atoll() || ymin.Atoll() > ymax.Atoll() || (std::atoi(fTextEntry7->GetText()) > 256) || (std::atoi(fTextEntry8->GetText()) > 32)){
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Invalid Input!", kMBIconExclamation, kMBOk);
		return;
	}
	if (!(IsNonNegativeInteger(fTextEntry1->GetText()) && IsInteger(fTextEntry2->GetText()) && IsInteger(fTextEntry3->GetText())
	&& IsNonNegativeInteger(fTextEntry4->GetText()) && IsInteger(fTextEntry5->GetText()) && IsInteger(fTextEntry6->GetText())
	&& IsNonNegativeInteger(fTextEntry7->GetText()) && IsNonNegativeInteger(fTextEntry8->GetText()))) {
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Invalid Input!", kMBIconExclamation, kMBOk);
		return;
	}
	if (mod == 0) {
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Only For 2-D Histo!", kMBIconExclamation, kMBOk);
		return;
	}	
	if(select){
		Module = std::atoi(fTextEntry7->GetText());
		Channel = std::atoi(fTextEntry8->GetText());
		canvas->Clear();
		cout<<"file"<<file<<endl;
		TGText* FileName1 = fFilename1->GetText();
		TString textContent = FileName1->AsString();
		file = textContent.Data();
		textContent.Strip(TString::kBoth);
		cout << "file" << textContent.Data() << endl;
		//source = new TFile("/ home / test / root / macros / mvmelstrun007.root", "READ");
		TFile* source = new TFile(textContent.Data(), "READ");
		TTree* fTree = (TTree*)source->Get("Data");
		if(x_name == nullptr || y_name == nullptr){
			new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Select Histo Label!", kMBIconExclamation, kMBOk);
			return;
		}
		TH2F* histogram;
		if (Module == 256 && Channel == 32){
			histogram = new TH2F(Form("hist_%s-%s", x_name, y_name), Form("Histogram of %s-%s", x_name, y_name), std::atoi(fTextEntry1->GetText()), 
			xmin.Atoll(), xmax.Atoll(), std::atoi(fTextEntry4->GetText()), ymin.Atoll(), ymax.Atoll());
			fTree->Project(histogram->GetName(), Form("%s:%s", y_name, x_name));
		}
		else if (Module != 256 && Channel == 32) {
			if(std::string(x_name) == "Mod") {
				histogram = new TH2F(Form("hist_%s-%s", x_name, y_name), Form("Histogram of %s-%s", x_name, y_name), std::atoi(fTextEntry1->GetText()), 
				xmin.Atoll(), xmax.Atoll(), std::atoi(fTextEntry4->GetText()), ymin.Atoll(), ymax.Atoll());
				if(std::string(y_name) == "TDC"){
					//TString expression = Form("(%s * 62.5 + CFD * 0.024) / 500000", branchName);
					TString expression = Form("(%s * 62.5 + CFD * 0.024):%s", y_name, x_name);
					fTree->Project(histogram->GetName(), expression.Data());
				}else{
					fTree->Project(histogram->GetName(), Form("%s:%s", y_name, x_name));
				}
			}else if(std::string(y_name) == "Mod") {
				histogram = new TH2F(Form("hist_%s-%s", x_name, y_name), Form("Histogram of %s-%s", x_name, y_name), std::atoi(fTextEntry1->GetText()), 
				xmin.Atoll(), xmax.Atoll(), std::atoi(fTextEntry4->GetText()), ymin.Atoll(), ymax.Atoll());
				if(std::string(x_name) == "TDC"){
					TString expression = Form("%s:(%s * 62.5 + CFD * 0.024)", y_name, x_name);
					fTree->Project(histogram->GetName(), expression.Data());
				}else{
					fTree->Project(histogram->GetName(), Form("%s:%s", y_name, x_name));
				}
			}
			else {
				histogram = new TH2F(Form("hist_%s-%s", x_name, y_name), Form("Histogram of %s-%s (Mod==%d)", x_name, y_name, Module), std::atoi(fTextEntry1->GetText()), 
				xmin.Atoll(), xmax.Atoll(), std::atoi(fTextEntry4->GetText()), ymin.Atoll(), ymax.Atoll());
				TString condition = Form("Mod==%d", Module);
				if(std::string(y_name) == "TDC"){
					TString expression = Form("(%s * 62.5 + CFD * 0.024):%s", y_name, x_name);
					fTree->Project(histogram->GetName(), expression.Data(), condition.Data());
				}else if(std::string(x_name) == "TDC"){
					TString expression = Form("%s:(%s * 62.5 + CFD * 0.024)", y_name, x_name);
					fTree->Project(histogram->GetName(), expression.Data(), condition.Data());
				}else{
					fTree->Project(histogram->GetName(), Form("%s:%s", y_name, x_name), condition.Data());
				}
			}
		}
		else if (Module == 256 && Channel != 32) {
			if(std::string(x_name) == "Ch") {
				histogram = new TH2F(Form("hist_%s-%s", x_name, y_name), Form("Histogram of %s-%s", x_name, y_name), std::atoi(fTextEntry1->GetText()), 
				xmin.Atoll(), xmax.Atoll(), std::atoi(fTextEntry4->GetText()), ymin.Atoll(),ymax.Atoll());
				if(std::string(y_name) == "TDC"){
					TString expression = Form("(%s * 62.5 + CFD * 0.024):%s", y_name, x_name);
					fTree->Project(histogram->GetName(), expression.Data());
				}else{
					fTree->Project(histogram->GetName(), Form("%s:%s", y_name, x_name));
				}
			}
			else if(std::string(y_name) == "Ch"){
				histogram = new TH2F(Form("hist_%s-%s", x_name, y_name), Form("Histogram of %s-%s", x_name, y_name), std::atoi(fTextEntry1->GetText()), 
				xmin.Atoll(), xmax.Atoll(), std::atoi(fTextEntry4->GetText()), ymin.Atoll(), ymax.Atoll());
				if(std::string(x_name) == "TDC"){
					TString expression = Form("%s:(%s * 62.5 + CFD * 0.024)", y_name, x_name);
					fTree->Project(histogram->GetName(), expression.Data());
				}else{
					fTree->Project(histogram->GetName(), Form("%s:%s", y_name, x_name));
				}
			}
			else {
				histogram = new TH2F(Form("hist_%s-%s", x_name, y_name), Form("Histogram of %s-%s (Channel==%d)", x_name, y_name, Channel), std::atoi(fTextEntry1->GetText()), 
				xmin.Atoll(), xmax.Atoll(), std::atoi(fTextEntry4->GetText()), ymin.Atoll(), ymax.Atoll());
				TString condition = Form("Ch==%d", Channel);
				if(std::string(y_name) == "TDC"){
					TString expression = Form("(%s * 62.5 + CFD * 0.024):%s", y_name, x_name);
					fTree->Project(histogram->GetName(), expression.Data(), condition.Data());
				}else if(std::string(x_name) == "TDC"){
					TString expression = Form("%s:(%s * 62.5 + CFD * 0.024)", y_name, x_name);
					fTree->Project(histogram->GetName(), expression.Data(), condition.Data());
				}else{
					fTree->Project(histogram->GetName(), Form("%s:%s", y_name, x_name), condition.Data());
				}
			}
		}
		else{
			if(std::string(x_name) == "Ch" && std::string(y_name) != "Mod") {
				histogram = new TH2F(Form("hist_%s-%s", x_name, y_name), Form("Histogram of %s-%s (Mod==%d)", x_name, y_name, Module), std::atoi(fTextEntry1->GetText()), 
				xmin.Atoll(), xmax.Atoll(), std::atoi(fTextEntry4->GetText()), ymin.Atoll(), ymax.Atoll());
				TString condition = Form("Mod==%d", Module);
				if(std::string(y_name) == "TDC"){
					TString expression = Form("(%s * 62.5 + CFD * 0.024):%s", y_name, x_name);
					fTree->Project(histogram->GetName(), expression.Data(), condition.Data());
				}else{
					fTree->Project(histogram->GetName(), Form("%s:%s", y_name, x_name), condition.Data());
				}
			}
			else if(std::string(x_name) != "Mod" && std::string(y_name) == "Ch") {
				histogram = new TH2F(Form("hist_%s-%s", x_name, y_name), Form("Histogram of %s-%s (Mod==%d)", x_name, y_name, Module), std::atoi(fTextEntry1->GetText()), 
				xmin.Atoll(), xmax.Atoll(), std::atoi(fTextEntry4->GetText()), ymin.Atoll(), ymax.Atoll());
				TString condition = Form("Mod==%d", Module);
				if(std::string(x_name) == "TDC"){
					TString expression = Form("%s:(%s+CFD)*62.5", y_name, x_name);
					fTree->Project(histogram->GetName(), expression.Data(), condition.Data());
				}else{
					fTree->Project(histogram->GetName(), Form("%s:%s", y_name, x_name), condition.Data());
				}
			}
			else if (std::string(x_name) == "Mod" || std::string(y_name) != "Ch"){
				histogram = new TH2F(Form("hist_%s-%s", x_name, y_name), Form("Histogram of %s-%s (Channel==%d)", x_name, y_name, Channel), std::atoi(fTextEntry1->GetText()), 
				xmin.Atoll(), xmax.Atoll(), std::atoi(fTextEntry4->GetText()), ymin.Atoll(), ymax.Atoll());
				TString condition = Form("Ch==%d", Channel);
				if(std::string(y_name) == "TDC"){
					TString expression = Form("(%s * 62.5 + CFD * 0.024):%s", y_name, x_name);
					fTree->Project(histogram->GetName(), expression.Data(), condition.Data());
				}else{
					fTree->Project(histogram->GetName(), Form("%s:%s", y_name, x_name), condition.Data());
				}
			}
			else if (std::string(x_name) != "Ch" || std::string(y_name) == "Mod"){
				histogram = new TH2F(Form("hist_%s-%s", x_name, y_name), Form("Histogram of %s-%s (Channel==%d)", x_name, y_name, Channel), std::atoi(fTextEntry1->GetText()), 
				xmin.Atoll(), xmax.Atoll(), std::atoi(fTextEntry4->GetText()), ymin.Atoll(), ymax.Atoll());
				TString condition = Form("Ch==%d", Channel);
				if(std::string(x_name) == "TDC"){
					TString expression = Form("%s:(%s * 62.5 + CFD * 0.024)", y_name, x_name);
					fTree->Project(histogram->GetName(), expression.Data(), condition.Data());
				}else{
					fTree->Project(histogram->GetName(), Form("%s:%s", y_name, x_name), condition.Data());
				}
			}
			else if((std::string(x_name) == "Ch" || std::string(y_name) == "Ch") && (std::string(x_name) == "Mod" || std::string(y_name) == "Mod")){
				histogram = new TH2F(Form("hist_%s-%s", x_name, y_name), Form("Histogram of %s-%s", x_name, y_name), std::atoi(fTextEntry1->GetText()), 
				xmin.Atoll(), xmax.Atoll(), std::atoi(fTextEntry4->GetText()), ymin.Atoll(), ymax.Atoll());
				fTree->Project(histogram->GetName(), Form("%s:%s", y_name, x_name));
			}
			else{
				histogram = new TH2F(Form("hist_%s-%s", x_name, y_name), Form("Histogram of %s-%s (Mod==%d Channel==%d)", x_name, y_name, Module, Channel), 
				std::atoi(fTextEntry1->GetText()), xmin.Atoll(), xmax.Atoll(), std::atoi(fTextEntry4->GetText()), ymin.Atoll(), ymax.Atoll());
				TString condition = Form("Mod==%d && Ch==%d", Module, Channel);
				if(std::string(x_name) == "TDC" && std::string(y_name) == "TDC"){
					TString expression = Form("(%s * 62.5 + CFD * 0.024):(%s * 62.5 + CFD * 0.024)", y_name, x_name);
					fTree->Project(histogram->GetName(), expression.Data(), condition.Data());
				}else if(std::string(x_name) == "TDC"){
					TString expression = Form("%s:(%s * 62.5 + CFD * 0.024)", y_name, x_name);
					fTree->Project(histogram->GetName(), expression.Data(), condition.Data());
				}
				else if(std::string(y_name) == "TDC"){
					TString expression = Form("(%s * 62.5 + CFD * 0.024):%s", y_name, x_name);
					fTree->Project(histogram->GetName(), expression.Data(), condition.Data());
				}
				else{
					fTree->Project(histogram->GetName(), Form("%s:%s", y_name, x_name), condition.Data());
				}
			}
		}
		canvas->cd();
		histogram->Draw("colz");
		canvas->Update();
		fTempButton->SetEnabled(true);
		return;
	}else {
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Select File First!", kMBIconExclamation, kMBOk);
		return;
	}
}

void CanvasFrame::NextChannel() {
	if (select) {
		Channel = (Channel + 1) % 33;
		TString str;
		str.Form("%d", Channel);
		fTextEntry8->SetText(str.Data());
		if(mod == 0){
			DrawHisto1D();
		}else {
			DrawHisto2D();
		}
	}
	else {
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Select File First!", kMBIconExclamation, kMBOk);
	}
	return;
}

void CanvasFrame::PrevChannel() {
	if (select) {
		Channel = (Channel - 1 + 33) % 33;
		TString str;
		str.Form("%d", Channel);
		fTextEntry8->SetText(str.Data());
		if(mod == 0){
			DrawHisto1D();
		}else {
			DrawHisto2D();
		}
	}
	else {
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Select File First!", kMBIconExclamation, kMBOk);
	}
	return;
}

void CanvasFrame::NextModule() {
	if (select) {
		Module = (Module + 1) % 257;
		TString str;
		str.Form("%d", Module);
		fTextEntry7->SetText(str.Data());
		if(mod == 0){
			DrawHisto1D();
		}else {
			DrawHisto2D();
		}
	}
	else {
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Select File First!", kMBIconExclamation, kMBOk);
	}
	return;
}

void CanvasFrame::PrevModule() {
	if (select) {
		Module = (Module - 1 + 257) % 257;
		TString str;
		str.Form("%d", Module);
		fTextEntry7->SetText(str.Data());
		if(mod == 0){
			DrawHisto1D();
		}else {
			DrawHisto2D();
		}
	}
	else {
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Select File First!", kMBIconExclamation, kMBOk);
	}
	return;
}

void CanvasFrame::NextBranch() {
	if (select) {
		if (mod == 0) {
			fCurrentBranchIndex = (fCurrentBranchIndex + 1) % fTotalBranches;
			DrawHisto1D();
		}else{
			new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Only For 1-D Histo!", kMBIconExclamation, kMBOk);
		}
	}
	else {
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Select File First!", kMBIconExclamation, kMBOk);
	}
	return;
}

void CanvasFrame::PrevBranch() {
	if (select) {
		if (mod == 0) {
			fCurrentBranchIndex = (fCurrentBranchIndex - 1 + fTotalBranches) % fTotalBranches;
			DrawHisto1D();
		}else{
			new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Only For 1-D Histo!", kMBIconExclamation, kMBOk);
		}
	}
	else {
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Select File First!", kMBIconExclamation, kMBOk);
	}
	return;
}

void CanvasFrame::changeMode() {
	TGLongPosition* pos = new TGLongPosition(0, 0);
	if(select){
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Please reset!", kMBIconExclamation, kMBOk);
		return;
	}
	if (mod == 0) {
		mod = 1;
		TGText* FileName1 = fFilename2->GetText();
		FileName1->Clear();
		FileName1->InsText(*pos, "Histo 2D");
	}
	else if (mod == 1) {
		mod = 0;
		TGText* FileName1 = fFilename2->GetText();
		FileName1->Clear();
		FileName1->InsText(*pos, "Histo 1D");
	}
	fFilename2->Update();
	return;
}

void CanvasFrame::Update() {
	if (select) {
		if (mod == 0) {
			DrawHisto1D();
		}else if(mod == 1){
			DrawHisto2D();
		}
	}
	else {
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Select File First!", kMBIconExclamation, kMBOk);
	}
	return;
}

void CanvasFrame::SetEnable(bool temp){
	fTextEntry1->SetEnabled(temp);
	fTextEntry2->SetEnabled(temp);
	fTextEntry3->SetEnabled(temp);
	fTextEntry4->SetEnabled(temp);
	fTextEntry5->SetEnabled(temp);
	fTextEntry6->SetEnabled(temp);
	fTextEntry7->SetEnabled(temp);
	fTextEntry8->SetEnabled(temp);
	fPrevBranchButton->SetEnabled(temp);
	fNextBranchButton->SetEnabled(temp);
	fPrevChannelButton->SetEnabled(temp);
	fNextChannelButton->SetEnabled(temp);
	fPrevModuleButton->SetEnabled(temp);
	fNextModuleButton->SetEnabled(temp);
	fSelectButton->SetEnabled(temp);
	fModeButton->SetEnabled(temp);
	fResetButton->SetEnabled(temp);
	fPDFButton->SetEnabled(temp);
	fDrawHisto1DButton->SetEnabled(temp);
	fDrawHisto2DButton->SetEnabled(temp);
	histo_x->SetEnabled(temp);
	histo_y->SetEnabled(temp);
	fDrawHistoButton->SetEnabled(temp);
}

void CanvasFrame::PDF() {
	SetEnable(false);
	TGText* FileName1 = fFilename1->GetText();
	TString textContent = FileName1->AsString();
	file = textContent.Data();
	textContent.Strip(TString::kBoth);
	TFile* source = new TFile(textContent.Data(), "READ");
	TTree* tree = (TTree*)source->Get("Data");
	fTotalBranches = tree->GetListOfBranches()->GetEntries();
	canvas = embeddedCanvas->GetCanvas();
	vector<TH1F*> histograms;
	TH1F* hist;
	TBranch *modBranch = tree->GetBranch("Mod");
	set<int> uniqueMods;
	int numEntries = tree->GetEntries();
	int Value;
	modBranch->SetAddress(&Value);
    	for (int i = 0; i < numEntries; ++i) {
        	modBranch->GetEntry(i);
		uniqueMods.insert(Value);
	}
	for (int num = 0; num < fTotalBranches; num++) {
		TBranch* branch = dynamic_cast<TBranch*>(tree->GetListOfBranches()->At(num));
		const char* branchName = branch->GetName();
		if (std::string(branchName) == "Mod") {
			for (int i = 0; i < 32; i++) {
				string condition = "Ch == " + to_string(i);
				canvas->cd();
 				tree->Draw((std::string(branchName) + ">>htemp_" + std::to_string(num) + "_"+ std::to_string(i)).c_str(), condition.c_str(), "");
				//hist = (TH1F*)gROOT->FindObject(("htemp_" + std::to_string(num) + "_" + std::to_string(i)).c_str());
				hist = (TH1F*)gDirectory->Get(("htemp_" + std::to_string(num) + "_" + std::to_string(i)).c_str());
				if (hist->GetEntries() > 0 && hist) {
					histograms.push_back(hist);
					//cout<<"done"<<endl;
				}
				canvas->Update();
			}
		}
		else if (std::string(branchName) == "Ch") {
			for (const int& Mod : uniqueMods) {
				string condition = "Mod == " + to_string(Mod);
				canvas->cd();
				tree->Draw((std::string(branchName) + ">>htemp_" + std::to_string(num) + "_" + std::to_string(Mod)+" "+ModInfo(Mod)).c_str(), condition.c_str(), "");
				//hist = (TH1F*)gROOT->FindObject(("htemp_" + std::to_string(num) + "_" + std::to_string(Mod)).c_str());
				hist = (TH1F*)gDirectory->Get(("htemp_" + std::to_string(num) + "_" + std::to_string(Mod)+" "+ModInfo(Mod)).c_str());
				if (hist->GetEntries() > 0 && hist) {
					histograms.push_back(hist);
					//cout<<"done"<<endl;
				}
				canvas->Update();
			}

		}
		else {
			cout<<branchName<<endl;
			string temp = std::string(branchName);
			for (const int& Mod : uniqueMods) {
				for (int j = 0; j < 32; j++) {
					//cout<<"j"<<j<<endl;
					string condition = "Ch == " + to_string(j)+" && " + "Mod == " + to_string(Mod);
					canvas->cd();
					//cout<<"1"<<endl;
					if (std::string(branchName) == "TDC") {
						tree->Draw(("("+temp+" * 62.5 + CFD * 0.024)" + ">>htemp_" + std::to_string(num) + "_" + std::to_string(Mod) + "_" + std::to_string(j)+" "+ModInfo(Mod)).c_str(), condition.c_str(), "");
 					}
 					else{
 						tree->Draw((temp + ">>htemp_" + std::to_string(num) + "_" + std::to_string(Mod) + "_" + std::to_string(j)+" "+ModInfo(Mod)).c_str(), condition.c_str(), "");
 					}
 					//hist = (TH1F*)gROOT->FindObject(("htemp_" + std::to_string(num) + "_" + std::to_string(Mod) + "_" + std::to_string(j)).c_str());
					hist = (TH1F*)gDirectory->Get(("htemp_" + std::to_string(num) + "_" + std::to_string(Mod) + "_" + std::to_string(j)+" "+ModInfo(Mod)).c_str());
					if (hist->GetEntries() > 0 && hist) {
						histograms.push_back(hist);
						//cout<<"done"<<endl;
					}
					canvas->Update();
				}
			}
		}
	}
	canvas->Clear();
	TCanvas* tempCanvas = new TCanvas();
	tempCanvas->cd();
	for (int k = 0; k < (int)histograms.size(); k++) {
		histograms[k]->Draw();
		tempCanvas->Update();
		if(!histograms[k]){
			cout<<"!!!!!!!!"<<endl;
		}
		if (k == 0) {
			tempCanvas->Print((textContent+"_Simple.pdf(").Data(), ("Title:"+std::to_string(k)).c_str());
		}
		else if (k == (int) histograms.size() - 1) {
			tempCanvas->Print((textContent+"_Simple.pdf)").Data(),  ("Title:"+std::to_string(k)).c_str());
		}
		else {
			tempCanvas->Print((textContent+"_Simple.pdf").Data(),  ("Title:"+std::to_string(k)).c_str());
		}

	}
	tempCanvas->Clear();
	tempCanvas->Close();
	Reset();
	return;
}

void CanvasFrame::Reset() {
	fTextEntry1->SetEnabled(false);
	fTextEntry2->SetEnabled(false);
	fTextEntry3->SetEnabled(false);
	fTextEntry4->SetEnabled(false);
	fTextEntry5->SetEnabled(false);
	fTextEntry6->SetEnabled(false);
	fTextEntry7->SetEnabled(false);
	fTextEntry8->SetEnabled(false);
	fSelectButton->SetEnabled(true);
	fModeButton->SetEnabled(true);
	fResetButton->SetEnabled(true);
	fDrawHisto1DButton->SetEnabled(false);
	fDrawHisto2DButton->SetEnabled(false);
	fPrevBranchButton->SetEnabled(false);
	fNextBranchButton->SetEnabled(false);
	fPrevChannelButton->SetEnabled(false);
	fNextChannelButton->SetEnabled(false);
	fPrevModuleButton->SetEnabled(false);
	fNextModuleButton->SetEnabled(false);
	fPDFButton->SetEnabled(false);
	histo_x->SetEnabled(false);
	histo_y->SetEnabled(false);
	fDrawHistoButton->SetEnabled(false);
	fTempButton->SetEnabled(false);
	histo_x->RemoveAll();
	histo_y->RemoveAll();
	
	fCurrentBranchIndex = 0;
	mod = 0;
	prev_x = 1;
	prev_y = 1;
	x_bin = 100;
	x_min = 0;
	x_max = 100;
	y_bin = 100;
	y_min = 0;
	y_max = 100;
	Module = 256;
	Channel = 32;
	file = nullptr;
	x_name = nullptr;
	y_name = nullptr;
	cutG = nullptr;
	fTextEntry1->SetText("100");
	fTextEntry2->SetText("0");
	fTextEntry3->SetText("100");
	fTextEntry4->SetText("100");
	fTextEntry5->SetText("0");
	fTextEntry6->SetText("100");
	fTextEntry7->SetText("256");
	fTextEntry8->SetText("32");
	select = false;
	cut_flag = false;
	drawLine = false;
	CutDone = false;
	TGText* FileName1 = fFilename1->GetText();
	FileName1->Clear();
	TGLongPosition* pos = new TGLongPosition(0, 0);
	FileName1->InsText(*pos, "None");
	fFilename1->Update();
	TGText* FileName2 = fFilename2->GetText();
	FileName2->Clear();
	FileName2->InsText(*pos, "Histo 1D");
	fFilename2->Update();
	canvas->Clear();
	canvas->Update();
	if(fPictureFrame){
		fPictureFrame->DeleteWindow();
		fPictureFrame = nullptr;
	}
	return;
}

void CanvasFrame::DrawWindow(){
	
	return;
}

void CanvasFrame::UpdateTGComboBox(TTree* fTree){
	if(select == false){
		histo_x->RemoveAll();
		histo_y->RemoveAll();
		int id;
		for(int i = 0 ; i< fTotalBranches; i++){
			TBranch* branch = dynamic_cast<TBranch*>(fTree->GetListOfBranches()->At(i));
			const char* branchName = branch->GetName();
			id = i+1;
			histo_x->AddEntry(branchName,id);
			histo_y->AddEntry(branchName,id);
		}
		x_name = nullptr;
		y_name = nullptr;
	}
	return;
}

void CanvasFrame::SelectItemX(){
	const char * x = histo_x->GetSelectedEntry()->GetTitle();
	if(mod == 0){
		return;
	}
	else if (select == true){
		x_name = x;
	}
	return;
}

void CanvasFrame::SelectItemY(){
	const char * y = histo_y->GetSelectedEntry()->GetTitle();
	if(mod == 0){
		return;
	}
	else if (select == true){
		y_name = y;
	}
	return;
}

void CanvasFrame::OnReturnProcessed() {
	const size_t max_int_length = std::to_string(std::numeric_limits<int>::max()).size();
	const size_t max_Long64_t_length = std::to_string(std::numeric_limits<Long64_t>::max()).size();
	TString xmin = fTextEntry2->GetText();
	TString xmax = fTextEntry3->GetText();
	TString ymin = fTextEntry5->GetText();
	TString ymax = fTextEntry6->GetText();
	//cout<<std::numeric_limits<ULong64_t>::max()<<endl;
	cout<<"Long64_t "<<max_Long64_t_length<<endl;
	cout<<strlen(fTextEntry3->GetText())<<endl;
	if(strlen(fTextEntry1->GetText()) > max_int_length || strlen(fTextEntry2->GetText()) > max_Long64_t_length || strlen(fTextEntry3->GetText()) > max_Long64_t_length
	|| strlen(fTextEntry7->GetText()) > 3 || strlen(fTextEntry8->GetText()) > 2){
		cout<<"1"<<endl;
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Invalid Input!", kMBIconExclamation, kMBOk);
		return;
	}
	if ((fTextEntry1->GetText() >= std::to_string(std::numeric_limits<int>::max()) && strlen(fTextEntry1->GetText()) == max_int_length)
	|| (fTextEntry2->GetText() >= std::to_string(std::numeric_limits<Long64_t>::max()) && strlen(fTextEntry2->GetText()) == max_Long64_t_length)
	|| (fTextEntry3->GetText() >= std::to_string(std::numeric_limits<Long64_t>::max()) && strlen(fTextEntry3->GetText()) == max_Long64_t_length)){
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Invalid Input!", kMBIconExclamation, kMBOk);
		cout<<"2"<<endl;
		return;
	}
	if(mod == 1 &&(strlen(fTextEntry4->GetText()) > max_int_length || strlen(fTextEntry5->GetText()) > max_Long64_t_length || strlen(fTextEntry6->GetText()) > max_Long64_t_length)){
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Invalid Input!", kMBIconExclamation, kMBOk);
		cout<<"3"<<endl;
		return;
	}
	if(mod == 1 && ((fTextEntry4->GetText() >= std::to_string(std::numeric_limits<int>::max()) && strlen(fTextEntry4->GetText()) == max_int_length)
	|| (fTextEntry5->GetText() >= std::to_string(std::numeric_limits<Long64_t>::max()) && strlen(fTextEntry5->GetText()) == max_Long64_t_length)
	|| (fTextEntry6->GetText() >= std::to_string(std::numeric_limits<Long64_t>::max()) && strlen(fTextEntry6->GetText()) == max_Long64_t_length))){
		new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Invalid Input!", kMBIconExclamation, kMBOk);
		cout<<"4"<<endl;
		return;
	}
	if (mod == 0){ 
		if(!(IsNonNegativeInteger(fTextEntry1->GetText()) && IsInteger(fTextEntry2->GetText()) && IsInteger(fTextEntry3->GetText())
		&& IsNonNegativeInteger(fTextEntry7->GetText()) && IsNonNegativeInteger(fTextEntry8->GetText()))) {
			new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Invalid Input!", kMBIconExclamation, kMBOk);
			cout<<"5"<<endl;
			return;
		}
		if(std::stoi(fTextEntry7->GetText()) > 256 || std::stoi(fTextEntry8->GetText()) > 32 || xmin.Atoll() > xmax.Atoll()){
			new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Invalid Input!", kMBIconExclamation, kMBOk);
			cout<<"6"<<endl;
			return;
		}
		Update();
	}
	else if (mod == 1){ 
		if(!(IsNonNegativeInteger(fTextEntry1->GetText()) && IsInteger(fTextEntry2->GetText()) && IsInteger(fTextEntry3->GetText())
	&& IsNonNegativeInteger(fTextEntry4->GetText()) && IsInteger(fTextEntry5->GetText()) && IsInteger(fTextEntry6->GetText())
	&& IsNonNegativeInteger(fTextEntry7->GetText()) && IsNonNegativeInteger(fTextEntry8->GetText()))) {
			new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Invalid Input!", kMBIconExclamation, kMBOk);
			return;
		}
		if(std::stoi(fTextEntry7->GetText()) >256 || std::stoi(fTextEntry8->GetText()) >32 || xmin.Atoll() > xmax.Atoll() || ymin.Atoll() > ymax.Atoll()){
			new TGMsgBox(gClient->GetDefaultRoot(), this, "Error", "Invalid Input!", kMBIconExclamation, kMBOk);
			return;
		}
		Update();
	}
	return;
}

void CanvasFrame::HistoEventHandle() {
	//cout<<"done"<<endl;
	if (mod == 0) {
		TList* primitives = canvas->GetListOfPrimitives();
		TObject *firstHisto = primitives->First();
		TH1F *hist = dynamic_cast<TH1F*>(firstHisto);
		if(hist) {
			//cout<<"!!"<<endl;
			x_bin = hist->GetXaxis()->GetNbins();
			x_min = hist->GetXaxis()->GetXmin();
			x_max = hist->GetXaxis()->GetXmax();
			//cout<<x_min<<" "<<x_max<<endl;
			fTextEntry1->Clear();
			fTextEntry2->Clear();
			fTextEntry3->Clear();
			fTextEntry1->SetText(to_string(x_bin).c_str());
			fTextEntry2->SetText(to_string(x_min).c_str());
			fTextEntry3->SetText(to_string(x_max).c_str());
		}
	} else if (mod == 1) {
		TList* primitives = canvas->GetListOfPrimitives();
		TObject *firstHisto = primitives->First();
		TH2F *hist = dynamic_cast<TH2F*>(firstHisto);
		if(hist) {
			//cout<<"!!"<<endl;
			x_bin = hist->GetXaxis()->GetNbins();
			//double xMin = canvas->GetUxmin();
    			//double xMax = canvas->GetUxmax();
			//cout<<xMin<<" "<<xMax<< endl;
			//x_min = hist->GetXaxis()->GetBinLowEdge(hist->GetXaxis()->GetFirst());
			//x_max = hist->GetXaxis()->GetBinUpEdge(hist->GetXaxis()->GetLast());
			x_min = hist->GetXaxis()->GetXmin();
			x_max = hist->GetXaxis()->GetXmax();
			y_bin = hist->GetYaxis()->GetNbins();
			y_min = hist->GetYaxis()->GetXmin();
			y_max = hist->GetYaxis()->GetXmax();
			//cout<<x_min<<" "<<x_max<<endl;
			//cout<<y_min<<" "<<y_max<<endl;
			fTextEntry1->Clear();
			fTextEntry2->Clear();
			fTextEntry3->Clear();
			fTextEntry4->Clear();
			fTextEntry5->Clear();
			fTextEntry6->Clear();
			fTextEntry1->SetText(to_string(x_bin).c_str());
			fTextEntry2->SetText(to_string(x_min).c_str());
			fTextEntry3->SetText(to_string(x_max).c_str());
			fTextEntry4->SetText(to_string(y_bin).c_str());
			fTextEntry5->SetText(to_string(y_min).c_str());
			fTextEntry6->SetText(to_string(y_max).c_str());
		}
	}
	return;
}

void CanvasFrame::DrawFile() {
	if (!fPictureFrame) {
		TGText* FileName1 = fFilename1->GetText();
		TString textContent = FileName1->AsString();
		textContent.Strip(TString::kBoth);
		cout<<"Begin to transform:"<<textContent.Data()<<endl;
		fPictureFrame = new PictureFrame(gClient->GetDefaultRoot(), 1920, 900, textContent.Data());
		fPictureFrame->Connect("CloseWindow()", "CanvasFrame", this, "CanvasClosed()");
	}
	else {
		fPictureFrame->RaiseWindow();
	}
	return;
}

void CanvasFrame::CanvasClosed() {
	fPictureFrame = nullptr;
	return;
}
ClassImp(CanvasFrame)
