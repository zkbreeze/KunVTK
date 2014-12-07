cmake_minimum_required(VERSION 2.8)
 
PROJECT(ReadUnstructuredGrid)
 
find_package(VTK COMPONENTS
  vtkFiltersSources
  vtkInteractionStyle
  vtkRenderingOpenGL)
include(${VTK_USE_FILE})
 
add_executable(ReadUnstructuredGrid MACOSX_BUNDLE ReadUnstructuredGrid)
 
if(VTK_LIBRARIES)
  target_link_libraries(ReadUnstructuredGrid ${VTK_LIBRARIES} vtkIOXML)
else()
  target_link_libraries(ReadUnstructuredGrid vtkHybrid vtkWidgets)
endif()