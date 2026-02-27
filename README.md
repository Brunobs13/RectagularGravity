# RectagularGravity - Artistic Computational Graphics Website

A production-structured evolution of the original academic Rectangle Gravity assignment into a professional project with:

- modular C simulation engine
- automated tests + CI
- interactive artistic graphics website
- interview-ready technical documentation

## Project Overview

`RectagularGravity` simulates rectangular rigid bodies under gravity constraints and instability rules. The system now exposes this logic through both:

- a robust CLI C engine
- an immersive web-based computational graphics experience

## Business Problem

Raw algorithm assignments are usually difficult to showcase in interviews or portfolios. They lack productization, visual storytelling, and engineering quality practices. This project solves that by turning a basic simulation into:

- a demonstrable visual product
- a maintainable codebase
- a credible engineering artifact for technical hiring

## Architecture Diagram (Text)

```text
Legacy Academic Files (preserved)
            |
            v
C Simulation Core (src/core/rect_world.c)
- collision model
- gravity iteration
- instability score
            |
            +--> CLI Runtime (src/cli/main.c)
            |
            +--> Visual Mapping (web/app.js, equivalent simulation logic)
                      |
                      v
               Artistic Web UI
               - live simulation canvas
               - control panel
               - generated image gallery
```

## Tech Stack

- C11 (core simulation engine)
- JavaScript + HTML5 Canvas (graphics website)
- CSS (art direction / responsive UI)
- GNU Make (build/test/run)
- GitHub Actions (CI)

## Project Structure

```text
.
├── .github/workflows/ci.yml
├── configs/
│   └── default_scene.json
├── docs/
│   ├── portfolio_ready.md
│   ├── repository_audit.md
│   └── technical_deep_dive.md
├── include/
│   └── rect_world.h
├── legacy/
│   ├── efolio_b_labprog.c
│   └── efolio_resolvido.c
├── src/
│   ├── cli/main.c
│   └── core/rect_world.c
├── tests/
│   └── test_rect_world.c
├── web/
│   ├── app.js
│   ├── index.html
│   └── styles.css
├── .env.example
├── .gitignore
├── Makefile
└── README.md
```

## Setup Instructions

### 1) Clone

```bash
git clone https://github.com/Brunobs13/RectagularGravity.git
cd RectagularGravity
```

### 2) Build C engine

```bash
make build
```

### 3) Run tests

```bash
make test
```

### 4) Run CLI demo

```bash
make run
```

### 5) Run website

```bash
make web-run
```

Open: `http://localhost:8080`

## Website Features

- dynamic gravity simulation rendered with Canvas
- artistic gradient/glow composition
- manual rectangle creation and movement controls
- instability + occupancy metrics panel
- generated gallery of simulation frames
- manual frame capture for visual curation

## CI/CD Overview

The CI workflow compiles and tests the C simulation layer on each push/PR.

- `make build`
- `make test`

This ensures simulation regressions are blocked before merge.

## Data Versioning Strategy

The project is deterministic and code-driven, with no external dataset dependency. If scene datasets grow (e.g., curated benchmark scenes), they should be tracked with semantic file versioning under `configs/` and optionally DVC if large binary assets are introduced.

## Model Tracking Strategy

No ML model is used. If future releases include predictive stability or reinforcement placement, MLflow can be integrated for experiment tracking.

## Deployment Strategy

Current target is static web hosting + compiled local CLI.

Production path:

1. publish `web/` to static hosting (Cloudflare Pages, Netlify, GitHub Pages)
2. release C binaries via CI artifacts
3. optional API wrapper for remote simulation execution

## Security Considerations

- no hardcoded secrets
- environment template provided (`.env.example`)
- strict `.gitignore` for local artifacts and sensitive files
- legacy files preserved in isolated path without runtime coupling

## Lessons Learned

- separating simulation core from presentation dramatically improves maintainability
- visual framing increases portfolio impact of algorithmic projects
- small C projects still benefit from CI, tests, and architecture discipline

## Future Improvements

1. WebAssembly bridge: run compiled C engine directly in browser.
2. Scene import/export via JSON schema.
3. Temporal animation timeline with keyframe interpolation.
4. Instability heatmaps and support-force visualization.
5. Multi-user shared galleries for collaborative visual studies.

## Additional Documentation

- Audit report: `docs/repository_audit.md`
- Technical deep dive: `docs/technical_deep_dive.md`
- Portfolio-ready assets: `docs/portfolio_ready.md`
