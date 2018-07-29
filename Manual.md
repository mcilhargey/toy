# toy engine manual

## project setup

## simple app
```cpp
class MyGame : public GameModule
{
public:
    virtual void init(GameShell& app, Game& game) {}
    
    virtual void start(GameShell& app, Game& game) {}
    
    virtual void pump(GameShell& app, Game& game) {}
    
    virtual void scene(GameShell& app, GameScene& scene) {}
};

int main()
{
    GameShell app(TOY_RESOURCE_PATH, argc, argv);

    GameModule module;
    app.run_game(module);
}
```

## 

## entities
In toy we separate the game entities and the rendered objects (models, meshes, particles).
Entities represent objects on the game logic side, localized in space, aggregating an array of components of your choice.

A very simple example of an entity could look like this :
```
class Human : public Complex
{
public:
	Human(Id id, Entity& parent, const vec3& position);

	Entity m_entity;
	Movable m_movable;
};
```

toy doesn't feature flexible entity which you add and remove components to on-the-fly, because we prefer the convenience and safety of defining entity types.
this feature will be added in the future for users wishing to assemble dynamic entities in the editor.

The Entity component holds the 3d transform of an object, and a list of child entities.
It is the root component for organizing a nested hierarchy of objects.
To move an entity, you can directly modify the `position`, `rotation` members, or use the mutating functions.

The Movable component is used for objects that move in space over time.
It holds a linear velocity and an angular velocity.

## reflection

### annotating code
The prerequisite to using generic [primitives](#primitives) and [operations](#operations) on any given c++ construct, is to produce the necessary reflection data, for this construct.  
This is done by annotating the code in the following manner, which is then fed to a precompiler or reflection [generator](#generator) :
```c++
enum class refl_ ShapeType
{
    Sphere,
    Cube
};

class refl_ MyObject
{
public:
    constr_ MyObject(int number, std::string name);

    meth_ int method();

    attr_ int m_number;
    attr_ ShapeType m_shape;
};

func_ void bar(MyObject& object, int value);
```
Notice how this is a standard c++ definition, with some added [reflection hints](#hints): `refl_` to reflect a class, `_constr_` to reflect a constructor, `_meth_` for a reflected method, and `_attr_` for a reflected attribute.

Using these annotations, the reflection [generator](#generator) produces a couple of reflection files, which declares and registers [constructs](#constructs) using mud corresponding types: [functions](../src/obj/Reflect/Method.h), [types](../src/obj/Reflect/Meta.h), [classes](../src/obj/Reflect/Class.h), [class members](../src/obj/Reflect/Member.h), [class methods](../src/obj/Reflect/Method.h), [enums](../src/obj/Reflect/Enum.h).  

### precompiling

## scripting

## context

## math

## geometry

## graphics

### resources

#### loading resources
the following graphics resources are loaded from files of standard formats:
- models: `.gltf` and `.obj` files
- programs: `.sc` files
- textures: `.png`, `.jpg`, `.raw`, etc (full list of formats supported by the bimg library)

the following graphics resources are created and managed by the graphics system:
- meshes
- materials

To load a resource, you need to use the AssetStore using the `load()` function, passing the path of the resource to be loaded.
Each Asset Store holds the specific operations to do in order to load a given type of resource. 

```
Texture* texture = gfx_system.textures().file("my_texture.png");
Model* model = gfx_system.textures().file("my_model.gltf");
```

Resources lifetimes are managed by the user explicitly: to free a resource, simply call

```
gfx_system.textures().release(texture);
```

#### asynchronous loading
resources can be loaded in a background thread. the returned pointer is null until the resource has actually loaded.

### basic rendering

#### scene, viewport and camera
Scenes represent a graph of objects to be rendered.
Cameras represent the properties of the viewpoint used for rendering the Scene.
Viewports represent a rectangle on a render target to render a given Camera to.

As a user of toy, you will mostly be creating Viewers, which are interface elements themselves holding a Camera and a Viewport.
In this case you can interact with the underlying Camera and Viewport directly to change their parameters.
If you want to interact with the rendering layer directly though, the only things needed to start rendering is a Camera, a Viewport, and a Scene.
```
Scene scene = { gfx_system };
Camera camera = { &scene };
Viewport viewport = { camera, scene };

gfx_context.m_viewports.push_back(&m_viewport);

while(true)
{
    // rendering logic
}
```

##### camera
Cameras are defined by two points in space: the eye, and the target.

Like most classes of toy, the Camera fields can be modified on-the-fly, for direct effect.
- `m_ortographic`: whether or not this camera is an ortographic projection (if not set, it's a perspective projection)
- `m_fov`: the vertical field of view of the camera (horizontal is derived from this and the aspect ratio)
- `m_aspect`: the aspect ratio of the camera (this is updated automatically from the viewport proportions)
- `m_height`: the span of the vertical side of the ortographic view, in real world units

On each frame, you can change these to update how the camera is rendered.

```
camera.m_ortographic = false;
camera.m_fov = 90.f;
```

The `m_lod_offsets` field holds 4 distances that can be tweaked to determine how the 4 LOD are distributed along the linear depth scale.

##### viewport
Everything related to the specific effects applied to a rendering is applied to the viewport.

#### declarative graph
Every rendered object in toy is added to a graph through a declarative idiom.
This means you don't need to manage the lifetimes of the rendered objects yourself: you just need to traverse, each frame, the declarations of what needs to be rendered, and the lifetime management is handled for you.

```
// inside the drawing loop
Gnode& root = scene.begin();
Model& model = gfx_system.models().file("my_model.obj");
gfx::item(root, model);
```

#### meshes and models

##### meshes
Meshes are the primitive geometry units used when rendering a scene. They map to the different primitives exposed by the rendering APIs: lines, line strips and loops, triangles, triangle strips and loops.

Meshes are never dealt with directly when rendering, because meshes are always rendered as part of a Model, which is simply a group of mesh with given transforms and materials for each mesh instance.

###### creating meshes
Meshes are just a primitive type, and a set of buffers filled with data describing their vertices and indices. Filling these buffers is usually done by the different Model importers, but you can also create Meshes manually.

###### cached meshes
Meshes are usually sent directly to the GPU, after the necessary attributes have been computed, like bounding volume. However you can specify a mesh to be kept accessible on the CPU side to access its vertices later-on.

To read a cached mesh, you need to fetch the `MeshData` stored in the `m_cache` attribute of the Mesh, and iterate over the vertices.
```
MeshData data = mesh.m_cache;

for(size_t i = 0; i < data.m_vertices.size(); ++i)
{
    vec3 vertex = data.position());
    // use the vertex position
}
```
    
##### models
###### importing models

Models are imported by importers defined for each specific format. So far, toy provides an OBJ importer and a glTF importer.

The import process can be configured using a ModelConfig structured passed to the importer.
- `m_transform`: apply the transformation in this matrix to each imported mesh. this matrix can be easily composed using the math utility functions : `translation()`, `rotation()`, `scale()`.
- `m_format`: force importing as a specific format
- `m_normals`: whether or not normals must be imported
- `m_tangents`: whether or not tangents must be imported
- `m_cache_meshes`: force caching of imported meshes on CPU-side for later access

When rendering a model, the materials applied are the material specified in each ModelItem.
You can override the material, either for the whole model (i.e. all meshes) by passing a material, or you can override specific materials by creating a model variant.

#### textures

#### shaders, programs, and materials
##### programs and shaders
A Program in toy is defined as a combination of Shaders for a given draw call.
Usually, a Program is composed of a Vertex Shader and a Fragment Shader, which are defined in the bgfx shader language, which compiles to the target shader language required by the platform toy is running on.

##### materials
A Material in toy is simply a set of parameters for a given (GPU) Program to execute correctly. In programming terms, the Program is a (very complex) function executed on the GPU, and the Material holds some of the specific parameters for that function invocation. Other parameters will come from other sources (Camera, Scene, Lights, Environment...).

To represent that distinction, Programs are composed of Blocks. A Program relies on a set of functional Blocks, some of which are Material Blocks, and some other Blocks. A Material associated to this Program holds one of each Material Blocks expected by the Program. The other Blocks are provided by the Scene, the Camera, etc...

A Block is simply a structure, holding data, uploaded to the GPU, to be used by the Program, on each render call.

For example, an Unshaded Material will be composed of the BaseMaterialBlock, and the UnshadedBlock. The Program expects only these two blocks, as well as the general RenderBlock holding camera, projection data.

The BasicBlock holds the following Material attributes, which are used by every Program:
- `m_cull_mode`: 
- `m_depth_draw_mode`:
- `m_depth_test`:
- `m_uvX_scale`: a scale applied to the X (0 or 1) set of UV 
- `m_uvX_offset`: an offset added to the X (0 or 1) set of UV
- `m_is_alpha`: whether this material uses alpha blending

```
// fetch the program
Program& program = gfx_system.programs().file("unshaded");

// initialize a material with the required blocks
BasicBlock base_block = {};
UnshadedBlock unshaded_block = {};
Material material = { program, base_block, { unshaded_block } };
```

##### material parameters
Material parameters, or Material Block parameters (since each one is accessed from its block), are set just like regular C++ attributes on any object.

Their only particularity is that most parameters are of the type MaterialParam<T>, which holds three things:
- the base value
- the texture bound to this parameters
- the texture channel on which this specific parameter value is encoded

```
material.block<UnshadedBlock>().m_color.m_value = Colour::White;
material.block<UnshadedBlock>().m_color.m_texture = gfx_system.textures().file("my_texture.png");
```

#### lights

#### skeletons and rigs

#### animations

### advanced rendering
#### custom programs and materials


### pbr 

#### environment and radiance

#### lights

#### shadows


### importers

#### gltf importer

#### obj importer


## user 

### input 

### declarative graph

### widgets

### viewers

### layout system

### styling

## sound

### audio files

### playing sounds

## physics

### colliders

### solids (aka rigid bodies)

### queries and raycasts

### mediums