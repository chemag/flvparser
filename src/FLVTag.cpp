//
// Created by raytine on 2019/4/11.
//

#include "FLVTag.h"

#include <iomanip>
#include <iostream>

#include "FLVDataTagFactory.h"

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
      return "!Unknown";
  };
}

std::string FLVTag::desc() const {
  std::string out_string;
  std::stringstream ss;
  ss << length;

  return "tag: " + typeName() + " " + data->desc() + " size:" + ss.str();
  //<< ": type: " << (int)(std::stringtagType &0b00011111) << std::endl
  //<< ": " << data->desc() << " size:" << length << std::endl;
  //<< " timestamp: " << timestamp
  //<< " streamId: " << streamId << std::endl
  //<< " detail: " << std::endl;
}

const std::shared_ptr<FLVBaseTagData> &FLVTag::getData() const { return data; }
