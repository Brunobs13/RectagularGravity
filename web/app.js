const PLANE_WIDTH = 80;
const PLANE_HEIGHT = 25;

const canvas = document.getElementById("worldCanvas");
const ctx = canvas.getContext("2d");
const cellW = canvas.width / PLANE_WIDTH;
const cellH = canvas.height / PLANE_HEIGHT;

const world = {
  rects: [],
  frameCount: 0,
};

const palette = [
  [98, 240, 203],
  [255, 191, 99],
  [255, 124, 165],
  [142, 165, 255],
  [112, 225, 255],
];

function setStatus(text, isError = false) {
  const status = document.getElementById("status");
  status.textContent = text;
  status.style.color = isError ? "#ff7ca5" : "#ffbf63";
}

function insidePlane(rect) {
  return (
    rect.x >= 1 &&
    rect.y >= 1 &&
    rect.x + rect.width - 1 <= PLANE_WIDTH &&
    rect.y + rect.height - 1 <= PLANE_HEIGHT
  );
}

function collides(a, b) {
  if (a.x + a.width <= b.x || b.x + b.width <= a.x) return false;
  if (a.y + a.height <= b.y || b.y + b.height <= a.y) return false;
  return true;
}

function collidesAny(candidate, ignoreIndex = -1) {
  for (let i = 0; i < world.rects.length; i += 1) {
    if (i === ignoreIndex) continue;
    if (collides(candidate, world.rects[i])) return true;
  }
  return false;
}

function applyGravity() {
  let moved;
  do {
    moved = false;
    for (let i = 0; i < world.rects.length; i += 1) {
      while (world.rects[i].y > 1) {
        world.rects[i].y -= 1;
        if (collidesAny(world.rects[i], i)) {
          world.rects[i].y += 1;
          break;
        }
        moved = true;
      }
    }
  } while (moved);
}

function createRect(x, y, width, height) {
  const color = palette[Math.floor(Math.random() * palette.length)];
  const rect = { x, y, width, height, color };

  if (!insidePlane(rect)) {
    return { ok: false, error: "Rectangle outside plane bounds." };
  }

  if (collidesAny(rect)) {
    return { ok: false, error: "Rectangle collides with existing one." };
  }

  world.rects.push(rect);
  applyGravity();
  render();
  return { ok: true };
}

function findRectAt(px, py) {
  for (let i = 0; i < world.rects.length; i += 1) {
    const r = world.rects[i];
    if (r.x <= px && px < r.x + r.width && r.y <= py && py < r.y + r.height) {
      return i;
    }
  }
  return -1;
}

function moveByPoint(px, py, delta, direction) {
  const idx = findRectAt(px, py);
  if (idx < 0) {
    return { ok: false, error: "No rectangle found at selected point." };
  }

  const originalX = world.rects[idx].x;
  world.rects[idx].x += direction * delta;

  if (!insidePlane(world.rects[idx])) {
    world.rects[idx].x = originalX;
    return { ok: false, error: "Movement would exit plane bounds." };
  }

  if (collidesAny(world.rects[idx], idx)) {
    world.rects[idx].x = originalX;
    return { ok: false, error: "Movement would cause collision." };
  }

  applyGravity();
  render();
  return { ok: true };
}

function instability() {
  if (world.rects.length < 2) return 0;

  let total = 0;
  let unstableCount = 0;

  for (let i = 0; i < world.rects.length; i += 1) {
    const top = world.rects[i];
    let supported = 0;

    for (let j = 0; j < world.rects.length; j += 1) {
      if (i === j) continue;
      const base = world.rects[j];
      if (top.y !== base.y + base.height) continue;

      const left = Math.max(top.x, base.x);
      const right = Math.min(top.x + top.width, base.x + base.width);
      if (right > left) supported += right - left;
    }

    const supportedRatio = Math.min(1, supported / top.width);
    const unsupported = 1 - supportedRatio;
    if (unsupported > supportedRatio) {
      total += unsupported;
      unstableCount += 1;
    }
  }

  if (!unstableCount) return 0;
  return total / unstableCount;
}

function coverage() {
  let occupied = 0;
  for (const r of world.rects) occupied += r.width * r.height;
  const total = PLANE_WIDTH * PLANE_HEIGHT;
  return (occupied / total) * 100;
}

function drawBackground() {
  const grad = ctx.createLinearGradient(0, 0, canvas.width, canvas.height);
  grad.addColorStop(0, "#081021");
  grad.addColorStop(0.55, "#10253f");
  grad.addColorStop(1, "#07111f");
  ctx.fillStyle = grad;
  ctx.fillRect(0, 0, canvas.width, canvas.height);

  ctx.save();
  ctx.globalAlpha = 0.11;
  ctx.strokeStyle = "#9ab6d8";
  ctx.lineWidth = 1;
  for (let x = 0; x <= PLANE_WIDTH; x += 1) {
    ctx.beginPath();
    ctx.moveTo(x * cellW, 0);
    ctx.lineTo(x * cellW, canvas.height);
    ctx.stroke();
  }
  for (let y = 0; y <= PLANE_HEIGHT; y += 1) {
    ctx.beginPath();
    ctx.moveTo(0, y * cellH);
    ctx.lineTo(canvas.width, y * cellH);
    ctx.stroke();
  }
  ctx.restore();
}

function drawRect(rect, index) {
  const x = (rect.x - 1) * cellW;
  const y = canvas.height - (rect.y + rect.height - 1) * cellH;
  const w = rect.width * cellW;
  const h = rect.height * cellH;

  const [r, g, b] = rect.color;
  const glow = ctx.createLinearGradient(x, y, x + w, y + h);
  glow.addColorStop(0, `rgba(${r}, ${g}, ${b}, 0.78)`);
  glow.addColorStop(1, `rgba(${r}, ${g}, ${b}, 0.25)`);

  ctx.save();
  ctx.shadowColor = `rgba(${r}, ${g}, ${b}, 0.65)`;
  ctx.shadowBlur = 25;
  ctx.fillStyle = glow;
  ctx.fillRect(x + 1, y + 1, w - 2, h - 2);

  ctx.shadowBlur = 0;
  ctx.strokeStyle = `rgba(${r}, ${g}, ${b}, 0.95)`;
  ctx.lineWidth = 2;
  ctx.strokeRect(x + 1, y + 1, w - 2, h - 2);

  ctx.fillStyle = "rgba(255,255,255,0.75)";
  ctx.font = '600 12px "Space Grotesk"';
  ctx.fillText(`#${index + 1}`, x + 6, y + 16);
  ctx.restore();
}

function render() {
  drawBackground();

  const spotlight = ctx.createRadialGradient(
    canvas.width * 0.7,
    canvas.height * 0.15,
    40,
    canvas.width * 0.7,
    canvas.height * 0.15,
    canvas.width * 0.8,
  );
  spotlight.addColorStop(0, "rgba(255,255,255,0.15)");
  spotlight.addColorStop(1, "rgba(255,255,255,0)");
  ctx.fillStyle = spotlight;
  ctx.fillRect(0, 0, canvas.width, canvas.height);

  world.rects.forEach((rect, idx) => drawRect(rect, idx));
  world.frameCount += 1;
  updateMetrics();
}

function updateMetrics() {
  document.getElementById("metricRects").textContent = world.rects.length;
  document.getElementById("metricInstability").textContent = instability().toFixed(3);
  document.getElementById("metricCoverage").textContent = `${coverage().toFixed(2)}%`;
  document.getElementById("metricFrames").textContent = world.frameCount;
}

function captureFrame(label = "Manual Capture") {
  const image = canvas.toDataURL("image/png");
  const grid = document.getElementById("galleryGrid");

  const card = document.createElement("article");
  card.className = "gallery-card";
  card.innerHTML = `
    <img src="${image}" alt="${label}" />
    <div class="meta">${label} · rects ${world.rects.length} · instability ${instability().toFixed(3)}</div>
  `;

  grid.prepend(card);
  const maxCards = 32;
  while (grid.children.length > maxCards) {
    grid.removeChild(grid.lastChild);
  }
}

function randomScene(seedCount = 12) {
  world.rects = [];
  let attempts = 0;
  while (world.rects.length < seedCount && attempts < 900) {
    attempts += 1;

    const width = 3 + Math.floor(Math.random() * 12);
    const height = 2 + Math.floor(Math.random() * 6);
    const x = 1 + Math.floor(Math.random() * (PLANE_WIDTH - width));
    const y = 6 + Math.floor(Math.random() * (PLANE_HEIGHT - height));

    createRect(x, y, width, height);
  }
  render();
}

function parseIntInput(id) {
  return Number.parseInt(document.getElementById(id).value, 10);
}

document.getElementById("btnCreate").addEventListener("click", () => {
  const result = createRect(
    parseIntInput("createX"),
    parseIntInput("createY"),
    parseIntInput("createW"),
    parseIntInput("createH"),
  );

  if (result.ok) {
    setStatus("Rectangle created.");
  } else {
    setStatus(result.error, true);
  }
});

document.getElementById("btnRight").addEventListener("click", () => {
  const result = moveByPoint(
    parseIntInput("movePX"),
    parseIntInput("movePY"),
    parseIntInput("moveDelta"),
    1,
  );
  setStatus(result.ok ? "Moved right." : result.error, !result.ok);
});

document.getElementById("btnLeft").addEventListener("click", () => {
  const result = moveByPoint(
    parseIntInput("movePX"),
    parseIntInput("movePY"),
    parseIntInput("moveDelta"),
    -1,
  );
  setStatus(result.ok ? "Moved left." : result.error, !result.ok);
});

document.getElementById("btnGravity").addEventListener("click", () => {
  applyGravity();
  render();
  setStatus("Gravity simulation applied.");
});

document.getElementById("btnRandom").addEventListener("click", () => {
  randomScene(6 + Math.floor(Math.random() * 10));
  setStatus("New artistic scene generated.");
});

document.getElementById("btnCapture").addEventListener("click", () => {
  captureFrame("Captured Render");
  setStatus("Frame captured to gallery.");
});

document.getElementById("btnClear").addEventListener("click", () => {
  world.rects = [];
  render();
  setStatus("World cleared.");
});

function bootstrapGallery() {
  for (let i = 0; i < 6; i += 1) {
    randomScene(7 + i);
    captureFrame(`Curated Frame ${i + 1}`);
  }
  randomScene(10);
  setStatus("Atelier ready.");
}

render();
bootstrapGallery();
