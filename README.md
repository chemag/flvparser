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
[1]   tag: Script unknown size:418
[2]   tag: Video CodecId: "AVC" FrameType: "Key Frame" AVCPacketType: "AVC sequence header" CompositionTime: 0 size:47
[3]   tag: Audio SoundFormat: "AAC" SoundRate: "44kHz" SoundSize: "16-bit" SoundType: "Stereo" size:4
[4]   tag: Audio SoundFormat: "AAC" SoundRate: "44kHz" SoundSize: "16-bit" SoundType: "Stereo" size:207
[5]   tag: Audio SoundFormat: "AAC" SoundRate: "44kHz" SoundSize: "16-bit" SoundType: "Stereo" size:206
[6]   tag: Audio SoundFormat: "AAC" SoundRate: "44kHz" SoundSize: "16-bit" SoundType: "Stereo" size:208
[7]   tag: Audio SoundFormat: "AAC" SoundRate: "44kHz" SoundSize: "16-bit" SoundType: "Stereo" size:208
...
```
