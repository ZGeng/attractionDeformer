#ifndef ATTRACTIONDEFORMER_H
#define ATTRACTIONDEFORMER_H

#include<maya/MPxDeformerNode.h>
#include<maya/MFnDagNode.h>
#include<maya/MObject.h>
#include<maya/MIntArray.h>
#include<maya/MFloatVectorArray.h>
#include<maya/MVectorArray.h>
#include<maya/MItGeometry.h>
#include<maya/MFnTypedAttribute.h>
#include<maya/MFnNumericAttribute.h>
#include<maya/MFnMesh.h>
#include<maya/MPointArray.h>
#include<maya/MGlobal.h>
#include<maya/MFnEnumAttribute.h>
#include<maya/MFnMatrixAttribute.h>
#include<maya/MDagModifier.h>
#include<maya/MMatrix.h>

class AttractionDeformer : public MPxDeformerNode
{
    public:

	AttractionDeformer();
	virtual ~AttractionDeformer();
	static void* creator();
	virtual MStatus deform(MDataBlock& data,MItGeometry& itGeo,
		                   const MMatrix& localToWorldMatrix,
						   unsigned int geomIndex);

	float shiftDistance(short attractorType,short falloffType, 
		                      MVector pToA, float maxDist,
							  float factor,float maxRepel);
	static MStatus initialize();
	static MTypeId id;

	virtual MObject& accessoryAttribute() const;
	virtual MStatus accessoryNodeSetup(MDagModifier& dagMod);

	static MObject aAttractorMatrix;
	static MObject aAttractorType;
	static MObject aFalloffType;
	static MObject aFactor;
	static MObject aMaxDistance;
	static MObject aMaxRepel;
	static MObject aPinch;
	static MObject aSphereSize;

};




#endif