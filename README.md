# Zyme
Playground for my engine development

# Structure
The structure is based on the ECS pattern. In-game objects are `Entities` and act as containers for `Components`.
`Components` are markers of some functionality - pieces of raw data, that entities with some behavious tipically have.
The main functionality are the `Systems` - things that perform some actions to the entities with particular components.
`TransformSystem` recalculates transform of an entity with `Transform` component, `MeshRendererSystem` renders to
the screen each entity that has `Mesh`, `Material` and `Transform` components attached to it.
The order of `Systems` work is important, so `Services` are introduced. They combine some systems inside itself to
simplify their usage. The order of `Services` updates must be unimportant.

`RenderingContext` is merely something that initializes common rendering context (dependencies (glfw, glew) and other things
that require initialization). `Visualizer` is a helper object that implements some common functionality required for
debugging rendering.

# Folders
- `auxiliary` - some funny izi haks that simplify development sometimes
- `util` - useful stuff not related to rendering directly
- `wrappers` - facades over OpenGL objects made to make programming less error-prone

`entities`, `components`, `systems`, `services` and `shaders` are self-explainatory

# Example
```c++
// to display error message 
// and prevent termination
#include <iostream>

// to initialize OpenGL context 
// and common staff
#include "RenderingContext.h"

// to use systems tipically required
// for a 3d game
#include "services/GameService.h"

// to define objects
#include "entities/Entity.h"


int main() {
  if (
    // initialize common staff
    !RenderingContext::initialize() ||
    // initialize required systems
    !GameService::initialize()
  ) {
    std::cout << "Error" << std::endl;
    std::cin.get();
    return -1;
  }
  
  Entity * scene = new Entity();
  
  // while window should not close
  while (RenderingContext::proceed()) {
    // update all systems (renderer systems will draw staff)
    GameService::update(scene);
  }
  
  RenderingContext::terminate();
}
```
