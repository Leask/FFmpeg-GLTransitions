/*
 * Muxer internal APIs - should not be included outside of ffmpeg_mux*
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef FFTOOLS_FFMPEG_MUX_H
#define FFTOOLS_FFMPEG_MUX_H

#include <stdatomic.h>
#include <stdint.h>

#include "thread_queue.h"

#include "libavformat/avformat.h"

#include "libavcodec/packet.h"

#include "libavutil/dict.h"
#include "libavutil/fifo.h"
#include "libavutil/thread.h"

typedef struct MuxStream {
    /* the packets are buffered here until the muxer is ready to be initialized */
    AVFifo *muxing_queue;

    /*
     * The size of the AVPackets' buffers in queue.
     * Updated when a packet is either pushed or pulled from the queue.
     */
    size_t muxing_queue_data_size;

    /* dts of the last packet sent to the muxer, in the stream timebase
     * used for making up missing dts values */
    int64_t last_mux_dts;
} MuxStream;

struct Muxer {
    AVFormatContext *fc;

    pthread_t    thread;
    ThreadQueue *tq;

    MuxStream *streams;

    AVDictionary *opts;

    int thread_queue_size;

    /* filesize limit expressed in bytes */
    int64_t limit_filesize;
    atomic_int_least64_t last_filesize;
    int header_written;

    AVPacket *sq_pkt;
};

#endif /* FFTOOLS_FFMPEG_MUX_H */
