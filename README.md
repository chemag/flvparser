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
[1]   TagType: Script Filter: 0 DataSize: 418 Timestamp: 0 StreamID: 0 unknown
[2]   TagType: Video Filter: 0 DataSize: 47 Timestamp: 0 StreamID: 0 CodecId: "AVC" FrameType: "Key Frame" AVCPacketType: "AVC sequence header" CompositionTime: 0
[3]   TagType: Audio Filter: 0 DataSize: 4 Timestamp: 0 StreamID: 0 SoundFormat: "AAC" SoundRate: "44kHz" SoundSize: "16-bit" SoundType: "Stereo"
[4]   TagType: Audio Filter: 0 DataSize: 207 Timestamp: 0 StreamID: 0 SoundFormat: "AAC" SoundRate: "44kHz" SoundSize: "16-bit" SoundType: "Stereo"
[5]   TagType: Audio Filter: 0 DataSize: 206 Timestamp: 29 StreamID: 0 SoundFormat: "AAC" SoundRate: "44kHz" SoundSize: "16-bit" SoundType: "Stereo"
[6]   TagType: Audio Filter: 0 DataSize: 208 Timestamp: 59 StreamID: 0 SoundFormat: "AAC" SoundRate: "44kHz" SoundSize: "16-bit" SoundType: "Stereo"
[7]   TagType: Audio Filter: 0 DataSize: 208 Timestamp: 99 StreamID: 0 SoundFormat: "AAC" SoundRate: "44kHz" SoundSize: "16-bit" SoundType: "Stereo"
...
```
