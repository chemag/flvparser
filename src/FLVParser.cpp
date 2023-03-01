//
// Created by raytine on 2019/4/9.
//

#include "FLVParser.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>

#include "FLVHeader.h"
#include "FLVTag.h"

size_t readBytesFromStream(std::ifstream &in, char *bytes,
                           const size_t length) noexcept(false) {
  in.read(bytes, sizeof(char) * length);
  if (length > in.gcount()) {
    throw std::string("Reach the end of the file");
  }
  return in.gcount();
  // or return std::shared_ptr<char>(bytes,
  // std::unique_ptr<char>(std::default_delete<char[]>()
  //  std::make_shared<char>(bytes); is wrong!! ref:
  //  https://stackoverflow.com/questions/43631415/using-shared-ptr-with-char
}

template <class T>
T readFromStream(std::ifstream &in, const size_t length_ = -1) noexcept(false) {
  size_t length = (length_ == -1 ? sizeof(T) : length_);

  char *bytes = new char[length];
  size_t read_bytes = readBytesFromStream(in, bytes, length);
  T sum = 0;
  for (int i = 0; i < length; ++i) {
    sum += ((uint8_t) * (bytes + i) << (8 * (length - i - 1)));
  }
  delete[] (bytes);
  return sum;
}

std::shared_ptr<FLVHeader> parseHeader(std::ifstream &in) {
  char *fileType = new char[3];
  size_t read_bytes = readBytesFromStream(in, fileType, 3);
  auto version = readFromStream<int8_t>(in);
  auto info = readFromStream<int8_t>(in);
  auto length = readFromStream<int32_t>(in);

  return std::make_shared<FLVHeader>(fileType, version, info, length, nullptr);
}

std::shared_ptr<FLVTag> parseTag(std::ifstream &in) {
  auto type = readFromStream<uint8_t>(in);
  auto length = readFromStream<uint32_t>(in, 3);
  auto timestamp = readFromStream<uint32_t>(in, 3);
  uint32_t timestampExtended = readFromStream<uint8_t>(in);
  if (timestampExtended > 0) {
    timestamp = (timestampExtended << 24) + timestamp;
  }
  auto streamId = readFromStream<uint32_t>(in, 3);
  char *data = new char[length];
  size_t read_bytes = readBytesFromStream(in, data, length);

  return std::make_shared<FLVTag>(type, length, timestamp, streamId, data);
}

void FLVParser::parse() {
  std::ifstream in(filePath, std::ios::binary);
  if (in.is_open()) {
    auto header = parseHeader(in);
    header->show();
    int tagIndex = 0;
    std::FILE *csvfd = nullptr;
    if (csvPath != "") {
      csvfd = std::fopen(csvPath.c_str(), "wb");
      fprintf(csvfd,
              "tag_id,frame_id,tag_type,filter,data_size,timestamp,"
              "timestamp_delta,stream_id,"
              "audio_format,audio_rate,audio_size,audio_type,"
              "video_codec_id,video_frame_type,video_avc_packet_type,"
              "video_composition_time\n");
    }
    int video_frame = 0;
    uint32_t video_prev_timestamp = 0;
    int audio_frame = 0;
    uint32_t audio_prev_timestamp = 0;
    while (true) {
      try {
        auto previousTagSize = readFromStream<int32_t>(in);
        auto tag = parseTag(in);
        std::cout << "[" << ++tagIndex << "] \t" << tag->desc() << std::endl;
        if (csvfd != nullptr) {
          int32_t timestamp_delta = 0;
          int frame_id = 0;
          if (tag->getTagType() == 8 && tag->getAacPacketType() == 1) {
            // audio
            frame_id = audio_frame++;
            timestamp_delta = tag->timestamp - audio_prev_timestamp;
            audio_prev_timestamp = tag->timestamp;
          } else if (tag->getTagType() == 8) {
            frame_id = audio_frame;
          } else if (tag->getTagType() == 9 && tag->getAvcPacketType() == 1) {
            // video
            frame_id = video_frame++;
            timestamp_delta = tag->timestamp - video_prev_timestamp;
            video_prev_timestamp = tag->timestamp;
          } else if (tag->getTagType() == 9) {
            // video
            frame_id = video_frame;
          }
          fprintf(csvfd, "%i,%i,%s", tagIndex, frame_id,
                  tag->csv(timestamp_delta).c_str());
        }
      } catch (std::string &error) {
        std::cout << "Exception: " << error << ", stop parsing.." << std::endl;
        break;
      }
    }
    if (csvfd != nullptr) {
      std::fclose(csvfd);
    }
    in.close();
  } else {
    std::cout << "can not open file :" << filePath << std::endl;
  }
}
