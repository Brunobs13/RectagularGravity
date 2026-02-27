# Technical Deep Dive

## 1) Architecture and Design Decisions

### End-to-end flow

1. C core validates rectangle operations (`create`, `move`, `gravity`).
2. Collision and support constraints are evaluated deterministically.
3. Instability score is computed from unsupported base proportion.
4. CLI mode exposes command-driven workflows.
5. Web mode renders equivalent logic artistically in Canvas and captures gallery frames.

### Why separate C core and Web layer

- C core remains the authoritative simulation artifact for systems-level interviews.
- Web layer turns algorithm behavior into a product experience.
- Separation improves testability and future wasm/API integration.

### Trade-offs

- Duplicating logic in C and JS increases sync responsibility.
- Advantage: immediate browser performance and zero backend dependency.
- Mitigation path: WebAssembly adapter to reuse C directly in browser.

## 2) Junior Interview Questions

1. Why do we use a struct-based world model in C?
2. How does collision detection work in axis-aligned rectangles?
3. What is the role of `apply_gravity` in the simulation loop?
4. Why are unit tests important even for small C projects?
5. What does instability score represent?
6. Why isolate legacy code instead of deleting it?

## 3) Senior Interview Questions

1. How would you guarantee parity between C core and JavaScript visual logic?
2. How would you migrate this to WebAssembly without breaking UX?
3. How would you redesign gravity for large-scale scenes (10k+ objects)?
4. What spatial indexing strategy would you introduce (grid, BVH, quadtree)?
5. How would you instrument this for frame-time and simulation profiling?
6. How would you handle concurrent scene editing in a collaborative version?

## 4) Critical Code Sections

### `src/core/rect_world.c`

- `collides()` and `collides_with_any()` define world consistency.
- `rg_world_apply_gravity()` enforces iterative relaxation until stable support.
- `rg_world_instability()` computes unsupported ratio and averages unstable cases.

Likely interview prompts:

- complexity analysis of gravity loop
- correctness under edge cases
- numerical interpretation of instability metric

### `src/cli/main.c`

- parses command format and maps to engine API.
- returns explicit errors for invalid operations.

Likely interview prompts:

- argument parsing robustness
- error propagation from core to interface

### `web/app.js`

- visual simulation rendering and interactive controls.
- gallery capture strategy through canvas snapshots.

Likely interview prompts:

- rendering-performance trade-offs
- deterministic replay and scene serialization
