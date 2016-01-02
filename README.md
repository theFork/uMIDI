```
       __  __ _____ _____ _____ 
      |  \/  |_   _|  __ \_   _|
 _   _| \  / | | | | |  | || |  
| | | | |\/| | | | | |  | || |  
| |_| | |  | |_| |_| |__| || |_ 
|  _,_|_|  |_|_____|_____/_____|
| |                             
|_|                             
```


The swiss army knife for quick and easy developement of MIDI applications. Possible applications are everything that uses MIDI.

![The uMIDI board](doc/hardware/board.jpg)

# Hardware
The board is created using the open-source EDA tool KiCad (BZR 5122) and updated to also work with BZR 6188. Other versions of KiCad *might* work as well.

# Software
You need Atmel's version of the `avr-gcc` toolchain, which you can download as a [precompiled version](http://www.atmel.com/tools/ATMELAVRTOOLCHAINFORLINUX.aspx) or [compile from source](http://distribute.atmel.no/tools/opensource/Atmel-AVR-GNU-Toolchain/3.4.5/). If you choose the latter, the updated and fixed version of the [build script](https://raw.githubusercontent.com/theFork/uMIDI/master/software/toolchain/build-avr8-gnu-toolchain-git.sh) in this repository may save you some trouble.
