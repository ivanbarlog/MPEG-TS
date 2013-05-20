#ifndef TYPES_H
#define TYPES_H

#define SYNC_BYTE 0x47

#include <stdint.h>
#include <inttypes.h>
#include <QHash>
#include <QString>

using namespace std;

class StreamType
{
public:
    StreamType()
    {
        m_types.insert(0x1, "ISO/IEC 11172 Video (MPEG-1)");
        m_types.insert(0x2, "ITU-T Rec. H.262 | ISO/IEC 13818-2 (MPEG-2) Video or ISO/IEC 11172-2 (MPEG-1) constrained parameter video stream");
        m_types.insert(0x3, "ISO/IEC 11172 Audio (MPEG-1) NOT USED by ATSC");
        m_types.insert(0x4, "ISO/IEC 13818-3 Audio (MPEG-2) NOT USED by ATSC");
        m_types.insert(0x5, "MPEG-2 private table sections");
        m_types.insert(0x6, "MPEG-2 Packetized Elementary Stream packets containing private data");
        m_types.insert(0x7, "MHEG Packets");
        m_types.insert(0x8, "MPEG-2 Annex A DSM CC");
        m_types.insert(0x9, "ITU-T Rec. H.222.1");
        m_types.insert(0xA, "ISO/IEC 13818-6 type A");
        m_types.insert(0xB, "ISO/IEC 13818-6 type B");
        m_types.insert(0xC, "ISO/IEC 13818-6 type C");
        m_types.insert(0xD, "ISO/IEC 13818-6 type D");
        m_types.insert(0xE, "ISO/IEC 13818-1 (MPEG-2) auxiliary");
        m_types.insert(0xF, "ISO/IEC 13818-7 Audio with ADTS transport syntax");
        m_types.insert(0x10, "ISO/IEC 14496-2 (MPEG-4) Visual");
        m_types.insert(0x11, "ISO/IEC 14496-3 Audio with the LATM transport syntax as defined in ISO/IEC 14496-3 / AMD 1");
        m_types.insert(0x12, "ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in PES packets");
        m_types.insert(0x13, "ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in ISO/IEC14496_sections.");
        m_types.insert(0x14, "ISO/IEC 13818-6 Synchronized Download Protocol");
        m_types.insert(0x15, "Metadata carried in PES packets");
        m_types.insert(0x16, "Metadata carried in metadata_sections");
        m_types.insert(0x81, "ATSC AC-3 Audio");
        //m_types.insert(0x80-0xFF, "MPEG-2 User Private");
    }

    QHash<uint8_t, QString> getStreamTypes()
    {
        return m_types;
    }

private:
    QHash<uint8_t, QString> m_types;
};


//Adaptation Field Header
class AFHeader
{
public:
    uint8_t length;
    /* If length > 0 */
    uint8_t discontinuityIndicator:1;
    uint8_t randomAccessIndicator:1;
    uint8_t elementaryStreamPriorityIndicator:1;
    uint8_t pcrFlag:1;
    uint8_t opcrFlag:1;
    uint8_t splicingPointFlag:1;
    uint8_t transportPrivateDataFlag:1;
    uint8_t extensionFlag:1;
    /* If splicingPointFlag == 1 */
    int8_t spliceCountdown;
    /* If transportPrivateDataFlag == 1 */
    uint8_t transportPrivateDataLength;
};

//Transport Stream Header
class TSHeader
{
public:
    uint8_t transportErrorIndicator:1;
    uint8_t payloadUnitStartIndicator:1;
    uint8_t transportPriority:1;
    uint16_t pid:13;
    uint8_t scramblingControl:2;
    uint8_t adaptationFieldControl:2;
    uint8_t continuityCounter:4;
};

class PCR
{
public:
    uint64_t referenceBase:33;
    uint8_t reserved:6;
    uint16_t referenceExtension:9;
};

class OPCR
{
public:
    uint64_t referenceBase:33;
    uint8_t reserved:6;
    uint16_t referenceExtension:9;
};

//Adaptation Field Extension
class AFExtension
{
    uint8_t length;
    uint8_t ltwFlag:1;
    uint8_t piecewiseRateFlag:1;
    uint8_t seamlessSpliceFlag:1;
    /* If ltw_flag == 1 */
    uint8_t ltwValidFlag:1;
    uint16_t ltwOffset:15;
    /* If piecewise_rate_flag == 1 */
    uint8_t piecewiseReserved:2;
    uint32_t piecewiseRate:22;
    /* If seamless_splice_flag == 1 */
    uint8_t spliceType:4;
    uint64_t dtsNextAu:33;
};

class PacketInfo
{
public:
    uint64_t start;
    int length;
    uint16_t pid:13;
};

class TSPacket
{
public:
    TSHeader tsheader;
    AFHeader afheader;
    AFExtension afextension;
};

class PAT //program association table
{
public:
    uint8_t pointerField;
    uint8_t tableId;
    uint8_t sectionSyntaxIndicator:1;
    //uint8_t patFlag:1; //always set to 0
    //uint8_t reserved:2; //always set to binary 3 - 11
    // two bits which must be 00
    uint16_t sectionLength:10;
    uint16_t transportStreamId;
    //uint8_t reserved:2; //always set to binary 3 - 11
    uint8_t versionNumber:5;
    uint8_t nextIndicator:1;
    uint8_t sectionNumber;
    uint8_t lastSectionNumber;

    int programCount;
};

class PMTProgram
{
public:
    uint8_t streamType;
    uint16_t elementaryPID:13;
    uint16_t esInfoLength:10;
};

class PMT //program map table
{
public:
    uint8_t pointerField;
    uint8_t tableId; //0x02 for PMT
    uint8_t sectionSyntaxIndicator:1;
    //uint8_t patFlag:1; //always set to 0
    //uint8_t reserved:2; //always set to binary 3 - 11
    // two bits which must be 00
    uint16_t sectionLength:10;
    uint16_t programNum;
    //uint8_t reserved:2; //always set to binary 3 - 11
    uint8_t versionNumber:5;
    uint8_t nextIndicator:1;
    //uint8_t sectionNumber; //always 0x0
    //uint8_t lastSectionNumber; //always 0x0
    //uint8_t reserved:3;
    uint16_t pcrPID:13;
    //uint8_t reserved:4;
    // two bits which must be 00
    uint16_t programInfoLength:10;

    uint16_t transportStreamId;
    //uint8_t reserved:2; //always set to binary 3 - 11
    //uint8_t versionNumber:5;
    //uint8_t nextIndicator:1;
    uint8_t sectionNumber;
    uint8_t lastSectionNumber;

    std::vector<PMTProgram> programs;
};

class Program
{
public:
    uint16_t programNumber;
    uint16_t programPID:13;
    bool filledPMT;
    PMT pmt;
};


#endif // TYPES_H
