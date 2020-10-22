# debug-fmp4

Debugging tool to verify fmp4 structure (mainly by ISO/IEC 14496-12)

## Installation

Use cmake version 3.4 or more to compile debug-fmp4. 

In project directory
```
mkdir build
cd build
cmake ..
make all install
```
**Root** privileges can be required to run *install* command

## Usage

```debug-fmp4 file.fmp4```

## Expected output

```
ftyp(66747970) size=24 position=0 major brand='isom' minor version=512 compatible brands={isom mp42}
moov(6d6f6f76) size=1158 position=24
   mvhd(6d766864) size=108 position=32 version=0 flags=0 ctime='20/10/2024 6:53:14 mtime='20/10/2024 6:53:14 timescale=1000 duration=0 rate=10000 volume=100 matrix={10000,0,0,0,10000,0,0,0,40000000} next track id=3
   trak(7472616b) size=543 position=140
      tkhd(746b6864) size=92 position=148 version=0 flags=7 ctime='20/10/2024 6:53:14 mtime='20/10/2024 6:53:14 track id=1 duration=0 layer=0 alternate group=0 volume: 0 matrix={10000,0,0,0,10000,0,0,0,40000000} width=640 height=360
      mdia(6d646961) size=443 position=240
         mdhd(6d646864) size=32 position=248 version=0 flags=0 ctime='20/10/2024 6:53:14 mtime='20/10/2024 6:53:14 timescale=30000 duration=0 language=rus
         hdlr(68646c72) size=45 position=280 handler='vide' name='VideoHandler^@'
         minf(6d696e66) size=358 position=325
            vmhd(766d6864) size=20 position=333 version=0 flags=1 graphicsmode=0 opcolor={ 0 0 0 }
            dinf(64696e66) size=36 position=353
               dref(64726566) size=28 position=361 version=0 flags=0 item count=1
                  url (75726c20) size=12 position=377 version=0 flags=1 location=''
            stbl(7374626c) size=294 position=389
               stsd(73747364) size=218 position=397 version=0 flags=0 entries=1
               size=202 format='vide' data reference index=1 width=640 height=360 hresolution=0x4800 vresolution=0x4800 frame count=1 name='' depth=24
                  hvcC(68766343) size=116 position=499 config: [ 01 60 0 0 0 90 0 0 0 0 0 3f ] min spacial segmentation: f000 framerate: 0
                  Config Set 32 [40 01 0c 01 ff ff 01 60 00 00 00 90 00 00 00 00 00 3f 92 80 90 ]
                  Config Set 33 [42 01 01 01 60 00 00 00 90 00 00 00 00 00 3f a0 05 02 01 69 65 92 a4 93 2b c0 40 40 00 00 fa 40 00 1d 4c 02 ]
                  Config Set 34 [44 01 c1 72 b4 62 40 ]
               stts(73747473) size=16 position=615 version=0 flags=0 samples={}
               stsc(73747363) size=16 position=631 version=0 flags=0 entries(first:samples:index)={ 0}
               stsz(7374737a) size=20 position=647 version=0 flags=0 samplesize=0 samplecount=0
               stco(7374636f) size=16 position=667
   trak(7472616b) size=427 position=683
      tkhd(746b6864) size=92 position=691 version=0 flags=7 ctime='20/10/2024 6:53:14 mtime='20/10/2024 6:53:14 track id=2 duration=0 layer=0 alternate group=0 volume: 100 matrix={10000,0,0,0,10000,0,0,0,40000000} width=0 height=0
. . .
```

## OS Support

Used on **Linux 64-bit** (Ubuntu 18.04.5 LTS)
