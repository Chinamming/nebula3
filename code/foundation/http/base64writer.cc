//------------------------------------------------------------------------------
//  base64writer.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "http/base64writer.h"

namespace Http
{
ImplementClass(Http::Base64Writer, 'B64W', IO::StreamWriter);

//------------------------------------------------------------------------------
/**
*/
Base64Writer::Base64Writer()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Base64Writer::~Base64Writer()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
    Write a block of data into a base64 encoded stream. NOTE: all
    data must be written at once!

    See: http://base64.sourceforge.net/b64.c
*/
void
Base64Writer::Write(void* data, SizeT numBytes)
{
    n_assert(0 != data);
    n_assert(numBytes > 0);
    static const char* cb64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const IndexT lineSize = 80;

    uchar* src = (uchar*) data;
    uchar in[3], out[4];
    SizeT blocksOut = 0;
    IndexT byteIndex = 0;
    while (byteIndex < numBytes)
    {
        SizeT len = 0;
        IndexT i;
        for (i = 0; i < 3; i++)
        {
            if (byteIndex < numBytes)
            {
                in[i] = src[byteIndex++];
                len++;
            }
            else
            {
                in[i] = 0;
            }
        }
        if (len > 0)
        {
            // encode a block of 3 bytes into 4 bytes
            out[0] = cb64[in[0]>>2];
            out[1] = cb64[((in[0] & 0x03) << 4) | ((in[1] & 0x0f) >> 4)];
            out[2] = (uchar) (len > 1 ? cb64[((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)] : '=');
            out[3] = (uchar) (len > 2 ? cb64[in[2] & 0x3f] : '=');
            this->stream->Write(out, sizeof(out));
            blocksOut++;
        }

        // write end-of-line if line-size reached
        if (blocksOut >= (lineSize / 4) || (byteIndex >= numBytes))
        {
            const char* crlf = "\r\n";
            this->stream->Write(crlf, 2);
        }
    }
}

} // namespace Http
