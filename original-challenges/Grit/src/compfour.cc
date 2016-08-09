/*
 * Copyright (c) 2015 Kaprica Security, Inc.
 *
 * Permission is hereby granted, cgc_free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#include <cstdint.h>
#include "compfour.h"

const int index_table[8] = {
    -1, -1, -1, -1, 1, 3, 6, 9
};

const int step_table[90] = {
    6, 7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 
    19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 
    50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 
    130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
    337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
    876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 
    2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
    5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899, 
    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767 
};

unsigned int cgc_CompressionFour::cgc_getId() const
{
    return COMPRESSION_4;
}

unsigned int cgc_CompressionFour::cgc_getMaxBufferSize(const cgc_AudioTrack &track) const
{
    if (track.cgc_getStereo())
        return track.cgc_getLength() + 7 * ((track.cgc_getLength() + 504) / 505);
    else
        return track.cgc_getLength() / 2 + 3 * ((track.cgc_getLength() + 508) / 509);
}

static void cgc_decode(int *pred, int *index, int nibble)
{
    int step;
    step = step_table[index[0]];
    index[0] += index_table[nibble & 7];

    step *= (nibble & 8) ? -1 : 1;
    if (nibble & 4)
        pred[0] += step;
    if (nibble & 2)
        pred[0] += step >> 1;
    if (nibble & 1)
        pred[0] += step >> 2;

    if (index[0] < 0)
        index[0] = 0;
    if (index[0] > 7)
        index[0] = 7;

    if (pred[0] >= INT16_MAX)
        pred[0] = INT16_MAX;
    else if (pred[0] <= INT16_MIN)
        pred[0] = INT16_MIN;
}

static int cgc_encode(int *pred, int *index, cgc_int16_t sample)
{
    int nibble = 0;

    // find best match
    int diff = sample - pred[0], sign;
    if (diff < 0)
    {
        sign = -1;
        diff = -diff;
    }
    else
    {
        sign = 1;
    }

    int step = step_table[index[0]];
    nibble = diff * 4 / step;
    if (nibble > 7)
        nibble = 7;
    if (sign < 0)
        nibble |= 8;

    // update state
    cgc_decode(pred, index, nibble);
    return nibble;
}

int cgc_bestIndex(int diff)
{
    if (diff < 0)
        diff = -diff;
    int best = 0;
    for (int i = 1; i < sizeof(step_table) / sizeof(step_table[0]); i++)
    {
        int diff1 = diff - step_table[best];
        int diff2 = diff - step_table[i];
        if (diff1 < 0)
            diff1 = -diff1;
        if (diff2 < 0)
            diff2 = -diff2;

        if (diff2 < diff1)
            best = i;
    }
    return best;
}

void cgc_CompressionFour::cgc_compress(const cgc_AudioTrack &track, cgc_uint8_t *dest) const
{
    unsigned int i, j;
    int index[2], pred[2];
    for (i = 0, j = 0; i < track.cgc_getLength(); i++)
    {
        if ((j % 512) == 0)
        {
            pred[0] = track.cgc_getChannel(0)->cgc_getSample(i) >> 16;
            index[0] = cgc_bestIndex((track.cgc_getChannel(0)->cgc_getSample(i+1) >> 16) - pred[0]);
            *(cgc_int16_t *)(dest + j) = pred[0];
            j += 2;
            *(cgc_int16_t *)(dest + j) = index[0];
            j += 2;
            if (track.cgc_getStereo())
            {
                pred[1] = track.cgc_getChannel(1)->cgc_getSample(i) >> 16;
                index[1] = cgc_bestIndex((track.cgc_getChannel(1)->cgc_getSample(i+1) >> 16) - pred[1]);
                *(cgc_int16_t *)(dest + j) = pred[1];
                j += 2;
                *(cgc_int16_t *)(dest + j) = index[1];
                j += 2;
            }
        }
        else
        {
            cgc_int16_t sample[2];
            sample[0] = track.cgc_getChannel(0)->cgc_getSample(i) >> 16;
            if (track.cgc_getStereo())
                sample[1] = track.cgc_getChannel(1)->cgc_getSample(i) >> 16;
            else
                sample[0] = track.cgc_getChannel(0)->cgc_getSample(++i) >> 16;

            cgc_uint8_t token = 0;
            token |= cgc_encode(&pred[0], &index[0], sample[0]) << 4;
            if (track.cgc_getStereo())
                token |= cgc_encode(&pred[1], &index[1], sample[1]);
            else
                token |= cgc_encode(&pred[0], &index[0], sample[1]);

            dest[j++] = token;
        }
    }
}

void cgc_CompressionFour::cgc_decompress(cgc_AudioTrack &track, cgc_uint8_t *src) const
{
    unsigned int i, j;
    int index[2], pred[2];
    for (i = 0, j = 0; i < track.cgc_getLength(); i++)
    {
        if ((j % 512) == 0)
        {
            pred[0] = *(cgc_int16_t *)(&src[j]);
            j += 2;
            index[0] = src[j];
            j += 2;
            track.cgc_getChannel(0)->cgc_setSample(i, pred[0] << 16);
            if (track.cgc_getStereo())
            {
                pred[1] = *(cgc_int16_t *)(&src[j]);
                j += 2;
                index[1] = src[j];
                j += 2;
                track.cgc_getChannel(1)->cgc_setSample(i, pred[1] << 16);
            }
        }
        else
        {
            cgc_uint8_t token1 = src[j] >> 4, token2 = src[j] & 0xf;
            j++;

            cgc_decode(&pred[0], &index[0], token1);
            track.cgc_getChannel(0)->cgc_setSample(i, pred[0] << 16);

            if (track.cgc_getStereo())
            {
                cgc_decode(&pred[1], &index[1], token2);
                track.cgc_getChannel(1)->cgc_setSample(i, pred[1] << 16);
            }
            else
            {
                cgc_decode(&pred[0], &index[0], token2);
                i++;
                track.cgc_getChannel(0)->cgc_setSample(i, pred[0] << 16);
            }
        }
    }
}
