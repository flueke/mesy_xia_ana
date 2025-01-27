#include "MyExperiment.h"

#include <filesystem>

#include <mesytec-mvlc/mesytec-mvlc.h>
#include <TFile.h>
#include <TTree.h>

using namespace mesytec;

// Helpers for extracting address and data values.

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
			auto value = mvlc::extract(fTimeStamp.dataCache, *wordPtr);
			//spdlog::info("timestamp matched (30 low bits): 0b{:030b}, 0x{:08x}", value, value);
			dest.timestamp |= value;
		}
		else if (mvlc::matches(fExtentedTs.filter, *wordPtr))
		{
			// optional 16 high bits of the extended timestamp if enabled
			auto value = mvlc::extract(fExtentedTs.dataCache, *wordPtr);
			//spdlog::info("extended timestamp matched (16 high bits): 0b{:016b}, 0x{:08x}", value, value);
			dest.timestamp |= static_cast<std::uint64_t>(value) << 30;
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
    // was present in the current event for the respective index.
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
			auto value = mvlc::extract(fTimeStamp.dataCache, *wordPtr);
			//spdlog::info("timestamp matched (30 low bits): 0b{:030b}, 0x{:08x}", value, value);
			dest.timestamp |= value;
		}
		else if (mvlc::matches(fExtentedTs.filter, *wordPtr))
		{
			// optional 16 high bits of the extended timestamp if enabled
			auto value = mvlc::extract(fExtentedTs.dataCache, *wordPtr);
			//spdlog::info("extended timestamp matched (16 high bits): 0b{:016b}, 0x{:08x}", value, value);
			dest.timestamp |= static_cast<std::uint64_t>(value) << 30;
		}
	}
}

MyExperiment::MyExperiment()
	: TObject()
{
	// Module fill functions by module index. handle_one_module() calls these.
	// This needs to be updated when the mvme VME config changes!
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

 #if 1
	// Verbose printing of extracted module data.
	fmt::print("  moduleIndex={}, moduleName={}, eventSize={} words\n",
		moduleIndex, moduleName, moduleData.dynamicSize);

    mvlc::util::log_buffer(std::cout, moduleData.data.data, moduleData.data.size,
        fmt::format("readout data of module {}", moduleName));
#endif

	// Lookup the fill function by moduleIndex, then call it passing in the raw module data buffer.
	if (static_cast<size_t>(moduleIndex) < ctx->experiment.moduleFillFunctions.size())
	{
		auto &fillFunction = ctx->experiment.moduleFillFunctions[moduleIndex];
		fillFunction(ctx->experiment, moduleIndex, moduleData.data.data, moduleData.data.size);
	}
	// Else if the index is out of range it is the "readout end" script or new
	// modules have been added to the mvme config and this code needs to be
	// udpated.
}

// Called for every event extracted from the input listfile.
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

// System events contain configuration information, DAQ start/stop/pause/resume
// info and periodic wallclock timeticks. These are just printed for now.
void handle_system_event(void *userContext_, int crateIndex, const std::uint32_t *header, std::uint32_t size)
{
	// Downcast of the context object if acces to it is needed. Careful as
	// handle_system_event() is invoked from within the readout_parser thread!.
	//auto ctx = reinterpret_cast<MyUserContext *>(userContext_);

	fmt::print("system_event: crate{}, header={:#08x}, size={} words, type={}\n",
		crateIndex, header[0], size, mvlc::decode_frame_header(header[0]));
}

bool process_one_listfile(const std::string &inputFilename)
{
	try
	{
		// Opens the file, reads configuration data and creates a fresh MVLCReplay instance.
		auto replay = mvlc::make_mvlc_replay(inputFilename);

		// This is the mesytec-mvlc library CrateConfig that was written by mvme
		// when the listfile was created.
		auto crateConfig = replay.crateConfig();

		// Print out the YAML CrateConfig data.
		std::cout << fmt::format(">>> Begin CrateConfig read from listfile {}:\n", inputFilename);
		std::cout << mvlc::to_yaml(crateConfig) << "\n";
		std::cout << fmt::format("<<< End CrateConfig\n");

		auto rootOutputFilename = std::filesystem::path(inputFilename).replace_extension(".root").string();
		fmt::print("Creating ROOT output file: {}\n", rootOutputFilename);

		// Initialize our local context object. This is the 'userContext' for
		// the readout_parser component.
		MyUserContext ctx = {};
		ctx.crateConfig = crateConfig;
		ctx.rootOutFile = new TFile(rootOutputFilename.c_str(), "RECREATE");
		ctx.outputTree = new TTree("event0", "event0");
		ctx.outputTree->Branch("MyExperiment", &ctx.experiment, 32000, 2);

		// Setup the callbacks for the readout parser.
		mvlc::readout_parser::ReadoutParserCallbacks callbacks =
		{
			handle_readout_event,
			handle_system_event
		};

        replay.setParserCallbacks(callbacks, &ctx);

		// Start the replay. This processes data in its own threads.
		if (auto ec = replay.start())
		{
			fmt::print("Error starting replay: {}\n", ec.message());
			return false;
		}

		// Periodically check if the replay is done. When writing a GUI this
		// could be done using a timer.
		while (!replay.finished())
			std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // Print final parser counters. This information is also available in
        // the mvme analysis under 'Debug & Stats'.
        fmt::print("Replay is done. Final parser counters:\n");
		auto parserCounters = replay.parserCounters();
		mvlc::readout_parser::print_counters(std::cout, parserCounters);

		// Flush output data.
		fmt::print("Closing output tree and file\n");
		ctx.rootOutFile->cd();
		ctx.outputTree->Write("",TObject::kOverwrite);
		ctx.rootOutFile->Close();
		fmt::print("Output tree and file closed.\n");

        return true;
	}
	catch(const std::exception& e)
	{
		// Some parts of mesytec-mvlc still throw. Exceptions are slowly being
		// phased out though.
        std::cerr << fmt::format("Error replaying data from {}: {}\n", inputFilename, e.what());
		return false;
	}
}
