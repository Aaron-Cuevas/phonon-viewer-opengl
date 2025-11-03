# Phonon Viewer (OpenGL, C)

Arriba: oscilación tipo **estado coherente** con frecuencia visible ajustable.  
Abajo: barra de **ocupación media** \(\bar n = 1 / (\exp(h f / k_B T) - 1)\).

**IA-Disclaimer:** ensamblado con ayuda de IA como referencia para futuros programas no asistidos.  
**Referencia de estudio:** Shreiner, D. et al. (2013) *OpenGL Programming Guide* (8th), Pearson.

## macOS
```bash
brew install glfw
SDK=$(xcrun --show-sdk-path)
clang phonon_viewer.c -std=c11 -O2 \
  -isysroot "$SDK" -mmacosx-version-min=13.0 \
  -I"$(brew --prefix)/include" -L"$(brew --prefix)/lib" \
  -lglfw \
  -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo \
  -o phonon_viewer
./phonon_viewer
Linux (Debian/Ubuntu)
sudo apt update
sudo apt install -y build-essential libglfw3-dev mesa-common-dev libx11-dev
gcc phonon_viewer.c -std=c11 -O2 -lglfw -lGL -ldl -lpthread -o phonon_viewer
./phonon_viewer
Controles
	•	↑/↓: T; ←/→: f; W/S y D/A: pasos grandes; +/-: frecuencia visible
	•	Espacio: pausa; R: reset
