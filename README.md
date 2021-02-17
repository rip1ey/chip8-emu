## Chip8 Emulator

This a Chip8 emulator (interpreter technically) developed
with c and the SDL2 library. This emulator does not support
audio at this time.

To test the emulator, use your system's c compiler and
ensure that SDL2 is installed and that the compiler
knows where to find the headers / library files that
correspond to the required SDL2 functionality.

### Testing it Out

Compiling and executing the emulator is as simple as entering
the following commands within the repo's `src` directory:

```
sherbs@nostromo -> src gcc chip8_util.c chip8.c main.c -lSDL2 -o chip8
sherbs@nostromo -> src ./chip8 ../roms/INVADERS
```

### References

Some of the more popular / older references may not
describe certain functionality well or there are
minor discrepancies in the details. The following
references were really helpful in explaining everything,
including the multiple ways certain ambiguous instructions
might be implemented.

* [Tobias Langhoff's guide](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/) is a well-written
and detailed guide that leaves the code out. Really helpful for learning about the system without getting
the actual implementation spoiled for you
* [mattmik Mastering Chip8](http://mattmik.com/files/chip8/mastering/chip8.html) also does a great job with
explaining the overall functionality of the chip8 instructions without giving the code away
* [r/EmuDev](https://www.reddit.com/r/EmuDev/) and its Discord, which is free to join has a huge community
of emulator devs who are open to helping out.
