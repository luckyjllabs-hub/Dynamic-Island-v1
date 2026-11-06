# Island for Windows: Design Spec

Visual and interaction spec for the Windows 11 island. It consolidates every mockup on the canvas and pairs with `PROJECT_BRIEF.md` (stack, performance budgets, milestones).

**Reference mockups:** https://claude.ai/artifact/JePY2fVj3fWQ1yufvu9LSy

**Chosen direction:** Apple Dynamic Island-style. One pure-black container holds lighter inner tiles. Layout reference is "Home option B" (`Apple-Home-Unified-B`), and the six other pages (`Apple-B-*`) follow it.

---

## 1. Principles

1. **One surface.** The expanded island is a single black container. Everything inside is a tile or control within it, never a separate floating card.
2. **Black is cheap.** Pure black needs no blur, so GPU and RAM stay minimal. No acrylic in v1.
3. **Color means state.** Green = battery, connected, monitoring. Red = recording, end, destructive. Orange = timer pause. Accent = progress and primary emphasis only.
4. **Soft geometry.** Large corner radii, fully round pills and buttons.
5. **Quiet when idle.** Collapsed pill shows at most one live element. Nothing animates unless something changed.
6. **Same layout rhythm on every page.** Header, hero tile, side tiles, pill row.

---

## 2. States

| State | Description |
|---|---|
| **Collapsed** | Small pill, idle (clock, battery, live dot) or showing one live activity |
| **Live activity** | Pill (or small card) for an event: call, recording, download, transfer, connection |
| **Expanded** | Single container with 7 pages, switched by swipe, scroll, or page dots |

### 2.1 Collapsed pills (mockup sizes at 100% scale)

| Pill | Size | Contents |
|---|---|---|
| Idle | 220×38, r19 | Accent dot, clock, battery % |
| Media | 300×44, r22 | 30px art (r10), title, artist, 4-bar waveform in accent |
| Timer | 260×44, r22 | 28px progress ring, "Focus", time left |
| Notch variant | 240×38, flat top, bottom r22 | Merges with the top screen edge |
| Ring only | 44px circle | Timer progress ring |
| Compact media | 130×36, r18 | Round art, waveform |

**Setting:** Idle appearance = Clock / Battery / Dot. Notch vs floating shape is a pending decision.

### 2.2 Live activities

| Activity | Card | Key elements |
|---|---|---|
| Screen recording | 84px tall | Red dot + time (red), title, red rounded-square stop button (44px, r14) |
| Call (in progress) | 172px tall | Avatar 44, name, "Voice call · time", five 48px round buttons: audio, mute (white = active), camera, share, end (red) |
| Incoming call | Pill 560×76, r38 | Avatar 52, "Incoming call" label, name 20px, 56px decline (red) and accept (green) |
| Download | 84px tall | Title, speed, 5px progress bar, filename and size |
| Nearby share | 84px tall | 40px ring, title, subtitle, accent stop button |
| Bluetooth connected | 84px tall | Green ring, green caps "CONNECTED", device name, battery % |
| Small pills | 273×52, r26 | Charging, headphones connected, recording with stop, USB drive with Eject chip |

Live activities appear as cards above the current page or replace the collapsed pill content. Priority and stacking rules are open (§9).

---

## 3. Design tokens

### 3.1 Color

| Token | Value | Use |
|---|---|---|
| `surface.container` | `#000000` | Island container, collapsed pills |
| `surface.tile` | `#1c1c1e` | Inner tiles |
| `surface.control` | `#2c2c2e` | Round buttons, pill buttons, segmented track, slider track |
| `surface.track` | `#3a3a3c` | Progress bars, ring tracks |
| `surface.selected` | `#636366` | Selected segment |
| `surface.switchOff` | `#39393d` | Switch off |
| `dot.inactive` | `#48484a` | Inactive page dots |
| `text.primary` | `#ffffff` | Main text |
| `text.secondary` | `#8e8e93` | Labels, subtitles (≥ 4.5:1 on black and `#1c1c1e`) |
| `state.green` | `#30d158` | Battery, connected, monitoring, accept, switch on |
| `state.red` | `#ff453a` | Recording, end call, clear all |
| `state.orange` | `#ff9f0a` | Pause |
| `accent` | Windows accent (default `#0a84ff`) | Focus ring, accent-tinted pills, active emphasis |

**Tinted pills** (for secondary actions such as Copy, Change, Connect):

| Style | Background | Text |
|---|---|---|
| Accent | accent @ 20% | accent, lightened for contrast (mockup: `#409cff`) |
| Destructive | `rgba(255,69,58,.20)` | `#ff6961` |
| Success | `rgba(48,209,88,.20)` | `#30d158` |
| Pause | `rgba(255,159,10,.25)` | `#ff9f0a` |

**Accent rule:** read the Windows accent once at startup and on its change event. Derive tinted pills from it. State colors (green, red, orange) stay fixed.

### 3.2 Typography

| Role | Mockup | Production |
|---|---|---|
| Family | Inter | **Segoe UI Variable** (system font, nothing bundled) |
| Letter spacing | −0.011em; −0.03 to −0.04em on large numerals | same |

| Style | Size / weight |
|---|---|
| Hero clock | 60 / 300 |
| Large readout (volume %) | 48 / 600 |
| Date, hero | 36 / 600 |
| Ring time (Focus) | 30 / 600 |
| Tile value | 20-26 / 600 |
| Body / row text | 15-16 / 500 |
| Page title | 16 / 600 |
| Label, secondary | 13 / 400, `text.secondary` |
| Pill button | 13 / 500-600 |

Use tabular numerals for all timers and percentages.

### 3.3 Radius

| Element | Radius |
|---|---|
| Expanded container | 46 |
| Tile | 30 |
| Pill button (52px high) | 26 (full) |
| Small pill button (40px high) | 20 (full) |
| Round button | 50% |
| Album art | 16-18 |
| Slider (56px high) | 28 (full) |
| Progress bar | 3 |

### 3.4 Spacing and sizes (at 100% scale; scale with DPI)

| Item | Value |
|---|---|
| Container width | 576 |
| Container padding | 20 |
| Gap between blocks | 14 |
| Pill row gap | 10 |
| Header height | 36 (horizontal padding 6) |
| Hero tile height | 180 |
| Side tile height | 83 each (two stacked, 14 gap) |
| Pill button height | 52 |
| Small pill button height | 40 |
| Round button | 52 (call controls 48) |
| Switch | 51×31, knob 27 |
| Segmented control | track padding 2, buttons 36-40 high |
| Page dots | active 14×5, inactive 5×5, gap 4 |
| Progress bar | 6 |
| Ring sizes | 40 / 56 / 68 / 140 |
| Shadow | `0 30px 60px rgba(0,0,0,.45)` |

---

## 4. Components

| Component | Spec |
|---|---|
| **Container** | `surface.container`, r46, padding 20, vertical stack with 14 gap |
| **Header** | Page name left (16/600), 7 page dots right, active dot elongated and white |
| **Tile** | `surface.tile`, r30, padding 14×18 (hero 18×22) |
| **Hero + side stack** | Two columns, `1.25fr / 1fr`, 14 gap. Left hero 180 high. Right: two 83px tiles. |
| **Pill row** | Equal-width pills, 52 high, 10 gap. Primary action is white fill with black text. |
| **Round button** | `surface.control`, icon 18-24 stroke 2-2.2. Active state: white fill, black icon. |
| **Slider** | 56 high, r28, `surface.control` track, white fill, speaker icon inside the fill, value on the track |
| **Progress bar** | 6 high, `surface.track`, white fill (media, volume) or accent (focus) |
| **Ring** | Circular progress, `surface.track` ring, accent or green arc, round caps |
| **Switch** | 51×31, green when on, `surface.switchOff` when off. `role="switch"` with `aria-checked`. |
| **Segmented control** | `surface.control` track, selected `surface.selected` |
| **Status label** | Small caps or 11-13px semibold in a state color, such as "CONNECTED" or "On battery" |

Icons: Segoe Fluent Icons in production. The mockups use stroke icons with round joins at 2px.

---

## 5. Pages

Seven pages in this order. Every page has the header, and every page except Clipboard and Settings uses the hero + side stack + pill row.

| # | Page | Hero tile | Side tiles | Pill row / actions |
|---|---|---|---|---|
| 1 | **Home** | 9:41 clock (60/300), date, battery ● 82% | Focus ring + time, volume bar | Focus (white), Sound, Clipboard, Settings |
| 2 | **Media** | 72px art, title, artist, progress with times | Output device, volume | Prev, Pause (white, wider), Next |
| 3 | **Today** | Date (36/600), week, battery with time left | Power mode ("On battery" green, "Balanced"), calendar status | Connect calendar (white), Power settings |
| 4 | **Controls** | 48px volume %, thick slider | Output + Change, Mute switch | 6 shortcuts in two rows of three: Sound, Display, Bluetooth, Network, Power, Notifications |
| 5 | **Clipboard** | none | Status row: "Monitoring on" (green) + Clear all (red tint). Then 3 item tiles (72 high): text, timestamp, Copy (latest in white, others accent tint). | none |
| 6 | **Focus** | 140px ring, time inside, "Deep work" | Session selector 15/25/50, "Ends at" time | Start (green tint), Pause (white), Reset |
| 7 | **Settings** | none | 2×2 tiles: idle appearance (Clock/Battery/Dot), accent swatches (Windows + 3 presets), clipboard monitoring switch, startup switch. Below: performance tile with memory, CPU, startup time. | none |

**State notes**
- **Focus:** Start and Pause swap by state. Mockup shows all three. Reset always available.
- **Clipboard:** Copy puts the item back on the clipboard. Clear all empties the list. Clipboard contents stay in memory only.
- **Today:** the calendar tile shows its connection state. Not connected shows a Connect action. Connected shows the next event.
- **Settings:** the performance tile shows live values (memory, CPU idle, startup time) for debugging against the budgets.

---

## 6. Motion

Starting values. Tune on device.

| Interaction | Behavior |
|---|---|
| **Expand** | Pill morphs into the container as one shape (width, height, corner radius together) using a spring: response ≈ 0.4-0.5 s, damping ratio ≈ 0.75-0.8 (slight overshoot) |
| **Content in** | Tiles fade and rise 8-12px when the shape is about 85% open, 30 ms stagger, 120-180 ms each |
| **Collapse** | Tiles fade out in about 100 ms first, then the shape springs back |
| **Page switch** | Horizontal slide about 220 ms. Container height springs to the new page height. Width never changes. |
| **Live activity in** | Pill scales from about 0.9 to 1 with a small overshoot |
| **Slider / toggle** | Direct manipulation, no easing lag. Switch knob springs. |
| **Reduced motion** | Replace springs with short cross-fades |

**Rendering rules** (details in `PROJECT_BRIEF.md`)
- Animate the container as a composition visual in the compositor. It follows the monitor's refresh rate automatically, including 144 Hz.
- No render loop when idle.
- Build tile content lazily on first expand, and release art bitmaps on collapse.

---

## 7. Windows adaptation

- **Font:** Segoe UI Variable, not Inter.
- **Accent:** follow the Windows accent (§3.1).
- **DPI:** per-monitor DPI v2. All sizes above are at 100% scale.
- **Position:** user-chosen anchor and offset, with a monitor selector. The container grows from the pill's anchor point.
- **Focus:** the collapsed pill never takes focus and is click-through outside its shape.
- **Fullscreen:** auto-hide when a fullscreen app or game is running.
- **Wallpaper:** the blue gradient in the mockups is only a backdrop. The real island sits on whatever is on screen, so the black container must hold its own edge (shadow, no transparency).

---

## 8. Accessibility

- Text contrast ≥ 4.5:1 (secondary gray `#8e8e93` meets this on both surfaces).
- Controls at least 36px high, with the main targets at 40-52px.
- Switches use `role="switch"`. Icon-only buttons have accessible names.
- Full keyboard navigation: arrow keys between pages, Tab through controls, Esc to collapse.
- State is never color-only: pair color with a label or icon (e.g. "CONNECTED", "Monitoring on").
- Respect the Windows "reduce animations" setting.

---

## 9. Open decisions

| Item | Status |
|---|---|
| Collapsed shape: floating pill vs notch | Pending |
| Look from the two Instagram reels | Pending (not viewable by Claude; need screenshots or description) |
| Live-activity priority and stacking when several are active | Pending |
| Which live activities ship first (call, download, and recording need notification or capture APIs) | Pending, after M1-M3 |
| Default accent when Windows accent is unreadable on black | Pending (lighten automatically) |
| Calendar source (connected account vs local) | Pending |

---

## 10. Canvas index

| Board group | Boards |
|---|---|
| **Final direction (use these)** | `Apple-Home-Unified-B` (Home), `Apple-B-Media`, `Apple-B-Today`, `Apple-B-Controls`, `Apple-B-Clipboard`, `Apple-B-Focus`, `Apple-B-Settings` |
| Live activities and pills | `Live-Activities`, `Call-Pill`, `Alt-Main`, `Main` |
| Earlier options (reference only) | `Apple-Home-Unified` (option A), `Apple-*`, `Ref-*`, `Alt-*`, and the first set (`Home`, `Media`, `Today`, `Controls`, `Clipboard`, `Focus`, `Settings`) |
