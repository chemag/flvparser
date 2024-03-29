//
// Created by raytine on 2019/4/11.
//

#ifndef TEST_FLVVIDEOTAG_H
#define TEST_FLVVIDEOTAG_H

#include <cstdint>
#include <vector>

#include "FLVBaseTagData.h"

class FLVVideoTag : public FLVBaseTagData {
 private:
  /**
   * 1: Key frame
   * 2:
   */
  uint8_t frameType;

  /**
   * 2: Sorenson H.263
   * 3: Screen Video
   * 4: On2 VP6
   * 7: AVC
   */
  uint8_t codecId;

  /**
   *  0: AVC header
   *  1: AVC NALU
   *  2: AVC end of sequence
   */
  uint8_t avcPacketType;
  uint32_t compositionTime;
  char *body;
  uint32_t bodyLength;
  std::string resolution;
  std::string framerate;
  std::string pic_order_cnt_type;
  std::string frame_num;

 public:
  FLVVideoTag(char *data, uint32_t length);
  FLVVideoTag(const FLVVideoTag &tag);
  FLVVideoTag &operator=(const FLVVideoTag &tag);
  ~FLVVideoTag();

  std::string desc() const override;
  static std::vector<std::string> csv_headers();
  std::string csv() const override;
  std::string frameTypeName() const;
  std::string codecIdName() const;
  std::string AVCPacketTypeName() const;
  std::string compositionTimeStr() const;
  virtual uint8_t getAvcPacketType() const override { return avcPacketType; };
};

#endif  // TEST_FLVVIDEOTAG_H
