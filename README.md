![toy](media/toybanner.png)
# toy
toy is a **small, simple, code oriented, modular** *c++ game engine*.  

toy puts the emphasis on games made of **simple, expressive native c++ code** extended through **tools, editors and scripting**, to design full featured 2d or 3d games in **fast iterations**.

Focused on **versatility** at the core, it gives full control over its **expressive rendering and user interface systems**, making it suitable to build **any kind of game**.

Designed for **quick iterations** and experimentation, it is deeply **hackable**, entirely **scriptable**, and provides a **fully programmable 3d renderer**, giving you direct access to *shaders, materials and render paths*.

As a collection of game programming **building blocks**, it also aims to foster an ecosystem of *simple minimal reusable components* for everyone, and it is suited for **building your own game technology**.

On top of these fundamentals, toy adds plenty of game programming systems such as physics, a navigation system, procedural generation algorithms, many tools, and more.

# examples
here are 3 live toy demo games: a [strategic space sim game](), a [tanks real time strategy game](), and a [procedural platformer]() game.

they were designed to showcase toy strongest features, and aspects that are notably hard to achieve using other engines:
- real-time procedural generation of world geometry
- complex user interfaces intertwined with game logic
- code-based dynamic rendering of thousands of instances
- custom rendering paths and custom shaders
- physics based game logic

each of these examples is written in less than a hundred lines of code and the source is fully available: reading these examples is the first place to start learning how to write a game with toy.

# overview
overarching principles:
- **simple and lightweight**, simplicity is the ultimate aim and philosophy behind toy, and toy is so lightweight, even the engine editor can be run on the web
- **modular**, each functionality is enclosed in a small, simple, easy to understand code building block
- **extensible**, as a collection of modules, toy can be used to build your own game technology out of a subset of components fitting your use case, complemented by your own functionalities
- **game code first**, toy is first and foremost meant to write games in native c++ code, directly, effortlessly, in direct contact with the fundamental systems
- **versatile**, typical engines are often biased towards specific kinds of games (platformers, shooters), toy on the other hand has been designed from the start with unconventional or complex games in mind, such as *real time strategy games* or *role playing games*. this is mostly enabled by its emphasis on native game code, and the powerful user interface and rendering systems.
- **tools second**, reflection automatically extends your game core code for seamless scripting, editing, inspection of your game objects, types and procedures in the built-in tools/editor. 
- **custom tools**, the modularity coupled with the beauty of the reflection and ui system, makes writing your own tools one of the strikingly easy things enabled by toy
- **educative**, toy is meant to be taken apart and studied, each technical solution aims to be a "most common denominator", a potential example of a minimal solution to a given problem
- **fast iteration**, providing seamless bindings of both built-in systems and game code to various scripting languages, as well as an experimental hot-reload of native code, the aim is to bring iteration speed to the maximum possible

features:
- **mud** is the low-level library providing most of the base features behind **toy**, in separate modules
  - reflection
  - scripting
  - rendering
  - user interface
- entity
- physics
- navigation

# how to build

on linux :

```bash
git clone https://github.com/hugoam/toy --recursive
cd toy
bin/genie --gcc=linux-gcc gmake
cd build/projects/gmake-linux
make config=debug64 -j8
```
