//
//  KVSMLStructuredVolumeReader.cpp
//
//
//  Created by Kun Zhao on 2014-12-09 12:47:00.
//
//

#include "KVSMLStructuredVolumeReader.h"

namespace kun
{


KVSMLStructuredVolumeReader::KVSMLStructuredVolumeReader( std::string filename )
{
	this->Read( filename );
}

void KVSMLStructuredVolumeReader::Read( std::string filename )
{
	m_volume = new kvs::StructuredVolumeImporter( filename );
}

vtkImageData* KVSMLStructuredVolumeReader::OutputVTKStructuredVolume()
{
	vtkImageData* volume = vtkImageData::New();
	volume->SetDimensions( m_volume->resolution().x(), m_volume->resolution().y(), m_volume->resolution().z() );

	int* dims = volume->GetDimensions();
	volume->AllocateScalars( VTK_FLOAT, 1);
	
	//fill every entry of the structured point data with the KVSML data.
	const int length = volume->GetNumberOfPoints();
	const float* pvalue_ori = static_cast<const float*>( m_volume->values().data() );
	float* buf = static_cast<float*>(volume->GetScalarPointer());
	memcpy( buf, pvalue_ori, length * sizeof( float ) );

	double* range = volume->GetScalarRange();
		
	std::cout << "Finish to load the KVSML as the vtkImageData." << std::endl;
	std::cout << "Dims: " << " x: " << dims[0] << " y: " << dims[1] << " z: " << dims[2] << std::endl;
	std::cout << "Number of points: " << volume->GetNumberOfPoints() << std::endl;
	std::cout << "Number of cells: " << volume->GetNumberOfCells() << std::endl;
	std::cout << "Min value: " << range[0] << std::endl;
	std::cout << "Max value: " << range[1] << std::endl;

	delete m_volume;
	return volume;
}

vtkStructuredGrid* KVSMLStructuredVolumeReader::OutputVTKStructuredVolumeGrid()
{
	vtkStructuredGrid* structuredGrid = vtkStructuredGrid::New();
	structuredGrid->SetDimensions( m_volume->resolution().x(), m_volume->resolution().y(), m_volume->resolution().z() );

	int* dims = structuredGrid->GetDimensions();

	vtkFloatArray* data = vtkFloatArray::New();
	data->SetNumberOfComponents(1);

	vtkPoints* points = vtkPoints::New();
	points->SetNumberOfPoints( m_volume->numberOfNodes() );

	const float* pvalue_ori = static_cast<const float*>( m_volume->values().data() );
	for( int k = 0; k < dims[2]; k++ )
		for( int j = 0; j < dims[1]; j++ )
			for( int i = 0; i < dims[0]; i++ )
			{
				size_t index = i + j * dims[0] + k * dims[0] * dims[1];
				float* p = new float[3];
				p[0] = i;
				p[1] = j;
				p[2] = k;
				points->InsertNextPoint( p );
				data->InsertNextValue( pvalue_ori[index] );				
			}

	structuredGrid->SetPoints( points );
	structuredGrid->GetPointData()->SetScalars( data );

	double* range = structuredGrid->GetScalarRange();

	std::cout << "Finish to load the KVSML as the vtkStructuredGrid." << std::endl;
	std::cout << "Dims: " << " x: " << dims[0] << " y: " << dims[1] << " z: " << dims[2] << std::endl;
	std::cout << "Number of points: " << structuredGrid->GetNumberOfPoints() << std::endl;
	std::cout << "Number of cells: " << structuredGrid->GetNumberOfCells() << std::endl;
	std::cout << "Min value: " << range[0] << std::endl;
	std::cout << "Max value: " << range[1] << std::endl;
	return structuredGrid;
}

} // end of namespace kun