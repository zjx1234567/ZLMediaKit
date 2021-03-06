/*
 * Copyright (c) 2016 The ZLMediaKit project authors. All Rights Reserved.
 *
 * This file is part of ZLMediaKit(https://github.com/xia-chu/ZLMediaKit).
 *
 * Use of this source code is governed by MIT license that can be found in the
 * LICENSE file in the root of the source tree. All contributing project authors
 * may be found in the AUTHORS file in the root of the source tree.
 */

#ifndef ZLMEDIAKIT_H264RTPCODEC_H
#define ZLMEDIAKIT_H264RTPCODEC_H

#include "Rtsp/RtpCodec.h"
#include "Util/ResourcePool.h"
#include "Extension/H264.h"
#include "Common/Stamp.h"
using namespace toolkit;

namespace mediakit{

/**
 * h264 rtp解码类
 * 将 h264 over rtsp-rtp 解复用出 h264-Frame
 * rfc3984
 */
class H264RtpDecoder : public RtpCodec{
public:
    typedef std::shared_ptr<H264RtpDecoder> Ptr;

    H264RtpDecoder();
    ~H264RtpDecoder() {}

    /**
     * 输入264 rtp包
     * @param rtp rtp包
     * @param key_pos 此参数忽略之
     */
    bool inputRtp(const RtpPacket::Ptr &rtp, bool key_pos = true) override;

    CodecId getCodecId() const override{
        return CodecH264;
    }

private:
    bool decodeRtp(const RtpPacket::Ptr &rtp);
    void onGetH264(const H264Frame::Ptr &frame);
    H264Frame::Ptr obtainFrame();

private:
    uint16_t _last_seq = 0;
    size_t _max_frame_size = 0;
    H264Frame::Ptr _frame;
    DtsGenerator _dts_generator;
};

/**
 * 264 rtp打包类
 */
class H264RtpEncoder : public H264RtpDecoder ,public RtpInfo{
public:
    typedef std::shared_ptr<H264RtpEncoder> Ptr;

    /**
     * @param ssrc ssrc
     * @param mtu mtu大小
     * @param sample_rate 采样率，强制为90000
     * @param pt pt类型
     * @param interleaved rtsp interleaved
     */
    H264RtpEncoder(uint32_t ssrc,
                   uint32_t mtu = 1400,
                   uint32_t sample_rate = 90000,
                   uint8_t pt = 96,
                   uint8_t interleaved = TrackVideo * 2);
    ~H264RtpEncoder() {}

    /**
     * 输入264帧
     * @param frame 帧数据，必须
     */
    void inputFrame(const Frame::Ptr &frame) override;

private:
    void makeH264Rtp(const void *pData, size_t uiLen, bool bMark,  bool gop_pos, uint32_t uiStamp);
};

}//namespace mediakit{

#endif //ZLMEDIAKIT_H264RTPCODEC_H
