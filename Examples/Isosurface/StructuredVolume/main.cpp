//
//	main.cpp
//
//
//	Created by Kun Zhao on 2014-12-22 18:05:05.
//
//

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleUser.h>
#include <vtkProperty.h>
#include <vtkOutlineFilter.h>
#include <vtkCommand.h>
#include <vtkSliderWidget.h>
#include <vtkSliderRepresentation.h>
#include <vtkSliderRepresentation3D.h>
#include <vtkImageData.h>
#include <vtkCellArray.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkContourFilter.h>
#include <vtkXMLImageDataWriter.h>
#include "KVSTransferFunctionReader.h"
#include <kvs/TransferFunction>
#include <kvs/ColorMap>
#include "KVSMLStructuredVolumeReader.h"
#include <vtkInteractorStyleTrackballCamera.h>
 
class vtkSliderCallback : public vtkCommand
{
public:
	static vtkSliderCallback *New()
	{
		return new vtkSliderCallback;
	}
	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{
		vtkSliderWidget *sliderWidget =
			reinterpret_cast<vtkSliderWidget*>(caller);
		double value = static_cast<vtkSliderRepresentation *>(sliderWidget->GetRepresentation())->GetValue();
		this->ContourFilter->GenerateValues(1, value, value);
 
	}
	vtkSliderCallback():ContourFilter(NULL) {}
	vtkContourFilter* ContourFilter;
};

int main(int argc, char** argv)
{
	// vtkSmartPointer<vtkImageData> data =
	// 	vtkSmartPointer<vtkImageData>::New();
	// CreateData(data);

	kun::KVSMLStructuredVolumeReader* reader = new kun::KVSMLStructuredVolumeReader( argv[1] );
	vtkImageData* data = reader->OutputVTKStructuredVolume();
 
	// Create an isosurface
	vtkSmartPointer<vtkContourFilter> contourFilter =
		vtkSmartPointer<vtkContourFilter>::New();
#if VTK_MAJOR_VERSION <= 5
	contourFilter->SetInput(data);
#else
	contourFilter->SetInputData(data);
#endif
	contourFilter->GenerateValues(1, 10, 10); // (numContours, rangeStart, rangeEnd)
 
	// Map the contours to graphical primitives
	vtkSmartPointer<vtkPolyDataMapper> contourMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	contourMapper->SetInputConnection(contourFilter->GetOutputPort());
 
	// Create an actor for the contours
	vtkSmartPointer<vtkActor> contourActor =
		vtkSmartPointer<vtkActor>::New();
	contourActor->SetMapper(contourMapper);
 
	// Create the outline
	vtkSmartPointer<vtkOutlineFilter> outlineFilter =
		vtkSmartPointer<vtkOutlineFilter>::New();
#if VTK_MAJOR_VERSION <= 5
	outlineFilter->SetInput(data);
#else
	outlineFilter->SetInputData(data);
#endif
	vtkSmartPointer<vtkPolyDataMapper> outlineMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	outlineMapper->SetInputConnection(outlineFilter->GetOutputPort());
	vtkSmartPointer<vtkActor> outlineActor =
		vtkSmartPointer<vtkActor>::New();
	outlineActor->SetMapper(outlineMapper);
 
	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	// vtkSmartPointer<vtkRenderWindowInteractor> interactor =
	// 	vtkSmartPointer<vtkRenderWindowInteractor>::New();
	// interactor->SetRenderWindow(renderWindow);

	vtkRenderWindowInteractor* interactor = vtkRenderWindowInteractor::New();
	interactor->SetRenderWindow( renderWindow );
	vtkInteractorStyleTrackballCamera* style = vtkInteractorStyleTrackballCamera::New();
	interactor->SetInteractorStyle( style );
 
	renderer->AddActor(contourActor);
	renderer->AddActor(outlineActor);
 
	vtkSmartPointer<vtkSliderRepresentation3D> sliderRep =
		vtkSmartPointer<vtkSliderRepresentation3D>::New();
	sliderRep->SetMinimumValue( data->GetScalarRange()[0] );
	sliderRep->SetMaximumValue( data->GetScalarRange()[1] );
	sliderRep->SetValue( (data->GetScalarRange()[1] - data->GetScalarRange()[0]) / 2.0 );
	sliderRep->SetTitleText("Contour value");
	sliderRep->SetPoint1InWorldCoordinates(0, -100, 0);
	sliderRep->SetPoint2InWorldCoordinates(300, -100, 0);
	sliderRep->SetSliderWidth(.1);
	// sliderRep->SetSliderLength(.2);
	sliderRep->SetLabelHeight(.1);
 
	vtkSmartPointer<vtkSliderWidget> sliderWidget =
		vtkSmartPointer<vtkSliderWidget>::New();
	sliderWidget->SetInteractor(interactor);
	sliderWidget->SetRepresentation(sliderRep);
	sliderWidget->SetAnimationModeToAnimate();
	sliderWidget->EnabledOn();
 
	vtkSmartPointer<vtkSliderCallback> callback =
		vtkSmartPointer<vtkSliderCallback>::New();
	callback->ContourFilter = contourFilter;
 
	sliderWidget->AddObserver(vtkCommand::InteractionEvent,callback);
 
	renderWindow->SetSize(500,500);
	renderWindow->Render();
	renderWindow->Render();
	interactor->Start();
 
	return EXIT_SUCCESS;
}