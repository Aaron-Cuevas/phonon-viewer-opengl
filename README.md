# Phonon Viewer (OpenGL)

Minimal C + OpenGL/GLFW viewer to explore the average phonon occupancy of a single vibrational mode (Einstein-like model) and how it changes with temperature and frequency. The app opens an interactive window with simple keyboard controls.

> **Core idea, explained without formulas:**
> In the Einstein picture each atom behaves like a quantum harmonic oscillator with a fixed frequency, which we call *f*. At a given temperature *T*, the average number of phonons in one vibrational mode follows Bose–Einstein statistics. To compute it in words: take the ratio **r** equal to “Planck’s constant *h* times the mode frequency *f*” divided by “Boltzmann’s constant *k* times the temperature *T*.” Compute the exponential of **r**, subtract 1, then take the reciprocal. The result is the mean phonon count per mode.

---

## Model, briefly (no math notation)

* Each vibrational mode behaves like a quantized oscillator.
* The mean phonon count per mode depends only on the dimensionless ratio described above. Larger frequency or lower temperature makes the ratio larger, which lowers the mean occupancy.
* If you only need the **per‑mode** average, the total number of atoms does not appear. If you want the **total phonon count** in the sample, multiply the per‑mode average by the total number of modes (for a monatomic solid in the Einstein picture this is roughly three times the number of atoms).

---

## Worked example (for this README)

**Problem.** A monatomic solid contains 10 mol of atoms. Each atom vibrates like a quantum harmonic oscillator with frequency 6.0×10^12 Hz. At temperature 483 K, find the **average phonon count per vibrational mode**.

**Solution (described).**

1. Form the ratio **r** = (h·f)/(k·T). Using the CODATA values for *h* and *k*, with *f* = 6.0×10^12 Hz and *T* = 483 K, the ratio is about **0.596**.
2. Exponentiate this ratio to get *e*^r ≈ 1.815, subtract 1 to get ≈ 0.815, then take the reciprocal. The result is about **1.23 phonons per mode**.

**Answer:** approximately **1.23** phonons per mode.
*(Optional)* If you want an order‑of‑magnitude of the **total** phonons in the solid, multiply by the number of modes (about three times Avogadro’s number times the number of moles) and by the per‑mode average computed above.

---

## Requirements

* **macOS:** Xcode Command Line Tools, Homebrew, GLFW.
* **Linux:** C toolchain (clang or gcc), GLFW and X11/Wayland development packages.
* **Windows:** MSYS2 or vcpkg with GLFW.

---

## Quick build

### macOS (Apple Silicon or Intel)

```bash
# Dependency
brew install glfw

# Build (C11 + macOS frameworks)
SDK=$(xcrun --show-sdk-path)
clang phonon_viewer.c -std=c11 -O2 \
  -isysroot "$SDK" -mmacosx-version-min=13.0 \
  -I"$(brew --prefix)/include" -L"$(brew --prefix)/lib" \
  -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo \
  -o phonon_viewer

# Run
./phonon_viewer
```

### Linux (Debian/Ubuntu example)

```bash
sudo apt update && sudo apt install -y build-essential clang pkg-config libglfw3-dev libx11-dev libxi-dev libxcursor-dev libxrandr-dev
clang phonon_viewer.c -std=c11 -O2 -lglfw -lGL -ldl -lm -o phonon_viewer
./phonon_viewer
```

### Windows (MSYS2)

```bash
# In MSYS2 MinGW64
pacman -S --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
clang -std=c11 -O2 phonon_viewer.c -lglfw3 -lopengl32 -lgdi32 -o phonon_viewer.exe
./phonon_viewer.exe
```

---

## Controls (default in this project)

* **Left/Right**: adjust visible frequency
* **Up/Down**: adjust temperature
* **W/S, D/A**: larger steps
* **+ / -**: frequency scale
* **Space**: pause/resume animation
* **R**: reset parameters
* **Esc**: quit

> If you change the key mapping in `phonon_viewer.c`, please update this section.

---

## Optional Makefile

```makefile
SDK := $(shell xcrun --show-sdk-path)
BREW := $(shell brew --prefix)
CFLAGS := -std=c11 -O2 -isysroot "$(SDK)" -mmacosx-version-min=13.0 -I"$(BREW)/include"
LDFLAGS := -L"$(BREW)/lib" -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

phonon_viewer: phonon_viewer.c
	clang $(CFLAGS) $< -o $@ $(LDFLAGS)

run: phonon_viewer
	./phonon_viewer

clean:
	rm -f phonon_viewer
```

---

## Quick numeric check (optional)

A tiny Python snippet to reproduce the example above without any equations:

```bash
python3 - << 'PY'
import math
h=6.62607015e-34
k=1.380649e-23
f=6.0e12
T=483.0
r = h*f/(k*T)
val = 1.0/(math.exp(r)-1.0)
print(f"ratio r = {r:.3f}")
print(f"mean phonons per mode = {val:.3f}")
PY
```

---

## Troubleshooting

* **`cd: no such file or directory`**: check the real folder name. GitHub ZIPs often append `-main` to the repository name.
* **`unknown sort specifier` in zsh**: you pasted curly quotes from a document. Re‑type straight quotes `'` or `"`.
* **`library not found for -lglfw`**: export Homebrew paths and rebuild:

  ```bash
  export CPPFLAGS="-I$(brew --prefix)/include"
  export LDFLAGS="-L$(brew --prefix)/lib"
  ```
* **Black window**: confirm the OpenGL context is created and your GPU supports the requested profile; check the console logs.

---

## Repository layout (suggested)

```
.
├── phonon_viewer.c       # Main app (GLFW + OpenGL)
├── include/              # Headers
├── src/                  # Additional sources if the project grows
├── assets/               # Shaders/textures if applicable
├── Makefile              # Optional
└── README.md             # This file
```

---

## References

* Shreiner, D. et al. *OpenGL Programming Guide*, 8th ed., Pearson.
* Kittel, C. *Introduction to Solid State Physics* (Einstein model and phonons).

---

## AI assistance notice

Some sections of this repository may have been drafted or edited with AI assistance for productivity and documentation purposes. Code authorship and technical decisions remain with the project maintainers.

---

## License

Use the license you prefer for this project. If the repository lacks a `LICENSE` file, consider adding one (MIT, BSD‑3‑Clause, or Apache‑2.0 are common options).
