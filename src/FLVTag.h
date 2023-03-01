//
// Created by raytine on 2019/4/11.
//

#ifndef TEST_FLVTAG_H
#define TEST_FLVTAG_H

#include <cstdint>
#include <memory>
#include <ostream>
#include <string>

#include "FLVBaseTagData.h"

class FLVTag {
 public:
  uint8_t tagType;     // 1byte
  uint32_t length;     // 3 bytes
  uint32_t timestamp;  // 3 bytes
  uint32_t streamId;   // 4 bytes
  std::shared_ptr<FLVBaseTagData> data;

  FLVTag(uint8_t tagType, uint32_t length, uint32_t timestamp,
         uint32_t streamId, char *data);
  FLVTag(const FLVTag &tag);
  FLVTag &operator=(const FLVTag &tag);

  ~FLVTag();

  const std::shared_ptr<FLVBaseTagData> &getData() const;

  std::string desc() const;
  std::string csv(int32_t timestamp_delta) const;
  std::string typeName() const;
  uint8_t getTagType() const { return tagType; };
  uint8_t getAvcPacketType() const { return data->getAvcPacketType(); };
};

#endif  // TEST_FLVTAG_H
