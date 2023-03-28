//
// Created by raytine on 2019/4/11.
//

#include "FLVVideoTag.h"

#include <h264_bitstream_parser.h>
#include <h264_common.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>

namespace {

// https://stackoverflow.com/a/26221725
template <typename... Args>
std::string string_format(const std::string &format, Args... args) {
  // Extra space for '\0'
  int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
  if (size_s <= 0) {
    throw std::runtime_error("Error during formatting.");
  }
  auto size = static_cast<size_t>(size_s);
  std::unique_ptr<char[]> buf(new char[size]);
  std::snprintf(buf.get(), size, format.c_str(), args...);
  // We do not want the '\0' inside
  return std::string(buf.get(), buf.get() + size - 1);
}

std::string AVCGetResolution(
    h264nal::H264NalUnitParser::NalUnitState &nal_unit) {
  if (nal_unit.nal_unit_payload == nullptr) {
    return "";
  }
  if (nal_unit.nal_unit_payload->sps == nullptr) {
    return "";
  }
  if (nal_unit.nal_unit_payload->sps->sps_data == nullptr) {
    return "";
  }
  int width = -1, height = -1;
  nal_unit.nal_unit_payload->sps->sps_data->getResolution(&width, &height);
  return string_format("%ix%i", width, height);
}

std::string AVCGetFramerate(
    h264nal::H264NalUnitParser::NalUnitState &nal_unit) {
  if (nal_unit.nal_unit_payload == nullptr) {
    return "";
  }
  if (nal_unit.nal_unit_payload->sps == nullptr) {
    return "";
  }
  if (nal_unit.nal_unit_payload->sps->sps_data == nullptr) {
    return "";
  }
  if (nal_unit.nal_unit_payload->sps->sps_data->vui_parameters == nullptr) {
    return "";
  }
  float framerate =
      nal_unit.nal_unit_payload->sps->sps_data->vui_parameters->getFramerate();
  return string_format("%f", framerate);
}

std::string AVCGetSliceHeaderFrameNum(
    h264nal::H264NalUnitParser::NalUnitState &nal_unit) {
  if (nal_unit.nal_unit_payload == nullptr) {
    return "";
  }
  if (nal_unit.nal_unit_payload->slice_layer_without_partitioning_rbsp ==
      nullptr) {
    return "";
  }
  if (nal_unit.nal_unit_payload->slice_layer_without_partitioning_rbsp
          ->slice_header == nullptr) {
    return "";
  }
  int frame_num =
      nal_unit.nal_unit_payload->slice_layer_without_partitioning_rbsp
          ->slice_header->frame_num;
  return string_format("%i", frame_num);
}

}  // namespace

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

  resolution = "";
  framerate = "";
  frame_num = "";

  if (body != nullptr && codecId == 7 /* AVC */ &&
      avcPacketType == 1 /* AVC NALU */) {
    // create state for parsing NALUs
    // bitstream parser state (to keep the SPS/PPS/SubsetSPS NALUs)
    static h264nal::H264BitstreamParserState bitstream_parser_state;
    static h264nal::ParsingOptions parsing_options;

    // get the indices for the NALUs in the stream. This is needed
    // because we will read Annex-B files, i.e., a bunch of appended NALUs
    // with escape sequences used to separate them.
    uint8_t *data = (uint8_t *)body;
    auto nalu_indices =
        h264nal::H264BitstreamParser::FindNaluIndicesExplicitFraming(data,
                                                                     length);

    printf("nalu_indices: %zu\n", nalu_indices.size());

    for (const auto &nalu_index : nalu_indices) {
      // parse 1 NAL unit
      // note: If the NALU comes from an unescaped bitstreams, i.e.,
      // one with an explicit NALU length mechanism (like mp4 mdat
      // boxes), the right function is `ParseNalUnitUnescaped()`.
      auto nal_unit = h264nal::H264NalUnitParser::ParseNalUnit(
          data + nalu_index.payload_start_offset, nalu_index.payload_size,
          &bitstream_parser_state, parsing_options);
      if (nal_unit->nal_unit_header->nal_unit_type == 7) {  // SPS
        resolution = AVCGetResolution(*nal_unit);
        framerate = AVCGetFramerate(*nal_unit);
      } else if (nal_unit->nal_unit_header->nal_unit_type == 1 ||
                 nal_unit->nal_unit_header->nal_unit_type ==
                     5) {  // slice_header
        frame_num = AVCGetSliceHeaderFrameNum(*nal_unit);
      }

      /*
            printf(
                "nal_unit { offset: %lu length: %lu parsed_length: %lu checksum:
         0x%s "
                "} nal_unit_header { forbidden_zero_bit: %i nal_ref_idc: %i "
                "nal_unit_type: %i }\n",
                nal_unit->offset, nal_unit->length, nal_unit->parsed_length,
                nal_unit->checksum->GetPrintableChecksum(),
                nal_unit->nal_unit_header->forbidden_zero_bit,
                nal_unit->nal_unit_header->nal_ref_idc,
                nal_unit->nal_unit_header->nal_unit_type);
      */
    }
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

std::string VideoFirstLong(const char *body) {
  if (body == nullptr) {
    return "";
  }
  char buf[1024];
  size_t rem = 1024;
  int i = 0;
  size_t bi = 0;
  for (i = 0; i < 8; i += 1) {
    bi += snprintf(buf + bi, rem - bi, "%02x", (unsigned char)body[i]);
  }
  buf[bi] = '\0';
  return buf;
}

std::vector<std::string> FLVVideoTag::csv_headers() {
  std::vector<std::string> out = {
      "video_codec_id",         "video_frame_type", "video_avc_packet_type",
      "video_composition_time", "video_resolution", "video_framerate",
      "video_frame_num",        "video_first_long",
  };
  return out;
}

std::string FLVVideoTag::csv() const {
  return codecIdName() + "," +    // CodecId
         frameTypeName() + "," +  // FrameType
         (codecId == 7 ? std::to_string(avcPacketType) : "") +
         "," +                         // AVCPacketType
         compositionTimeStr() + "," +  // CompositionTime
         resolution + "," +            // resolution
         framerate + "," +             // framerate
         frame_num + "," +             // frame_num
         VideoFirstLong(body);         // video first long word (64 bytes)
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
