# Dynamic Island — Product Specification

## Product vision

Island is a minimal Windows 11 glance-and-controls overlay. It should feel calm, polished, fast, and useful to anyone without pretending to have data that Windows or another application has not provided.

## Target platform

- Windows 11 desktop PCs
- x64 and ARM64 builds
- Native Win32 application with no third-party plugin runtime
- Portable `.exe` first; installer can follow later
- Per-monitor DPI-aware and suitable for multi-monitor setups

## Core behavior

### Minimized pill

- A compact, top-centered capsule
- Idle presentation is user-selectable:
  - clock, battery, and status dot
  - clock only
  - minimal status indicator
  - hidden
- The clock is centered and visually dominant
- Battery percentage and status remain readable at a glance
- Active media, timers, or other activities replace idle content
- The pill remains click-through outside its interactive bounds

### Expansion

The pill expands through:

- click
- hover
- `Ctrl + Alt + Space`

The expanded surface uses a slim horizontal navigation bar across the top. It must expand and collapse repeatedly without exiting, stealing focus during ordinary use, or crashing.

## Pages in v1

1. Home
2. Media
3. Today
4. Controls
5. Clipboard
6. Focus
7. Settings

## Real built-in modules

All modules must use real Windows data where supported:

- Clock: system time
- Battery: battery percentage and AC/charging state
- Media: metadata and playback controls through Windows Global System Media Transport Controls
- Volume: Windows master endpoint volume through Core Audio
- Clipboard: memory-only previews, enabled by default, with visible disable and clear actions
- Focus: start, pause, reset, and display a real countdown timer
- Today: only data supplied by supported local Windows integrations; no invented events

When a feature is unavailable, show it disabled with a concise explanation. Never display fake tracks, meetings, progress, device values, or simulated activity.

## Visual system

The default visual choice is user-selectable between:

1. Follow Windows light/dark mode
2. Solid dark mode

The interface should remain sleek and minimal in both modes:

- restrained graphite/neutral surfaces
- teal accent for active state and focus
- clear typography and generous spacing
- minimal borders and no unnecessary cards
- subtle, purposeful animation only

## Customization

Use simple presets rather than unrestricted visual editing. Settings should cover:

- idle presentation
- enabled modules
- visual preset: Windows mode or solid dark
- screen position and monitor
- startup behavior
- fullscreen behavior
- clipboard monitoring
- keyboard shortcut

## Fullscreen behavior

Hide automatically while fullscreen applications are active. The behavior should be reliable for games and productivity apps and avoid interrupting fullscreen content.

## Reliability and accessibility

- Keyboard-accessible navigation and controls
- Clear unavailable/inactive states
- No ordinary-use focus stealing
- Correct scaling under per-monitor DPI
- Graceful degradation when an API or hardware capability is unavailable
- Single-instance behavior
- Low idle CPU and memory usage

## Implementation direction

- Preserve the native Win32 rendering approach
- Organize features as independent built-in modules with availability/status reporting
- Use a Windows build toolchain capable of the required media APIs
- Keep the portable build reproducible for x64 and ARM64
- Do not add a third-party plugin system in v1

## Acceptance criteria

- Launches into the selected idle presentation
- Click, hover, and hotkey expansion all work reliably
- Expanded navigation opens and collapses repeatedly without crashes
- Real media metadata and controls work when a supported session exists
- Volume changes affect Windows master volume and reflect external changes
- Focus timer supports start, pause, reset, and active-pill display
- Clipboard monitoring obeys its toggle, remains session-only, and clears on request
- Theme preset, startup, fullscreen, multi-monitor placement, and DPI scaling work
- README claims are measured against the redesigned build
- Startup time, idle/expanded memory, and idle CPU are measured and documented

## Current milestone

Milestone 0 is the minimized pill only. The next implementation milestone should add the unified expanded shell, horizontal navigation, and real module contracts before wiring each Windows integration.
