# Table of contents

## Abstract

The series has two parts. **Lectures 1 to 7** teach modern C++ from the ground up, each one a small program built from an empty folder: types and arithmetic, memory and ownership, contiguous data, the standard library, reading and writing images and JSON, and finally a window with a Vulkan device behind it.

## C++ lectures

| Lecture | Title | What it builds |
|---|---|---|
| [1](lecture_1/lecture_1.md) | Fundamentals | A loan calculator: types, arithmetic, asking the compiler for a type |
| [2](lecture_2/lecture_2.md) | Memory, ownership and RAII | A tracer that watches objects live and die, and a memory arena: scopes, references, pointers, ownership, layout and alignment |
| [3](lecture_3/lecture_3.md) | Contiguous data | Conway's Game of Life on a wrapping board: arrays, spans, strings |
| [4](lecture_4/lecture_4.md) | Type aliases and the standard library | A batch calculator: naming types, arithmetic that can't silently overflow, parsing, summaries, callbacks |
| [5](lecture_5/lecture_5.md) | Reading and writing PNG images | An image type and a PNG round trip with LodePNG |
| [6](lecture_6/lecture_6.md) | JSON-driven images | Gradient swatches described in JSON (nlohmann_json), encoded to PNG in memory, with a JSON report |
| [7](lecture_7/lecture_7.md) | A Vulkan window | An SDL window, a Vulkan instance, device and swapchain, and a frame loop |

## Game engine

### Foundations

| Chapter | Title | What it adds |
|---|---|---|
| [0](game-engine/00-project-window-device.md) | Project, window and device | The build, an SDL window, a Vulkan 1.4 device with the descriptor heap, the swapchain and the frame loop |
| [1](game-engine/01-slang-first-pipeline.md) | Slang shaders and the first pipeline | Shaders compiled at build time, a pipeline with no pipeline layout, a triangle |
| [2](game-engine/02-buffers-device-addresses-push-data.md) | GPU buffers, device addresses and push data | Buffers in GPU memory, read by the shader through 64-bit addresses handed over as push data |
| [3](game-engine/03-camera-and-depth.md) | Camera, matrices and depth | glm, a depth buffer, a fly camera with the Unreal Editor's controls |
| [4](game-engine/04-gltf-geometry.md) | Loading glTF geometry | tinygltf, Sponza's geometry, per-draw data in a buffer, clear refusals for files it can't read |
| [5](game-engine/05-textures-and-descriptor-heap.md) | Textures and the descriptor heap | Image decoding, textures in the heap, sampling by index |
| [6](game-engine/06-materials.md) | Materials | Every core glTF material input, samplers, alpha modes (opaque, mask, blend), normal maps, the material views (keys 1-8) |

### A physically lit image

| Chapter | Title | What it adds |
|---|---|---|
| [7](game-engine/07-light-exposure-hdr.md) | Light, exposure and HDR | Sun and sky in physical units, the file's lights, glTF's BRDF, an HDR image, auto exposure, tone mapping, time of day |
| [8](game-engine/08-image-based-lighting.md) | Image-based lighting | The sky lights the scene, diffuse and specular (split sum), from a photographed or simulated sky (E); the sky behind the scene; compute pipelines |
| [9](game-engine/09-ambient-occlusion.md) | Ambient occlusion | A depth and normal prepass, ground-truth AO (GTAO) in compute (O toggles) |
| [10](game-engine/10-order-independent-transparency.md) | Order-independent transparency | Weighted blended transparency: two sums and a composite, no sorting |
| [11](game-engine/11-ray-traced-shadows.md) | Ray-traced shadows | Acceleration structures, a shadow ray per pixel per light by ray queries, alpha-tested and see-through casters |
