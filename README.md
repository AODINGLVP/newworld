# 🎮 DirectX 12 FPS Graphics Demo

A real-time rendering focused first-person shooter demo built with **C++** and **DirectX 12**.

This project was developed as part of a **Computer Graphics coursework**, with emphasis on modern rendering techniques, engine-level systems, and performance optimization rather than gameplay complexity.

---

## ✨ Features

- ⚙️ **DirectX 12 Rendering Pipeline**
- 🌱 **Instanced Rendering** for large-scale scenes
- 💡 **Directional Lighting & Normal Mapping**
- 🦴 **Skeletal Animation System**
- 🌿 **Vertex Animation** (grass wind effect)
- 💥 **Particle Effects** (spark-like impacts)
- 📦 **AABB & Ray-based Collision Detection**
- 🌌 **Skybox Rendering**
- 🧾 **Data-Driven Scene Construction (CSV)**
- 🖥️ **UI Rendering in Screen Space**

---

## 🎮 Controls

| Action | Input |
|------|------|
| Move | W / A / S / D |
| Look / Aim | Mouse |
| Fire | Left Mouse Button |
| Aim Down Sight | Right Mouse Button |
| Reload | R |

---

## 🧠 Technical Overview

### Rendering System
- Multiple rendering pipelines support different object types.
- Instanced rendering significantly reduces CPU draw-call overhead for large numbers of similar objects.
- Alpha testing is applied in pixel shaders for foliage and transparent materials.

### Lighting & Materials
- Directional lighting based on Lambert’s cosine law.
- Normal mapping using TBN matrices.
- Ambient lighting ensures minimum scene illumination.

### Animation System
- Skeletal animation evaluated per frame and applied in shaders.
- Gameplay-driven animation state control:
  - **T-Rex:** run / attack / death
  - **UZI:** idle / aim / fire / reload

### Particle Effects
- Spark-like particles triggered on ray hit events.
- Pseudo-random direction and speed generation.
- Billboard rendering with distance-based intensity falloff.

---

## ⚡ Performance

Performance comparison between **standard rendering** and **instanced rendering** demonstrates that instancing effectively mitigates CPU bottlenecks caused by excessive draw calls.

---

## ⚠️ Limitations

- Tight coupling due to incremental development.
- Some systems would benefit from clearer abstraction.
- Minor z-fighting artifacts in closely positioned meshes.

---

## 🚀 Future Improvements

- Refactor project architecture.
- Introduce clearer inheritance hierarchies.
- Improve texture management to reduce redundancy.
- Further optimize rendering and animation systems.

---

## 📂 Repository

🔗 https://github.com/AODINGLVP/newworld.git
