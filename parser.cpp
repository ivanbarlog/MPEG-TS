#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <iostream>
#include <vector>
#include "parser.h"
#include "types.h"
#include <QString>

Parser::Parser() : m_file(NULL)
{
}

std::vector<PacketInfo> Parser::getPacketsInfo(QString filename)
{
    uint8_t read_data;
    std::vector<PacketInfo> list;

    m_file = fopen64(filename.toStdString().c_str(), "rb");

    //int i = 1;
    while (fread(&read_data, sizeof(uint8_t), 1, m_file) == 1)
    {
        //if (i == 31) break;
        if (read_data == SYNC_BYTE)
        {
            uint8_t arr2B[2] = {0};

            PacketInfo tmp;
            tmp.start = (uint64_t)(ftello64(m_file) - 1);

            int affected;
            for (int incr = 0; incr < 2; incr++)
            {
               affected = fread(&arr2B[incr], sizeof(uint8_t), 1, m_file);
               if (affected != 1)
               {
                   qDebug("Packet not complete.");
                   throw exception();
               }
            }

            uint16_t first2B = ((arr2B[0] << 8) | arr2B[1]);
            tmp.pid = first2B & 0x1fff;
            tmp.length = getPacketLength();

            list.push_back(tmp);



            printf("PID: %X, size of packet %d, packet started at %lu\n", tmp.pid, tmp.length, tmp.start);

            //i++;
        }
        else
        {
            continue;
        }
    }


    getTSHeader(list.at(0));

    return list;
}

int Parser::getPacketLength()
{
    if (m_file == NULL) return -1;

    int result;
    uint8_t sync;

    fseeko64(m_file, 185L, SEEK_CUR);
    result = fread(&sync, sizeof(uint8_t), 1, m_file);

    if (result != 1) return -1;
    if (sync == SYNC_BYTE)
    {
        fseeko64(m_file, -1L, SEEK_CUR);
        return 188;
    }


    fseeko64(m_file, 15L, SEEK_CUR);
    result = fread(&sync, sizeof(uint8_t), 1, m_file);

    if (result != 1) return -1;
    if (sync == SYNC_BYTE)
    {
        fseeko64(m_file, -1L, SEEK_CUR);
        return 204;
    }

    fseeko64(m_file, 3L, SEEK_CUR);
    result = fread(&sync, sizeof(uint8_t), 1, m_file);

    if (result != 1) return -1;
    if (sync == SYNC_BYTE)
    {
        fseeko64(m_file, -1L, SEEK_CUR);
        return 208;
    }

    return -1;
}

TSHeader Parser::getTSHeader(PacketInfo packetInfo)
{
    if (m_file == NULL)
    {
        qDebug("File not initialized.");
        throw exception();
    }

    TSHeader header;
    uint8_t read_data;
    int result = 0;

    //go to first byte of file
    rewind(m_file);
    //seek bytes with current packet
    fseeko64(m_file, packetInfo.start, SEEK_CUR);

    fread(&read_data, sizeof(uint8_t), 1, m_file);

    if (read_data == SYNC_BYTE)
    {
        std::cout << "Found SYNC_BYTE" << std::endl;

        uint8_t arr2B[2] = { 0 };

        for (int i = 0; i < 2; i++)
        {
            result = fread(&arr2B[i], sizeof(uint8_t), 1, m_file);
            if (result != 1)
            {
                qDebug("Something went wrong.");
                throw exception();
            }
        }

        uint16_t first2B;
        first2B = ((arr2B[0] << 8) | arr2B[1]);

        #define BIT16 0x8000
        #define BIT15 0x4000
        #define BIT14 0x2000

        header.transportErrorIndicator = !!(first2B & BIT16);
        header.payloadUnitStartIndicator = !!(first2B & BIT15);
        header.transportPriority = !!(first2B & BIT14);
        header.pid = first2B & 0x1fff;

        std::cout << "TEI: " << std::hex << static_cast<int>(header.transportErrorIndicator) << ", PUSI: " << std::hex << int(header.payloadUnitStartIndicator) << ", TP: " << std::hex << int(header.transportPriority) << std::endl;
        std::cerr << "Found PID: " << std::hex << static_cast<int>(header.pid) << ", Old PID: " << std::hex << static_cast<int>(packetInfo.pid) << std::endl;

        uint8_t lastB;
        result = fread(&lastB, sizeof(uint8_t), 1, m_file);
        if (result != 1)
        {
            qDebug("Something went wrong.");
            throw exception();
        }

        header.scramblingControl = (lastB >> 6) & 0x3;
        header.adaptationFieldControl = (lastB >> 4) & 0x3;
        header.continuityCounter = lastB & 0xf;

        std::cout << "SC: " << std::hex << static_cast<int>(header.scramblingControl) << ", AFC: " << std::hex << static_cast<int>(header.adaptationFieldControl) << ", CC: " << std::hex << static_cast<int>(header.continuityCounter) << std::endl;


        //AFC = 01 -> only payload
        //AFC = 10 -> only adaptation field
        //AFC = 11 -> payload & adaptation field

        if(header.adaptationFieldControl == 2 || header.adaptationFieldControl == 3)
        {
            AFHeader af;

            result = fread(&af.length, sizeof(uint8_t), 1, m_file);
            if (result != 1)
            {
                qDebug("Something went wrong.");
                throw exception();
            }

            if (af.length != 0)
            {
                result = fread(&read_data, sizeof(uint8_t), 1, m_file);
                if (result != 1)
                {
                    qDebug("Something went wrong.");
                    throw exception();
                }

                af.discontinuityIndicator = !!(read_data & (1 << 7));
                af.randomAccessIndicator = !!(read_data & (1 << 6));
                af.elementaryStreamPriorityIndicator = !!(read_data & (1 << 5));
                af.pcrFlag = !!(read_data & (1 << 4));
                af.opcrFlag = !!(read_data & (1 << 3));
                af.splicingPointFlag = !!(read_data & (1 << 2));
                af.transportPrivateDataFlag = !!(read_data & (1 << 1));
                af.extensionFlag = !!(read_data & 1);

                header.afheader = af;
            }
        }

        return header;
    }
    else
    {
        //cannot find SYNC_BYTE
        qDebug("Cannot find SYNC_BYTE.");
        throw exception();
    }
}
