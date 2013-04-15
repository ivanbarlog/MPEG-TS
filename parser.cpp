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

    int i = 1;
    while (fread(&read_data, sizeof(uint8_t), 1, m_file) == 1)
    {
        if (i == 31) break;
        if (read_data == SYNC_BYTE)
        {
            uint8_t nextTwoBytes[2] = {0};

            PacketInfo tmp;
            tmp.start = (uint64_t)(ftello64(m_file) - 1);

            int affected;
            for (int incr = 0; incr < 2; incr++)
            {
               affected = fread(&nextTwoBytes[incr], sizeof(uint8_t), 1, m_file);
               if (affected != 1)
               {
                   //qDebug("Packet not complete.");
                   //throw exception();
               }
            }

            uint16_t pidAssembly = ((nextTwoBytes[0] << 8) | nextTwoBytes[1]);

            tmp.pid = pidAssembly & 0x1fff;

            tmp.length = getPacketLength();

            list.push_back(tmp);



            printf("PID: %X, size of packet %d, packet started at %lu\n", tmp.pid, tmp.length, tmp.start);

            i++;
        }
        else
        {
            continue;
        }
    }


    getTSHeader(list.at(0));

    std::cout << "Hura, hura, zvladli sme to!" << std::endl;

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

    //go to first byte of file
    rewind(m_file);
    //seek bytes with current packet
    fseeko64(m_file, packetInfo.start, SEEK_CUR);

    fread(&read_data, sizeof(uint8_t), 1, m_file);

    if (read_data == SYNC_BYTE)
    {
        std::cout << "Found SYNC_BYTE" << std::endl;

        //parse other TSHeader...

        return header;
    }
    else
    {
        //cannot find SYNC_BYTE
        qDebug("Cannot find SYNC_BYTE.");
        throw exception();
    }
}
