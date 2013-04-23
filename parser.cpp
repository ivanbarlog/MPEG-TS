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

std::vector<PacketInfo> Parser::getPacketList(QString filename)
{
    uint8_t read_data;
    std::vector<PacketInfo> list;

    m_file = fopen64(filename.toStdString().c_str(), "rb");

    while (fread(&read_data, sizeof(uint8_t), 1, m_file) == 1)
    {
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

            if (tmp.pid == 0x0000)
            {
                m_patPackets.push_back(tmp);
            }


            //printf("PID: %X, size of packet %d, packet started at %lu\n", tmp.pid, tmp.length, tmp.start);

        }
        else
        {
            continue;
        }
    }

    /* parse Program Association Table */
    m_pat = getPAT();



    return list;
}

std::vector<Program> Parser::getPAT(/*PacketInfo packetInfo*/)
{
    if (m_file == NULL)
    {
        qDebug("File not initialized.");
        throw exception();
    }

    //PAT packet;
    uint8_t read_data;
    int result = 0;
    std::vector<PacketInfo> v = m_patPackets;
    std::vector<Program> programs;

    for (std::vector<PacketInfo>::iterator it = v.begin(); it != v.end(); ++it)
    {
        PacketInfo p = *it;

        //go to first byte of file
        rewind(m_file);
        //seek bytes with current packet
        fseeko64(m_file, p.start, SEEK_CUR);

        fread(&read_data, sizeof(uint8_t), 1, m_file);

        if (read_data == SYNC_BYTE)
        {
            //std::cout << "Found SYNC_BYTE" << std::endl;

            for (int i = 0; i < 3; i++)
            {
                result = fread(&read_data, sizeof(uint8_t), 1, m_file);
                if (result != 1)
                {
                    qDebug("Something went wrong.");
                    throw exception();
                }
            }

            // get adaptation control field to determine if adaptation field or payload is present
            uint8_t adaptationFieldControl;
            adaptationFieldControl = (read_data >> 4) & 0x3;

            //AFC = 01 -> only payload
            //AFC = 10 -> only adaptation field
            //AFC = 11 -> payload & adaptation field

            //PAT packets contains only payload
            if (adaptationFieldControl == 1)
            {
                PAT tmp;

                //std::cout << "I am in, creating PAT instance" << std::endl;

                result = fread(&read_data, sizeof(uint8_t), 1, m_file);
                if (result != 1)
                {
                    qDebug("Something went wrong.");
                    throw exception();
                }

                tmp.pointerField = read_data;

                //std::cout << "PointerField: " << std::hex << tmp.pointerField << std::endl;

                result = fread(&read_data, sizeof(uint8_t), 1, m_file);
                if (result != 1)
                {
                    qDebug("Something went wrong.");
                    throw exception();
                }

                tmp.tableId = read_data;

                //std::cout << "TableId: " << std::hex << tmp.tableId << std::endl;

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

                uint16_t sorted2B;
                sorted2B = ((arr2B[0] << 8) | arr2B[1]);

                #define BIT16 0x8000
                #define BIT15 0x4000

                if (!!(sorted2B & BIT16) == 1 && !!(sorted2B & BIT15) == 0)
                {
                    qDebug("This is PAT packet.");
                }
                else
                {
                    qDebug("Not PAT packet - continue...");
                    std::cout << "Not PAT packet: " << std::endl;
                    std::cout << "1 ? " << std::hex << !!(sorted2B & BIT16) << std::endl;
                    std::cout << "0 ? " << std::hex << !!(sorted2B & BIT15) << std::endl;
                    continue;
                }

                tmp.sectionLength = sorted2B & 0x03FF;

                for (int i = 0; i < 2; i++)
                {
                    result = fread(&arr2B[i], sizeof(uint8_t), 1, m_file);
                    if (result != 1)
                    {
                        qDebug("Something went wrong.");
                        throw exception();
                    }
                }

                sorted2B = ((arr2B[0] << 8) | arr2B[1]);

                tmp.transportStreamId = sorted2B;

                result = fread(&read_data, sizeof(uint8_t), 1, m_file);
                if (result != 1)
                {
                    qDebug("Something went wrong.");
                    throw exception();
                }

                /*
                 * NEED TO BE FIXED - compare 2 bits
                 *
                 *if (!!(read_data & (3 << 6)) == 3)
                {
                    qDebug("This is PAT packet.");
                }
                else
                {
                    qDebug("Not PAT packet - continue...");
                    std::cout << "11 ? " << std::dec << !!(read_data & (3 << 6)) << std::endl;
                    continue;
                }*/

                tmp.versionNumber = !!(read_data & (31 << 1));
                tmp.nextIndicator = !!(read_data & 1);

                std::cout << "VersionNumber: " << std::hex << tmp.versionNumber << ", nextIndicator: " << std::hex << tmp.nextIndicator << std::endl;

                result = fread(&read_data, sizeof(uint8_t), 1, m_file);
                if (result != 1)
                {
                    qDebug("Something went wrong.");
                    throw exception();
                }

                tmp.sectionNumber = read_data;

                result = fread(&read_data, sizeof(uint8_t), 1, m_file);
                if (result != 1)
                {
                    qDebug("Something went wrong.");
                    throw exception();
                }

                tmp.lastSectionNumber = read_data;

                int countOfPrograms = (tmp.sectionLength - 5 - 4) / 4;


                programs = parsePATPrograms(countOfPrograms);

                std::cout << "Program table: ***********************" << std::endl;
                for (std::vector<Program>::iterator itr = programs.begin(); itr != programs.end(); ++itr)
                {
                    Program tmp = *itr;
                    std::cout << "Program number: " << std::hex << tmp.programNumber << "; Program PID: " << std::hex<< tmp.programPID << std::endl;
                }
                std::cout << "**************************************" << std::endl;

            }
        }
        else
        {
            //cannot find SYNC_BYTE
            qDebug("Cannot find SYNC_BYTE.");
            throw exception();
        }


    }

    return programs;
}

std::vector<Program> Parser::parsePATPrograms(int programCount)
{
    std::vector<Program> v;

    int result = 0;
    uint8_t arr2B[2] = { 0 };
    uint16_t sorted2B;

    for (int i = 0; i < programCount; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            result = fread(&arr2B[j], sizeof(uint8_t), 1, m_file);
            if (result != 1)
            {
                qDebug("Something went wrong.");
                throw exception();
            }
        }

        sorted2B = ((arr2B[0] << 8) | arr2B[1]);

        Program tmp;

        tmp.programNumber = sorted2B;

        for (int j = 0; j < 2; j++)
        {
            result = fread(&arr2B[j], sizeof(uint8_t), 1, m_file);
            if (result != 1)
            {
                qDebug("Something went wrong.");
                throw exception();
            }
        }

        sorted2B = ((arr2B[0] << 8) | arr2B[1]);

        tmp.programPID = sorted2B & 0x1fff;

        v.push_back(tmp);
    }

    return v;
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

TSPacket Parser::getTSPacket(PacketInfo packetInfo)
{
    if (m_file == NULL)
    {
        qDebug("File not initialized.");
        throw exception();
    }

    TSPacket packet;
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

        TSHeader ts;

        uint16_t first2B;
        first2B = ((arr2B[0] << 8) | arr2B[1]);

        #define BIT16 0x8000
        #define BIT15 0x4000
        #define BIT14 0x2000

        ts.transportErrorIndicator = !!(first2B & BIT16);
        ts.payloadUnitStartIndicator = !!(first2B & BIT15);
        ts.transportPriority = !!(first2B & BIT14);
        ts.pid = first2B & 0x1fff;

        //std::cout << "TEI: " << std::hex << static_cast<int>(header.transportErrorIndicator) << ", PUSI: " << std::hex << int(header.payloadUnitStartIndicator) << ", TP: " << std::hex << int(header.transportPriority) << std::endl;
        //std::cerr << "Found PID: " << std::hex << static_cast<int>(header.pid) << ", Old PID: " << std::hex << static_cast<int>(packetInfo.pid) << std::endl;

        uint8_t lastB;
        result = fread(&lastB, sizeof(uint8_t), 1, m_file);
        if (result != 1)
        {
            qDebug("Something went wrong.");
            throw exception();
        }

        ts.scramblingControl = (lastB >> 6) & 0x3;
        ts.adaptationFieldControl = (lastB >> 4) & 0x3;
        ts.continuityCounter = lastB & 0xf;

        packet.tsheader = ts;

        //std::cout << "SC: " << std::hex << static_cast<int>(header.scramblingControl) << ", AFC: " << std::hex << static_cast<int>(header.adaptationFieldControl) << ", CC: " << std::hex << static_cast<int>(header.continuityCounter) << std::endl;


        //AFC = 01 -> only payload
        //AFC = 10 -> only adaptation field
        //AFC = 11 -> payload & adaptation field

        if (packet.tsheader.adaptationFieldControl == 2 || packet.tsheader.adaptationFieldControl == 3)
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

                packet.afheader = af;
            }
        }

        return packet;
    }
    else
    {
        //cannot find SYNC_BYTE
        qDebug("Cannot find SYNC_BYTE.");
        throw exception();
    }
}
