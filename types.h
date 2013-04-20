#ifndef TYPES_H
#define TYPES_H

#define SYNC_BYTE 0x47

#include <stdint.h>
#include <inttypes.h>

using namespace std;

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
    AFHeader afheader;
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

#endif // TYPES_H
