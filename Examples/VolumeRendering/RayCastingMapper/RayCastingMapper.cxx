//
//  RayCastingMapper.cxx
//
//
//  Created by Kun Zhao on 2014-12-07 17:09:13.
//
//

#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkXMLStructuredGridReader.h>
#include <vtkStructuredPointsReader.h>
#include <vtkStructuredGridGeometryFilter.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include "KVSMLStructuredVolumeReader.h"

int main( int argc, char** argv )
{
    vtkRenderer* ren = vtkRenderer::New();
    vtkRenderWindow* renWin = vtkRenderWindow::New();
    renWin->AddRenderer( ren );

    // Interactor
    vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
    iren->SetRenderWindow( renWin );
    vtkInteractorStyleTrackballCamera* style = vtkInteractorStyleTrackballCamera::New();
    iren->SetInteractorStyle( style );

    // Reader
    // vtkStructuredPointsReader* reader = vtkStructuredPointsReader::New();
    // reader->SetFileName( argv[1] );
    // reader->Update();
    kun::KVSMLStructuredVolumeReader* reader = new kun::KVSMLStructuredVolumeReader( argv[1] );
    vtkStructuredPoints* points = reader->OutputVTKStructuredVolume();

    // Transfer function: opacity map and color map
    vtkPiecewiseFunction* opacityTransferFunction = vtkPiecewiseFunction::New();
    opacityTransferFunction->AddPoint(80, 0.0);
    opacityTransferFunction->AddPoint(120, 0.2);
    opacityTransferFunction->AddPoint(255, 0.2);
    vtkColorTransferFunction* colorTransferFunction = vtkColorTransferFunction::New();
    colorTransferFunction->AddRGBPoint(80.0,  0.0, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(120.0, 0.0, 0.0, 1.0);
    colorTransferFunction->AddRGBPoint(160.0, 1.0, 0.0, 0.0);
    colorTransferFunction->AddRGBPoint(200.0, 0.0, 1.0, 0.0);
    colorTransferFunction->AddRGBPoint(255.0, 0.0, 1.0, 1.0);

    // Set the volume property
    vtkVolumeProperty* volumeProperty = vtkVolumeProperty::New();
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opacityTransferFunction);
    volumeProperty->ShadeOn();
    volumeProperty->SetInterpolationTypeToLinear();

    // Ray casting mapper
    vtkVolumeRayCastCompositeFunction* compositeFunction = vtkVolumeRayCastCompositeFunction::New();
    vtkVolumeRayCastMapper* volumeMapper = vtkVolumeRayCastMapper::New();
    volumeMapper->SetVolumeRayCastFunction(compositeFunction);
    volumeMapper->SetInputData( points );

    // Set the volume
    vtkVolume* volume = vtkVolume::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);

    ren->AddVolume( volume );
    ren->SetBackground( 1, 1, 1);
    renWin->SetSize(600, 600);
    renWin->Render();
    iren->Start();

    return EXIT_SUCCESS;
}
