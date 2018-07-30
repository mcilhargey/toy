<p align="center"><img src="https://github.com/hugoam/toy/blob/master/media/toyengine.png" /></p>

# toy
toy is a **small, simple, code oriented, modular** *c++ game engine*.  

toy puts the emphasis on games made of **simple, expressive native c++ code** extended through **tools, editors and scripting**, to design full featured 2d or 3d games in **fast iterations**.

Focused on **versatility** at the core, it gives full control over its **expressive rendering and user interface systems**, making it suitable to build **any kind of game**.

Designed for **quick iterations** and experimentation, it is deeply **hackable**, entirely **scriptable**, and provides a **fully programmable 3d renderer**, giving you direct access to *shaders, materials and render paths*.

As a collection of game programming **building blocks**, it also aims to foster an ecosystem of *simple minimal reusable components* for everyone, and it is suited for **building your own game technology**.

On top of these fundamentals, toy adds plenty of game programming systems such as physics, a navigation system, procedural generation algorithms, many tools, and more.

### principles
- **simple and lightweight**, simplicity is the core aim and philosophy behind toy. the codebase is about one-tenth the size of competing engines, and toy is so light, the whole editor runs flawlessly in your browser !
- **modular**, each functionality is enclosed in a small, simple, easy to understand code building block. most of these blocks lie in the underlying mud library.
- **extensible**, as a collection of modules, toy is a perfect fit to build your own game technology, keeping full control over the components you use, the application design and the control flow.
- **game code first**, toy is first and foremost meant to build games in native c++ code, in direct contact with the core systems. this allows for much greater control than typical scripting in-engine.
- **versatile**, toy is designed from the start with complex games in mind, such as strategy or role playing games, by giving full control over its powerful user interface and rendering systems.
- **zero-cost tools**, reflection automatically extends your game core code for seamless scripting, editing, inspection of your game objects, types and procedures in the built-in tools/editor.
- **educative**, toy aims to provide simplest technical solutions to typical game programming problems, to be easily studied and understood, hoping to be a driver of education on game development topics.
- **fast iteration**, coupling seamless bindings of both built-in systems and game code to various scripting languages, hot-reload of native code, and immediate UI and rendering, toy provides fast iteration speeds.

### features
- **mud** is the low-level library providing most of the base features behind **toy**, in separate modules
  - reflection
  - **scripting**: Reflection automatically extends your game core code for seamless scripting via text-based languages (lua, Wren), and a node visual-scripting language, including interfaces for editing these. toy scripting component is so lean, it can be added to the game itself, for live scripting or for an in-game console.
  - rendering
  - **user interface**: A simple expressive user interface API is the fundamental block for writing any robust game or application. toy offers the best-in-class : immediate-mode widget declarations, fully automatic layout, css-like skinning, image-based skinning, style sheets, various input widgets, docking windows and tabs.
- entity
- physics
- navigation

### examples
here are 3 live toy demo games: a [strategic space sim game](), a [tanks real time strategy game](), and a [procedural platformer]() game.

they were designed to showcase toy strongest features, and aspects that are notably hard to achieve using other engines:
- real-time procedural generation of world geometry
- complex user interfaces intertwined with game logic
- code-based dynamic rendering of thousands of instances
- custom rendering paths and custom shaders
- physics based game logic

each of these examples is written in less than a hundred lines of code and the source is fully available: reading these examples is the first place to start learning how to write a game with toy.

### how to build

on linux :

```bash
git clone https://github.com/hugoam/toy --recursive
cd toy
bin/genie --gcc=linux-gcc gmake
cd build/projects/gmake-linux
make config=debug64 -j8
```
