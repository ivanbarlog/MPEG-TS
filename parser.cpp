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

uint8_t Parser::readNext1B()
{
    int result = 0;
    uint8_t read_data;

    result = fread(&read_data, sizeof(uint8_t), 1, m_file);
    if (result != 1)
    {
        qDebug("Something went wrong.");
        throw exception();
    }

    return read_data;
}

uint16_t Parser::readNext2B()
{
    uint8_t arr2B[2] = {0};
    int affected;

    for (int i = 0; i < 2; i++)
    {
       affected = fread(&arr2B[i], sizeof(uint8_t), 1, m_file);
       if (affected != 1)
       {
           qDebug("Packet not complete.");
           throw exception();
       }
    }

    return ((arr2B[0] << 8) | arr2B[1]);
}

void Parser::skipBytes(int count)
{
    int result = 0;
    uint8_t read_data;

    for (int i = 0; i < count; i++)
    {
        result = fread(&read_data, sizeof(uint8_t), 1, m_file);
        if (result != 1)
        {
            qDebug("Something went wrong.");
            throw exception();
        }
    }
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

PacketInfo Parser::getFirstPacket(uint16_t pid, std::vector<PacketInfo> list)
{
    bool found = false;
    PacketInfo packet;

    for (std::vector<PacketInfo>::iterator itr = list.begin(); itr != list.end(); ++itr)
    {
        if (found) break;

        PacketInfo tmp = *itr;

        if (tmp.pid == pid)
        {
            packet = tmp;
            found = true;
        }
    }

    return packet;
}

PAT Parser::getPATPacket(PacketInfo packetInfo)
{
    if (m_file == NULL)
    {
        qDebug("File not initialized.");
        throw exception();
    }

    PAT tmp;
    uint8_t read_data;
    uint16_t next2B;

    //go to first byte of file
    rewind(m_file);
    //seek bytes with current packet
    fseeko64(m_file, packetInfo.start, SEEK_CUR);

    read_data = readNext1B();

    if (read_data == SYNC_BYTE)
    {
        skipBytes(2);

        // get adaptation control field to determine if adaptation field or payload is present
        read_data = readNext1B();
        uint8_t adaptationFieldControl = (read_data >> 4) & 0x3;

        //AFC = 01 -> only payload
        //AFC = 10 -> only adaptation field
        //AFC = 11 -> payload & adaptation field

        //PAT packets contains only payload
        if (adaptationFieldControl == 1)
        {
            read_data = readNext1B();

            tmp.pointerField = read_data;

            read_data = readNext1B();

            tmp.tableId = read_data;

            next2B = readNext2B();

            #define BIT16 0x8000
            #define BIT15 0x4000

            if (!!(next2B & BIT16) == 1 && !!(next2B & BIT15) == 0)
            {
                //commented while developing
                //qDebug("This is PAT packet.");
            }
            else
            {
                qDebug("Not PAT packet - continue...");
                std::cout << "Not PAT packet: " << std::endl;
                //std::cout << "1 ? " << std::hex << !!(sorted2B & BIT16) << std::endl;
                //std::cout << "0 ? " << std::hex << !!(sorted2B & BIT15) << std::endl;
                throw exception();
            }

            tmp.sectionLength = next2B & 0x03FF;

            next2B = readNext2B();

            tmp.transportStreamId = next2B;

            read_data = readNext1B();

            tmp.versionNumber = !!(read_data & (31 << 1));
            tmp.nextIndicator = !!(read_data & 1);

            read_data = readNext1B();

            tmp.sectionNumber = read_data;

            read_data = readNext1B();

            tmp.lastSectionNumber = read_data;

            tmp.programCount = (tmp.sectionLength - 5 - 4) / 4;
        }
    }
    else
    {
        qDebug("Cannot find SYNC_BYTE.");
        throw exception();
    }

    return tmp;
}

PMT Parser::parsePMT(PacketInfo packetInfo)
{
    if (m_file == NULL)
    {
        qDebug("File not initialized.");
        throw exception();
    }

    PMT tmp;
    uint8_t read_data;
    uint16_t next2B;

    //go to first byte ofreadNext1B file
    rewind(m_file);
    //seek bytes with current packet
    fseeko64(m_file, packetInfo.start, SEEK_CUR);

    read_data = readNext1B();

    if (read_data == SYNC_BYTE)
    {
        skipBytes(2);

        // get adaptation control field to determine if adaptation field or payload is present
        read_data = readNext1B();
        uint8_t adaptationFieldControl = (read_data >> 4) & 0x3;

        //AFC = 01 -> only payload
        //AFC = 10 -> only adaptation field
        //AFC = 11 -> payload & adaptation field

        //PMT packets contains only payload
        if (adaptationFieldControl == 1)
        {
            tmp.pointerField = readNext1B();
            tmp.tableId = readNext1B();

            next2B = readNext2B();
            tmp.sectionSyntaxIndicator = next2B & 0x8000;
            tmp.sectionLength = next2B & 0x03FF;

            tmp.programNum = readNext2B();

            read_data = readNext1B();
            tmp.versionNumber = read_data & 0x003E;
            tmp.nextIndicator = read_data & 1;

            tmp.sectionNumber = readNext1B();
            tmp.lastSectionNumber = readNext1B();

            next2B = readNext2B();
            tmp.pcrPID = next2B & 0x1FFF;

            next2B = readNext2B();
            tmp.programInfoLength = next2B & 0x03FF;

            //now we skip program descriptors and we perform section parsing
            fseeko64(m_file, tmp.programInfoLength, SEEK_CUR);

            //length of program info
            int length = tmp.sectionLength - 9 - tmp.programInfoLength - 4;


            int readBytes = 0;
            while (1)
            {
                if (readBytes >= length) break;

                PMTProgram prog;

                prog.streamType = readNext1B();
                prog.elementaryPID = readNext2B() & 0x1FFF;
                prog.esInfoLength = readNext2B() & 0x03FF;

                readBytes += 5 + prog.esInfoLength;

                //skip bytes with descriptor
                skipBytes(prog.esInfoLength);

                tmp.programs.push_back(prog);
            }
        }
    }

    return tmp;
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
            PacketInfo tmp;
            tmp.start = (uint64_t)(ftello64(m_file) - 1);

            uint16_t first2B = readNext2B();
            tmp.pid = first2B & 0x1fff;
            tmp.length = getPacketLength();

            list.push_back(tmp);

            // pid 0x0000 is for PAT
            if (m_programList.empty())
            {
                if (tmp.pid == 0x0000)
                {
                    PAT pat = getPATPacket(tmp);
                    m_programList = parsePATPrograms(pat.programCount);
                }
            }
            else
            {
                if (m_programList2.contains(tmp.pid))
                {
                    if (m_programList2[tmp.pid].filledPMT)
                        continue;

                    PMT pmt = parsePMT(tmp);
                    m_programList2[tmp.pid].pmt = pmt;
                    m_programList2[tmp.pid].filledPMT = true;
                }
            }
        }
        else
        {
            continue;
        }
    }

    /* parse Program Association Table */
    printPrograms();


    for (std::vector<Program>::iterator itr = m_programList.begin(); itr != m_programList.end(); ++itr)
    {
        Program tmp = *itr;

        PacketInfo pi = getFirstPacket(tmp.programPID, list);
        std::cout << "Program with PID " << tmp.programPID << " starts at " << std::hex << pi.start << std::endl;

        //todo: zistit pre vsetky PMT co sa vysiela na jednotlivych PIDoch a vratit ako strukturu / cize najprv PMT parse
    }

    return list;
}

void Parser::printPrograms()
{
    std::cout << "Program table: ***********************" << std::endl;
    for (std::vector<Program>::iterator itr = m_programList.begin(); itr != m_programList.end(); ++itr)
    {
        Program tmp = *itr;
        std::cout << "Program number: " << std::hex << tmp.programNumber << "; Program PID: " << std::hex<< tmp.programPID << std::endl;
    }
    std::cout << "**************************************" << std::endl;
}

std::vector<Program> Parser::parsePATPrograms(int programCount)
{
    std::vector<Program> v;
    //QHash<uint16_t, Program> programs;

    for (int i = 0; i < programCount; i++)
    {
        Program tmp;

        tmp.programNumber = readNext2B();
        tmp.programPID = readNext2B() & 0x1fff;
        tmp.filledPMT = false;

        v.push_back(tmp);

        m_programList2.insert(tmp.programPID, tmp);
    }

    return v;
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

    //go to first byte of file
    rewind(m_file);
    //seek bytes with current packet
    fseeko64(m_file, packetInfo.start, SEEK_CUR);

    read_data = readNext1B();

    if (read_data == SYNC_BYTE)
    {
        TSHeader ts;

        uint16_t first2B = readNext2B();

        #define BIT16 0x8000
        #define BIT15 0x4000
        #define BIT14 0x2000

        ts.transportErrorIndicator = !!(first2B & BIT16);
        ts.payloadUnitStartIndicator = !!(first2B & BIT15);
        ts.transportPriority = !!(first2B & BIT14);
        ts.pid = first2B & 0x1fff;

        read_data = readNext1B();

        ts.scramblingControl = (read_data >> 6) & 0x3;
        ts.adaptationFieldControl = (read_data >> 4) & 0x3;
        ts.continuityCounter = read_data & 0xf;

        packet.tsheader = ts;

        //AFC = 01 -> only payload
        //AFC = 10 -> only adaptation field
        //AFC = 11 -> payload & adaptation field

        if (packet.tsheader.adaptationFieldControl == 2 || packet.tsheader.adaptationFieldControl == 3)
        {
            AFHeader af;

            af.length = readNext1B();

            if (af.length != 0)
            {
                read_data = readNext1B();

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
        qDebug("Cannot find SYNC_BYTE.");
        throw exception();
    }
}

QHash<uint16_t, Program> Parser::getProgramInfo()
{
    return m_programList2;
}
