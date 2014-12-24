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
#include <vtkCamera.h>
#include <vtkLight.h>
#include <vtkMatrix4x4.h>
#include "KVSView.h"
#include <kvs/Timer>
#include <vtkCommand.h>

namespace
{
    kvs::Timer rendering_time;         
}

// Callback for the interaction
class FPS : public vtkCommand
{
public:
  static FPS *New()
    { return new FPS; }
  virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
        rendering_time.stop();
        std::cout << "FPS: " << rendering_time.fps() << std::endl;
        rendering_time.start();
    }
};

int main( int argc, char** argv )
{
    vtkRenderer* ren = vtkRenderer::New();
    // vtkCamera* camera = vtkCamera::New();
    // camera->SetParallelScale( 2.0 );
    // camera->SetPosition( 192, 174, 768 );
    // camera->SetFocalPoint( 192, 174, 38.5 );
    // camera->SetViewUp( 0, 1, 0 );
    // camera->SetViewAngle( 45 );
    // ren->SetActiveCamera( camera );

    vtkLight* light = vtkLight::New();
    light->SetPosition( 150, 150, 500 );
    light->SetFocalPoint( 150, 150, 0 );
    ren->AddLight( light );

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
    vtkImageData* data = reader->OutputVTKStructuredVolume();
    // vtkStructuredGrid* data = reader->OutputVTKStructuredVolumeGrid();

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
    tfunc->SetRange( data->GetScalarRange() );
    vtkPiecewiseFunction* opacityTransferFunction = tfunc->vtkOpacityMap();
    vtkColorTransferFunction* colorTransferFunction = tfunc->vtkColorMap();

    // Set the volume property
    vtkVolumeProperty* volumeProperty = vtkVolumeProperty::New();
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opacityTransferFunction);
    // volumeProperty->ShadeOn();
    // volumeProperty->SetDiffuse(0.7); 
    // volumeProperty->SetAmbient(0.1); 
    // volumeProperty->SetSpecular(0.5); 
    // volumeProperty->SetSpecularPower(70.0);
    volumeProperty->SetInterpolationTypeToLinear();

    // Ray casting mapper
    // vtkVolumeRayCastCompositeFunction* compositeFunction = vtkVolumeRayCastCompositeFunction::New();
    vtkGPUVolumeRayCastMapper* volumeMapper = vtkGPUVolumeRayCastMapper::New();
    // volumeMapper->SetVolumeRayCastFunction(compositeFunction);
    volumeMapper->SetInputData( data );



    vtkVolume* volume = vtkVolume::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);

    int* dims = data->GetDimensions();
    volume->SetPosition( -dims[0] * 0.5, -dims[1] * 0.5, -dims[2] * 0.5 );
    // volume->RotateX( 100 );
    // volume->RotateY( 100 );
    // volume->RotateZ( 100 );
    vtkMatrix4x4* matrixVolume = volume->GetMatrix();
    matrixVolume->Print( std::cout );

    ren->AddVolume( volume );
    ren->SetBackground( 1, 1, 1);
    renWin->SetSize( 512,  512);
    // Set the volume
    kun::KVSView* view = new kun::KVSView( 1.0 );
    view->SetVolume( data );
    ren->SetActiveCamera( view->ToVTKCamera() );
    vtkMatrix4x4* matrix = ren->GetActiveCamera()->GetViewTransformMatrix();
    matrix->Print( std::cout );

    FPS* fps = FPS::New();
    ren->AddObserver( vtkCommand::StartEvent, fps);
    rendering_time.start();
    renWin->Render();
    iren->Start();

    return EXIT_SUCCESS;
}
