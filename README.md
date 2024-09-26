# 3D Mesh Renderer with Ray Tracing in C++

This project is a **C++ implementation** of a 3D mesh renderer using **Ray Tracing** from scratch. It takes a 3D model (in the form of a mesh) and renders it into a 2D image, simulating realistic lighting and shadows.


## Project Overview

In this project, we create a ray-traced image from a 3D mesh. Ray tracing is a technique used in computer graphics to generate an image by tracing the path of light as pixels in an image plane and simulating the effects of its encounters with virtual objects.


## Installation

### Prerequisites

Make sure you have the following installed on your system:

- A C++ compiler (supports C++11 or above).
- CMake (optional, but recommended for building).

### Building the Project

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/3d-mesh-raytracing.git
   cd 3d-mesh-raytracing
   ```

2. Build using CMake:

    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build build```

### Excecution

Once built, you can run the program by executing:

    ```bash
    ./MyRayTracer```