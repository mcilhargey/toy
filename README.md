toy
====

toy is a hackable game engine for the 21st century


how to build
============

on linux :

```bash
git clone https://github.com/hugoam/toy --recursive
cd toy
bin/genie --gcc=linux-gcc gmake
cd build/projects/gmake-linux
make config=debug64 -j8
```