#include "attractionDeformer.h"
#include <maya/MFnPlugin.h>

MStatus initializePlugin( MObject obj )
{
    MStatus status;

    MFnPlugin fnPlugin( obj, "ZG", "1.0", "Any" );

    status = fnPlugin.registerNode( "attractionDeformer",
        AttractionDeformer::id,
		AttractionDeformer::creator,
		AttractionDeformer::initialize,
        MPxNode::kDeformerNode );
    CHECK_MSTATUS_AND_RETURN_IT( status );


    return MS::kSuccess;
}


MStatus uninitializePlugin( MObject obj )
{
    MStatus status;

    MFnPlugin fnPlugin( obj );

	status = fnPlugin.deregisterNode( AttractionDeformer::id );
    CHECK_MSTATUS_AND_RETURN_IT( status );

    return MS::kSuccess;
}