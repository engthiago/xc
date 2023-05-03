# Static integrators

Static integrators are incremental integrators provided to implement the
common methods among integrator classes used in performing a static
analysis on the FE\_Model.

Determing the next time step for an analysis can be done by one of the following schemes:

- Load Control -- Specifies the incremental load factor to be applied to the loads in the domain
- Displacement Control -- Specifies the incremental displacement at a specified DOF in the domain
- Minimum Unbalanced Displacement Norm -- Specifies the incremental load factor such that the residual displacement norm in minimized
- Arc Length -- Specifies the incremental arc-length of the load-displacement path


## References
- [Direct displacement control](https://www.eurocode.us/finite-element-method/direct-displacement-control.html)
- [Arc-Length Control](https://opensees.berkeley.edu/OpenSees/manuals/usermanual/684.htm)
- [Arc Length Parameters](https://portwooddigital.com/2023/05/02/arc-length-parameters/)
