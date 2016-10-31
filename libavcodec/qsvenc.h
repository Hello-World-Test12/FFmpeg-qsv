/*
 * Intel MediaSDK QSV encoder utility functions
 *
 * copyright (c) 2013 Yukinori Yamazoe
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

#ifndef AVCODEC_QSVENC_H
#define AVCODEC_QSVENC_H

#include <stdint.h>
#include <sys/types.h>

#include <mfx/mfxvideo.h>

#include "libavutil/avutil.h"
#include "libavutil/fifo.h"

#include "avcodec.h"
#include "qsv_internal.h"

typedef struct QSVEncContext {
    AVCodecContext *avctx;

    QSVFrame *work_frames;
    mfxFrameAllocator *frame_allocator;

    mfxSession session;
    QSVSession internal_qs;

    int packet_size;
    int width_align;
    int height_align;

    mfxVideoParam param;
    mfxFrameAllocRequest req;
    mfxFrameAllocResponse resq;
    int nb_frame_requested;

    mfxExtCodingOption  extco;
#if QSV_VERSION_ATLEAST(1,6)
    mfxExtCodingOption2 extco2;
    mfxExtBuffer *extparam[2];
#else
    mfxExtBuffer *extparam[1];
#endif

    AVFifoBuffer *async_fifo;

    // options set by the caller
    int async_depth;
    int idr_interval;
    int profile;
    int preset;
    int avbr_accuracy;
    int avbr_convergence;
    int pic_timing_sei;
    int look_ahead;
    int look_ahead_depth;
    int look_ahead_downsampling;
    int iopattern;
    int maxQPI;
    int minQPI;
    int maxQPP;
    int minQPP;
    int maxQPB;
    int minQPB;
    int MBBRC;
    int BRefControl;

    // options for mfxExtCodingOption
    int rate_distor_opt;
    int cavlc;
    int nal_hrd_con;
    int single_sei_nal;
    int reset_reflist;
    int ref_pic_mark_rep;
    int field_output;
    int max_dec_frame_buffering;
    int audelimiter;
    int vui_nal_hrd_parameters;
    int frame_picture;
    int recovery_pointSEI;


    char *load_plugins;
} QSVEncContext;

typedef struct QSVH264EncContext {
    AVClass *class;
    QSVEncContext qsv;
} QSVH264EncContext;

typedef struct QSVHEVCEncContext {
    AVClass *class;
    QSVEncContext qsv;
    int load_plugin;
} QSVHEVCEncContext;


typedef struct QSVMpeg2EncContext {
    AVClass *class;
    QSVEncContext qsv;
} QSVMpeg2EncContext;

int ff_qsv_enc_init(AVCodecContext *avctx, QSVEncContext *q);

int ff_qsv_encode(AVCodecContext *avctx, QSVEncContext *q,
                  AVPacket *pkt, const AVFrame *frame, int *got_packet);

int ff_qsv_enc_close(AVCodecContext *avctx, QSVEncContext *q);

#endif /* AVCODEC_QSVENC_H */
