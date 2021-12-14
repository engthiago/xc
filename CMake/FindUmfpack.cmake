# ----------------------------------------------------------------------------
#   XC program; finite element analysis code
#   for structural analysis and design.
# 
#   Copyright (C)  Luis C. Pérez Tato
# 
#   Except for the restrictions that may arise from the copyright
#   of the original program (see copyright below if any)
#   XC is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or 
#   (at your option) any later version.
# 
#   This software is distributed in the hope that it will be useful, but 
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details. 
# 
# 
#  You should have received a copy of the GNU General Public License 
#  along with this program.
#  If not, see <http:// www.gnu.org/licenses/>.
# ----------------------------------------------------------------------------

FIND_LIBRARY(UMFPACK_LIB NAMES umfpack)

SET(UMFPACK_FOUND FALSE)
IF(UMFPACK_LIB)
    SET(UMFPACK_FOUND TRUE)
ENDIF (UMFPACK_LIB)

IF(UMFPACK_FOUND)
    MESSAGE(STATUS "Found Umfpack: ${UMFPACK_LIB}")
ELSE(UMFPACK_FOUND)
    MESSAGE(FATAL_ERROR "Could not find Umfpack")
ENDIF (UMFPACK_FOUND)



