# Blending & Draw Order Policy

## Blend Function
- `GL_SRC_ALPHA`, `GL_ONE_MINUS_SRC_ALPHA` (standard alpha blending)
- Enabled once at `OpenGLRendererAPI::Init()`

## Draw Order
- 2D quads are drawn in **submission order** (painter's algorithm)
- Back-to-front depth ordering is achieved by the user calling `DrawQuad`/`DrawRotatedQuad` in the desired order
- No automatic sort is performed — the batch renderer preserves call order within a scene

## Depth Buffer
- Depth testing is enabled (`GL_DEPTH_TEST`) for correct 3D rendering
- For 2D blended quads: depth writes are enabled. This means:
  - Opaque quads should be drawn first (back to front)
  - Transparent quads should be drawn after opaque ones
  - For overlapping transparent quads, disable depth writes or sort manually
