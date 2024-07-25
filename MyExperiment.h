#ifndef F49FE770_A261_4ACB_8239_A9F1729833EC
#define F49FE770_A261_4ACB_8239_A9F1729833EC

#include <TObject.h>
#include <vector>
#include <cstdint>
#include <functional>

// The bit filters where copied from the mvme default filters for the respective
// module. Timestamp handling works for both the standard and extended
// timestamps.

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

using FillFunction = std::function<void (
    MyExperiment &exp, unsigned moduleIndex, const std::uint32_t *data, size_t dataSize)>;

// The full experiment containing all the modules defined in 'event0' in mvme.
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
    std::vector<FillFunction> moduleFillFunctions; // ! Do not serialize this.

    MyExperiment();
    ~MyExperiment();

    ClassDef(MyExperiment, 1);
};

// Main entry point for processing one input .zip listfile archive.
bool process_one_listfile(const std::string &inputFilename);

#endif /* F49FE770_A261_4ACB_8239_A9F1729833EC */
