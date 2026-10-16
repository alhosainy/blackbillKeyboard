<img src="assets/icon.svg" width="64" height="64" alt="blackbillKeyboard">

# blackbillKeyboard

A custom **hand-wired 104-key full-size ANSI** mechanical keyboard built as a
[QMK Firmware](https://github.com/qmk/qmk_firmware) keyboard definition.

- **Keyboard id / folder:** `blackbillkeyboard`
- **Display name:** `blackbillKeyboard`
- **MCU / dev board:** WeAct Black Pill (STM32F401, `blackpill_f401`)
- **Diode direction:** `COL2ROW`
- **Matrix:** 6 rows × 21 columns (104 keys, all in the diode matrix)
- **Lighting:** dual WS2812B underglow (`B0`, 16 LEDs = 2 × 8-LED strips)
- **Maintainer:** `alhosainy` — https://github.com/alhosainy/blackbillKeyboard

---

## Layout

Standard 104-key ANSI full-size, defined as `LAYOUT_fullsize_ansi` in `keyboard.json`.
Two layers:

| Layer | Name | How to reach |
|-------|------|--------------|
| 0 | `_BASE` | default |
| 1 | `_FN` | hold **Fn** (`MO(1)`) |

### Base layer (`_BASE`)

Plain ANSI. The bottom-right cluster, left to right, is:

`RAlt` · `Fn (MO(1))` · `Menu (KC_APP)` · `RCtrl`

### FN layer (`_FN`)

| Key(s) | Action |
|--------|--------|
| `Fn + Esc`, `Fn + Pause` | `QK_BOOT` — enter the bootloader |
| `Fn + F12` | `KC_KLOCK` — **keyboard lock** (all keys ignored; press `Fn + F12` again to unlock) |
| `Fn + Space` | `KC_MUTE` |
| Numpad `Num / * −` | RGB underglow toggle / next / prev / value |
| Numpad `7 8 9` | hue − / hue + / saturation − |
| Numpad `4 5 6` | saturation + / speed − / speed + |
| Numpad `+` | value + |
| Arrows `↑ ← ↓ →` | media Stop / Prev / Play / Next |

---

## Matrix wiring

Row pins: `A0 A1 A2 A3 A4 A5`
Column pins (index 0 → 20):

```
A6 A7 A8 A9 B4 A15 B1 C15 B5 B6 B7 B8 B9 B10 B3 B12 B13 B14 B15 C14 C13
```

`matrix_wiring_guide.html` is the **authoritative** wiring reference and must stay in
sync with `keyboard.json`.

### Deviations from the "ideal" KLE matrix (measured on the built board)

The physical wiring ended up different from the first KLE plan, so the matrix coordinates
in `keyboard.json` were re-mapped to match reality (verified with the diagnostic keymap,
see below). The layout-argument order in `keymap.c` never changed — only the matrix
coordinates did.

- **Row 4 (`ZXCV` row):** the letters are one column left of the original plan —
  `LShift`=`[4,0]`, `Z`=`[4,1]` … `/`=`[4,10]`, `RShift`=`[4,13]`, `Up`=`[4,15]`.
- **Bottom row cluster:** `RAlt`=`[5,9]`, `Fn`=`[5,10]`, `Menu`=`[5,12]`, `RCtrl`=`[5,13]`
  (column `[5,11]` is unwired).
- **Numpad:** `+`=`[3,20]`, `Enter`=`[5,20]`, `0`=`[5,17]` (the original `[2,20]`, `[4,20]`,
  `[5,18]` cells did not scan).

### Black Pill pin caveats

Per <https://docs.qmk.fm/platformdev_blackpill_f4x1>:

- `B11` is **not bonded** on the 48-pin package → column 14 uses `B3`.
- `A11` / `A12` are USB — unusable.
- `A13` / `A14` (SWD) are **not broken out** on this board → recovery/flashing is via
  `BOOT0` + DFU only.
- `B2` is `BOOT1` — avoid; it was moved off column 7 (now `C15`).
- `A9` (column 3) is the VBUS-sense pin with an internal pull-down; a **~5.1 kΩ pull-up to
  3V3** (5.6 kΩ confirmed working) is required for it to work as a column.
- `A10` was tried as column 4 but **did not scan reliably** (the vendor doc warns any
  connection there can block DFU entry), so column 4 moved to `B4` and `A10` is unused.
- `A4` is the SPI-flash CS (fine while the SOI8 footprint is unused).
- `C13`–`C15` are **input-only** — OK as columns (`C13`=col 20, `C14`=col 19, `C15`=col 7);
  `C13` also drives the onboard LED.

### History / changes from the original plan

- **Rotary encoder removed.** Its `B3` pin was needed to replace the nonexistent `B11`, so
  `B3` is now matrix column 14 and there is no encoder in `keyboard.json`.
- **`F13`–`F16` removed** — the pasted KLE layout carried four extra keys above the numpad
  that do nothing; the layout is now a plain 104-key ANSI. Matrix `[0,17]`–`[0,20]` are unwired.
- **Column pin remaps:** column 7 `B2`→`C15`, column 20 `?`→`C13`, column 4 `A10`→`B4`,
  column 14 `B11`→`B3`.
- **Measured-wiring remaps** listed above (row 4, bottom cluster, numpad).
- **FN layer additions:** `Space` = mute, `F12` = keyboard lock.

---

## Build & flash

The QMK toolchain lives under `D:\kayboard\QMK_MSYS`. It is not on `PATH` by default; run
these from the `F:\keyboard` checkout:

```powershell
$env:MSYSTEM='MINGW64'; $env:QMK_HOME='F:\keyboard'
$env:SHELL='D:\kayboard\QMK_MSYS\usr\bin\bash.exe'; $env:PYTHONUTF8='1'
$env:Path = 'D:\kayboard\QMK_MSYS\mingw64\bin;D:\kayboard\QMK_MSYS\usr\bin;D:\kayboard\QMK_MSYS\opt\qmk\bin;' + $env:Path
& 'D:\kayboard\QMK_MSYS\mingw64\bin\qmk.exe' compile -kb blackbillkeyboard -km default
& 'D:\kayboard\QMK_MSYS\mingw64\bin\qmk.exe' lint    -kb blackbillkeyboard -km default
& 'D:\kayboard\QMK_MSYS\mingw64\bin\qmk.exe' flash   -kb blackbillkeyboard -km default
```

`PYTHONUTF8=1` is required or `qmk lint` crashes on the box-drawing characters in
`keymap.c`. Build artifacts land in `.build/blackbillkeyboard_default.{bin,hex,elf}`.

**Entering the bootloader** (needed before `qmk flash`):

1. **Bootmagic:** hold the top-left key (`Esc`, matrix `[0,0]`) while plugging in USB.
2. The physical **reset button** on the Black Pill.
3. The `QK_BOOT` keycode in firmware: **`Fn + Pause`** or **`Fn + Esc`**.

The board then enumerates as ST DFU `0483:df11` and `qmk flash` writes it. Manual fallback:

```powershell
dfu-util -a 0 -d 0483:df11 -s 0x08000000:leave -D .build\blackbillkeyboard_default.bin
```

---

## The diagnostic keymap (`keymaps/diag`)

A second keymap used to discover the real wiring. Every key sends its **matrix coordinate**
(e.g. pressing `Z` types `R4C1`) instead of a normal keycode.

```powershell
# from F:\keyboard, with the env vars above already set:
& 'D:\kayboard\QMK_MSYS\mingw64\bin\qmk.exe' compile -kb blackbillkeyboard -km diag
& 'D:\kayboard\QMK_MSYS\mingw64\bin\qmk.exe' flash   -kb blackbillkeyboard -km diag
```

Open a text editor and press each physical key; read off the `RxCy` string. Use it whenever
a key types the wrong character or does nothing — it tells you exactly which matrix cell the
switch is soldered to, so you can fix `keyboard.json`.

Format for the result: row `R` is 0–5, column `C` is 0–20.

---

## Modifying the keymap: layers and new functions

All keymap logic lives in `keymaps/default/keymap.c`. There is also an (empty)
`keymaps/default/rules.mk` for enabling extra QMK features.

### How the layout macro maps to the matrix

`keymap.c` calls `LAYOUT_fullsize_ansi(...)` with one keycode per key. **The argument order
must exactly match the order of the entries in the `layouts.LAYOUT_fullsize_ansi.layout`
array in `keyboard.json`.** Each entry there pairs a physical position (`x`,`y`) with a
matrix coordinate (`[row, col]`). So:

- To change what a key does → change the keycode at the matching position in `keymap.c`.
- To move a key in the matrix (new wiring) → change its `matrix` value in `keyboard.json`;
  do **not** reorder the array or you will break the `keymap.c` argument order.
- Adding/removing physical keys does require reordering both together.

### Editing a key on the current layers

Find the position in the `_BASE` or `_FN` block and swap the keycode. Example — make
`Fn + 1` do nothing:

```c
[_FN] = LAYOUT_fullsize_ansi(
    //          Esc       F1
    QK_BOOT,    KC_TRNS,  KC_NO,   ...
```

`KC_TRNS` = fall through to the layer below; `KC_NO` = do nothing.

### Adding a whole new layer

1. Add it to the enum (order defines the layer number):

   ```c
   enum layers {
       _BASE,
       _FN,
       _NAV,   // new layer, index 2
   };
   ```

2. Add a block to the `keymaps` array with that designator:

   ```c
   [_NAV] = LAYOUT_fullsize_ansi(
       KC_TRNS, KC_TRNS, /* ... one keycode per position ... */
   ),
   ```

3. Add a key that reaches it, e.g. on `_FN`:
   - `MO(2)` — momentary (hold).
   - `TG(2)` — toggle.
   - `TO(2)` — switch to it.
   - `LT(2, KC_SPC)` — layer-tap (tap = space, hold = layer 2).

### Adding a custom keycode / function

The keyboard already does this for the lock key. Follow the same pattern:

```c
enum custom_keycodes {
    KC_KLOCK = SAFE_RANGE,   // existing
    KC_MYKEY,                // add new ones here
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_KLOCK:
            if (record->event.pressed) keyboard_locked = !keyboard_locked;
            return false;
        case KC_MYKEY:
            if (record->event.pressed) {
                SEND_STRING("hello");   // type text
                // or: tap_code16(LGUI(KC_L));  // send Win+L
            }
            return false;
    }
    return true;
}
```

Then place `KC_MYKEY` in any layer like a normal keycode. Common tools available in
`process_record_user`:

- `SEND_STRING("text")` / `SS_TAP(X_ENT)` — type text or individual keys.
- `tap_code(KC_X)` / `register_code(KC_X)` + `unregister_code(KC_X)` — send a key.
- `tap_code16(LGUI(KC_L))` — send a modified key (here Win+L).
- `layer_on(n)` / `layer_off(n)` / `layer_invert(n)` — manipulate layers.

### Enabling extra QMK features

Add flags to `keymaps/default/rules.mk`, e.g.:

```make
TAP_DANCE_ENABLE = yes
COMBO_ENABLE     = yes
CAPS_WORD_ENABLE = yes
```

(Win+L etc. need no extra flag; tap-dance and combos do.)

### Validate before flashing

Always run, in this order:

```powershell
& 'D:\kayboard\QMK_MSYS\mingw64\bin\qmk.exe' lint    -kb blackbillkeyboard -km default
& 'D:\kayboard\QMK_MSYS\mingw64\bin\qmk.exe' compile -kb blackbillkeyboard -km default
```

`lint` catches keymap/matrix mismatches and bad keycodes; `compile` builds
`.build/blackbillkeyboard_default.bin`. When matrix coordinates change, also update
`keyboard.json` and the two HTML guides (`matrix_wiring_guide.html`, `keyboard_guide.html`)
so they stay in sync.

---

## Switching to Vial (live web remapping)

[Vial](https://vial.rocks) lets you remap keys and RGB **live from the browser** over
WebHID — no recompiling, no install. Vial is a **fork of QMK**, not an upstream feature, so
this requires building from the fork.

> Current state: v1 is plain upstream QMK (`master`) with a static keymap. A `vial` git
> remote and a local `vial` branch have already been fetched, but no Vial files exist yet.

### 1. Get the Vial fork

```powershell
cd F:\keyboard
git remote add vial https://github.com/vial-kb/vial-qmk   # already added
git fetch vial
git checkout -b vial vial/vial
```

Submodules are required by the fork and the working tree uses `core.autocrlf=true`, which
makes submodules look dirty so a plain update aborts ("Aborting"). Force it:

```powershell
git submodule update --force --init --recursive
# or, per submodule:
git -C lib/chibios checkout -f <sha>
```

To go back to v1 later: `git checkout master` (and re-sync submodules the same way).

### 2. Add `vial.json` metadata

Create `keyboards/blackbillkeyboard/vial.json` mirroring `keyboard.json`: the
`matrix` size (6 × 21), the `layouts` (same `LAYOUT_fullsize_ansi` key order and KLE
coordinates), and a `keymap` with the layer count. Vial reads this to draw the board in
the web app. Keep the key order identical to `keyboard.json` or the positions will be wrong.

A minimal shape:

```json
{
  "name": "blackbillKeyboard",
  "vendorId": "0xFEED",
  "productId": "0x0000",
  "matrix": { "rows": 6, "cols": 21 },
  "layouts": {
    "keymap": [
      ["Esc", {"x": 2}, "F1", ...]
    ]
  }
}
```

Use the KLE layout from `keyboard.json` (each entry: `"key"` and `{"x":..,"y":..}` /
`{"w":..}`) — the web tool can convert an existing QMK `info.json`/`keyboard.json` layout.

### 3. Enable Vial in `rules.mk`

In `keyboards/blackbillkeyboard/rules.mk` (create it if missing):

```make
VIAL_ENABLE = yes
VIALRGB_ENABLE = yes   # only because the board uses rgblight underglow
```

### 4. Build and flash

```powershell
& 'D:\kayboard\QMK_MSYS\mingw64\bin\qmk.exe' compile -kb blackbillkeyboard -km default
& 'D:\kayboard\QMK_MSYS\mingw64\bin\qmk.exe' flash   -kb blackbillkeyboard -km default
```

Then open <https://vial.rocks> in Chrome/Edge and click **Authorize** — the keyboard
appears and can be remapped live. (Vial poses as a raw-HID device; the OS may warn about an
unknown device — that is expected.)

### Notes & trade-offs

- The `vial` branch is based on an **older QMK** than current `master`, so you give up newer
  upstream changes while on it. v1 (static `master` build) is unaffected — switch back with
  `git checkout master`.
- Vial has its own keycode set; a custom keycode like `KC_KLOCK` (defined in `keymap.c`)
  is not editable in the web app unless you expose it, but it keeps working in the compiled
  base keymap.
- Keep `keyboard.json`, `vial.json`, the `keymap.c` argument order, and both HTML guides in
  sync whenever the matrix or layout changes.
