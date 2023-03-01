//
// Created by raytine on 2019/4/9.
//

#ifndef TEST_FLVPARSER_H
#define TEST_FLVPARSER_H

#include <iostream>
#include <string>

class FLVParser {
 private:
  std::string filePath;
  std::string csvPath;

 public:
  FLVParser(std::string filePath, std::string csvPath)
      : filePath(filePath), csvPath(csvPath) {}

  ~FLVParser() {}

  void parse();
};

#endif  // TEST_FLVPARSER_H
