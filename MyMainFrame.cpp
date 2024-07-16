#include <TMath.h>
#include <filesystem>
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

#include <mesytec-mvlc/mesytec-mvlc.h>

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

using namespace mesytec;

struct FilterWithCache
{
	mvlc::DataFilter filter;
	mvlc::CacheEntry addrCache; // for the 'A' marker to extract address values
	mvlc::CacheEntry dataCache; // for the 'D" marker to extract data values
};

FilterWithCache make_filter(const std::string &pattern)
{
	FilterWithCache result;
	result.filter = mvlc::make_filter(pattern);
	result.addrCache = mvlc::make_cache_entry(result.filter, 'A');
	result.dataCache = mvlc::make_cache_entry(result.filter, 'D');
	return result;
}

void fill_mdpp32_scp_data(MDPP32_SCP_Data &dest, const std::uint32_t *data, size_t size)
{
	static FilterWithCache fModuleId       = make_filter("0100 XXXX DDDD DDDD XXXX XXXX XXXX XXXX");
	static FilterWithCache fChannelTime    = make_filter("0001 XXXP O01A AAAA DDDD DDDD DDDD DDDD");
	static FilterWithCache fAmplitude      = make_filter("0001 XXXP O00A AAAA DDDD DDDD DDDD DDDD");
	static FilterWithCache fTriggerTime    = make_filter("0001 XXXX X100 000A DDDD DDDD DDDD DDDD");
	static FilterWithCache fTimeStamp      = make_filter("11DD DDDD DDDD DDDD DDDD DDDD DDDD DDDD");
	static FilterWithCache fExtentedTs     = make_filter("0010 XXXX XXXX XXXX DDDD DDDD DDDD DDDD");

    // Start from a clean state. Non-float values are set to 0, float values
    // are set to NaN initially. The NaNs are used to indicate that no value
    // was present in the current event.
	std::memset(&dest, 0, sizeof(dest));
	std::fill(std::begin(dest.channelTime), std::end(dest.channelTime), std::numeric_limits<float>::quiet_NaN());
	std::fill(std::begin(dest.amplitude), std::end(dest.amplitude), std::numeric_limits<float>::quiet_NaN());
	std::fill(std::begin(dest.triggerTime), std::end(dest.triggerTime), std::numeric_limits<float>::quiet_NaN());

	for (auto wordPtr = data, dataEnd = data + size; wordPtr < dataEnd; ++wordPtr)
	{
		if (mvlc::matches(fModuleId.filter, *wordPtr))
		{
			dest.moduleId = mvlc::extract(fModuleId.dataCache, *wordPtr);
		}
		else if (mvlc::matches(fChannelTime.filter, *wordPtr))
		{
			auto addr = mvlc::extract(fChannelTime.addrCache, *wordPtr);
			auto value = mvlc::extract(fChannelTime.dataCache, *wordPtr);
			dest.channelTime[addr] = value;
		}
		else if (mvlc::matches(fAmplitude.filter, *wordPtr))
		{
			auto addr = mvlc::extract(fAmplitude.addrCache, *wordPtr);
			auto value = mvlc::extract(fAmplitude.dataCache, *wordPtr);
			dest.amplitude[addr] = value;
		}
		else if (mvlc::matches(fTriggerTime.filter, *wordPtr))
		{
			auto addr = mvlc::extract(fTriggerTime.addrCache, *wordPtr);
			auto value = mvlc::extract(fTriggerTime.dataCache, *wordPtr);
			dest.triggerTime[addr] = value;
		}
		else if (mvlc::matches(fTimeStamp.filter, *wordPtr))
		{
			// 30 low bits of the timestamp
			dest.timestamp |= mvlc::extract(fTimeStamp.dataCache, *wordPtr);
		}
		else if (mvlc::matches(fExtentedTs.filter, *wordPtr))
		{
			// optional 16 high bits of the extended timestamp if enabled
			dest.timestamp |= mvlc::extract(fExtentedTs.dataCache, *wordPtr) << 30;
		}
	}
}

void fill_mdpp32_qdc_data(MDPP32_QDC_Data &dest, const std::uint32_t *data, size_t size)
{
	static FilterWithCache fModuleId         = make_filter("0100 XXXX DDDD DDDD XXXX XXXX XXXX XXXX");
	static FilterWithCache fChannelTime      = make_filter("0001 XXXX X01A AAAA DDDD DDDD DDDD DDDD");
	static FilterWithCache fIntegrationLong  = make_filter("0001 XXXX X00A AAAA DDDD DDDD DDDD DDDD");
	static FilterWithCache fIntegrationShort = make_filter("0001 XXXX X11A AAAA DDDD DDDD DDDD DDDD");
	static FilterWithCache fTriggerTime      = make_filter("0001 XXXX X100 000A DDDD DDDD DDDD DDDD");
	static FilterWithCache fTimeStamp        = make_filter("11DD DDDD DDDD DDDD DDDD DDDD DDDD DDDD");
	static FilterWithCache fExtentedTs       = make_filter("0010 XXXX XXXX XXXX DDDD DDDD DDDD DDDD");

    // Start from a clean state. Non-float values are set to 0, float values
    // are set to NaN initially. The NaNs are used to indicate that no value
    // was present in the current event.
	std::memset(&dest, 0, sizeof(dest));
	std::fill(std::begin(dest.channelTime), std::end(dest.channelTime), std::numeric_limits<float>::quiet_NaN());
	std::fill(std::begin(dest.integrationLong), std::end(dest.integrationLong), std::numeric_limits<float>::quiet_NaN());
	std::fill(std::begin(dest.integrationShort), std::end(dest.integrationShort), std::numeric_limits<float>::quiet_NaN());
	std::fill(std::begin(dest.triggerTime), std::end(dest.triggerTime), std::numeric_limits<float>::quiet_NaN());

	for (auto wordPtr = data, dataEnd = data + size; wordPtr < dataEnd; ++wordPtr)
	{
		if (mvlc::matches(fModuleId.filter, *wordPtr))
		{
			dest.moduleId = mvlc::extract(fModuleId.dataCache, *wordPtr);
		}
		else if (mvlc::matches(fChannelTime.filter, *wordPtr))
		{
			auto addr = mvlc::extract(fChannelTime.addrCache, *wordPtr);
			auto value = mvlc::extract(fChannelTime.dataCache, *wordPtr);
			dest.channelTime[addr] = value;
		}
		else if (mvlc::matches(fIntegrationLong.filter, *wordPtr))
		{
			auto addr = mvlc::extract(fIntegrationLong.addrCache, *wordPtr);
			auto value = mvlc::extract(fIntegrationLong.dataCache, *wordPtr);
			dest.integrationLong[addr] = value;
		}
		else if (mvlc::matches(fIntegrationShort.filter, *wordPtr))
		{
			auto addr = mvlc::extract(fIntegrationShort.addrCache, *wordPtr);
			auto value = mvlc::extract(fIntegrationShort.dataCache, *wordPtr);
			dest.integrationShort[addr] = value;
		}
		else if (mvlc::matches(fTriggerTime.filter, *wordPtr))
		{
			auto addr = mvlc::extract(fTriggerTime.addrCache, *wordPtr);
			auto value = mvlc::extract(fTriggerTime.dataCache, *wordPtr);
			dest.triggerTime[addr] = value;
		}
		else if (mvlc::matches(fTimeStamp.filter, *wordPtr))
		{
			// 30 low bits of the timestamp
			dest.timestamp |= mvlc::extract(fTimeStamp.dataCache, *wordPtr);
		}
		else if (mvlc::matches(fExtentedTs.filter, *wordPtr))
		{
			// optional 16 high bits of the extended timestamp if enabled
			dest.timestamp |= mvlc::extract(fExtentedTs.dataCache, *wordPtr) << 30;
		}
	}
}

MyExperiment::MyExperiment()
	: TObject()
{
	// Module fill functions by module index. handle_one_module() calls these.
	moduleFillFunctions =
	{
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_qdc_data(exp.MDPP32_QDCTOF1TOF2, data, dataSize);
		},
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_scp_data(exp.MDPP32_SCPsi0, data, dataSize);
		},
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_scp_data(exp.MDPP32_SCPSI1_xiao, data, dataSize);
		},
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_scp_data(exp.MDPP32_SCPSI2_xiao, data, dataSize);
		},
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_scp_data(exp.MDPP32_SCPSI3_xiao, data, dataSize);
		},
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_scp_data(exp.MDPP32_SCPSI4_xiao, data, dataSize);
		},
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_scp_data(exp.MDPP32_SCPSI1_da, data, dataSize);
		},
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_scp_data(exp.MDPP32_SCPSI2_da, data, dataSize);
		},
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_scp_data(exp.MDPP32_SCPSI3_da, data, dataSize);
		},
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_scp_data(exp.MDPP32_SCPSI4_da, data, dataSize);
		},
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_scp_data(exp.MDPP32_SCPSI5, data, dataSize);
		},
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_scp_data(exp.MDPP32_SCPSI6, data, dataSize);
		},
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_scp_data(exp.MDPP32_SCPsi7, data, dataSize);
		},
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_scp_data(exp.MDPP32_SCPHPGe, data, dataSize);
		},
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_qdc_data(exp.MDPP32_QDCCSI25, data, dataSize);
		},
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_qdc_data(exp.MDPP32_QDCPSD, data, dataSize);
		},
		[] (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)
		{
			fill_mdpp32_qdc_data(exp.MDPP_32QDCBGO, data, dataSize);
		},
	};
}

MyExperiment::~MyExperiment() {}

// Passed to the readout parser callbacks. Put anything you need inside the
// callback functions into this struct. Be careful as the readout_parser runs in
// a different thread, so the callbacks are invoked from within that thread.
struct MyUserContext
{
	mvlc::CrateConfig crateConfig;
	MyExperiment experiment;
	TFile *rootOutFile = nullptr;
	TTree *outputTree = nullptr;
};

void handle_one_module(MyUserContext *ctx, int crateIndex, int eventIndex, int moduleIndex,
	mvlc::ModuleData moduleData)
{
	auto eventReadoutCommands = ctx->crateConfig.stacks[eventIndex];
    auto moduleName = eventReadoutCommands.getGroup(moduleIndex).name;

	fmt::print("  moduleIndex={}, moduleName={}, eventSize={} words\n",
		moduleIndex, moduleName, moduleData.dynamicSize);

    mvlc::util::log_buffer(std::cout, moduleData.data.data, moduleData.data.size,
        fmt::format("readout data of module {}", moduleName));

	if (moduleIndex < ctx->experiment.moduleFillFunctions.size())
	{
		auto &fillFunction = ctx->experiment.moduleFillFunctions[moduleIndex];
		fillFunction(ctx->experiment, moduleIndex, moduleData.data.data, moduleData.data.size);
	}
	// else if the index is out of range it is the "readout end" script
}

void handle_readout_event(void *userContext_, int crateIndex, int eventIndex,
	const mvlc::ModuleData *moduleDataList, unsigned moduleCount)
{
	fmt::print("readout_event: crateIndex={}, eventIndex={}, moduleCount={}\n",
		crateIndex, eventIndex, moduleCount);

	auto ctx = reinterpret_cast<MyUserContext *>(userContext_);

	if (static_cast<size_t>(eventIndex) >= ctx->crateConfig.stacks.size())
	{
		fmt::print("Error: eventIndex {} out of range\n", eventIndex);
		return;
	}

	for (unsigned moduleIndex=0; moduleIndex < moduleCount; ++moduleIndex)
	{
		auto moduleData = moduleDataList[moduleIndex];
		handle_one_module(ctx, crateIndex, eventIndex, moduleIndex, moduleData);
	}

	// All modules have been handled, the structures in MyExperiment are filled,
	// time to copy the data to the output tree.
	ctx->outputTree->Fill();
}

void handle_system_event(void *userContext_, int crateIndex, const std::uint32_t *header, std::uint32_t size)
{
	auto ctx = reinterpret_cast<MyUserContext *>(userContext_);

	fmt::print("system_event: crate{}, header={:#08x}, size={} words, type={}\n",
		crateIndex, header[0], size, mvlc::decode_frame_header(header[0]));
}

bool MyMainFrame::restore_ROOT(string inputFilename)
{
	try
	{
		auto replay = mvlc::make_mvlc_replay(inputFilename);
		auto crateConfig = replay.crateConfig();

		std::cout << fmt::format(">>> Begin CrateConfig read from listfile {}:\n", inputFilename);
		std::cout << mvlc::to_yaml(crateConfig) << "\n";
		std::cout << fmt::format("<<< End CrateConfig\n");

		auto rootOutputFilename = std::filesystem::path(inputFilename).replace_extension(".root").string();
		fmt::print("Creating ROOT output file: {}\n", rootOutputFilename);

		MyUserContext ctx = {};
		ctx.crateConfig = crateConfig;
		ctx.rootOutFile = new TFile(rootOutputFilename.c_str(), "RECREATE");
		ctx.outputTree = new TTree("event0", "event0");
		ctx.outputTree->Branch("MyExperiment", &ctx.experiment, 32000, 2);

		mvlc::readout_parser::ReadoutParserCallbacks callbacks =
		{
			handle_readout_event,
			handle_system_event
		};

        replay.setParserCallbacks(callbacks, &ctx);

		if (auto ec = replay.start())
		{
			fmt::print("Error starting replay: {}\n", ec.message());
			return false;
		}

		while (!replay.finished())
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

        fmt::print("Replay is done. Final parser counters:\n");
		auto parserCounters = replay.parserCounters();
		mvlc::readout_parser::print_counters(std::cout, parserCounters);

		fmt::print("Closing output tree and file\n");
		ctx.outputTree->Write("",TObject::kOverwrite);
		ctx.rootOutFile->Write();
		fmt::print("Output tree and file closed.\n");

        return true;
	}
	catch(const std::exception& e)
	{
        std::cerr << fmt::format("Error replaying data from {}: {}\n", inputFilename, e.what());
		return false;
	}
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
