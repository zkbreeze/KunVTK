//
//  KVSMLStructuredVolumeReader.cpp
//
//
//  Created by Kun Zhao on 2014-12-09 12:47:00.
//
//

#include "KVSMLStructuredVolumeReader.h"
#include <vtkInformation.h>

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

vtkStructuredPoints* KVSMLStructuredVolumeReader::OutputVTKStructuredVolume()
{
	vtkStructuredPoints* structuredPoints = vtkStructuredPoints::New();
	structuredPoints->SetDimensions( m_volume->resolution().x(), m_volume->resolution().y(), m_volume->resolution().z() );

	int* dims = structuredPoints->GetDimensions();
	structuredPoints->AllocateScalars( VTK_UNSIGNED_CHAR, 1);
	
	std::cout << "Dims: " << " x: " << dims[0] << " y: " << dims[1] << " z: " << dims[2] << std::endl;
	std::cout << "Number of points: " << structuredPoints->GetNumberOfPoints() << std::endl;
	std::cout << "Number of cells: " << structuredPoints->GetNumberOfCells() << std::endl;

	//fill every entry of the image data with "2.0"
	const unsigned char* pvalue_ori = static_cast<const unsigned char*>( m_volume->values().data() );
	for( int k = 0; k < dims[2]; k++ )
		for( int j = 0; j < dims[1]; j++ )
			for( int i = 0; i < dims[0]; i++ )
			{
				size_t index = i + j * dims[0] + k * dims[0] * dims[1];
				unsigned char* pixel = static_cast<unsigned char*>(structuredPoints->GetScalarPointer(i,j,k));
				pixel[0] = pvalue_ori[index];
			}

	std::cout << "Finish to load the KVSML as the vtkStructuredPoints." << std::endl;
	return structuredPoints;
}

} // end of namespace kun