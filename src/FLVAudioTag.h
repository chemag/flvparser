//
// Created by raytine on 2019/4/11.
//

#ifndef TEST_FLVAUDIOTAG_H
#define TEST_FLVAUDIOTAG_H

#include <cstdint>
#include <string>

#include "FLVBaseTagData.h"

class FLVAudioTag : public FLVBaseTagData {
 private:
  uint8_t soundFlags;
  uint8_t aacPacketType;
  char *body;
  uint32_t bodyLength;

 public:
  FLVAudioTag(char *data, uint32_t length);
  FLVAudioTag(const FLVAudioTag &tag);
  FLVAudioTag &operator=(const FLVAudioTag &tag);
  ~FLVAudioTag();

  std::string desc() const override;
  std::string csv() const override;
  std::string soundFormatName() const;
  std::string soundRateName(bool concise) const;
  std::string soundSizeName(bool concise) const;
  bool isStereo() const;
  virtual uint8_t getAacPacketType() const override { return aacPacketType; };
};

#endif  // TEST_FLVAUDIOTAG_H
