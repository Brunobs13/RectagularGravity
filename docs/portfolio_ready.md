# Portfolio Ready Assets

## LinkedIn Version (Short)

Rebuilt an academic C rectangle-gravity assignment into a production-structured graphics project: modular simulation engine, unit tests, CI pipeline, and an artistic interactive website with live metrics and generated frame gallery.

## CV Version (Technical)

Architected and implemented a dual-layer system for rectangular gravity simulation: a modular C11 core (`collision`, `gravity`, `instability`) and a Canvas-based computational graphics website for real-time visualization and artistic frame capture. Added Makefile build/test automation, CI checks, repo hardening, and technical documentation for interview readiness.

## 60-Second Pitch

This project started as two monolithic C files from an academic assignment. I transformed it into a production-grade repository by modularizing the simulation core, introducing tests and CI, and building a professional website that visualizes the gravity world as computational art. The web layer supports scene creation, movement controls, instability tracking, and frame capture into a gallery. It demonstrates both systems-level C engineering and product-focused frontend execution in one coherent portfolio artifact.

## 5-Minute Technical Pitch

The core challenge was preserving algorithmic correctness while raising engineering quality and visual impact. I first decomposed the simulation into explicit C modules with an API contract: create rectangle, move by point, apply gravity, and compute instability. This removed ad-hoc global coupling and made behaviors testable.

Next, I implemented a deterministic test suite and CI pipeline to guard the simulation invariants. In parallel, I built an artistic computational graphics website using Canvas: it mirrors simulation constraints, renders glowing geometry compositions, and provides real-time metrics and gallery snapshots.

Architecturally, this separation gives clear benefits:

- C core remains interview-credible systems code.
- Website enables immediate interactive demonstration.
- Legacy files remain archived for traceability.

Trade-off: simulation logic currently exists in C and JS. Future evolution should compile the C core to WebAssembly for a single source of truth.

The result is a repository that is both technically rigorous and presentation-ready for international hiring contexts.
