#!/bin/sh
# 
#   Copyright (C)  Luis C. Pérez Tato
# 
#   XC utils is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or 
#   (at your option) any later version.
# 
#   This software is distributed in the hope that it will be useful, but 
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.  
# 
#  You should have received a copy of the GNU General Public License 
#  along with this program.
#  If not, see <http://www.gnu.org/licenses/>.
# ----------------------------------------------------------------------------

ERT="\\033[1;32m"
NORMAL="\\033[0;39m"
ROUGE="\\033[1;31m"
ROSE="\\033[1;35m"
BLEU="\\033[1;34m"
BLANC="\\033[0;02m"
BLANCLAIR="\\033[1;08m"
JAUNE="\\033[1;33m"
CYAN="\\033[1;36m"

echo ""

mplbackend_backup="nil"
# Trying to avoid Matplotlib complaining about the XServer
if [ -n "$MPLBACKEND" ]; then
    echo "$JAUNE" "MPLBACKEND already set as: $MPLBACKEND" "$NORMAL"
    mplbackend_backup="$MPLBACKEND"
else
    echo "$BLEU" "Setting MPLBACKEND to avoid Matplotlib complaints." "$NORMAL"
    MPLBACKEND=Agg
    export MPLBACKEND
fi

START=$(date +%s.%N)

# Misc. tests
echo "$BLEU" "Misc. tests." "$NORMAL"
python tests/utility/test_evalPy.py
python tests/utility/test_execPy.py
python tests/utility/test_copy_properties.py
python tests/utility/miscUtils/testStairCaseFunction.py
python tests/utility/miscUtils/test_linear_interpolation.py
python tests/utility/miscUtils/test_remove_accents.py
echo "$BLEU" "  Geometry tests." "$NORMAL"
echo "$BLEU" "    Vectors." "$NORMAL"
python tests/utility/geom/vector2d_test_01.py
python tests/utility/geom/vector2d_test_02.py
python tests/utility/geom/vector3d_test_01.py
python tests/utility/geom/vector3d_test_02.py
python tests/utility/geom/vector3d_angle_test_01.py
echo "$BLEU" "    Points." "$NORMAL"
python tests/utility/geom/pos2d_test_01.py
python tests/utility/geom/pos3d_test_01.py
python tests/utility/geom/pos3d_list_test_01.py
python tests/utility/geom/closest_pair_of_points.py
echo "$BLEU" "    Lines, rays and segments." "$NORMAL"
python tests/utility/geom/line2d_test_01.py
python tests/utility/geom/line3d_test_01.py
python tests/utility/geom/line3d_test_02.py
python tests/utility/geom/line2d_intersection_test.py
python tests/utility/geom/segment2d_test_01.py
python tests/utility/geom/segment2d_test_02.py
python tests/utility/geom/segment2d_connect_01.py
python tests/utility/geom/segment2d_connect_02.py
python tests/utility/geom/line_segment_intersection_test.py
python tests/utility/geom/segment3d_test_01.py
python tests/utility/geom/segment3d_connect_01.py
python tests/utility/geom/segment3d_connect_02.py
python tests/utility/geom/ray2d_test.py
python tests/utility/geom/pos2d_list_test_01.py
echo "$BLEU" "    Polylines." "$NORMAL"
python tests/utility/geom/polylines/polyline2d_test_01.py
python tests/utility/geom/polylines/polyline2d_test_02.py
python tests/utility/geom/polylines/polyline2d_test_03.py
python tests/utility/geom/polylines/polyline2d_test_04.py
python tests/utility/geom/polylines/polyline3d_test_01.py
python tests/utility/geom/polylines/polyline3d_test_02.py
python tests/utility/geom/polylines/polyline3d_test_03.py
python tests/utility/geom/polylines/polyline3d_test_04.py
echo "$BLEU" "    Polygons." "$NORMAL"
python tests/utility/geom/polygons/polygon2D_test_2d_01.py
python tests/utility/geom/polygons/polygon2D_test_2d_02.py
python tests/utility/geom/polygons/polygon2D_test_2d_03.py
python tests/utility/geom/polygons/polygon2D_test_2d_04.py
python tests/utility/geom/polygons/polygon2D_test_2d_05.py
python tests/utility/geom/polygons/polygon2D_test_2d_06.py
python tests/utility/geom/polygons/polygon2D_test_2d_07.py
python tests/utility/geom/polygons/polygon2D_test_2d_08.py
python tests/utility/geom/polygons/polygon2D_test_2d_09.py
python tests/utility/geom/polygons/polygon2D_test_2d_10.py
python tests/utility/geom/polygons/polygon2D_test_2d_11.py
python tests/utility/geom/polygons/polygon2D_test_2d_12.py
python tests/utility/geom/polygons/polygon3D_test_01.py
python tests/utility/geom/polygons/polygon3D_test_02.py
python tests/utility/geom/polygons/polygon3D_test_03.py
python tests/utility/geom/polygons/polygon3D_test_04.py
python tests/utility/geom/polygons/polygon3D_test_05.py
python tests/utility/geom/polygons/polygon3D_test_06.py
python tests/utility/geom/polygons/polygon3D_test_07.py
echo "$BLEU" "    Alpha shapes." "$NORMAL"
python tests/utility/geom/basic_alpha_shape2d_test.py
echo "$BLEU" "    Circles." "$NORMAL"
python tests/utility/geom/circle2d_test_01.py
python tests/utility/geom/circle2d_test_02.py
echo "$BLEU" "    Parabolas." "$NORMAL"
python tests/utility/geom/parabola_test_01.py
echo "$BLEU" "    Sliding vectors." "$NORMAL"
python tests/utility/geom/sliding_vectors_systems/sliding_vector_2d_test_01.py
python tests/utility/geom/sliding_vectors_systems/sliding_vector_3d_test_01.py
python tests/utility/geom/sliding_vectors_systems/sliding_vector_3d_test_02.py
python tests/utility/geom/sliding_vectors_systems/sliding_vector_3d_test_03.py
python tests/utility/geom/sliding_vectors_systems/sliding_vectors_system_2d_test_01.py
python tests/utility/geom/sliding_vectors_systems/sliding_vectors_system_2d_test_02.py
python tests/utility/geom/sliding_vectors_systems/sliding_vectors_system_2d_test_03.py
python tests/utility/geom/sliding_vectors_systems/sliding_vectors_system_2d_test_04.py
python tests/utility/geom/sliding_vectors_systems/sliding_vectors_system_2d_test_05.py
python tests/utility/geom/sliding_vectors_systems/sliding_vectors_system_2d_test_06.py
python tests/utility/geom/sliding_vectors_systems/sliding_vectors_system_2d_test_07.py
python tests/utility/geom/sliding_vectors_systems/sliding_vectors_system_3d_test_01.py
python tests/utility/geom/sliding_vectors_systems/sliding_vectors_system_3d_test_02.py
python tests/utility/geom/sliding_vectors_systems/sliding_vectors_system_3d_test_03.py
python tests/utility/geom/sliding_vectors_systems/sliding_vectors_system_3d_test_04.py
python tests/utility/geom/sliding_vectors_systems/sliding_vectors_system_3d_test_05.py
python tests/utility/geom/sliding_vectors_systems/sliding_vectors_system_3d_test_06.py
echo "$BLEU" "    Planes." "$NORMAL"
python tests/utility/geom/plane_test_01.py
python tests/utility/geom/plane_test_02.py
python tests/utility/geom/plane_test_03.py
#python tests/utility/geom/plane_test_04.py
python tests/utility/geom/plane_test_05.py
# echo "$BLEU" "  3D half spaces." "$NORMAL"
# python tests/utility/geom/halfspace3d_test_01.py
# python tests/utility/geom/halfspace3d_test_02.py
# python tests/utility/geom/halfspace3d_test_03.py
# python tests/utility/geom/halfspace3d_test_04.py
echo "$BLEU" "    Reference systems." "$NORMAL"
python tests/utility/geom/coosys_test_01.py
python tests/utility/geom/refsys3d3d_test_01.py
python tests/utility/geom/refsys2d3d_test_01.py
echo "$BLEU" "    Transformations." "$NORMAL"
python tests/utility/geom/rotation2d_test_01.py
echo "$BLEU" "    Mechanical properties." "$NORMAL"
python tests/utility/geom/principal_axes_of_inertia_2d_test_01.py
echo "$BLEU" "    DXF reading." "$NORMAL"
python tests/utility/geom/ezdxf_test_01.py

## Soil mechanics (basic utilities)
echo "$BLEU" "  Soil mechanics (basic utilities)." "$NORMAL"
python tests/utility/soil_mechanics/test_mononobe_okabe.py
python tests/utility/soil_mechanics/test_coulomb.py
python tests/utility/soil_mechanics/test_janssen_limited_backfill_widht.py
python tests/utility/soil_mechanics/test_rankine.py
python tests/utility/soil_mechanics/test_brinch_hansen01.py
python tests/utility/soil_mechanics/test_brinch_hansen02.py
python tests/utility/soil_mechanics/test_stratified_soil.py
python tests/utility/soil_mechanics/test_boussinesq.py
python tests/utility/soil_mechanics/test_cohesive_soil_01.py

# Tests about actions.
echo "$BLEU" "Actions tests." "$NORMAL"
python tests/actions/test_prestressing.py
echo "$BLEU" "  Earth pressure tests." "$NORMAL"
python tests/actions/earth_pressure/test_peck_pressure_envelope.py
python tests/actions/earth_pressure/test_earth_pressure.py
python tests/actions/earth_pressure/test_earth_pressure_sloped_wall.py
python tests/actions/earth_pressure/culmann_method_test_01.py
python tests/actions/earth_pressure/culmann_method_test_02.py
python tests/actions/earth_pressure/culmann_method_test_03.py
python tests/actions/earth_pressure/test_embankment_weight_distr.py
python tests/actions/earth_pressure/test_earth_pressure_point_load.py
echo "$BLEU" "  Earthquake action tests." "$NORMAL"
python tests/actions/quake/mononobe_okabe_test_01.py
python tests/actions/quake/test_NCSP_01.py
echo "$BLEU" "  Snow action tests." "$NORMAL"
python tests/actions/snow/test_snowSIA.py
python tests/actions/snow/test_snowASCE7.py
echo "$BLEU" "  Wind action tests." "$NORMAL"
python tests/actions/wind/test_windSIA.py
python tests/actions/wind/test_windASCE7.py
python tests/actions/wind/test_windASCE7_16.py
python tests/actions/wind/test_wind_cylindr_01.py
python tests/actions/wind/test_wind_cylindr_02.py
python tests/actions/wind/test_wind_beam_truss.py
python tests/actions/wind/test_cte_wind_load.py
python tests/actions/wind/test_iap_wind_load_01.py
python tests/actions/wind/test_iap_wind_load_02.py
python tests/actions/wind/test_iap_wind_load_03.py
python tests/actions/wind/test_iap_hiding_ratio.py
echo "$BLEU" "  Thermal action tests." "$NORMAL"
python tests/actions/thermal/test_thermal_grad_shell_01.py 
python tests/actions/thermal/test_thermal_grad_shell_02.py 
python tests/actions/thermal/test_iap_thermal.py
echo "$BLEU" "  Traffic loads tests." "$NORMAL"
python tests/actions/traffic_loads/test_derailment_SIA.py
python tests/actions/traffic_loads/test_pedestrian_load_ec1.py

# Load combinations tests.
echo "$BLEU" "Forming load combination tests." "$NORMAL"
#python tests/actions/load_combinations/testLoadCombinations.py
python tests/actions/load_combinations/test_action.py
python tests/actions/load_combinations/test_secondaries_00.py
python tests/actions/load_combinations/test_accidentales.py
python tests/actions/load_combinations/test_iap11_01.py
python tests/actions/load_combinations/test_iap11_02.py
python tests/actions/load_combinations/test_combination_dict.py
python tests/actions/load_combinations/test_split_combination.py

echo "$BLEU" "Elements tests." "$NORMAL"
echo "$BLEU" "  Truss element tests." "$NORMAL"
python tests/elements/trusses/truss_test0.py
python tests/elements/trusses/truss_test1.py
python tests/elements/trusses/truss_test2.py
python tests/elements/trusses/truss_test3.py
echo "$BLEU" "  Coordinate transformations tests." "$NORMAL"
python tests/elements/crd_transf/test_linear_crd_transf_2d_01.py
python tests/elements/crd_transf/test_pdelta_crd_transf_2d_01.py
python tests/elements/crd_transf/test_corot_crd_transf_2d_01.py
python tests/elements/crd_transf/test_linear_crd_transf_3d_01.py
python tests/elements/crd_transf/test_linear_crd_transf_3d_02.py
python tests/elements/crd_transf/test_pdelta_crd_transf_3d_01.py
python tests/elements/crd_transf/test_corot_crd_transf_3d_01.py
python tests/elements/crd_transf/test_corot_crd_transf_3d_02.py
python tests/elements/crd_transf/test_element_axis_01.py
python tests/elements/crd_transf/test_element_axis_02.py
python tests/elements/crd_transf/test_element_axis_03.py
python tests/elements/crd_transf/test_element_point_01.py
python tests/elements/crd_transf/test_crd_transf2d_01.py
python tests/elements/crd_transf/test_crd_transf3d_01.py
echo "$BLEU" "  Beam column tests." "$NORMAL"
echo "$BLEU" "    Elastic beam-column 2D tests." "$NORMAL"
python tests/elements/beam_column/elastic_beam_2d/beam_test1.py
python tests/elements/beam_column/elastic_beam_2d/elastic_beam2d_sign_criteria_01.py
python tests/elements/beam_column/elastic_beam_2d/elastic_beam2d_sign_criteria_02.py
python tests/elements/beam_column/elastic_beam_2d/elastic_beam2d_sign_criteria_03.py
python tests/elements/beam_column/elastic_beam_2d/modify_section_properties.py
python tests/elements/beam_column/elastic_beam_2d/elastic_beam2d_test1.py
python tests/elements/beam_column/elastic_beam_2d/test_frame_01.py
python tests/elements/beam_column/elastic_beam_2d/elastic_beam2d_frequency_01.py
echo "$BLEU" "    Elastic beam-column 3D tests." "$NORMAL"
python tests/elements/beam_column/elastic_beam_3d/cantilever3d_01.py
python tests/elements/beam_column/elastic_beam_3d/cantilever3d_02.py
python tests/elements/beam_column/elastic_beam_3d/cantilever3d_03.py
python tests/elements/beam_column/elastic_beam_3d/cantilever3d_04.py
python tests/elements/beam_column/elastic_beam_3d/cantilever3d_05.py
python tests/elements/beam_column/elastic_beam_3d/cantilever3d_06.py
python tests/elements/beam_column/elastic_beam_3d/cantilever3d_07.py
python tests/elements/beam_column/elastic_beam_3d/cantilever3d_08.py
python tests/elements/beam_column/elastic_beam_3d/cantilever3d_09.py
python tests/elements/beam_column/elastic_beam_3d/cantilever3d_10.py
python tests/elements/beam_column/elastic_beam_3d/elastic_beam3d_sign_criteria_01.py
python tests/elements/beam_column/elastic_beam_3d/elastic_beam3d_sign_criteria_02.py
python tests/elements/beam_column/elastic_beam_3d/elastic_beam3d_sign_criteria_03.py
python tests/elements/beam_column/elastic_beam_3d/test_beam3d_sections_axes_orientation.py
python tests/elements/beam_column/elastic_beam_3d/elastic_beam3d_test1.py
python tests/elements/beam_column/elastic_beam_3d/elastic_beam3d_test2.py
python tests/elements/beam_column/elastic_beam_3d/test_crdTransf_rotation_01.py
python tests/elements/beam_column/elastic_beam_3d/test_torsion_01.py
python tests/elements/beam_column/elastic_beam_3d/test_torsion_02.py
echo "$BLEU" "    Timoshenko beam 2D tests." "$NORMAL"
python tests/elements/beam_column/timoshenko_beam2d_test1.py
python tests/elements/beam_column/timoshenko_beam2d_sign_criteria_01.py
echo "$BLEU" "    Timoshenko beam 3D tests." "$NORMAL"
python tests/elements/beam_column/timoshenko_beam3d_sign_criteria_01.py
python tests/elements/beam_column/timoshenko_beam3d_test1.py
python tests/elements/beam_column/timoshenko_beam3d_test2.py
echo "$BLEU" "    Force beam-column 2D tests." "$NORMAL"
python tests/elements/beam_column/test_force_beam_column_2d_01.py
python tests/elements/beam_column/test_force_beam_column_2d_02.py
python tests/elements/beam_column/test_force_beam_column_2d_03.py
echo "$BLEU" "    Force beam-column 3D tests." "$NORMAL"
python tests/elements/beam_column/test_force_beam_column_3d_01.py
python tests/elements/beam_column/test_force_beam_column_3d_02.py
python tests/elements/beam_column/test_force_beam_column_3d_03.py
python tests/elements/beam_column/test_force_beam_column_3d_04.py
python tests/elements/beam_column/test_force_beam_column_3d_05.py
python tests/elements/beam_column/test_force_beam_column_3d_06.py
python tests/elements/beam_column/plastic_hinge_on_cantilever.py
python tests/elements/beam_column/test_integration_options_01.py
python tests/elements/beam_column/test_strains_sign_01.py
echo "$BLEU" "  Zero length elements tests." "$NORMAL"
python tests/elements/zero_length/zero_length_element_test_01.py
python tests/elements/zero_length/zero_length_element_test_02.py
python tests/elements/zero_length/zero_length_element_test_03.py
python tests/elements/zero_length/zero_length_element_test_04.py
python tests/elements/zero_length/zero_length_element_test_05.py
python tests/elements/zero_length/zero_length_element_test_06.py
python tests/elements/zero_length/test_zero_length_section_sign_criteria.py
python tests/elements/zero_length/test_strains_sign00.py
python tests/elements/zero_length/test_strains_sign01.py
python tests/elements/zero_length/test_strains_sign02.py
python tests/elements/zero_length/test_strains_sign03.py
python tests/elements/zero_length/test_strains_sign04.py
python tests/elements/zero_length/test_winkler.py
python tests/elements/zero_length/test_quasi_winkler.py
echo "$BLEU" "  Plane elasticity elements tests." "$NORMAL"
python tests/elements/plane/test_tri31.py
python tests/elements/plane/test_four_node_quad_01.py
python tests/elements/plane/test_four_node_quad_02.py
python tests/elements/plane/test_maximum_corner_angle.py
python tests/elements/plane/test_extrapolate_values_quad_01.py
python tests/elements/plane/test_extrapolate_values_quad_02.py
python tests/elements/plane/test_von_mises_stresses_01.py
echo "$BLEU" "  Shell elements tests." "$NORMAL"
python tests/elements/shell/test_shell_mitc4_01.py
python tests/elements/shell/test_shell_mitc4_02.py
python tests/elements/shell/test_shell_mitc4_03.py
python tests/elements/shell/test_shell_mitc4_04.py
python tests/elements/shell/test_shell_mitc4_05.py
python tests/elements/shell/test_shell_mitc4_06.py
python tests/elements/shell/test_shell_mitc4_07.py
python tests/elements/shell/test_shell_mitc4_08.py
python tests/elements/shell/test_shell_mitc4_09.py
python tests/elements/shell/test_shell_mitc4_10.py
python tests/elements/shell/test_shell_mitc4_11.py
python tests/elements/shell/test_shell_mitc4_12.py
python tests/elements/shell/test_shell_mitc4_13.py
python tests/elements/shell/test_shell_mitc4_14.py
python tests/elements/shell/test_shell_mitc4_15.py
python tests/elements/shell/test_shell_mitc4_16.py
python tests/elements/shell/test_shell_mitc4_17.py
python tests/elements/shell/test_shell_mitc4_18.py
python tests/elements/shell/test_shell_mitc4_19.py
python tests/elements/shell/test_shell_mitc4_20.py
python tests/elements/shell/test_shell_mitc4_21.py
python tests/elements/shell/test_shell_mitc4_22.py
python tests/elements/shell/test_shell_mitc4_23.py
python tests/elements/shell/test_shell_mitc4_24.py
python tests/elements/shell/test_shell_mitc4_25.py
python tests/elements/shell/test_shell_mitc4_26.py
python tests/elements/shell/test_shell_mitc4_27.py
python tests/elements/shell/test_shell_mitc4_28.py
python tests/elements/shell/test_shell_mitc9_01.py
python tests/elements/shell/test_shell_mitc9_02.py
python tests/elements/shell/test_shell_mitc9_03.py
python tests/elements/shell/test_tributary_area_01.py
python tests/elements/shell/test_shell_mitc4_natural_coordinates_01.py
python tests/elements/shell/test_transformInternalForces.py
python tests/elements/shell/test_gauss_points_cartesian_coordinates.py

echo "$BLEU" "  Solid elements tests." "$NORMAL"
python tests/elements/volume/test_brick_00.py
python tests/elements/volume/test_brick_01.py
python tests/elements/volume/test_brick_02.py
python tests/elements/volume/test_extrapolation_matrix.py
python tests/elements/volume/test_brick_shape_functions.py
python tests/elements/volume/test_extrapolate_values_brick.py

echo "$BLEU" "  Misc elements tests." "$NORMAL"
python tests/elements/spring_test_01.py
python tests/elements/spring_test_02.py
python tests/elements/test_elastomeric_bearing_01.py
python tests/elements/test_pot_bearing_01.py
python tests/elements/test_pot_bearing_02.py
python tests/elements/test_pot_bearing_03.py

echo "$BLEU" "    Element birth and death tests." "$NORMAL"
python tests/elements/birth_and_death/kill_elements_01.py
python tests/elements/birth_and_death/kill_elements_02.py
python tests/elements/birth_and_death/kill_elements_03.py
python tests/elements/birth_and_death/kill_elements_04.py
python tests/elements/birth_and_death/kill_elements_05.py
python tests/elements/birth_and_death/kill_elements_06.py
python tests/elements/birth_and_death/kill_elements_07.py
python tests/elements/birth_and_death/kill_elements_08.py

echo "$BLEU" "Solution tests." "$NORMAL"
python tests/solution/superlu_solver_test_01.py
python tests/solution/superlu_solver_test_02.py
python tests/solution/umf_solver_test_01.py
python tests/solution/mumps_solver_test_01.py
echo "$BLEU" "  Ill conditioning tests." "$NORMAL"
python tests/solution/ill_conditioning/ill_conditioning_01.py
python tests/solution/ill_conditioning/get_floating_nodes_01.py
echo "$BLEU" "  Integrators tests." "$NORMAL"
python tests/solution/integrator/test_displacement_control_integrator_01.py
python tests/solution/integrator/test_displacement_control_integrator_02.py

## Constraint handlers tests.
echo "$BLEU" "  Constraint handler tests." "$NORMAL"
python tests/solution/constraint_handler/plain_handler_test_01.py
python tests/solution/constraint_handler/transformation_handler_test_01.py
python tests/solution/constraint_handler/transformation_handler_test_02.py
python tests/solution/constraint_handler/transformation_handler_test_03.py
python tests/solution/constraint_handler/lagrange_handler_test_01.py

## Eigenvalues.
echo "$BLEU" "  Eigenvalue solution tests." "$NORMAL"
python tests/solution/eigenvalues/test_string_under_tension.py
python tests/solution/eigenvalues/test_cqc_01.py
python tests/solution/eigenvalues/test_band_arpackpp_solver_01.py
python tests/solution/eigenvalues/test_ordinary_eigenvalues.py
echo "$BLEU" "    Eigenmode computation." "$NORMAL"
python tests/solution/eigenvalues/eigenmodes/cantilever_eigenmodes_01.py
python tests/solution/eigenvalues/eigenmodes/cantilever_eigenmodes_02.py
python tests/solution/eigenvalues/eigenmodes/cantilever_eigenmodes_03.py
python tests/solution/eigenvalues/eigenmodes/cantilever_eigenmodes_04.py
python tests/solution/eigenvalues/eigenmodes/cantilever_eigenmodes_05.py
python tests/solution/eigenvalues/eigenmodes/cantilever_eigenmodes_06.py
python tests/solution/eigenvalues/eigenmodes/cantilever_eigenmodes_07.py
python tests/solution/eigenvalues/eigenmodes/cantilever_eigenmodes_08.py
python tests/solution/eigenvalues/eigenmodes/cantilever_eigenmodes_09.py
echo "$BLEU" "    Modal analysis tests." "$NORMAL"
python tests/solution/eigenvalues/modal_analysis/modal_analysis_test_01.py
python tests/solution/eigenvalues/modal_analysis/modal_analysis_test_02.py
python tests/solution/eigenvalues/modal_analysis/modal_analysis_test_03.py
python tests/solution/eigenvalues/modal_analysis/modal_analysis_test_04.py
python tests/solution/eigenvalues/modal_analysis/modal_analysis_test_05.py
echo "$BLEU" "    Linear buckling analysis tests." "$NORMAL"
python tests/solution/eigenvalues/linear_buckling_analysis/linear_buckling_column01.py
python tests/solution/eigenvalues/linear_buckling_analysis/linear_buckling_column02.py
python tests/solution/eigenvalues/linear_buckling_analysis/linear_buckling_column03.py
python tests/solution/eigenvalues/linear_buckling_analysis/linear_buckling_column04.py
python tests/solution/eigenvalues/linear_buckling_analysis/linear_buckling_column05.py
python tests/solution/eigenvalues/linear_buckling_analysis/square_plate_buckling_01.py

## Geometric non-linearity.
echo "$BLEU" "  PDelta solution tests." "$NORMAL"
python tests/solution/geom_nl/test_pdelta_01.py
python tests/solution/geom_nl/test_pdelta_02.py

echo "$BLEU" "  AISC 360-16 benchmark tests." "$NORMAL"
python tests/solution/geom_nl/aisc_360_benchmark_problem_case_01.py
python tests/solution/geom_nl/aisc_360_benchmark_problem_case_02.py
python tests/solution/geom_nl/aisc_360_benchmark_problem_case_03.py
python tests/solution/geom_nl/aisc_360_benchmark_problem_case_04.py
python tests/solution/geom_nl/aisc_360_benchmark_problem_case_05.py
python tests/solution/geom_nl/aisc_360_benchmark_problem_case_06.py
python tests/solution/geom_nl/aisc_360_benchmark_problem_case_07.py
python tests/solution/geom_nl/aisc_360_benchmark_problem_case_08.py

## Time history.
echo "$BLEU" "  Time history solution tests." "$NORMAL"
python tests/solution/time_history/test_time_history_00.py
python tests/solution/time_history/test_time_history_01.py
python tests/solution/time_history/test_pseudo_time_history.py

## Convergence tests.
echo "$BLEU" "  Convergence tests." "$NORMAL"
python tests/solution/convergence_test/relative_total_norm_disp_incr_test_01.py

#Preprocessor tests
echo "$BLEU" "Preprocessor tests." "$NORMAL"
echo "$BLEU" "  Simpson rule tests." "$NORMAL"
python tests/preprocessor/integra_simpson_fila_i.py
python tests/preprocessor/integra_simpson_fila_j.py
python tests/preprocessor/integra_simpson_fila_k.py

echo "$BLEU" "  Preprocessor entities tests." "$NORMAL"
python tests/preprocessor/geom_entities/line.py
python tests/preprocessor/geom_entities/line_02.py
python tests/preprocessor/geom_entities/k_points.py
python tests/preprocessor/geom_entities/linea_tramos.py
python tests/preprocessor/geom_entities/test_line_seq_01.py
python tests/preprocessor/geom_entities/test_line_seq_02.py
python tests/preprocessor/geom_entities/circumference_arc_01.py
python tests/preprocessor/geom_entities/circumference_arc_02.py
python tests/preprocessor/geom_entities/circumference_arc_03.py
python tests/preprocessor/geom_entities/circumference_arc_04.py
python tests/preprocessor/geom_entities/circumference_arc_05.py
python tests/preprocessor/geom_entities/create_points_line.py
python tests/preprocessor/geom_entities/test_translation_01.py
python tests/preprocessor/geom_entities/test_rotation_01.py
python tests/preprocessor/geom_entities/test_rotation_02.py
python tests/preprocessor/geom_entities/test_rotation_03.py
python tests/preprocessor/geom_entities/test_reflection_01.py
python tests/preprocessor/geom_entities/test_cartesian_ref_sys_01.py
python tests/preprocessor/geom_entities/test_cartesian_ref_sys_02.py
python tests/preprocessor/geom_entities/test_cartesian_ref_sys_03.py
python tests/preprocessor/geom_entities/test_unif_grid_01.py
python tests/preprocessor/geom_entities/test_unif_grid_02.py
python tests/preprocessor/geom_entities/test_unif_grid_03.py
python tests/preprocessor/geom_entities/test_quadrilateral_surface_01.py
python tests/preprocessor/geom_entities/test_quadrilateral_surface_02.py
python tests/preprocessor/geom_entities/test_quadrilateral_surface_03.py
python tests/preprocessor/geom_entities/test_quadrilateral_surface_04.py
python tests/preprocessor/geom_entities/test_surface_set_orientation.py
python tests/preprocessor/geom_entities/test_polygonal_surface_01.py
python tests/preprocessor/geom_entities/test_cross-stitch_01.py
python tests/preprocessor/geom_entities/test_2d_scheme.py
python tests/preprocessor/geom_entities/test_3d_scheme.py
python tests/preprocessor/geom_entities/test_nearest_node_01.py
python tests/preprocessor/geom_entities/test_nearest_element_01.py
python tests/preprocessor/geom_entities/split_line_01.py
python tests/preprocessor/geom_entities/split_line_02.py
python tests/preprocessor/geom_entities/split_line_03.py
python tests/preprocessor/geom_entities/test_split_lines_01.py
python tests/preprocessor/geom_entities/test_split_lines_02.py
python tests/preprocessor/geom_entities/test_block_definition.py
echo "$BLEU" "  Meshing routines tests." "$NORMAL"
python tests/preprocessor/meshing/test_surface_axes_01.py
python tests/preprocessor/meshing/test_surface_meshing_01.py
python tests/preprocessor/meshing/test_surface_meshing_02.py
python tests/preprocessor/meshing/test_surface_meshing_03.py
python tests/preprocessor/meshing/test_surface_meshing_04.py
python tests/preprocessor/meshing/test_surface_meshing_05.py
python tests/preprocessor/meshing/test_surface_meshing_06.py
python tests/preprocessor/meshing/test_imposed_meshing.py
python tests/preprocessor/meshing/test_truss_generator_01.py
python tests/preprocessor/meshing/test_paving_01.py
python tests/preprocessor/meshing/test_paving_02.py
python tests/preprocessor/meshing/test_paving_03.py
python tests/preprocessor/meshing/test_paving_04.py
python tests/preprocessor/meshing/test_paving_05.py
python tests/preprocessor/meshing/test_paving_06.py
python tests/preprocessor/meshing/test_paving_07.py
python tests/preprocessor/meshing/test_gmsh_01.py
python tests/preprocessor/meshing/test_gmsh_02.py
python tests/preprocessor/meshing/test_gmsh_03.py
python tests/preprocessor/meshing/test_gmsh_04.py
python tests/preprocessor/meshing/test_gmsh_05.py
python tests/preprocessor/meshing/test_gmsh_06.py
python tests/preprocessor/meshing/test_gmsh_07.py
python tests/preprocessor/meshing/test_gmsh_08.py
python tests/preprocessor/meshing/test_gmsh_09.py
python tests/preprocessor/meshing/test_gmsh_10.py

echo "$BLEU" "  Sets handling tests." "$NORMAL"
python tests/preprocessor/sets/test_exist_set.py
python tests/preprocessor/sets/test_set_01.py
python tests/preprocessor/sets/une_sets.py
python tests/preprocessor/sets/sets_boolean_operations_01.py
python tests/preprocessor/sets/sets_boolean_operations_02.py
python tests/preprocessor/sets/test_set_rename_01.py
python tests/preprocessor/sets/test_set_rename_02.py
python tests/preprocessor/sets/test_resisting_svd01.py
python tests/preprocessor/sets/test_get_contours_01.py
python tests/preprocessor/sets/test_get_contours_02.py
python tests/preprocessor/sets/test_pick_entities.py
python tests/preprocessor/sets/test_sets_and_grids.py
python tests/preprocessor/sets/test_sets_and_grids_02.py
python tests/preprocessor/sets/test_get_bnd_01.py
python tests/preprocessor/sets/test_fill_downwards_01.py
python tests/preprocessor/sets/test_get_mesh_components_01.py
python tests/preprocessor/sets/test_set_transformation_01.py
python tests/preprocessor/sets/test_set_transformation_02.py
echo "$BLEU" "  Preprocessor grid model tests." "$NORMAL"
python tests/preprocessor/grid_model/test_grid_model_01.py
python tests/preprocessor/grid_model/test_grid_model_02.py
python tests/preprocessor/grid_model/test_elliptical_coord_01.py
echo "$BLEU" "  Import/export tests." "$NORMAL"
echo "$BLEU" "    DXF tests." "$NORMAL"
python tests/preprocessor/import_export/dxf/test_split_lines.py
python tests/preprocessor/import_export/dxf/test_dxf_import_01.py
python tests/preprocessor/import_export/dxf/test_dxf_groups.py
python tests/preprocessor/import_export/dxf/test_dxf_surfaces.py
python tests/preprocessor/import_export/dxf/test_dxf_export_01.py
echo "$BLEU" "    FreeCAD tests." "$NORMAL"
python tests/preprocessor/import_export/freecad/test_freecad_groups.py
python tests/preprocessor/import_export/freecad/test_ifc_points.py
python tests/preprocessor/import_export/freecad/test_ifc_lines.py
python tests/preprocessor/import_export/freecad/test_ifc_surface_01.py
python tests/preprocessor/import_export/freecad/test_ifc_surface_02.py
python tests/preprocessor/import_export/freecad/test_ifc_pinned_member.py
python tests/preprocessor/import_export/freecad/test_ifc_mixed_blocks_01.py

#Tests about constraints.
echo "$BLEU" "Displacement constraints tests." "$NORMAL"
python tests/constraints/remove_fix_01.py
python tests/constraints/remove_fix_02.py
python tests/constraints/test_elastic_bearing_01.py
python tests/constraints/test_elastic_bearing_02.py
python tests/constraints/test_elastic_bearing_03.py
python tests/constraints/test_elastic_bearing_04.py
python tests/constraints/test_rigid_beam_01.py
python tests/constraints/test_rigid_rod_01.py
python tests/constraints/test_huge_beam.py
python tests/constraints/test_huge_truss.py
python tests/constraints/test_fulcrum_01.py
python tests/constraints/test_release_beam_node_01.py
python tests/constraints/test_pile_01.py
echo "$BLEU" "  Glue node to element constraints tests." "$NORMAL"
python tests/constraints/test_glue_node_to_element_01.py
python tests/constraints/test_glue_node_to_element_02.py
python tests/constraints/test_glue_node_to_element_03.py
python tests/constraints/test_glue_node_to_element_04.py
python tests/constraints/test_glue_node_to_element_05.py
python tests/constraints/test_glue_node_to_element_06.py
python tests/constraints/test_glue_node_to_element_07.py
python tests/constraints/test_glue_node_to_element_08.py
python tests/constraints/test_glue_node_to_element_09.py

#Load tests
echo "$BLEU" "Loads tests." "$NORMAL"
echo "$BLEU" "  Load time series tests." "$NORMAL"
python tests/loads/time_series/test_linear_01.py
python tests/loads/time_series/test_constant_01.py
python tests/loads/time_series/test_trig_01.py
python tests/loads/time_series/test_triang_01.py
python tests/loads/time_series/test_rectang_01.py
python tests/loads/time_series/test_pulse_01.py
echo "$BLEU" "    Ground motion time series tests." "$NORMAL"
python tests/loads/time_series/test_ground_motion_01.py
python tests/loads/time_series/test_ground_motion_02.py
python tests/loads/time_series/test_ground_motion_03.py
python tests/loads/time_series/test_ground_motion_04.py
python tests/loads/time_series/test_ground_motion_05.py
python tests/loads/time_series/test_ground_motion_06.py
python tests/loads/time_series/test_ground_motion_07.py
python tests/loads/time_series/test_ground_motion_08.py
echo "$BLEU" "    Path time series tests." "$NORMAL"
python tests/loads/time_series/test_path_01.py
python tests/loads/time_series/test_path_time_01.py
echo "$BLEU" "  Loads tests." "$NORMAL"
python tests/loads/beam2dNodalLoad.py
python tests/loads/beam2dNodalLoad02.py
python tests/loads/beam2dPointLoad.py
python tests/loads/beam2dUniformLoad.py
python tests/loads/beam2dUniformLoad02.py
python tests/loads/beam3dPointLoad.py
python tests/loads/beam3dUniformLoad.py
python tests/loads/beam3dUniformLoad02.py
python tests/loads/beam3dUniformLoad03.py
python tests/loads/test_vector2d_uniform_load_local.py
python tests/loads/test_vector2d_uniform_load_global01.py
python tests/loads/test_vector3d_uniform_load_local.py
python tests/loads/test_vector3d_uniform_load_global01.py
python tests/loads/test_vector3d_uniform_load_global02.py
python tests/loads/test_vector3d_uniform_load_global03.py
python tests/loads/element_load_on_already_active_pattern.py
echo "$BLEU" "    Concentrated loads." "$NORMAL"
python tests/loads/concentrated_loads/test_vector2d_point_load_local.py
python tests/loads/concentrated_loads/test_vector2d_point_load_global_01.py
python tests/loads/concentrated_loads/test_vector2d_point_load_global_02.py
python tests/loads/concentrated_loads/test_vector2d_point_load_global_03.py
python tests/loads/concentrated_loads/test_vector3d_point_load_local.py
python tests/loads/concentrated_loads/test_vector3d_point_load_global_01.py
python tests/loads/concentrated_loads/test_vector3d_point_load_global_02.py
python tests/loads/concentrated_loads/test_vector3d_point_load_global_03.py
echo "$BLEU" "    Load distribution." "$NORMAL"
python tests/loads/load_distribution/test_sliding_vector_load_01.py
python tests/loads/load_distribution/test_sliding_vector_load_02.py
python tests/loads/load_distribution/test_surf_unif_load_distributed.py
echo "$BLEU" "    Strain loads." "$NORMAL"
python tests/loads/strain_loads/truss_strain_load_01.py
python tests/loads/strain_loads/truss_strain_load_02.py
python tests/loads/strain_loads/truss_strain_load_03.py
python tests/loads/strain_loads/truss_strain_load_04.py
python tests/loads/strain_loads/truss_strain_load_05.py
python tests/loads/strain_loads/truss_strain_load_06.py
python tests/loads/strain_loads/beam_strain_load_01.py
python tests/loads/strain_loads/beam_strain_load_02.py
python tests/loads/strain_loads/beam_strain_load_03.py
python tests/loads/strain_loads/beam_strain_load_04.py
python tests/loads/strain_loads/beam_strain_load_05.py
python tests/loads/strain_loads/beam_strain_load_06.py
python tests/loads/strain_loads/beam_strain_load_07.py
python tests/loads/strain_loads/beam_strain_load_08.py
python tests/loads/strain_loads/beam_strain_load_09.py
python tests/loads/strain_loads/shell_strain_load_01.py
python tests/loads/strain_loads/shell_strain_load_02.py
python tests/loads/strain_loads/shell_strain_load_03.py
python tests/loads/strain_loads/shell_strain_load_04.py
python tests/loads/strain_loads/shell_strain_load_05.py
python tests/loads/strain_loads/shell_grad_strain_load_02.py
python tests/loads/strain_loads/shell_grad_strain_load_03.py
echo "$BLEU" "    Inertia loads." "$NORMAL"
python tests/loads/inertia_loads/test_inertia_loads_00.py
python tests/loads/inertia_loads/test_inertia_loads_01.py
python tests/loads/inertia_loads/test_inertia_loads_02.py
python tests/loads/inertia_loads/test_inertia_loads_03.py
python tests/loads/inertia_loads/test_inertia_loads_04.py
python tests/loads/inertia_loads/test_inertia_loads_05.py
python tests/loads/inertia_loads/test_inertia_loads_06.py
python tests/loads/inertia_loads/test_inertia_loads_07.py
python tests/loads/inertia_loads/test_inertia_loads_08.py
python tests/loads/inertia_loads/test_inertia_loads_09.py
python tests/loads/inertia_loads/test_inertia_loads_10.py
python tests/loads/inertia_loads/test_inertia_loads_11.py
echo "$BLEU" "    Dynamic loads." "$NORMAL"
python tests/loads/dynamic_loads/test_node_load_history_01.py
python tests/loads/dynamic_loads/test_node_load_history_02.py
python tests/loads/dynamic_loads/test_node_load_history_03.py
python tests/loads/dynamic_loads/test_node_load_history_04.py
echo "$BLEU" "    Computing load combination tests." "$NORMAL"
python tests/loads/combinations/add_loads_01.py
python tests/loads/combinations/test_add_load_cases.py
python tests/loads/combinations/test_combination01.py
python tests/loads/combinations/test_combination02.py
python tests/loads/combinations/test_combination03.py
#XXX sqlitepp extremely slow. Seek for another solution.
# python tests/combinations/test_combination04.py
python tests/loads/combinations/test_combination05.py
python tests/loads/combinations/test_combination06.py
python tests/loads/combinations/test_combination07.py
python tests/loads/combinations/test_combination08.py
python tests/loads/combinations/test_combination09.py
python tests/loads/combinations/test_davit_01.py
python tests/loads/combinations/test_davit_02.py

#Materials tests
#Uniaxial materials.
echo "$BLEU" "Materials tests." "$NORMAL"
python tests/materials/test_get_material_names.py
echo "$BLEU" "  XC materials tests." "$NORMAL"
python tests/materials/xc_materials/test_change_element_material.py
echo "$BLEU" "    Uniaxial materials tests." "$NORMAL"
python tests/materials/xc_materials/uniaxial/test_elasticpp_material_01.py
python tests/materials/xc_materials/uniaxial/test_steel01.py
python tests/materials/xc_materials/uniaxial/test_steel02.py
python tests/materials/xc_materials/uniaxial/test_steel02_prestressing.py
python tests/materials/xc_materials/uniaxial/test_concrete01.py
python tests/materials/xc_materials/uniaxial/test_concrete02_01.py
python tests/materials/xc_materials/uniaxial/test_concrete02_02.py
python tests/materials/xc_materials/uniaxial/test_HA25_01.py
python tests/materials/xc_materials/uniaxial/test_HA25_02.py
python tests/materials/xc_materials/uniaxial/test_HP45_01.py
python tests/materials/xc_materials/uniaxial/test_HP45_02.py
python tests/materials/xc_materials/uniaxial/test_init_strain_material_01.py
python tests/materials/xc_materials/uniaxial/test_multilinear_material_01.py
python tests/materials/xc_materials/uniaxial/test_multilinear_material_02.py
echo "$BLEU" "      EHE reinforcing steel." "$NORMAL"
python tests/materials/xc_materials/uniaxial/test_B400S_01.py
python tests/materials/xc_materials/uniaxial/test_B400S_02.py
python tests/materials/xc_materials/uniaxial/test_B500S_01.py
python tests/materials/xc_materials/uniaxial/test_B500S_02.py
echo "$BLEU" "      SIA reinforcing steel." "$NORMAL"
python tests/materials/xc_materials/uniaxial/test_B500A_01.py
python tests/materials/xc_materials/uniaxial/test_B500A_02.py
python tests/materials/xc_materials/uniaxial/test_B500B_01.py
python tests/materials/xc_materials/uniaxial/test_B500B_02.py
echo "$BLEU" "      cable material." "$NORMAL"
python tests/materials/xc_materials/uniaxial/test_cable_material01.py
python tests/materials/xc_materials/uniaxial/test_cable_material02.py
python tests/materials/xc_materials/uniaxial/test_cable_material03.py
python tests/materials/xc_materials/uniaxial/test_cable_material04.py
python tests/materials/xc_materials/uniaxial/test_cable_material05.py
python tests/materials/xc_materials/uniaxial/test_cable_material06.py
python tests/materials/xc_materials/uniaxial/test_cable_material07.py
python tests/materials/xc_materials/uniaxial/test_elast_no-tension_material.py
echo "$BLEU" "      Connected materials." "$NORMAL"
python tests/materials/xc_materials/uniaxial/connected/test_series_material_01.py
python tests/materials/xc_materials/uniaxial/connected/test_series_material_02.py
echo "$BLEU" "      Soil structure interaction materials." "$NORMAL"
python tests/materials/xc_materials/uniaxial/soil_structure_interaction/py_simple1_test.py
python tests/materials/xc_materials/uniaxial/soil_structure_interaction/test_soil_response_01.py
python tests/materials/xc_materials/uniaxial/soil_structure_interaction/test_soil_response_02.py
python tests/materials/xc_materials/uniaxial/soil_structure_interaction/test_soil_response_03.py
python tests/materials/xc_materials/uniaxial/soil_structure_interaction/test_soil_response_04.py
python tests/materials/xc_materials/uniaxial/soil_structure_interaction/test_soil_response_05.py
#nD Materials
echo "$BLEU" "      nD materials tests." "$NORMAL"
python tests/materials/xc_materials/nD/test_elastic_isotropic_plane_strain_2d_01.py
python tests/materials/xc_materials/nD/test_elastic_isotropic_plane_stress_2d_01.py
python tests/materials/xc_materials/nD/test_elastic_isotropic_3d_01.py
python tests/materials/xc_materials/nD/test_elastic_isotropic_3d_strain_load.py
echo "$BLEU" "      J2 (Von Mises) materials tests." "$NORMAL"
python tests/materials/xc_materials/nD/test_j2_plate_fibre.py
python tests/materials/xc_materials/nD/test_j2_plate_fibre_strain_load.py
echo "$BLEU" "      Plate and membrane materials." "$NORMAL"
python tests/materials/xc_materials/sections/test_material_elastic_membrane_plate_section_01.py
python tests/materials/xc_materials/sections/test_material_elastic_plate_section_01.py

#Cross sections.
echo "$BLEU" "    Sections." "$NORMAL"
#Cross sections. Geometry.
echo "$BLEU" "      Section geometry tests." "$NORMAL"
python tests/materials/xc_materials/sections/section_geom/test_spot_01.py
python tests/materials/xc_materials/sections/section_geom/test_segment_01.py
python tests/materials/xc_materials/sections/section_geom/test_circular_region_01.py
python tests/materials/xc_materials/sections/section_geom/test_quad_region_01.py
python tests/materials/xc_materials/sections/section_geom/test_quad_region_02.py
python tests/materials/xc_materials/sections/section_geom/test_quad_region_03.py
python tests/materials/xc_materials/sections/section_geom/test_annular_sector_rg_01.py
python tests/materials/xc_materials/sections/section_geom/test_annular_sector_rg_02.py
python tests/materials/xc_materials/sections/section_geom/test_mass_properties_01.py
python tests/materials/xc_materials/sections/section_geom/test_mass_properties_02.py
python tests/materials/xc_materials/sections/section_geom/test_mass_properties_03.py
python tests/materials/xc_materials/sections/section_geom/test_mass_properties_04.py
python tests/materials/xc_materials/sections/section_geom/test_geom_params_torsion_ehe_01.py
python tests/materials/xc_materials/sections/section_geom/test_box_girder_torsional_stiffness.py
echo "$BLEU" "      Cross-sections region tests." "$NORMAL"
python tests/materials/xc_materials/sections/section_geom/regions/quad_sect_region_test_01.py
python tests/materials/xc_materials/sections/section_geom/regions/quad_sect_region_test_02.py

# Compound sections.
echo "$BLEU" "      Compound sections." "$NORMAL"
python tests/materials/xc_materials/sections/test_compound_section_01.py
python tests/materials/xc_materials/sections/test_compound_section_02.py
echo "$BLEU" "      Elastic section tests." "$NORMAL"
python tests/materials/xc_materials/sections/elastic_section/test_elastic_section_2d_sign_convention_01.py
python tests/materials/xc_materials/sections/elastic_section/test_elastic_section_2d_01.py
python tests/materials/xc_materials/sections/elastic_section/test_elastic_shear_section_2d_01.py
python tests/materials/xc_materials/sections/elastic_section/test_elastic_section_3d_01.py
python tests/materials/xc_materials/sections/elastic_section/test_elastic_section_3d_02.py
python tests/materials/xc_materials/sections/elastic_section/test_elastic_shear_section_3d_01.py
python tests/materials/xc_materials/sections/elastic_section/test_tangent_stiffness_02.py
python tests/materials/xc_materials/sections/elastic_section/test_section_rotation_3d_01.py
python tests/materials/xc_materials/sections/elastic_section/test_section_rotation_3d_02.py
python tests/materials/xc_materials/sections/elastic_section/test_section_rotation_3d_03.py
python tests/materials/xc_materials/sections/elastic_section/test_section_rotation_3d_04.py
echo "$BLEU" "      Fiber section tests." "$NORMAL"
python tests/materials/xc_materials/sections/fiber_section/test_tangent_stiffness_01.py
python tests/materials/xc_materials/sections/fiber_section/test_reg_cuad_01.py
python tests/materials/xc_materials/sections/fiber_section/test_straight_reinf_layer_01.py
python tests/materials/xc_materials/sections/fiber_section/test_fiber2d_01.py
python tests/materials/xc_materials/sections/fiber_section/test_fiber3d_01.py
python tests/materials/xc_materials/sections/fiber_section/test_fiber3d_02.py
python tests/materials/xc_materials/sections/fiber_section/test_fiber3d_03.py
python tests/materials/xc_materials/sections/fiber_section/test_fiber3d_04.py
python tests/materials/xc_materials/sections/fiber_section/test_fiber3d_05.py
python tests/materials/xc_materials/sections/fiber_section/test_fiber3d_06.py
python tests/materials/xc_materials/sections/fiber_section/test_fiber3d_07.py
python tests/materials/xc_materials/sections/fiber_section/test_fiber3d_08.py
python tests/materials/xc_materials/sections/fiber_section/test_fiber3d_09.py
echo "$BLEU" "        Beam fiber section tests." "$NORMAL"
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/test_section_aggregator_01.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/test_fiber_section_sign_convention01.py
echo "$BLEU" "          2D Beam fiber section tests." "$NORMAL"
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/2d/test_fiber_section2d_01.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/2d/test_fiber_section2d_02.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/2d/test_fiber_section_shear2d_01.py
echo "$BLEU" "          3D Beam fiber section tests." "$NORMAL"
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section3d_01.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section3d_02.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section3d_03.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section3d_04.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section3d_05.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section3d_06.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section3d_07.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section3d_08.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section3d_09.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section3d_10.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section3d_11.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section3d_12.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section3d_13.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section_shear3d_01.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section_shear3d_02.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section_prop.py
python tests/materials/xc_materials/sections/fiber_section/beam_fiber_sections/3d/test_fiber_section_discretization_error_01.py
echo "$BLEU" "        Fiber section interaction diagrams." "$NORMAL"
python tests/materials/xc_materials/sections/fiber_section/interaction_diagram/test_interaction_diagram01.py
python tests/materials/xc_materials/sections/fiber_section/interaction_diagram/test_interaction_diagram02.py
python tests/materials/xc_materials/sections/fiber_section/interaction_diagram/test_interaction_diagram03.py
python tests/materials/xc_materials/sections/fiber_section/interaction_diagram/test_interaction_diagram04.py
python tests/materials/xc_materials/sections/fiber_section/interaction_diagram/test_interaction_diagram05.py
python tests/materials/xc_materials/sections/fiber_section/interaction_diagram/test_interaction_diagram06.py
python tests/materials/xc_materials/sections/fiber_section/plastic_hinge_on_IPE200.py
echo "$BLEU" "        Membrane plate fiber section tests." "$NORMAL"
python tests/materials/xc_materials/sections/fiber_section/membrane_plate/test_membrane_plate_fiber_material_01.py
python tests/materials/xc_materials/sections/fiber_section/membrane_plate/test_membrane_plate_fiber_material_02.py
python tests/materials/xc_materials/sections/fiber_section/membrane_plate/test_membrane_plate_fiber_material_03.py
python tests/materials/xc_materials/sections/fiber_section/membrane_plate/test_j2_plate_fibre_material_01.py
python tests/materials/xc_materials/sections/fiber_section/membrane_plate/test_j2_plate_fibre_material_02.py
python tests/materials/xc_materials/sections/fiber_section/membrane_plate/test_j2_plate_fibre_material_03.py
echo "$BLEU" "  RC sections test." "$NORMAL"
echo "$BLEU" "    concrete shapes tests." "$NORMAL"
python tests/materials/concrete_shapes/test_mass_properties_rc_section.py
python tests/materials/concrete_shapes/test_reinf_row_01.py
python tests/materials/concrete_shapes/test_reinf_layers_01.py
python tests/materials/concrete_shapes/test_rc_section_01.py
python tests/materials/concrete_shapes/test_rc_section_02.py
python tests/materials/concrete_shapes/test_reinforcement_placement_01.py
python tests/materials/concrete_shapes/test_reinforcement_placement_02.py
python tests/materials/concrete_shapes/test_reinforcement_placement_03.py
python tests/materials/concrete_shapes/test_reinforcement_placement_04.py
python tests/materials/concrete_shapes/test_reinforcement_placement_05.py
python tests/materials/concrete_shapes/test_reinforcement_placement_06.py
echo "$BLEU" "    Misc. (EHE-08)." "$NORMAL"
python tests/materials/ehe/test_concrete_corbel_ehe.py
python tests/materials/ehe/test_Ecm_concrete.py
python tests/materials/ehe/test_EHEconcrete.py
python tests/materials/ehe/test_anchorage_length_01.py
python tests/materials/ehe/test_anchorage_length_02.py
python tests/materials/ehe/test_anchorage_length_03.py
python tests/materials/ehe/test_transmission_length_01.py
echo "$BLEU" "    Crack amplitude determination tests (EHE-08)." "$NORMAL"
python tests/materials/ehe/test_crack_control_01.py
python tests/materials/ehe/test_crack_control_02.py
python tests/materials/ehe/test_crack_control_03.py
python tests/materials/ehe/test_crack_control_04.py
python tests/materials/ehe/test_crack_control_05.py
python tests/materials/ehe/test_crack_control_06.py
python tests/materials/ehe/test_crack_control_07.py
echo "$BLEU" "    Shear resistance determination tests (EHE-08)." "$NORMAL"
python tests/materials/ehe/test_shear_ehe08_01.py
python tests/materials/ehe/test_shear_ehe08_02.py
python tests/materials/ehe/test_shear_ehe08_03.py
python tests/materials/ehe/test_shear_ehe08_04.py
python tests/materials/ehe/test_shear_ehe08_05.py
python tests/materials/ehe/test_shear_stress_joints_01.py
python tests/materials/ehe/test_shear_stress_joints_02.py
python tests/materials/ehe/test_fiber_section_shear_01.py
python tests/materials/ehe/test_fiber_section_shear_02.py
python tests/materials/ehe/test_fiber_section_shear_03.py
echo "$BLEU" "    Shrinkage and creep determination tests (EHE-08)." "$NORMAL"
python tests/materials/ehe/test_creep_01.py
python tests/materials/ehe/test_creep_02.py
python tests/materials/ehe/test_creep_03.py
python tests/materials/ehe/test_shrinkage_01.py
#python tests/materials/ehe/test_shrinkage_02.py
python tests/materials/ehe/test_shrinkage_03.py
echo "$BLEU" "    Steel relaxation determination tests (EHE-08)." "$NORMAL"
python tests/materials/ehe/test_steel_relaxation_01.py

echo "$BLEU" "    EC2 tests." "$NORMAL"
python tests/materials/ec2/test_anchorage_length_01.py
python tests/materials/ec2/test_lap_length_01.py
python tests/materials/ec2/test_EC2Concrete.py
python tests/materials/ec2/test_EC2creep.py
python tests/materials/ec2/test_EC2_2004_shrinkage.py
python tests/materials/ec2/test_EC2_2021_shrinkage.py
python tests/materials/ec2/test_smearedCracking_01.py
python tests/materials/ec2/test_smearedCracking_02.py
python tests/materials/ec2/test_smearedCracking_03.py
python tests/materials/ec2/test_smearedCracking_04.py
python tests/materials/ec2/test_smearedCracking_05.py
python tests/materials/ec2/test_smearedCracking_06.py
python tests/materials/ec2/test_ec2_interaction_diagram.py
python tests/materials/ec2/test_EC2_bending_01.py
python tests/materials/ec2/test_EC2_bending_02.py
python tests/materials/ec2/test_EC2_shear_01.py
python tests/materials/ec2/test_EC2_shear_02.py
python tests/materials/ec2/test_EC2_shear_03.py
python tests/materials/ec2/test_EC2_shear_04.py
python tests/materials/ec2/test_EC2_shear_05.py
python tests/materials/ec2/test_EC2_shear_between_web_and_flanges_01.py

echo "$BLEU" "    SIA 262 tests." "$NORMAL"
python tests/materials/sia262/sia262_concrete_01.py
python tests/materials/sia262/shear_01.py
python tests/materials/sia262/shear_02.py
python tests/materials/sia262/test_anchorage_length.py
python tests/materials/sia262/test_crack_control_01.py
echo "$BLEU" "    ACI 318 tests." "$NORMAL"
python tests/materials/aci/test_aci_development_01.py
python tests/materials/aci/test_aci_shear_01.py
python tests/materials/aci/test_aci_corbel_design.py
echo "$BLEU" "    ACI 349 tests." "$NORMAL"
python tests/materials/aci/test_aci_anchor_tension.py
python tests/materials/aci/test_aci_anchor_shear.py
python tests/materials/aci/test_aci_anchor_comb_tension_shear.py
echo "$BLEU" "    TM 5-809-3 tests." "$NORMAL"
python tests/materials/tm5_809_3/test_equivalent_wall_thickness.py
python tests/materials/tm5_809_3/tm_5_809_3_test01.py

echo "$BLEU" "  Steel sections test." "$NORMAL"
echo "$BLEU" "    steel shapes tests." "$NORMAL"
python tests/materials/steel_shapes/test_aisc_w_shapes.py
python tests/materials/steel_shapes/test_aisc_hss_shapes.py
python tests/materials/steel_shapes/test_steel_shape_01.py
python tests/materials/steel_shapes/test_steel_shape_02.py
python tests/materials/steel_shapes/test_arcelor_metric_shapes_01.py
python tests/materials/steel_shapes/test_arcelor_metric_shapes_02.py
echo "$BLEU" "    EC3 tests." "$NORMAL"
python tests/materials/ec3/compare_mech_properties.py
python tests/materials/ec3/test_buckling_resistance_01.py
python tests/materials/ec3/test_lateral_torsional_buckling00.py
python tests/materials/ec3/test_lateral_torsional_buckling01.py
python tests/materials/ec3/test_lateral_torsional_buckling02.py
python tests/materials/ec3/test_lateral_torsional_buckling03.py
python tests/materials/ec3/test_cross_section_verification.py
python tests/materials/ec3/test_beam_contrpnt.py
python tests/materials/ec3/test_biax_bend_coeff.py
python tests/materials/ec3/test_classif.py
python tests/materials/ec3/test_beam_with_full_lateral_restraint.py
python tests/materials/ec3/test_bolt_shear_01.py
echo "$BLEU" "    EAE tests." "$NORMAL"
python tests/materials/eae/test_steel_corbel_weld.py
python tests/materials/eae/test_simple_shear_welded_web.py
python tests/materials/eae/test_weld_bead_strength.py
python tests/materials/eae/bolt_test_01.py
echo "$BLEU" "    SIA 263 tests." "$NORMAL"
python tests/materials/sia263/test_coefs_fatique_SIA263.py
echo "$BLEU" "    ASTM/AISC tests." "$NORMAL"
python tests/materials/astm_aisc/create_self_weight_load_01.py
python tests/materials/astm_aisc/create_self_weight_load_02.py
python tests/materials/astm_aisc/combined_internal_forces_test_01.py
python tests/materials/astm_aisc/combined_internal_forces_test_02.py
echo "$BLEU" "      ASTM/AISC member design tests." "$NORMAL"
python tests/materials/astm_aisc/member_design/hss12x8_test_01.py
python tests/materials/astm_aisc/member_design/hss12x8_test_02.py
python tests/materials/astm_aisc/member_design/hss8x8_test_01.py
python tests/materials/astm_aisc/member_design/hss8x8_test_02.py
python tests/materials/astm_aisc/member_design/aisc_single_angle_test.py
python tests/materials/astm_aisc/member_design/flexural_member_design_test_01.py
python tests/materials/astm_aisc/member_design/flexural_member_design_test_02.py
python tests/materials/astm_aisc/member_design/flexural_member_design_test_03.py
python tests/materials/astm_aisc/member_design/flexural_member_design_test_04.py
python tests/materials/astm_aisc/member_design/flexural_member_design_test_05.py
python tests/materials/astm_aisc/member_design/flexural_member_design_test_06.py
python tests/materials/astm_aisc/member_design/flexural_member_design_test_07.py
python tests/materials/astm_aisc/member_design/flexural_member_design_test_08.py
python tests/materials/astm_aisc/member_design/flexural_member_design_test_09.py
python tests/materials/astm_aisc/member_design/flexural_member_design_test_10.py
python tests/materials/astm_aisc/member_design/flexural_member_design_IShape_test_11.py
python tests/materials/astm_aisc/member_design/flexural_member_design_test_12.py
python tests/materials/astm_aisc/member_design/flexural_member_design_test_13.py
python tests/materials/astm_aisc/member_design/compression_member_design_test_01.py
python tests/materials/astm_aisc/member_design/compression_member_design_test_02.py
python tests/materials/astm_aisc/member_design/compression_member_design_test_03.py
python tests/materials/astm_aisc/member_design/compression_member_design_test_04.py
python tests/materials/astm_aisc/member_design/shear_member_design_test_01.py
python tests/materials/astm_aisc/member_design/shear_member_design_test_02.py
python tests/materials/astm_aisc/member_design/shear_member_design_test_03.py
python tests/materials/astm_aisc/member_design/shear_member_design_test_04.py
python tests/materials/astm_aisc/member_design/shear_member_design_test_05.py
python tests/materials/astm_aisc/member_design/shear_member_design_test_06.py
python tests/materials/astm_aisc/member_design/shear_member_design_test_07.py

echo "$BLEU" "      ASTM/AISC connection design tests." "$NORMAL"
python tests/materials/astm_aisc/connection_design/anchor_bolt_design_test_01.py
python tests/materials/astm_aisc/connection_design/bolt_fastener_design_test_01.py
python tests/materials/astm_aisc/connection_design/bolt_fastener_design_test_02.py
python tests/materials/astm_aisc/connection_design/bolted_plate_test_01.py
python tests/materials/astm_aisc/connection_design/bolted_flange_plate_connection_01.py
python tests/materials/astm_aisc/connection_design/bolted_flange_plate_connection_02.py
python tests/materials/astm_aisc/connection_design/bolted_web_shear_tab_connection_01.py
python tests/materials/astm_aisc/connection_design/single_plate_connection_01.py
python tests/materials/astm_aisc/connection_design/single_plate_connection_02.py
python tests/materials/astm_aisc/connection_design/test_end_plate_connection_01.py
python tests/materials/astm_aisc/connection_design/test_end_tee_connection_01.py
python tests/materials/astm_aisc/connection_design/test_weld_01.py
python tests/materials/astm_aisc/connection_design/test_weld_02.py

echo "$BLEU" "      Tie bar tests." "$NORMAL"
python tests/materials/tie_bars/test_tie_bars_01.py

echo "$BLEU" "  Wood sections tests." "$NORMAL"
echo "$BLEU" "    AWC NDS tests." "$NORMAL"
python tests/materials/awc_nds/test_nds_table_4A.py
python tests/materials/awc_nds/awc_nds2018_test01.py
python tests/materials/awc_nds/rsdg_exemple_5_1.py
python tests/materials/awc_nds/plywood_sheating_test.py
python tests/materials/awc_nds/southern_pine_joist_test.py
python tests/materials/awc_nds/i-joist_bending_and_shear_test.py
python tests/materials/awc_nds/douglas-fir-larch_rafter_test.py
python tests/materials/awc_nds/withdrawal_design_value_test_01.py
python tests/materials/awc_nds/withdrawal_design_value_test_02.py
python tests/materials/awc_nds/lateral_design_value_test_01.py
python tests/materials/awc_nds/lateral_design_value_test_02.py
python tests/materials/awc_nds/test_beam_stability_factor.py
python tests/materials/awc_nds/test_linear_density_01.py
python tests/materials/awc_nds/lvl_beam_test_01.py
python tests/materials/awc_nds/test_load_comb_duration_factor.py
python tests/materials/awc_nds/test_bending_and_axial_tension.py
python tests/materials/awc_nds/test_bending_and_axial_compression.py
python tests/materials/awc_nds/test_bi-axial_bending_and_axial_compression.py
python tests/materials/awc_nds/test_axial_compression.py
python tests/materials/awc_nds/test_built-up_column.py
python tests/materials/awc_nds/stud_arrangement_test.py
python tests/materials/awc_nds/wall_top_plates_test.py

echo "$BLEU" "    EC5 tests." "$NORMAL"
python tests/materials/ec5/test_EC5Wood_01.py
python tests/materials/ec5/test_EC5Wood_02.py
python tests/materials/ec5/test_EC5Wood_03.py
python tests/materials/ec5/test_EC5Wood_04.py
python tests/materials/ec5/test_critical_stress.py
python tests/materials/ec5/timber_beam_test_01.py

echo "$BLEU" "  Prestressing." "$NORMAL"
python tests/materials/prestressing/test_layout2D_frictLoss_prestress_tendon.py
python tests/materials/prestressing/test_layout3D_frictLoss_prestress_tendon.py
python tests/materials/prestressing/test_anchorageLoss_prestress_tendon_01.py
python tests/materials/prestressing/test_anchorageLoss_prestress_tendon_02.py
python tests/materials/prestressing/test_anchorageLoss_prestress_tendon_03.py
python tests/materials/prestressing/test_anchorageLoss_prestress_tendon_04.py
python tests/materials/prestressing/test_loss_prestress_units_01.py
python tests/materials/prestressing/test_deflection_prestressed_beam_01.py
python tests/materials/prestressing/test_short_term_loss_prestress_01.py
python tests/materials/prestressing/test_short_term_loss_prestress_02.py
python tests/materials/prestressing/test_strand_data_01.py
python tests/materials/prestressing/test_straight_pre_tension_strand.py
python tests/materials/prestressing/test_prestressing_01.py
python tests/materials/prestressing/test_prestressing_02.py
python tests/materials/prestressing/test_prestressing_03.py
python tests/materials/prestressing/test_prestressing_04.py
python tests/materials/prestressing/test_prestressing_05.py

echo "$BLEU" "  EOTA TR-29." "$NORMAL"
python tests/materials/eota_tr_29/test_anchor_bolt01.py
python tests/materials/eota_tr_29/test_shear_anchor.py

echo "$BLEU" "  Other materials tests." "$NORMAL"
python tests/materials/test_elastomeric_bearing_stiffness.py

#Database tests
echo "$BLEU" "Database tests (MySQL, Berkeley db, sqlite,...)." "$NORMAL"
# XXX sqlitepp is toooooo slooooow. Seek for another solution.
# python tests/database/test_database_01.py
# python tests/database/test_database_02.py
python tests/database/test_database_03.py
# python tests/database/test_database_04.py
# python tests/database/test_database_05.py
# python tests/database/test_database_06.py
# python tests/database/test_database_07.py
# XXX
python tests/database/test_database_08.py
python tests/database/test_database_09.py
python tests/database/test_database_10.py
python tests/database/test_database_11.py
python tests/database/test_database_12.py
python tests/database/test_database_13.py
python tests/database/test_database_14.py
python tests/database/test_database_15.py
python tests/database/test_database_16.py
python tests/database/sqlite_test_01.py
python tests/database/sqlite_test_02.py
python tests/database/sqlite_test_03.py
python tests/database/readln_test_01.py

echo "$BLEU" "Verifiyng misc. utilities." "$NORMAL"
python tests/utility/rcond_01.py
python tests/utility/rcond_02.py
python tests/utility/rcond_03.py
python tests/utility/import_combinations.py
python tests/utility/test_suitable_xzvector.py

echo "$BLEU" "Verifiying routines for rough calculations,..." "$NORMAL"
python tests/rough_calculations/test_min_dim_abut_support.py
python tests/rough_calculations/test_earth_pressure_kray.py
python tests/rough_calculations/test_suspension_bridge_simple_model.py
python tests/rough_calculations/test_cable_stayed_bridge_simple_model.py
python tests/rough_calculations/test_arch_bridge_simple_model.py
python tests/rough_calculations/test_tied_arch_simple_model.py
python tests/rough_calculations/test_footing_near_slope01.py
python tests/rough_calculations/masonry_bridge_verif_01.py
python tests/rough_calculations/masonryBridgeVerif_test01_minim.py
python tests/rough_calculations/test_predim_diaphragm_wall.py
python tests/rough_calculations/test_dome.py
python tests/rough_calculations/test_dome_02.py
python tests/rough_calculations/test_hypar.py
python tests/rough_calculations/test_base_plate_design.py
python tests/rough_calculations/test_simple_beam_oscillation.py
echo "$BLEU" "  Earth retaining structures..." "$NORMAL"
python tests/rough_calculations/earth_retaining/retaining_wall_test_01.py
python tests/rough_calculations/earth_retaining/sheet_pile_example_01.py
python tests/rough_calculations/earth_retaining/sheet_pile_example_02.py
python tests/rough_calculations/earth_retaining/sheet_pile_example_03.py
python tests/rough_calculations/earth_retaining/sheet_pile_tricked_example_01.py
python tests/rough_calculations/earth_retaining/sheet_pile_tricked_example_02.py
python tests/rough_calculations/earth_retaining/pile_wall_displacement_estimation.py
echo "$BLEU" "  Beam formulas..." "$NORMAL"
python tests/rough_calculations/beam_formulas/test_cantilever01.py
python tests/rough_calculations/beam_formulas/test_cantilever02.py
python tests/rough_calculations/beam_formulas/test_simple_beam_01.py
python tests/rough_calculations/beam_formulas/test_simple_beam_02.py
python tests/rough_calculations/beam_formulas/test_simple_beam_03.py
python tests/rough_calculations/beam_formulas/test_simple_beam_04.py
python tests/rough_calculations/beam_formulas/beam_deflection_mohr_01.py
python tests/rough_calculations/beam_formulas/beam_deflection_mohr_02.py
python tests/rough_calculations/beam_formulas/beam_deflection_mohr_03.py
python tests/rough_calculations/beam_formulas/beam_deflection_mohr_04.py
python tests/rough_calculations/beam_formulas/beam_deflection_mohr_05.py
python tests/rough_calculations/beam_formulas/beam_deflection_mohr_06.py
echo "$BLEU" "  Reinforced concrete rough calculations,..." "$NORMAL"
python tests/rough_calculations/reinforced_concrete/test_punzo01.py
python tests/rough_calculations/reinforced_concrete/test_punzo02.py
python tests/rough_calculations/reinforced_concrete/ng_rc_section_test_sia262.py
python tests/rough_calculations/reinforced_concrete/ng_rc_section_test_ehe.py
python tests/rough_calculations/reinforced_concrete/ng_rc_section_test_ec2.py
python tests/rough_calculations/reinforced_concrete/ng_pile_cap_test.py

#Postprocess tests
echo "$BLEU" "Verifiying routines for post processing." "$NORMAL"
python tests/postprocess/test_export_shell_internal_forces.py
echo "$BLEU" "  limit state checking." "$NORMAL"
echo "$BLEU" "    SIA 262 limit state checking." "$NORMAL"
python tests/postprocess/limit_state_checking/sia262/test_shell_normal_stresses_uls_checking.py
echo "$BLEU" "    ACI limit state checking." "$NORMAL"
python tests/postprocess/limit_state_checking/aci/test_shear_uls_checking_aci.py
echo "$BLEU" "    EHE limit state checking." "$NORMAL"
python tests/postprocess/limit_state_checking/ehe08/test_shear_uls_checking_01.py
python tests/postprocess/limit_state_checking/ehe08/test_shear_uls_checking_02.py
python tests/postprocess/limit_state_checking/ehe08/test_shear_uls_checking_03.py
python tests/postprocess/limit_state_checking/ehe08/test_shear_uls_checking_04.py
python tests/postprocess/limit_state_checking/ehe08/test_shear_uls_checking_05.py
python tests/postprocess/limit_state_checking/ehe08/test_shell_normal_stresses_uls_checking.py
echo "$BLEU" "    AISC limit state checking." "$NORMAL"
python tests/postprocess/limit_state_checking/aisc/test_uls_checking_aisc_01.py
python tests/postprocess/limit_state_checking/aisc/test_uls_checking_aisc_02.py
python tests/postprocess/limit_state_checking/aisc/test_uls_checking_aisc_03.py
python tests/postprocess/limit_state_checking/aisc/test_uls_checking_aisc_04.py
python tests/postprocess/limit_state_checking/aisc/test_uls_checking_aisc_05.py
python tests/postprocess/limit_state_checking/aisc/test_uls_checking_aisc_06.py
echo "$BLEU" "    NBE EA-95 limit state checking." "$NORMAL"
python tests/postprocess/limit_state_checking/nbe_ea95/test_uls_checking_von_mises.py
echo "$BLEU" "    AWC/NDS limit state checking." "$NORMAL"
python tests/postprocess/limit_state_checking/awc_nds/test_uls_checking_nds_01.py
python tests/postprocess/limit_state_checking/awc_nds/test_uls_checking_nds_02.py
python tests/postprocess/limit_state_checking/awc_nds/test_uls_checking_nds_03.py

echo "$BLEU" "    serviceability limit state checking." "$NORMAL"
python tests/postprocess/limit_state_checking/serviceability_limit_states/test_ibc2018_deflection.py

# Graphic output tests
##python tests/postprocess/vtk/dibuja_edges.py
echo "$BLEU" "  Graphic output." "$NORMAL"
python tests/postprocess/vtk/test_beam3d_bending_moment_belly_01.py
python tests/postprocess/vtk/test_beam3d_bending_moment_belly_02.py

END=$(date +%s.%N)
DIFF=$(echo "$END - $START" | bc)
echo $DIFF seconds
NT=$(grep -c '^python' $0)
echo ${NT} tests
Q=$(echo "$DIFF / $NT" | bc -l)
echo $Q seconds/test

# Restore MPLBACKEND to its previous value.
mplbackend_backup="nil"
# Trying to avoid Matplotlib complaining about the XServer
if [ "$mplbackend_backup"=="nil" ]; then
    echo "$BLEU" "Removing MPLBACKEND from environment variables" "$NORMAL"
    MPLBACKEND=''
else
   echo "$BLEU" "Restoring MPLBACKEND to its previous value." "$NORMAL"
   MPLBACKEND=mplbackend_backup
fi
export MPLBACKEND

# Clean garbage if any

rm -f -r ./annex
rm -f -r ./tmp
