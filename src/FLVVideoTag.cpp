//
// Created by raytine on 2019/4/11.
//

#include "FLVVideoTag.h"

#include <algorithm>
#include <cstring>
#include <iostream>

FLVVideoTag::FLVVideoTag(char *data, uint32_t length_) {
  char *pt = data;
  frameType = (*pt >> 4) & 0b00001111;
  codecId = *pt & 0b00001111;
  pt++;
  if (codecId == 7) {
    // AVC
    avcPacketType = (uint8_t)*pt;
    pt++;
    compositionTime = (*pt << 16) + (*(pt + 1) << 8) + *(pt + 2);
    pt += 3;
  }

  size_t length = length_ - (pt - data);
  if (length > 0) {
    body = new char[length];
    std::copy(pt, data + length_, body);
  } else {
    body = nullptr;
  }

  /*
    static int avc_decoder_configuration_id = 0;
    static int avc_key_frame_id = 0;
    if (codecId == 7) {  // AVC
      char filename[1024] = {0};
      if (avcPacketType == 0) {
        // dump the AVCDecoderConfigurationRecord
        snprintf(filename, 1024, "avc_decoder_configuration.%02i.bin",
    ++avc_decoder_configuration_id); } else if (avcPacketType == 1 && frameType
    == 1) {
        // dump key frames
        snprintf(filename, 1024, "avc_key_frame.%02i.bin", ++avc_key_frame_id);
      }
      if (filename[0] != '\0') {
        std::FILE* f = std::fopen(filename, "wb");
        if (f) {
          std::fwrite(body, 1, length, f);
          std::fclose(f);
        }
      }
    }
  */

  delete[] (data);
}

FLVVideoTag::FLVVideoTag(const FLVVideoTag &tag) {
  frameType = tag.frameType;
  codecId = tag.codecId;
  avcPacketType = tag.avcPacketType;
  compositionTime = tag.compositionTime;
  bodyLength = tag.bodyLength;
  body = new char[bodyLength];
  std::copy(tag.body, tag.body + bodyLength, body);
}

FLVVideoTag &FLVVideoTag::operator=(const FLVVideoTag &tag) {
  if (this == &tag) {
    if (body != nullptr) {
      delete[] (body);
    }

    frameType = tag.frameType;
    codecId = tag.codecId;
    avcPacketType = tag.avcPacketType;
    compositionTime = tag.compositionTime;
    bodyLength = tag.bodyLength;
    body = new char[bodyLength];
    std::copy(tag.body, tag.body + bodyLength, body);
  }
  return *this;
}

FLVVideoTag::~FLVVideoTag() {
  if (body != nullptr) {
    delete[] (body);
  }
}

std::string FLVVideoTag::desc() const {
  return "CodecId: \"" + codecIdName() + "\" " + "FrameType: \"" +
         frameTypeName() + "\" " +
         (codecId == 7 ? ("AVCPacketType: \"" + AVCPacketTypeName() + "\" ")
                       : "") +
         "CompositionTime: " + compositionTimeStr();
}

std::string FLVVideoTag::csv() const {
  return codecIdName() + "," +                              // CodecId
         frameTypeName() + "," +                            // FrameType
         (codecId == 7 ? std::to_string(avcPacketType) : "") + "," +  // AVCPacketType
         compositionTimeStr();                              // CompositionTime
}

std::string FLVVideoTag::frameTypeName() const {
  switch (frameType) {
    case 1:
      return "Key";
    case 2:
      return "Inter";
    case 3:
      return "DisposableInter";
    case 4:
      return "GeneratedKey";
    case 5:
      return "VideoInfo";
    default:
      return "Unknown-" + std::to_string(frameType);
  }
}

std::string FLVVideoTag::codecIdName() const {
  switch (codecId) {
    case 2:
      return "sorenson h263";
    case 3:
      return "screen video";
    case 4:
      return "on2 vp6";
    case 5:
      return "on2 vp6 with alpha channel";
    case 6:
      return "screen video v2";
    case 7:
      return "AVC";
    default:
      return "Other-" + std::to_string(codecId);
  }
}

std::string FLVVideoTag::AVCPacketTypeName() const {
  switch (avcPacketType) {
    case 0:
      return "AVC sequence header";
    case 1:
      return "AVC NALU";
    case 2:
      return "AVC EOS";
    default:
      return "Invalid-" + std::to_string(avcPacketType);
  }
}

std::string FLVVideoTag::compositionTimeStr() const {
  if (codecId != 7) {
    return std::to_string(0);
  } else {
    return std::to_string(compositionTime);
  }
}
