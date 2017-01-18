# Audio Pass

Tools for real-time audio re-synthesis.

## Required libs 

- For Testing: [cunit](http://cunit.sourceforge.net/)
- For Compliation: [portaudio](http://www.portaudio.com/docs.html)

For now the makefile expects that portaudio be compiled manually. The make file
expects to find `./portaudio/`  `./audio-pass/` in the same directory.

```
./portaudio/
./audio-pass/
```

### Debian/Ubuntu Dependencies

```bash
$ # Install tools
$ sudo apt install make binutils gcc
$ # Install dependencies
$ sudo apt install libncursesw5-dev libasound-dev libcunit1-dev libjack-jackd2-dev
```

### OSX Dependencies

```bash
$ # Assuming you have developer tools and brew installed
$ brew install cunit
```

### Compile/Run

```bash
# First install dependencies for you system
$ make tests && ./bin/testCCRing
$ make && ./bin/pass
```
