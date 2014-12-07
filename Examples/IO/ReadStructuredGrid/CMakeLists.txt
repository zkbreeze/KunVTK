cmake_minimum_required(VERSION 2.8)
 
PROJECT(ReadStructuredGrid)
 
find_package(VTK COMPONENTS
  vtkFiltersSources
  vtkInteractionStyle
  vtkRenderingOpenGL)
include(${VTK_USE_FILE})
 
add_executable(ReadStructuredGrid MACOSX_BUNDLE ReadStructuredGrid)
 
if(VTK_LIBRARIES)
  target_link_libraries(ReadStructuredGrid ${VTK_LIBRARIES} vtkRenderingVolume vtkIOXML)
else()
  target_link_libraries(ReadStructuredGrid vtkHybrid vtkWidgets)
endif()