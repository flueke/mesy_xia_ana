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
	const char *fileType[] = {"MVMELSTRUN files","*.mvlclst",0,0};
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

//Extract data from mvlclst
bool MyMainFrame::restore_ROOT(string name) {
	UInt_t* buff;
	UInt_t adc, rt, ra, adc_ch[32], cfd, adc_long, adc_short, adc_long_ch[32], adc_short_ch[32], prev_time, EventSize, i, ChSel, ModSel;
	ULong64_t tdc, tdc_ch[32], time, temp;
	buff = (UInt_t*)malloc(8194 * sizeof(*buff));
	bool adc_find[32], tdc_find, tdc_extend_find, cfd_find[32], qdc_long_find[32], qdc_short_find[32], qdc_cfd_find[32], qdc_tdc_find, qdc_tdc_extend_find;

	int ModuleType[255];
	ULong64_t SCP_ERROR = 0;
	ULong64_t QDC_ERROR = 0;

	//Read the mvlclst file
	char ss[40000];
	Char_t str;

	string root_name, root_test_name;

	size_t lastSlashPos = name.find_last_of("/\\");
	size_t lastDotPos = name.find_last_of('.');
	root_name = name.substr(lastSlashPos + 1,lastDotPos-lastSlashPos-1);
	root_test_name = root_name;
	root_test_name = root_test_name.append("_test.root");
	root_name = root_name.append(".root");
	ifstream* raw = new ifstream(name);
	if (!raw->good()) {
		printf("Error: Fail to open file. If you are using Windows system, please change it.\n");
		return false;
	}

	//set the output file(.root)
	TFile* fout = new TFile(root_name.c_str(), "recreate");
	TTree* tree = new TTree("Data", "Particle Data");
	tree->Branch("ADC", &adc, "adc/i"); // ADC (MDPP32-SCP)
	tree->Branch("Ch", &ChSel, "ch/i"); //Channel
	tree->Branch("Mod", &ModSel, "mod/i"); //Base Address of the Module
	tree->Branch("TDC", &temp, "tdc/l"); // time_stamp (include the extend time stamp)
	tree->Branch("CFD", &cfd, "cfd/i"); // channel time
	tree->Branch("ADC_Long", &adc_long, "adc_long/i"); // ADC Long (MDPP32-QDC) 
	tree->Branch("ADC_Short", &adc_short, "adc_short/i"); // ADC Short (MDPP32-QDC)
	
	//initialization
	tdc = 0; time = 0;

	//the type of the module
	for (int i = 0; i < 255; i++) {
		ModuleType[i] = 0;
	}

	// find the configuration of the Module
	while (raw->good()) {
		raw->getline(ss, sizeof(ss));
		char end1[40000] = { "      groups:" };
		if (strcmp(ss, end1) == 0) {
			cout << "Module Begin!" << endl;
			break;
		}
	}
	//restore the type of the Module
	while (raw->good()) {
		raw->getline(ss, sizeof(ss));
		char end2[40000] = { "        - name: readout_end" };
		cout<<ss<<endl;
		//the module is SCP
		if ((ss[23] == 0x73 && ss[24] == 0x63 && ss[25] == 0x70)||(ss[23] == 0x53 && ss[24] == 0x43 && ss[25] == 0x50)) {
			cout << ss[23] << ss[24] << ss[25] << endl;
			raw->getline(ss, sizeof(ss));
			raw->getline(ss, sizeof(ss));
			cout << ss[42] << ss[43] << endl;
			int m1 = 0; 
			int m2 = 0;
			if (ss[42] >= 'a' && ss[42] <= 'f') {
				m1 = ss[42] - 'a' + 10;
			}
			else if (ss[42] >= '0' && ss[42] <= '9') {
				m1 = ss[42] - '0';
			}else{
				return false;
			}
			if (ss[43] >= 'a' && ss[43] <= 'f') {
				m2 = ss[43] - 'a' + 10;
			}
			else if (ss[43] >= '0' && ss[43] <= '9') {
				m2 = ss[43] - '0';
			}else{
				return false;
			}
			ModuleType[m1 * 16 + m2] = 1;
		}
		//the module is QDC
		else if ((ss[23] == 0x71 && ss[24] == 0x64 && ss[25] == 0x63)||(ss[23] == 0x51 && ss[24] == 0x44 && ss[25] == 0x43)) {
			cout << ss[23] << ss[24] << ss[25] << endl;
			raw->getline(ss, sizeof(ss));
			raw->getline(ss, sizeof(ss));
			cout << ss[42] << ss[43] << endl;
			int m1 = 0; 
			int m2 = 0;
			if (ss[42] >= 'a' && ss[42] <= 'f') {
				m1 = ss[42] - 'a' + 10;
			}
			else if (ss[42] >= '0' && ss[42] <= '9') {
				m1 = ss[42] - '0';
			}else {
				return false;
			}
			if (ss[43] >= 'a' && ss[43] <= 'f') {
				m2 = ss[43] - 'a' + 10;
			}
			else if (ss[43] >= '0' && ss[43] <= '9') {
				m2 = ss[43] - '0';
			}else {
				return false;
			}
			ModuleType[m1 * 16 + m2] = 2;
		}
		if (strcmp(ss, end2) == 0) {
			cout << "Module End!" << endl;
			break;
		}
	}
	// find the data
	while (raw->good()) {
		raw->getline(ss, sizeof(ss));
		char end3[40000] = { "            \"type\": \"MVLC_ETH\"" };
		char end4[40000] = { "            \"type\": \"MVLC_USB\"" };
		if (strcmp(ss, end3) == 0 || strcmp(ss, end4) == 0) {
			cout << "Find End!" << endl;
			break;
		}
	}
	raw->getline(ss, sizeof(ss));
	cout << ss << endl;
	raw->getline(ss, sizeof(ss));
	cout << ss << endl;
	raw->getline(ss, sizeof(ss));
	cout << ss << endl;
	//cout << "Done!!!" << endl;
	int num = 0;
	while (raw->get(str)) {
		num++;
		cout << hex << (int)(unsigned char)str << endl;
		if ((int)(unsigned char)str == 0x2) {
			cout << "Get02!!!" << endl;
			break;
		}
	}
	//cout << num << endl;
	raw->read((char*)buff, sizeof(unsigned int));
	//cout << hex << buff[0] << endl;
	raw->read((char*)buff, sizeof(unsigned int));
	//cout << hex << buff[0] << endl;
	raw->get(str);
	raw->get(str);
	raw->get(str);
	raw->read((char*)buff, sizeof(unsigned int));
	//cout << hex << buff[0] << endl;
	raw->read((char*)buff, sizeof(unsigned int));
	//cout << hex << buff[0] << endl;

	int timeNumber = 0;
	ULong64_t Same_Channel = 0;
	ULong64_t Same_Channel2 = 0;
	ULong64_t SCPNumber[255][32];
	ULong64_t QDCNumber[255][32];
	prev_time = 0;
	int Ch;

	//the number of particle of the SCP and QDC 
	ULong64_t SCP_Num = 0;
	ULong64_t QDC_Num = 0;
	
	//int errorNumber = 0;
	for (int nu1 =0 ; nu1<255;nu1++){
		for (int nu2 =0 ; nu2<32;nu2++){
			SCPNumber[nu1][nu2]=0;
			QDCNumber[nu1][nu2]=0;
		}
	}
	// begin to search and restore
	while (raw->read((char*)buff, sizeof(unsigned int))) {
		//initialization
		for (i = 0; i < 32; i++) {
			//MDPP32-SCP
			adc_ch[i] = 0;
			tdc_ch[i] = 0;
			
			adc_find[i] = false;
			cfd_find[i] = false;
			tdc_find = false;
			tdc_extend_find = false;


			//MDPP32-QDC
			adc_long_ch[i] = 0;
			adc_short_ch[i] = 0;

			qdc_long_find[i] = false;
			qdc_short_find[i] = false;
			qdc_cfd_find[i] = false;
			qdc_tdc_find = false;
			qdc_tdc_extend_find = false;
			
			time = 0;  // extend time stamp
			temp = 0; //time stamp + extend time stamp
			Ch = 0;  // channel
		}

		//if(buff[0] == 0xF5200000){
			//errorNumber++;
		//}

		// header_found flag
		if ((buff[0] >> 28) == 0x4) {
			ModSel = (buff[0] >> 16) & 0xff;  //module id
			rt = (buff[0] >> 13) & 0x7; //timestamp resolution
			ra = (buff[0] >> 10) & 0x7; //adc resolution
			EventSize = buff[0] & 0x3ff; // number of following data words
			//cout<<"ModSel"<<ModSel<<endl;
			if (ModSel < 0 || ModSel > 255) {
				cout << "Header " << ModSel << "  " << rt << "  " << ra << "  " << EventSize << endl;
				return false;
			}
			else {
				UInt_t EventCount = 0;  //the number we found
				int adc_Channel = -1; 
				int cfd_Channel = -1;
				if (ModuleType[ModSel] == 1) {
					vector<int> temp_string;
					while (EventCount < EventSize) {
						raw->read((char*)buff, sizeof(unsigned int));
						if((buff[0] >> 28) == 0x4){ // The actual existing data words are less than the event size
							// Reset and continue to search
							ModSel = (buff[0] >> 16) & 0xff;
							rt = (buff[0] >> 13) & 0x7; //timestamp resolution
							ra = (buff[0] >> 10) & 0x7; //adc resolution
							EventSize = buff[0] & 0x3ff; // Event Size
							EventCount = 0; // The number of the data event we found
							adc_Channel = -1;
							cfd_Channel = -1;
							for (i = 0; i < 32; i++) {
								adc_ch[i] = 0;
								tdc_ch[i] = 0;

								adc_long_ch[i] = 0;
								adc_short_ch[i] = 0;

								adc_find[i] = false;
								cfd_find[i] = false;
								tdc_find = false;
								tdc_extend_find = false;
								qdc_long_find[i] = false;
								qdc_short_find[i] = false;
								qdc_cfd_find[i] = false;
								qdc_tdc_find = false;
								qdc_tdc_extend_find = false;
								time = 0;
								temp = 0;
								Ch = 0;
								ChSel = 999;
								//tdc_ch_qdc[i] = 0;
							}
							continue;
						}
						if(buff[0] != 0xfa022002){   // it was found that a series of data words starting with '0xfa022002' appeared in the data event, 
						//such as EventSize: 5 EventCount: 5
						// 10221000 10020b3d 20000000 0 c0026082 f5200000 
						int tp = buff[0];
						temp_string.push_back(tp);
						//this data event belongs to SCP
						if (buff[0] >> 30 == 0x0) {
							// we found the extend time stamp (maybe???)
							if (buff[0] >> 28 == 0x2) {
								UInt_t temp_time = buff[0] & 0xffff;
								// Prevent the data words such as 
								//40 20 18 05 | 10 36 10 00 | 10 16 ff 76 | 2d 03 01 6e | 00 01 00 3a | 10 21 0f 95 | 10 01 ff ec | 20 00 00 00
								// 20 00 00 00 is overwrote by 2d 03 01 6e
								// The difference of the extend time stamps must be within 1
								if((((temp_time - prev_time) <= 1 && temp_time >= prev_time) || ((prev_time - temp_time) <= 1 && temp_time <= prev_time))
								&& tdc_extend_find == false){
								//if (tdc_extend_find && buff[0] & 0xffff< time) {
									tdc_extend_find = true;
									time = buff[0] & 0xffff;
									EventCount++;
									prev_time = buff[0] & 0xffff;
									//cout<<"!!!!"<<endl;
								//}
								// else if(tdc_extend_find == false){
								// 	tdc_extend_find = true;
								// 	time = buff[0] & 0xffff;
								// 	EventCount++;
								// }
								}
							}
							//data event
							else if (buff[0] >> 28 == 0x1) {
								Ch = buff[0] >> 16 & 0x7f;
								if (Ch <= 31) { //  ADC value
									adc_ch[Ch] = buff[0] & 0xffff;
									if (adc_find[Ch]) {
										Same_Channel++;
									}
									adc_find[Ch] = true;
									EventCount++;
									adc_Channel = Ch;
								}
								else if (Ch >= 32 && Ch <= 63) { // TDC time difference
									tdc_ch[Ch - 32] = buff[0] & 0xffff;
									cfd_find[Ch - 32] = true;
									//adc_find[ChSel-32] = true;
									EventCount++;
									cfd_Channel = Ch-32;
								}
								else { // Errot Channel , Sometimes, it gets 0x86,0x85.
									EventCount++;
									//cout << "SCP ChSel: " << dec << ChSel << endl;
									//SCP_ERROR++;
									/*
									cout << dec << "EventSize: " << EventSize << " ModSel: " << ModSel <<" ChSel: "<<ChSel<< " " <<hex << buff <<endl;
									for(int op = 0 ; op < EventSize ; op++){
										cout << hex << buff[op] <<" ";
									}
									cout<<endl;*/
								}
							}
							else if (buff[0] == 0x0) { //fill dummy
								EventCount++;
							}
						}
						else if (buff[0] >> 30 == 0x3) { // End of Event mark , get the time_stamp, but sometimes,it could not find it
							//if(EventCount == EventSize-1){
								tdc_find = true;
								//if(buff[i] >> 24 == 0xf5){
									//cout<< buff[i] <<endl;
								//}
								tdc = buff[0] & 0x3fffffff;
								EventCount++;
								timeNumber++;
							//}
						}
						}
						else{
							//  found the "0xfa022002", skip it!!!
							raw->read((char*)buff, sizeof(unsigned int));
							raw->read((char*)buff, sizeof(unsigned int));
							raw->read((char*)buff, sizeof(unsigned int));
							raw->read((char*)buff, sizeof(unsigned int));
						}
					}
					
					for (i = 0; i < 32; i++) {
						// We have found everything
						if (adc_find[i] && cfd_find[i] && tdc_find && tdc_extend_find) {
							//begin to restore
							SCP_Num++;
							SCPNumber[ModSel][i]++; // record
							adc = adc_ch[i]; // adc value
							temp = (ULong64_t)(tdc + time * pow(2, 30)); // calculate the actual time_stamp
							cfd = tdc_ch[i]; // TDC time difference
							ChSel = i; // channel
							adc_long = 0; // this beglongs to QDC, so i set it as 0
							adc_short = 0; // this beglongs to QDC, so i set it as 0
							tree->Fill();
						}
						else if (((adc_find[i] || cfd_find[i]) && (tdc_find || tdc_extend_find))) { // it found at least two data feature
							// Error , sometimes, it lost the timestamp . sometimes it lost the ADC or TDC. 
							// sometimes, the number of the data event is less than the event size 
						 	SCP_ERROR++;
							// debug log
							if(SCP_ERROR < 100){
								cout<<"SCP ERROR: "<<endl;
								cout<<"EventSize: "<< EventSize <<  " EventCount: " << EventCount  << " ChSel:" << ChSel <<endl;
								for (ULong64_t p = 0; p < temp_string.size(); p++) {
									cout << hex <<temp_string[p] << " ";
								}
								cout << endl;
								cout<< dec << "adc_Channel " << adc_Channel << " cfd_Channel " << cfd_Channel << endl;
								cout<<"i: " <<i << endl;
								cout<< adc_ch[i] << " " << tdc_ch[i] << endl;
								if(!adc_find[i]){
									cout << "adc null!!!" << endl;
								}
								if(!cfd_find[i]){
									cout << "cfd null!!!" << endl;
								}
								if(!tdc_find){
									cout << "tdc null!!!"<< endl;
								}
								if(!tdc_extend_find){
									cout << "tdc extend null!!!"<< endl;
								}
								cout << "tdc: "<< tdc << " extend time stamp: " << time << " prev extend time stamp: " << prev_time << endl;
								
							}
						}
						else{
							// cout<<"EventSize: "<< EventSize <<  " EventCount: " << EventCount <<endl;
							// for (int p = 0; p < temp_string.size(); p++) {
							// 	cout << hex <<temp_string[p] << " ";
							// }
							// cout << endl;
							// SCP_ERROR++;
						}
					}
				}
				// this data event belongs to QDC
				else if (ModuleType[ModSel] == 2) {
				//cout << "EventSize:" << EventSize << endl;
				vector<int> temp_string;
					while (EventCount < EventSize) {			
						raw->read((char*)buff, sizeof(unsigned int));
						// The actual existing data words are less than the event size
						// reset the data and continue to search
						if((buff[0] >> 28) == 0x4){
							ModSel = (buff[0] >> 16) & 0xff;
							rt = (buff[0] >> 13) & 0x7; //timestamp resolution
							ra = (buff[0] >> 10) & 0x7; //adc resolution
							EventSize = buff[0] & 0x3ff;
							EventCount = 0;
							adc_Channel = -1;
							cfd_Channel = -1;
							
							for (i = 0; i < 32; i++) {
								adc_ch[i] = 0;
								tdc_ch[i] = 0;

								adc_find[i] = false;
								cfd_find[i] = false;
								tdc_find = false;
								tdc_extend_find = false;

								adc_long_ch[i] = 0;
								adc_short_ch[i] = 0;

								qdc_long_find[i] = false;
								qdc_short_find[i] = false;
								qdc_cfd_find[i] = false;
								qdc_tdc_find = false;
								qdc_tdc_extend_find = false;

								time = 0;
								temp = 0;
								Ch = 0;
								ChSel = 999;
								//tdc_ch_qdc[i] = 0;
							}
							continue;
						}
						if(buff[0]!=0xfa022002)
						{
							//cout<<static_cast<const char*>(typeid(buff[0]).name())<<endl;
							int tp = buff[0];
							//cout<<"tp: "<<hex<<(char*)tp[0]<<" ";
							temp_string.push_back(tp);
							//cout << "EventCount:" << EventCount << endl;
							//cout << buff[0] << endl;
							if (buff[0] >> 30 == 0x0) {
								if (buff[0] >> 28 == 0x2) { // extend time stamp
									UInt_t temp_time = buff[0] & 0xffff;
									if((((temp_time - prev_time) <= 1 && temp_time >= prev_time) || ((prev_time - temp_time) <= 1 && temp_time <= prev_time))
									&& qdc_tdc_extend_find == false){
										qdc_tdc_extend_find = true;
										time = buff[0] & 0xffff;
										EventCount++;
										prev_time = buff[0] & 0xffff;
									}
								}

								else if (buff[0] >> 28 == 0x1) {
									Ch = buff[0] >> 16 & 0x7f;
									if (Ch < 32) { // adc value long intergration
										adc_long_ch[Ch] = buff[0] & 0xffff;
									
										if (qdc_long_find[Ch]) {
											Same_Channel2++;
										}
										qdc_long_find[Ch] = true;
										EventCount++;
									}
									else if (Ch >= 32 && Ch <= 63) { // TDC time difference
										tdc_ch[Ch - 32] = buff[0] & 0xffff;
										qdc_cfd_find[Ch - 32] = true;
										EventCount++;
									}
									else if (Ch >= 96 && Ch <= 127) { // adc value short intergration
										adc_short_ch[Ch - 96] = buff[0] & 0xffff;
										qdc_short_find[Ch - 96] = true;
										EventCount++;
									}
									else{
										EventCount++;
									}
								}
								else if (buff[0] == 0x0) { // fill dummy
									EventCount++;
								}
							}
							else if (buff[0] >> 30 == 0x3) { // time stamp
								//if(EventCount == EventSize-1){
								tdc = buff[0] & 0x3fffffff;
								qdc_tdc_find = true;
								EventCount++;
								//}
							}
							else {
							//cout << buff[0] <<endl;
							}
						}
						else
						{
							// skip
							raw->read((char*)buff, sizeof(unsigned int));
							raw->read((char*)buff, sizeof(unsigned int));
							raw->read((char*)buff, sizeof(unsigned int));
							raw->read((char*)buff, sizeof(unsigned int));
						}
					}
					for (i = 0; i < 32; i++) {
						// we have found everything
						if (qdc_long_find[i] && qdc_short_find[i] && qdc_cfd_find[i] && qdc_tdc_extend_find && qdc_tdc_find) {
							//restore
							QDC_Num++;
							QDCNumber[ModSel][i]++;
							adc_long = adc_long_ch[i];
							adc_short = adc_short_ch[i];
							//temp = (tdc);
							temp = (tdc + time * pow(2, 30));
							//dt_qdc = (tdc_qdc + time * pow(2, 30) - preTemp_qdc[i]) * 62.5;
							//preTemp_qdc[i] = tdc + time_qdc * pow(2, 30);
							cfd = tdc_ch[i];
							ChSel = i;
							adc = 0; // this belongs to SCP
							tree->Fill();
						}
						else if ((qdc_long_find[i] || qdc_short_find[i] || qdc_cfd_find[i] ) && (qdc_tdc_find || qdc_tdc_extend_find)) { //we have found at least two data features
							//debug log
							QDC_ERROR++;
							// if(QDC_ERROR < 100){
							// 	cout<<"QDC ERROR: "<<endl;
							// 	cout<<"EventSize: "<< EventSize <<  " EventCount: " << EventCount  << " ChSel:" << ChSel <<endl;
							// 	for (int p = 0; p < temp_string.size(); p++) {
							// 		cout << hex <<temp_string[p] << " ";
							// 	}
							// 	cout << endl;
							// 	cout<<"i: " <<i <<endl;
							// 	cout<< qdc_long_find[i] << " " << qdc_short_find[i] << " " << qdc_cfd_find[i] << endl;
							// 	if(!qdc_long_find[i]){
							// 		cout << "adc null!!!" << endl;
							// 	}
							// 	if(!qdc_short_find[i]){
							// 		cout << "cfd null!!!" << endl;
							// 	}
							// 	if(!qdc_cfd_find[i]){
							// 		cout << "cfd null!!!" << endl;
							// 	}
							// 	if(!qdc_tdc_find){
							// 		cout << "tdc null!!!" << endl;
							// 	}
							// 	if(!qdc_tdc_extend_find){
							// 		cout << "tdc extend null!!!" << endl;
							// 	}
							// 	cout << "tdc: "<< tdc << " extend time stamp: " << time << " prev extend time stamp: " << prev_time << endl;
								
							// }
						}
						else{
							//cout<<"EventSize: "<< EventSize <<  " EventCount: " << EventCount <<endl;
							//for (int p = 0; p < temp_string.size(); p++) {
								//cout << hex <<temp_string[p] << " ";
							//}
							//cout << endl;
							//cout<< qdc_long_find[i] << qdc_short_find[i] << qdc_cfd_find [i] << qdc_tdc_find <<endl;
							//cout << hex << time;
							//QDC_ERROR++;
						}
					}
				}
			}

		}
	}

	// output the result
	cout<< "SCP_Number: " << std::dec << SCP_Num << endl;
	cout<< "QDC_Number: " << std::dec << QDC_Num << endl;
	cout<< "SCP_ERROR: "<< std::dec <<SCP_ERROR<<endl;
	cout<< "QDC_ERROR: "<< std::dec <<QDC_ERROR<<endl;
	cout <<"timeNumber: " << std::dec<< timeNumber <<endl;
	cout <<"Same_Channel: "<<std::dec<<Same_Channel<<endl; 
	cout <<"Same_Channel2: "<<std::dec<<Same_Channel2<<endl; 
	for (int nu1 =0 ; nu1<255;nu1++){
		bool exist = false;
		//The statistics of MDPP32-SCP
		for (int nu2 =0 ; nu2<32;nu2++){
			if(SCPNumber[nu1][nu2]!=0){
				cout<<"Mod: "<<nu1<<" Channel: "<<nu2<< " Number: " << SCPNumber[nu1][nu2] <<" ";
				exist = true;
			}
		}
		if(exist){
			cout << endl;
		}
	}
	for (int nu1 =0 ; nu1<255;nu1++){
		bool exist = false;
		//The statistics of MDPP32-QDC
		for (int nu2 =0 ; nu2<32;nu2++){
			if(QDCNumber[nu1][nu2]!=0){
				cout << "Mod: " << nu1 << " Channel: " << nu2 << " Number: " << QDCNumber[nu1][nu2] <<" ";
				exist = true;
			}
		}
		if(exist){
			cout << endl;
		}
	}
	// End
	raw->close();
	fout->cd();
	tree->Write("",TObject::kOverwrite);
	//fout->Write();
	fout->Close();
	return true;
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
