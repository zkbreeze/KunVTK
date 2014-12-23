//
//  KVSMLStructuredVolumeReader.h
//
//
//  Created by Kun Zhao on 2014-12-09 12:42:46.
//
//

#ifndef KUN__KVSMLSTRUCTUREDVOLUMEREADER_H_INCLUDE
#define KUN__KVSMLSTRUCTUREDVOLUMEREADER_H_INCLUDE

#include <kvs/StructuredVolumeImporter>
#include <kvs/StructuredVolumeObject> 
#include <vtkImageData.h>
#include <vtkStructuredGrid.h>
#include <vtkPoints.h>
#include <vtkFloatArray.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>

namespace kun
{

class KVSMLStructuredVolumeReader
{

	const kvs::StructuredVolumeObject* m_volume;

public:
	KVSMLStructuredVolumeReader(){}
	KVSMLStructuredVolumeReader( std::string filename );

	void Read( std::string filename );
	void SetKVSStructuredVolume( kvs::StructuredVolumeObject* object ){ m_volume = object; }
	vtkImageData* OutputVTKStructuredVolume();
	vtkStructuredGrid* OutputVTKStructuredVolumeGrid();
};


} // end of namespace kun

 
#endif // KUN__KVSMLSTRUCTUREDVOLUMEREADER_H_INCLUDE