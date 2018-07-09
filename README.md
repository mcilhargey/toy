# toy
toy is a small, hackable, code oriented, modular c++ game engine  
write portable games from scratch using all of toy, or assemble your own engine and editor using toy modules

# examples
here are 3 live toy demo games:
- space opera
- tank rts
- procedural platformer

these examples were designed to showcase toy strong points, and particularly, some things other engines are notably **not** good at :
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
- **tools second**, the modularity coupled with the beauty of the reflection and ui system, makes writing your own tools one of the strikingly easy things enabled by toy
- **educative**, toy is meant to be taken apart and studied, the solution aims to be a "most common denominator" that can be studied as the simplest solution to achieve a given end
- **fast iteration**

features:
- **mud** is the low-level library providing most of the base features behind **toy**, in separate modules
  - reflection
  - scripting
  - rendering
  - user interface
- entity
- physics
- navigation

# intent
toy stems from the curious wandering mind of a child: how do you make a game ? what's the machinery behind it ? how complex is it ? what are all the individual steps to put a game experience together ? can it be simple ? can all these steps be simple ? can I understand each of them ? can I design a game myself, using some building blocks and armed with that understanding ?

toy is the result of a quest to demonstrate that the machinery required **can** be simple, given enough effort to make it so; to design such building blocks, and to make them accessible to everyone to write their games, whether they are young beginners, lone coders, indie teams, or AAA teams, with a stack of technology that they can master, understand, adapt, and extend easily.

### educative
toy strives for education and accessibility of game development techniques and knowledge, by pushing for simple, accessible, auditable building blocks. particularly, toy wishes to take down the idea that the technical machinery behind games is of the highest complexity, and could be grasped and mastered only by a select few, by providing not only the components to make games from scratch, but an example of how simple these components can be.

in doing so, toy hopes to be studied by many aspiring game developers, and hopes to show them a potential example of how some typical game engine design mistakes can be avoided by selecting slightly different idioms.

### for games

### simple
simplicity is the core and most important principle underlying toy. toy aims to lower the quantity of information necessary to define a specific game to its lowest possible value. by reducing the complexity of the technology needed to make a game, both user-facing and underlying, toy sets to reduce the cognitive overhead that can cripple down many projects progress.

in general, toy is allergic to "inversion of control" object-oriented designs, and prefers top-down, high-level-to-low-level imperative control flows. this as well as other design decisions allows toy to fit in a whopping 80 thousands lines of code. many engines similar in scope go in the >500 kloc range.

### hackable
### modular 
every component can be replaced
the simplest possible paradigm for writing ui. this allows writing tools for toy to be unbelievably simple.
toy is perfectly suited to making your own tools

### code first
toy puts native code at the forefront of the development of your game. that way, the user remains in total control , the primary and preferred way of putting a game together is through assembling your own native, using
each game has their specific requirements, and each development team consists of at least one coder capable of reading through toy examples and putting an application together.

this design decision makes toy a perfect fit for teams that want control over the 

By combining systems both high and low level,
luxe is like a toolbox, and a game is the connection of these into a whole.
When configuring a type of project to reuse (like a 2D platformer or 3D first person project), luxe provides outlines, which jump start projects into predefined workflows. Outlines are where custom workflows are defined above the engine level.

luxe provides pieces that snap together,
ready to make games.

### tools second
in toy, there is no such thing as a monolithic editor: tools are independent reusable blocks, using the expressive strength of the ui system, to provide user operations for specific components (for example, manipulating scene entities), or, using the power of reflection, operating in a generic manner on any object.

this approach is key to empowering development teams, by enabling them to design and use their own custom suite of editors tailored to the needs of the specific game they are working on. an editor, in that design, is simply a collection of tools bound together by an overarching user interface managing global state and credentials.

toy provides a range of tools, such as a scene outliner, a generic object inspector, a text script editor, a visual script editor, and others. a team that doesn't need any custom tools can just build the editor template provided by toy, which bundles together all those built-in tools in a nice sleek editor.

rather than try to provide too many built-in tools from the start, toy bets on an ecosystem of specialized tools growing progressively, thanks to a terribly simple and expressive user interface paradigm, and the clear and open outline of the engine core systems. first, the basics, then tools for animating, painting, assembling will naturally 

### rendering experiments

### procedural generation

# how to build

on linux :

```bash
git clone https://github.com/hugoam/toy --recursive
cd toy
bin/genie --gcc=linux-gcc gmake
cd build/projects/gmake-linux
make config=debug64 -j8
```
