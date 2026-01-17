# 🎮 DirectX 12 FPS Graphics Demo

This is a real-time first-person shooter (FPS) rendering demo built with **C++** and **DirectX 12**.

The project was developed as part of a **Computer Graphics coursework**, with a primary focus on modern rendering techniques, engine-level system design, and performance optimization rather than complex gameplay mechanics.

---

## ✨ Features

- ⚙️ **DirectX 12 Rendering Pipeline**
- 🌱 **Instanced Rendering** (supports large-scale scenes)
- 💡 **Directional Lighting & Normal Mapping**
- 🦴 **Skeletal Animation System**
- 🌿 **Vertex Animation** (grass wind effect)
- 💥 **Particle Effects** (hit spark effects)
- 📦 **AABB and Ray-based Collision Detection**
- 🌌 **Skybox Rendering**
- 🧾 **CSV-based Data-Driven Scene Construction**
- 🖥️ **Screen-Space UI Rendering**

---

## 🎮 Controls

| Action | Input |
|------|------|
| Move | W / A / S / D |
| Look / Aim | Mouse |
| Fire | Left Mouse Button |
| Aim | Right Mouse Button |
| Reload | R |

---

## 🧠 Technical Overview

### Rendering System
- Multiple rendering pipelines are implemented to support different object types.
- Instanced rendering significantly reduces CPU draw-call overhead when rendering large numbers of similar objects.
- Alpha testing is applied in pixel shaders for vegetation and other cutout materials.

### Lighting & Materials
- Directional lighting based on Lambert’s cosine law.
- Normal mapping with tangent-space normals transformed using the TBN matrix.
- Ambient lighting ensures a minimum level of scene illumination.

### Animation System
- Vertex animation is used to simulate wind effects on grass.
- Skeletal animation is evaluated per frame and applied in shaders.
- Gameplay-driven animation state control:
  - **T-Rex:** walk / attack / death
  - **UZI:** idle / aim / fire / reload

### Particle Effects
- Spark-like particle effects are triggered when a ray hits a target.
- Particle directions and speeds are generated using pseudo-random values.
- Billboard rendering is used with distance-based intensity attenuation.

---

## ⚡ Performance Analysis

By comparing frame rates between **standard rendering** and **instanced rendering**, the results demonstrate that instancing effectively mitigates CPU bottlenecks caused by a large number of draw calls.

---

## 📂 Project Repository

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
- 引入了法线贴图并且使用TBN矩阵进行转换。
- 引入环境光以保证场景的最低亮度。

### 动画系统
- 模拟吹动草场的顶点动画
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



## 📂 项目仓库

🔗 https://github.com/AODINGLVP/newworld.git

