## FLVParser

A simple FLV file parser, pure C++ implementation


# Instructions


(1) build the binary
```
$ mkdir build
$ cd build
$ cmake ..
$ cd ..
```

(2) Run the parser in the test file:
```
$ ./build/src/flvparser media/888888.flv |head
FLV header:   stream info: [audio: true, video: true]
[1]   Script Tag: Script Tag, nothing special,  size:418
[2]   Video Tag: Format:AVC, Key Frame,   size:47
[3]   Audio Tag: Format:AAC, 44 kHz, 16-bit samples, Stereo,  size:4
[4]   Audio Tag: Format:AAC, 44 kHz, 16-bit samples, Stereo,  size:207
[5]   Audio Tag: Format:AAC, 44 kHz, 16-bit samples, Stereo,  size:206
[6]   Audio Tag: Format:AAC, 44 kHz, 16-bit samples, Stereo,  size:208
[7]   Audio Tag: Format:AAC, 44 kHz, 16-bit samples, Stereo,  size:208
...
```
