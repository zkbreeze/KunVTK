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
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include "KVSMLStructuredVolumeReader.h"
#include <vtkStructuredGrid.h>
#include <vtkLookUpTable.h>
#include "KVSTransferFunctionReader.h"
#include <kvs/TransferFunction>
#include <kvs/ColorMap>

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
    vtkImageData* points = reader->OutputVTKStructuredVolume();
    // vtkStructuredGrid* points = reader->OutputVTKStructuredVolumeGrid();

    // Transfer function: opacity map and color map
    // vtkPiecewiseFunction* opacityTransferFunction = vtkPiecewiseFunction::New();
    // opacityTransferFunction->AddPoint(0, 0.3);
    // opacityTransferFunction->AddPoint(80, 0.3);
    // opacityTransferFunction->AddPoint(120, 0.2);
    // opacityTransferFunction->AddPoint(255, 0.2);
    // vtkColorTransferFunction* colorTransferFunction = vtkColorTransferFunction::New();
    // colorTransferFunction->AddHSVPoint(31.0, 0.7, 1.0, 1.0);
    // colorTransferFunction->AddHSVPoint(32.0, 0.5, 1.0, 1.0);
    // colorTransferFunction->AddHSVPoint(33.0, 0.3, 1.0, 1.0);
    // colorTransferFunction->AddHSVPoint(34.0, 0.1, 1.0, 1.0);
    // colorTransferFunction->AddHSVPoint(35.0, 0.0, 1.0, 1.0);

    // Set the transfer function with KVS transfer function
    kvs::TransferFunction t( 256 );
    kun::KVSTransferFunctionReader* tfunc = new kun::KVSTransferFunctionReader( t );
    tfunc->SetRange( points->GetScalarRange() );
    vtkPiecewiseFunction* opacityTransferFunction = tfunc->vtkOpacityMap();
    vtkColorTransferFunction* colorTransferFunction = tfunc->vtkColorMap();

    // Set the volume property
    vtkVolumeProperty* volumeProperty = vtkVolumeProperty::New();
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opacityTransferFunction);
    // volumeProperty->ShadeOn();
    // volumeProperty->SetDiffuse(0.2); 
    // volumeProperty->SetAmbient(0.1); 
    // volumeProperty->SetSpecular(0.5); 
    // volumeProperty->SetSpecularPower(10.0);
    volumeProperty->SetInterpolationTypeToLinear();

    // Ray casting mapper
    // vtkVolumeRayCastCompositeFunction* compositeFunction = vtkVolumeRayCastCompositeFunction::New();
    vtkGPUVolumeRayCastMapper* volumeMapper = vtkGPUVolumeRayCastMapper::New();
    // volumeMapper->SetVolumeRayCastFunction(compositeFunction);
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
