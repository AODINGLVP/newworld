DirectX 12 FPS Graphics Demo

A real-time rendering focused first-person shooter demo built with C++ and DirectX 12.

This project was developed as part of a Computer Graphics coursework, with emphasis on modern rendering techniques, engine-level systems, and performance optimization rather than gameplay complexity.

Features

⚙️ DirectX 12 Rendering Pipeline

🌱 Instanced Rendering for large-scale scenes

💡 Directional Lighting & Normal Mapping

🦴 Skeletal Animation System

🌬️ Vertex Animation (grass wind effect)

💥 Particle Effects (spark-like impacts)

📦 AABB & Ray-based Collision Detection

🌌 Skybox Rendering

🧾 Data-Driven Scene Construction (CSV)

🖥️ UI Rendering in Screen Space
🎮 Controls
Action	Input
Move	W / A / S / D
Look / Aim	Mouse
Fire	Left Mouse Button
Aim Down Sight	Right Mouse Button
Reload	R
🧠 Technical Overview
🔺 Rendering System

Multiple rendering pipelines are implemented to support different object types.

Instanced rendering significantly reduces CPU draw-call overhead when rendering large numbers of similar objects (e.g. grass, trees).

Alpha testing is applied in the pixel shader for foliage and other transparent materials.

💡 Lighting & Materials

Directional lighting based on Lambert’s cosine law.

Normal mapping implemented using TBN matrices to enhance surface detail.

Ambient lighting ensures minimum illumination across the scene.

🦴 Animation System

Skeletal animation evaluated per frame and applied in shaders.

Gameplay-driven animation state control:

T-Rex: run / attack / death

UZI: idle / aim / fire / reload (five-state animation system)

🌿 Vertex Animation (Grass)

Grass motion simulated in the vertex shader using:

Time- and wind-based offsets

UV-weighted displacement (larger motion at tips)

Spatial phase offsets to avoid synchronized motion

📦 Collision Detection

AABB collision detection for object interaction.

Ray–AABB intersection for hit detection.

Rotated bounding boxes are handled by recomputing bounds from transformed corner vertices.

💥 Particle Effects

Spark-like particles are spawned on successful ray hits.

Particle direction and speed are generated using pseudo-random functions.

Rendered on billboards with distance-based intensity falloff.

🧾 Data-Driven Design

Scene data (object placement, instancing information) is loaded from CSV files.

Meshes and textures are managed through centralized manager systems to reduce redundancy and memory usage.

⚡ Performance

Performance was evaluated by comparing standard rendering and instanced rendering under identical object counts.

Instanced rendering achieves significantly higher frame rates when rendering many similar objects.

Demonstrates how CPU draw-call overhead becomes a major bottleneck without instancing.

⚠️ Limitations

Project structure is relatively tightly coupled due to incremental development.

Some systems would benefit from clearer abstraction and inheritance.

Minor z-fighting artifacts exist in the UZI model due to closely positioned meshes.

🚀 Future Improvements

Refactor project structure with clearer module boundaries.

Introduce inheritance hierarchies to reduce duplicated logic.

Improve texture management to avoid redundant storage.

Further optimize animation and rendering pipelines.
