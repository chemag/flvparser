//
// Created by raytine on 2019/4/11.
//

#include "FLVScriptTag.h"

#include <iostream>

std::string FLVScriptTag::desc() const { return "unknown"; }
std::string FLVScriptTag::csv() const { return ""; }

FLVScriptTag::FLVScriptTag(char *data, uint32_t length)
    : data(data), length(length) {}

FLVScriptTag::FLVScriptTag(const FLVScriptTag &tag) {
  length = tag.length;
  data = new char[length];
  std::copy(tag.data, tag.data + length, data);
}

FLVScriptTag &FLVScriptTag::operator=(const FLVScriptTag &tag) {
  if (this != &tag) {
    delete[] (data);

    length = tag.length;
    data = new char[length];
    std::copy(tag.data, tag.data + length, data);
  }
  return *this;
}

FLVScriptTag::~FLVScriptTag() { delete[] (data); }
