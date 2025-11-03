# Phonon Viewer (OpenGL, C)
Este programa es auxiliar en el entendimiento del siguiente problema en interferometría:

Dos ondas de luz de igual frecuencia y en fase viajan paralelos en el aire. Uno de los rayos atraviesa perpendicularmente una placa plano-paralela de vidrio con índice de refracción n1 = 1.49, mientras que el otro rayo atraviesa otra placa de igual espesor pero con índice n2 = 2.46. Si el espesor de las placas es d = 39  y la longitud de onda en el aire es  544 nm, ¿con qué diferencia de fase en radianes emergerán las ondas de las placas?

El programa consta de los siguientes elementos:

Arriba: oscilación tipo **estado coherente** con frecuencia visible ajustable.  
Abajo: barra de **ocupación media** \(\bar n = 1 / (\exp(h f / k_B T) - 1)\).

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
