//
// Created by raytine on 2019/4/13.
//

#ifndef TEST_FLVBASETAGDATA_H
#define TEST_FLVBASETAGDATA_H

#include <string>

class FLVBaseTagData {
 public:
  virtual ~FLVBaseTagData() {}
  virtual std::string desc() const;
  virtual std::string csv() const;
  virtual uint8_t getAvcPacketType() const { return -1; };
  virtual uint8_t getAacPacketType() const { return -1; };
};

#endif  // TEST_FLVBASETAGDATA_H
