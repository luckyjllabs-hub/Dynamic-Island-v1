# Island for Windows

Native Windows 11 overlay based on `design.md`. The current x64 preview builds to `bin\Island.exe`.

## Current preview

- Expand or collapse with `Ctrl + Alt + Space`; hover or click the pill to expand.
- Seven pages: Home, Media, Today, Controls, Clipboard, Focus, Settings.
- Real system clock and battery state, Windows master volume and mute, focus timer, and session-only clipboard text previews.
- Media page sends Windows playback keys. It shows an inactive state because media session metadata is not integrated yet.
- System shortcuts open the relevant Windows Settings page. Calendar remains unconnected.
- Settings persist alongside the portable executable in `bin\Island.ini`. Clipboard contents are never written there.
- Fullscreen windows hide the overlay.

## Build

Install an x64 MinGW-w64 C++20 compiler and run `build.bat`. The current build is x64 only.

## Remaining work

This preview does not yet meet the full `project.md` acceptance criteria. It still needs Windows media session metadata and direct transport controls, ARM64 packaging, per-monitor DPI scaling, complete keyboard focus navigation, animation and layout refinement, optional module controls, and on-device measurements. The image mockup contains illustrative values; the app never displays those as live data.

