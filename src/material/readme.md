# C++ classes for definition of materials

This directory contains the source code that represents constitutive equations of the finite element model. There are three families of material models:

## Contents
  - nD: n-dimensional (two and three dimensional) relationships between stresses (2D or 3D stress tensor) and strains (2D or 3D strain tensor).
  - Section materials: representation of the relationship between cross section generalized stresses (internal forces: N,My,Mz,T,Vy,Vz) and the corresponding generalized strains (elongations and curvatures).
  - Uniaxial materials: representation of a uniaxial generalized stress-strain relationship.

It contains also the damage models.
