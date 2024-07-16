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

// mdpp32_qdc
//   channel time       0001 XXXX X01A AAAA DDDD DDDD DDDD DDDD
//   integration long   0001 XXXX X00A AAAA DDDD DDDD DDDD DDDD
//   integration short  0001 XXXX X11A AAAA DDDD DDDD DDDD DDDD
//   trigger time       0001 XXXX X100 000A DDDD DDDD DDDD DDDD
//   timestamp:         11DD DDDD DDDD DDDD DDDD DDDD DDDD DDDD
//   extended ts:       0010 XXXX XXXX XXXX DDDD DDDD DDDD DDDD
struct MDPP32_QDC_Data
{
    std::uint8_t moduleId;
    float channelTime[32];
    float integrationLong[32];
    float integrationShort[32];
    float triggerTime[2];
    std::uint64_t timestamp;
};

// mdpp32_scp
//   amplitude          0001 XXXP O00A AAAA DDDD DDDD DDDD DDDD
//   channel time       0001 XXXP O01A AAAA DDDD DDDD DDDD DDDD
//   trigger time       0001 XXXX X100 000A DDDD DDDD DDDD DDDD
//   timestamp          11DD DDDD DDDD DDDD DDDD DDDD DDDD DDDD
//   extended ts        0010 XXXX XXXX XXXX DDDD DDDD DDDD DDDD
struct MDPP32_SCP_Data
{
    std::uint8_t moduleId;
    float channelTime[32];
    float amplitude[32];
    float triggerTime[2];
    std::uint64_t timestamp;
};

struct MyExperiment;

using FillFunction = std::function<void (MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)>;

struct MyExperiment: public TObject
{
    MDPP32_QDC_Data MDPP32_QDCTOF1TOF2;
    MDPP32_SCP_Data MDPP32_SCPsi0;
    MDPP32_SCP_Data MDPP32_SCPSI1_xiao;
    MDPP32_SCP_Data MDPP32_SCPSI2_xiao;
    MDPP32_SCP_Data MDPP32_SCPSI3_xiao;
    MDPP32_SCP_Data MDPP32_SCPSI4_xiao;
    MDPP32_SCP_Data MDPP32_SCPSI1_da;
    MDPP32_SCP_Data MDPP32_SCPSI2_da;
    MDPP32_SCP_Data MDPP32_SCPSI3_da;
    MDPP32_SCP_Data MDPP32_SCPSI4_da;
    MDPP32_SCP_Data MDPP32_SCPSI5;
    MDPP32_SCP_Data MDPP32_SCPSI6;
    MDPP32_SCP_Data MDPP32_SCPsi7;
    MDPP32_SCP_Data MDPP32_SCPHPGe;
    MDPP32_QDC_Data MDPP32_QDCCSI25;
    MDPP32_QDC_Data MDPP32_QDCPSD;
    MDPP32_QDC_Data MDPP_32QDCBGO;

    // Module fill functions by module index. handle_one_module() calls these.
    std::vector<FillFunction> moduleFillFunctions;

    MyExperiment();
    ~MyExperiment() override;

    ClassDef(MyExperiment, 1);
};

#endif
