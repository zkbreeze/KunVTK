//
//  KVSTransferFunctionReader.h
//
//
//  Created by Kun Zhao on 2014-12-17 15:08:38.
//
//

// This is to convert the KVS transfer function to VTK format

#ifndef KUN__KVSTRANSFERFUNCTIONREADER_H_INCLUDE
#define KUN__KVSTRANSFERFUNCTIONREADER_H_INCLUDE

#include <kvs/TransferFunction>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>

namespace kun
{

class KVSTransferFunctionReader
{

	kvs::TransferFunction m_tfunc;
	double m_min;
	double m_max;

public:
	KVSTransferFunctionReader();
	KVSTransferFunctionReader( std::string filename );
	KVSTransferFunctionReader( kvs::TransferFunction tfunc );

	// This is must
	void SetRange( double* range );

	void Read( std::string filename );
	void SetKVSTransferFunction( kvs::TransferFunction tfunc );
	vtkColorTransferFunction* vtkColorMap();
	vtkPiecewiseFunction* vtkOpacityMap();

};


} // end of namespace kun

 
#endif // KUN__KVSTRANSFERFUNCTIONREADER_H_INCLUDE