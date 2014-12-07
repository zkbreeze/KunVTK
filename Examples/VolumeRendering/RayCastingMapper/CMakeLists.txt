cmake_minimum_required(VERSION 2.8)
 
PROJECT(RayCastingMapper)
 
find_package(VTK COMPONENTS
  vtkFiltersSources
  vtkInteractionStyle
  vtkRenderingOpenGL
  vtkRenderingVolumeOpenGL
  )
include(${VTK_USE_FILE})
 
add_executable(RayCastingMapper MACOSX_BUNDLE RayCastingMapper)
 
if(VTK_LIBRARIES)
  target_link_libraries(RayCastingMapper ${VTK_LIBRARIES} vtkRenderingVolume vtkIOXML vtkIOLegacy)
else()
  target_link_libraries(RayCastingMapper vtkHybrid vtkWidgets)
endif()
