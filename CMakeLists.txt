cmake_minimum_required(VERSION 2.8)
 
PROJECT(KunVTK)

set( KVS_DIR "/Users/breeze/local/kvs" )
include_directories( ${KVS_DIR}/include )
link_directories( ${KVS_DIR}/lib )
 
find_package(VTK COMPONENTS
  vtkFiltersSources
  vtkInteractionStyle
  vtkRenderingOpenGL
  vtkRenderingVolumeOpenGL
  vtkCommonCore
  vtkInteractionWidgets
  vtkRenderingFreeTypeOpenGL
  )

find_package(OpenGL)
find_package(GLEW)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${GLEW_INCLUDE_DIRS}")

include(${VTK_USE_FILE})

# Build library
file(GLOB_RECURSE SOURCE "${PROJECT_SOURCE_DIR}/lib/*.cpp")
add_library(util ${SOURCE}) 

# Build Examples
include_directories(${PROJECT_SOURCE_DIR}/lib)
set(EXAMPLES "${PROJECT_SOURCE_DIR}/EXAMPLES")
file(GLOB_RECURSE RayCastingMapperFile "${EXAMPLES}/VolumeRendering/RayCastingMapper/*.cxx")
add_executable(RayCastingMapper ${RayCastingMapperFile})
target_link_libraries(RayCastingMapper ${VTK_LIBRARIES} util kvsCore vtkRenderingVolume vtkIOXML vtkIOLegacy )

file(GLOB_RECURSE HAVSVolumeMapperFile "${EXAMPLES}/VolumeRendering/HAVSVolumeMapper/*.cxx")
add_executable(HAVSVolumeMapper ${HAVSVolumeMapperFile})
target_link_libraries(HAVSVolumeMapper ${VTK_LIBRARIES} vtkRenderingVolume vtkIOLegacy vtkRenderingFreeType)

file(GLOB_RECURSE DistancePointToLineFile "${EXAMPLES}/SimpleOperations/DistancePointToLine/*.cxx")
add_executable(DistancePointToLine ${DistancePointToLineFile})
target_link_libraries(DistancePointToLine ${VTK_LIBRARIES})

file(GLOB_RECURSE ReadStructuredGridFile "${EXAMPLES}/IO/ReadStructuredGrid/*.cxx")
add_executable(ReadStructuredGrid ${ReadStructuredGridFile})
target_link_libraries(ReadStructuredGrid ${VTK_LIBRARIES} vtkRenderingVolume vtkIOXML)

file(GLOB_RECURSE ReadUnstructuredGridFile "${EXAMPLES}/IO/ReadUnstructuredGrid/*.cxx")
add_executable(ReadUnstructuredGrid ${ReadUnstructuredGridFile})
target_link_libraries(ReadUnstructuredGrid ${VTK_LIBRARIES} vtkRenderingVolume vtkIOXML)

# file(GLOB_RECURSE StructuredPointsToUnstructuredGridFile "${EXAMPLES}/IO/StructuredPointsToUnstructuredGrid/*.cxx")
# add_executable(StructuredPointsToUnstructuredGrid ${StructuredPointsToUnstructuredGridFile})
# target_link_libraries(StructuredPointsToUnstructuredGrid ${VTK_LIBRARIES} vtkRenderingVolume vtkIOXML)

file(GLOB_RECURSE UnstructuredRayCastingFile "${EXAMPLES}/VolumeRendering/UnstructuredRayCasting/*.cpp")
add_executable( UnstructuredRayCasting ${UnstructuredRayCastingFile} )
target_link_libraries( UnstructuredRayCasting ${VTK_LIBRARIES} vtkRenderingVolume vtkIOLegacy vtkRenderingFreeType )

file(GLOB_RECURSE IsosurfaceFile "${EXAMPLES}/Isosurface/StructuredVolume/*.cpp")
add_executable( IsosurfaceFile ${IsosurfaceFile} )
target_link_libraries( IsosurfaceFile ${VTK_LIBRARIES} util kvsCore vtkRenderingVolume vtkIOLegacy vtkRenderingFreeType vtkIOXML )


# install(TARGETS util 
# 		ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/lib)
