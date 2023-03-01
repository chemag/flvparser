#include <getopt.h>
#include <stdlib.h>

#include <climits>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>

#include "FLVParser.h"

extern int optind;

// default option values

typedef struct arg_options {
  int debug;
  char *infile;
  char *csvfile;
  int nrem;
  char **rem;
} arg_options;

// default option values
arg_options DEFAULTS{
    .debug = 0,
    .infile = nullptr,
    .csvfile = nullptr,
};

void usage(char *name) {
  fprintf(stderr, "usage: %s [options] infile\n", name);
  fprintf(stderr, "where options are:\n");
  fprintf(stderr, "\t-d:\t\tIncrease debug verbosity [%i]\n", DEFAULTS.debug);
  fprintf(stderr, "\t-q:\t\tZero debug verbosity\n");
  fprintf(stderr, "\t-c <csvfile>:\t\tDump results in CSV file\n");
  fprintf(stderr, "\t-h:\t\tHelp\n");
  exit(-1);
}

// long options with no equivalent short option
enum {
  QUIET_OPTION = CHAR_MAX + 1,
  HELP_OPTION,
};

arg_options *parse_args(int argc, char **argv) {
  int c;
  char *endptr;
  static arg_options options;

  // set default option values
  options = DEFAULTS;

  // getopt_long stores the option index here
  int optindex = 0;

  // long options
  static struct option longopts[] = {
      // matching options to short options
      {"debug", no_argument, nullptr, 'd'},
      // options without a short option
      {"quiet", no_argument, nullptr, QUIET_OPTION},
      {"help", no_argument, NULL, HELP_OPTION},
      {nullptr, 0, nullptr, 0}};

  // parse arguments
  while (true) {
    c = getopt_long(argc, argv, "c:dh", longopts, &optindex);
    if (c == -1) {
      break;
    }
    switch (c) {
      case 0:
        // long options that define flag
        // if this option set a flag, do nothing else now
        if (longopts[optindex].flag != nullptr) {
          break;
        }
        printf("option %s", longopts[optindex].name);
        if (optarg) {
          printf(" with arg %s", optarg);
        }
        break;

      case 'd':
        options.debug += 1;
        break;

      case QUIET_OPTION:
        options.debug = 0;
        break;

      case 'c':
        options.csvfile = optarg;
        break;

      case HELP_OPTION:
      case 'h':
        usage(argv[0]);

      default:
        printf("Unsupported option: %c\n", c);
        usage(argv[0]);
    }
  }

  // require 1 extra parameter
  if (argc - optind != 1) {
    fprintf(stderr, "need infile\n");
    usage(argv[0]);
    return nullptr;
  }

  options.infile = argv[optind];
  return &options;
}

int main(int argc, char **argv) {
  arg_options *options;

  // parse args
  options = parse_args(argc, argv);
  if (options == nullptr) {
    usage(argv[0]);
    exit(-1);
  }

  // print args
  if (options->debug > 1) {
    printf("options->debug = %i\n", options->debug);
    printf("options->infile = %s\n",
           (options->infile == nullptr) ? "nullptr" : options->infile);
    printf("options->csvfile = %s\n",
           (options->csvfile == nullptr) ? "nullptr" : options->csvfile);
    for (int i = 0; i < options->nrem; ++i) {
      printf("options->rem[%i] = %s\n", i, options->rem[i]);
    }
  }

  std::unique_ptr<FLVParser> parser = std::make_unique<FLVParser>(
      options->infile, options->csvfile != nullptr ? options->csvfile : "");
  parser->parse();

  return 0;
}
