cmake_minimum_required(VERSION 2.8)
 
PROJECT(HAVSVolumeMapper)
 
find_package(VTK COMPONENTS
	vtkCommonCore
  vtkFiltersSources
  vtkInteractionWidgets
  vtkInteractionStyle
  vtkRenderingOpenGL
  vtkRenderingVolumeOpenGL
  vtkRenderingFreeTypeOpenGL)
include(${VTK_USE_FILE})
 
add_executable(HAVSVolumeMapper MACOSX_BUNDLE HAVSVolumeMapper)
 
if(VTK_LIBRARIES)
  target_link_libraries(HAVSVolumeMapper ${VTK_LIBRARIES} vtkRenderingVolume vtkIOLegacy vtkRenderingFreeType)
else()
  target_link_libraries(HAVSVolumeMapper vtkHybrid vtkWidgets)
endif()