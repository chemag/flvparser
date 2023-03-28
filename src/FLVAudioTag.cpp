//
// Created by raytine on 2019/4/11.
//

#include "FLVAudioTag.h"

#include <algorithm>
#include <iostream>
#include <vector>

FLVAudioTag::FLVAudioTag(char *data, uint32_t length_) {
  char *pt = data;
  soundFlags = *pt++;
  int soundFormat = (soundFlags >> 4) & 0b00001111;
  if (soundFormat == 10) {  // AAC
    aacPacketType = *pt++;
  } else {
    aacPacketType = 0;
  }

  size_t length = length_ - (pt - data);
  body = new char[length];
  std::copy(pt, pt + length, body);
  delete[] (data);
}

FLVAudioTag::FLVAudioTag(const FLVAudioTag &tag) {
  soundFlags = tag.soundFlags;
  aacPacketType = tag.aacPacketType;
  body = new char[tag.bodyLength];
  std::copy(tag.body, tag.body + tag.bodyLength, body);
}

FLVAudioTag &FLVAudioTag::operator=(const FLVAudioTag &tag) {
  if (this == &tag) {
    delete[] (body);

    soundFlags = tag.soundFlags;
    aacPacketType = tag.aacPacketType;
    body = new char[tag.bodyLength];
    std::copy(tag.body, tag.body + tag.bodyLength, body);
  }
  return *this;
}

FLVAudioTag::~FLVAudioTag() { delete[] (body); }

std::string FLVAudioTag::soundFormatName() const {
  int soundFormat = (soundFlags >> 4) & 0b00001111;
  switch (soundFormat) {
    case 0:
      return "Linear PCM, platform endian";
    case 1:
      return "ADPCM";
    case 2:
      return "MP3";
    case 3:
      return "Linear PCM, little endian";
    case 4:
      return "Nellymoser 16kHz mono";
    case 5:
      return "Nellymoser 8kHz mono";
    case 6:
      return "Nellymoser";
    case 7:
      return "G.711 A-law";
    case 8:
      return "G.711 mu-law";
    case 9:
      return "Reserved";
    case 10:
      return "AAC";
    case 11:
      return "Speex";
    case 14:
      return "MP3 8kHz";
    case 15:
      return "Device-Specific sound";
    default:
      return "Unknown (" + std::to_string(soundFormat) + ")";
  }
}

std::string FLVAudioTag::soundRateName(bool concise = false) const {
  int sampleRate = (soundFlags & 0b00001100) >> 2;
  switch (sampleRate) {
    case 0:
      return concise ? "5.5" : "5.5kHz";
    case 1:
      return concise ? "11" : "11kHz";
    case 2:
      return concise ? "22" : "22kHz";
    case 3:
      return concise ? "44" : "44kHz";
    default:
      return "Unknown (" + std::to_string(sampleRate) + ")";
  }
}

std::string FLVAudioTag::soundSizeName(bool concise = false) const {
  int bitDepth = (soundFlags & 0b00000010) >> 1;
  switch (bitDepth) {
    case 0:
      return concise ? "8" : "8-bit";
    case 1:
      return concise ? "16" : "16-bit";
    default:
      return "Unknown (" + std::to_string(bitDepth) + ")";
  }
}

std::string FLVAudioTag::desc() const {
  return "SoundFormat: \"" + soundFormatName() + "\" " + "SoundRate: \"" +
         soundRateName() + "\" " + "SoundSize: \"" + soundSizeName() + "\" " +
         "SoundType: \"" + (isStereo() ? "Stereo" : "Mono") + "\"";
}

std::vector<std::string> FLVAudioTag::csv_headers() {
  std::vector<std::string> out = {
    "audio_format",
    "audio_rate",
    "audio_size",
    "audio_type",
    "audio_aac_packet_type",
  };
  return out;
}

std::string FLVAudioTag::csv() const {
  return soundFormatName() + "," +                 // SoundFormat
         soundRateName(true) + "," +               // SoundRate
         soundSizeName(true) + "," +               // SoundSize
         (isStereo() ? "stereo" : "mono") + "," +  // SoundType
         std::to_string(aacPacketType);            // AACPacketType
}

bool FLVAudioTag::isStereo() const { return soundFlags & 0b00000001; }
