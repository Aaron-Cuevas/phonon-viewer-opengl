# Phonon Viewer (OpenGL)

Visualizador mínimo en C + OpenGL/GLFW para explorar la ocupación promedio de fonones (\bar n) de un modo vibracional (modelo tipo Einstein) y su variación con la temperatura y la frecuencia. Proporciona una ventana interactiva con controles de teclado y visualización simple.

> **Idea:** cada átomo vibra como un oscilador armónico cuántico con frecuencia (f). A una temperatura (T), la ocupación promedio por **modo** es
> [\bar n=\frac{1}{\exp(\tfrac{hf}{k_B T})-1}.]

---

## Física del modelo

* **Distribución Bose–Einstein (oscilador armónico cuántico):**
  [\bar n( f, T ) = \frac{1}{\exp!\big(\frac{h f}{k_B T}\big)-1},]
  con (h) la constante de Planck y (k_B) la constante de Boltzmann.
* **Nota importante:** si solo se pide la ocupación **por modo**, el número total de átomos (N) no interviene. Si se desea el **número total de fonones**, entonces se multiplica por el número de modos (p. ej., (3N) en un sólido monoatómico tipo Einstein).

---

## Ejemplo trabajado (para el README)

**Enunciado.** Una muestra de un sólido monoatómico contiene (N=10) mol de átomos. Cada átomo vibra como un oscilador armónico cuántico con frecuencia (f=6\times 10^{12},\text{Hz}). A temperatura (T=483,\text{K}), hallar el número promedio de fonones **por modo** vibracional.

**Solución.** Usamos (\bar n = 1/(e^{hf/k_BT}-1)):

* (h=6.62607015\times 10^{-34},\text{J·s}), (k_B=1.380649\times10^{-23},\text{J/K})
* Cociente adimensional (x = hf/(k_B T)):
  [
  x \approx \frac{6.626\times10^{-34}\cdot 6.0\times10^{12}}{1.38065\times10^{-23}\cdot 483} \approx 0.596.
  ]
* Entonces
  [
  \bar n = \frac{1}{e^{0.596}-1} \approx 1.23,\text{fonones por modo}.
  ]

**Respuesta:** (\boxed{\bar n\approx 1.23}) fonones por modo.
*(Opcional)* Número total de fonones si hay (3N) modos: (1.23\times 3\times 10,\text{mol}\times N_A \approx 2.22\times 10^{25}.)

---

## Requisitos

* **macOS:** Xcode Command Line Tools, Homebrew, GLFW.
* **Linux:** toolchain C (clang o gcc), GLFW y X11/Wayland dev packages.
* **Windows:** MSYS2 o vcpkg con GLFW.

---

## Compilación rápida

### macOS (Apple Silicon o Intel)

```bash
# Dependencia
brew install glfw

# Compilar (C11 + frameworks de macOS)
SDK=$(xcrun --show-sdk-path)
clang phonon_viewer.c -std=c11 -O2 \
  -isysroot "$SDK" -mmacosx-version-min=13.0 \
  -I"$(brew --prefix)/include" -L"$(brew --prefix)/lib" \
  -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo \
  -o phonon_viewer

# Ejecutar
./phonon_viewer
```

### Linux (ejemplo Debian/Ubuntu)

```bash
sudo apt update && sudo apt install -y build-essential clang pkg-config libglfw3-dev libx11-dev libxi-dev libxcursor-dev libxrandr-dev
clang phonon_viewer.c -std=c11 -O2 -lglfw -lGL -ldl -lm -o phonon_viewer
./phonon_viewer
```

### Windows (MSYS2)

```bash
# En MSYS2 MinGW64
pacman -S --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
clang -std=c11 -O2 phonon_viewer.c -lglfw3 -lopengl32 -lgdi32 -o phonon_viewer.exe
./phonon_viewer.exe
```

---

## Controles (por defecto en este proyecto)

* **←/→**: ajustar frecuencia visible
* **↑/↓**: ajustar temperatura (T)
* **W/S, D/A**: pasos más grandes
* **+ / -**: escala de frecuencia
* **Espacio**: pausar/continuar animación
* **R**: reiniciar parámetros
* **Esc**: salir

> Los nombres y mapeos exactos pueden variarse en `phonon_viewer.c`. Si modificas los atajos, actualiza esta sección.

---

## Makefile opcional

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

## Comprobación numérica rápida (opcional)

Pequeño fragmento para verificar (\bar n) desde la terminal usando Python:

```bash
python3 - << 'PY'
import math
h=6.62607015e-34
k=1.380649e-23
f=6.0e12
T=483.0
x=h*f/(k*T)
n=1.0/(math.exp(x)-1.0)
print(f"x = {x:.3f}\n n̄ = {n:.3f} fonones por modo")
PY
```

---

## Solución de problemas

* **`cd: no such file or directory`**: verifica el nombre de carpeta real. GitHub en ZIP usa sufijo `-main`.
* **`unknown sort specifier` en zsh**: copiaste comillas tipográficas. Reescribe comillas rectas `'` `"`.
* **`library not found for -lglfw`**: exporta rutas de Homebrew y recompila:

  ```bash
  export CPPFLAGS="-I$(brew --prefix)/include"
  export LDFLAGS="-L$(brew --prefix)/lib"
  ```
* **Pantalla en negro**: confirma que el contexto OpenGL se crea y que tu GPU soporta el perfil usado. Revisa logs en consola.

---

## Estructura del repositorio (sugerida)

```
.
├── phonon_viewer.c       # App principal (GLFW + OpenGL)
├── include/              # Headers
├── src/                  # Fuentes adicionales si crece el proyecto
├── assets/               # Shaders/texturas si aplica
├── Makefile              # Opcional
└── README.md             # Este archivo
```

---

## Referencias

* Shreiner, D. et al. *OpenGL Programming Guide*, 8th ed., Pearson.
* Kittel, C. *Introduction to Solid State Physics* (modelo de Einstein y fonones).

---

## Aviso de uso de IA

Este repositorio puede incluir secciones redactadas o revisadas con asistencia de IA, con fines de productividad y documentación. La autoría del código y las decisiones técnicas permanecen en los mantenedores del proyecto.

---

## Licencia

El código se distribuye bajo la licencia elegida por el autor del repositorio. Si no hay un archivo `LICENSE`, añade uno (MIT/BSD-3-Clause/Apache-2.0 son opciones comunes).
