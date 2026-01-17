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

# 🎮 DirectX 12 FPS 图形演示项目

这是一个基于 **C++** 与 **DirectX 12** 的第一人称射击（FPS）实时渲染演示项目。

本项目作为 **计算机图形学课程作业** 开发，重点放在现代渲染技术、引擎级系统设计以及性能优化上，而非复杂的玩法设计。

---

## ✨ 功能特性

- ⚙️ **DirectX 12 渲染管线**
- 🌱 **实例化渲染**（支持大规模场景）
- 💡 **方向光照与法线贴图**
- 🦴 **骨骼动画系统**
- 🌿 **顶点动画**（草地风场效果）
- 💥 **粒子效果**（命中火花）
- 📦 **AABB 与射线碰撞检测**
- 🌌 **天空盒渲染**
- 🧾 **基于 CSV 的数据驱动场景构建**
- 🖥️ **屏幕空间 UI 渲染**

---

## 🎮 操作方式

| 操作 | 输入 |
|------|------|
| 移动 | W / A / S / D |
| 视角 / 瞄准 | 鼠标 |
| 射击 | 鼠标左键 |
| 瞄准 | 鼠标右键 |
| 装填 | R |

---

## 🧠 技术概览

### 渲染系统
- 实现了多条渲染管线以支持不同类型的物体。
- 使用实例化渲染显著减少大量相似物体绘制时的 CPU Draw Call 开销。
- 在像素着色器中对植被等材质应用 Alpha Test。

### 光照与材质
- 基于 Lambert 余弦定律的方向光照模型。
- 使用 TBN 矩阵实现法线贴图。
- 引入环境光以保证场景的最低亮度。

### 动画系统
- 骨骼动画在每帧计算并在着色器中应用。
- 基于游戏逻辑的动画状态控制：
  - **T-Rex：** 行走 / 攻击 / 死亡
  - **UZI：** 待机 / 瞄准 / 射击 / 装填

### 粒子效果
- 射线命中目标时触发火花粒子效果。
- 使用伪随机数生成粒子的方向与速度。
- 通过 Billboard 渲染并基于距离进行亮度衰减。

---

## ⚡ 性能分析

通过对比 **普通渲染** 与 **实例化渲染** 的帧率表现，可以看出实例化渲染有效缓解了由于大量 Draw Call 导致的 CPU 性能瓶颈。

---

## ⚠️ 项目局限

- 由于项目逐步迭代开发，部分模块耦合较紧。
- 某些系统在抽象层次上仍有优化空间。
- 个别模型存在轻微的 Z-Fighting 现象。

---

## 🚀 未来改进方向

- 重构整体项目架构。
- 引入更清晰的继承与模块关系。
- 优化纹理管理方式以减少冗余。
- 进一步优化渲染与动画系统。

---

## 📂 项目仓库

🔗 https://github.com/AODINGLVP/newworld.git

