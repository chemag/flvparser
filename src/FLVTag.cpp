//
// Created by raytine on 2019/4/11.
//

#include "FLVTag.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "FLVAudioTag.h"
#include "FLVDataTagFactory.h"
#include "FLVVideoTag.h"

FLVTag::FLVTag(uint8_t tagType, uint32_t length, uint32_t timestamp,
               uint32_t streamId, char *data)
    : tagType(tagType),
      length(length),
      timestamp(timestamp),
      streamId(streamId),
      data(FLVDataTagFactory::createTagDataWith(tagType & 0b00011111, data,
                                                length)) {}

FLVTag::FLVTag(const FLVTag &tag) {
  tagType = tag.tagType;
  length = tag.length;
  timestamp = tag.timestamp;
  streamId = tag.streamId;
  data = std::make_shared<FLVBaseTagData>(*tag.data.get());
}

FLVTag &FLVTag::operator=(const FLVTag &tag) {
  if (this != &tag) {
    data = nullptr;

    tagType = tag.tagType;
    length = tag.length;
    timestamp = tag.timestamp;
    streamId = tag.streamId;
    data = std::make_shared<FLVBaseTagData>(*tag.data.get());
  }
  return *this;
}

FLVTag::~FLVTag() {}

std::string FLVTag::typeName() const {
  switch (tagType & 0b00011111) {
    case 18:
      return "Script";
    case 8:
      return "Audio";
    case 9:
      return "Video";
    default:
      return "Unknown-" + std::to_string(tagType & 0b00011111);
  };
}

std::string FLVTag::desc() const {
  return "TagType: " + typeName() + " " +
         "Filter: " + std::to_string((int)((tagType & 0b00100000) >> 5)) + " " +
         "DataSize: " + std::to_string(length) + " " +
         "Timestamp: " + std::to_string(timestamp) + " " +
         "StreamID: " + std::to_string(streamId) +
         ((data != nullptr) ? (" " + data->desc()) : "");
}

namespace {
std::string getDefaultValue(std::vector<std::string> v) {
  return std::string(v.size() - 1, ',');
}
}  // namespace

std::string FLVTag::csv(int32_t timestamp_delta) const {
  static std::string default_audio_str =
      getDefaultValue(FLVAudioTag::csv_headers());
  static std::string default_video_str =
      getDefaultValue(FLVVideoTag::csv_headers());
  return typeName() + "," +                                          // TagType
         std::to_string((int)((tagType & 0b00100000) >> 5)) + "," +  // Filter
         std::to_string(length) + "," +                              // DataSize
         std::to_string(timestamp) + "," +        // Timestamp
         std::to_string(timestamp_delta) + "," +  // TimestampDelta
         std::to_string(streamId) + "," +         // StreamID
         (tagType == 8 ? data->csv() : default_audio_str) +
         "," +                                               // audio fields
         (tagType == 9 ? data->csv() : default_video_str) +  // video fields
         "\n";
}

const std::shared_ptr<FLVBaseTagData> &FLVTag::getData() const { return data; }
