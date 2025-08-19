Mini Treatment Planning System (TPS) – Educational Project

This project is a simplified Treatment Planning System (TPS) implemented in C++/Qt.
It was developed as part of a medical physics course to demonstrate the core principles of radiotherapy treatment planning, from patient volume visualization to simplified dose calculation.

--------------
Features:

3D patient volume (Bloc3D)
Loads a voxelized patient (e.g., density maps from ASCII or medical image files).
Stores tissue density / dose values voxel by voxel.
Interactive user interface (Qt/Widgets)
Three synchronized views: axial, coronal, sagittal.
Sliders to navigate through slices in each orientation.
Color maps (grayscale, pseudocolor).
Interactive visualization
Mouse tracking with ClickableLabel: real-time coordinates on slices.
Synchronization between different slice views.
Simplified dose calculation
Basic computation of TERMA / KERMA maps inside the volume.
Visualization of deposited energy distribution.
Beam parameters (field width, initial flux, angle) configurable in the UI.

--------------
Technical details :

Language: C++17
Framework: Qt 6 (Widgets)
Build system: CMake
Main components:
Bloc3D → voxelized patient model
FenetreBase → main Qt window (UI, sliders, image display)
ClickableLabel → extended QLabel with mouse tracking

--------------
Build instructions:

# Configure & build
mkdir build && cd build
cmake ..
cmake --build . --config Release

# Run the app
./MiniTPS

--------------
Educational purpose :

Disclaimer: This software is for educational purposes only.
It does not provide clinical-grade dose calculation and must never be used for patient treatment.
The goal is to give students an introduction to:
Volume representation in radiotherapy
Basic radiotherapy imaging navigation (axial / sagittal / coronal)
Simplified dose calculation workflow (TERMA / KERMA)
