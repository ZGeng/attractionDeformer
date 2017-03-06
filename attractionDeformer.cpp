
/* 2016 ZhenGeng - Attraction Deformer 
* You may use, distribute and modify this code for education or 
* learning purpose.
*/

#include "attractionDeformer.h"
MTypeId    AttractionDeformer::id( 0x00000233 );

MObject AttractionDeformer::aAttractorMatrix;
MObject AttractionDeformer::aAttractorType;
MObject AttractionDeformer::aFalloffType;
MObject AttractionDeformer::aFactor;
MObject AttractionDeformer::aPinch;
MObject AttractionDeformer::aMaxDistance;
MObject AttractionDeformer::aMaxRepel;

MObject AttractionDeformer::aSphereSize;



AttractionDeformer::AttractionDeformer()
{

}

AttractionDeformer::~AttractionDeformer()
{

}

void* AttractionDeformer::creator()
{
  return new AttractionDeformer();
}

MObject& AttractionDeformer::accessoryAttribute() const
{
	return aAttractorMatrix;
}


MStatus AttractionDeformer::accessoryNodeSetup(MDagModifier& dagMod)
{
	MStatus status;

	//create a sphere to make the locator easier to select 
	MObject oSphereTrans = dagMod.createNode(MString("transform"), MObject::kNullObj, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MFnDagNode fnSphereTrans(oSphereTrans);
	fnSphereTrans.setName("attractor", false, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MObject oSphereShape = dagMod.createNode(MString("mesh"), oSphereTrans, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	//create the sphere data node 
	MFnDependencyNode fnSphere;
	MObject	oSphere = fnSphere.create("polySphere");
	
	MPlug plugSphereData = fnSphere.findPlug("output");
	MPlug plugRadius = fnSphere.findPlug("radius");
	MFnDagNode fnSphereShape(oSphereShape);
	MPlug plugInMesh = fnSphereShape.findPlug("inMesh");
	
	dagMod.connect(plugSphereData, plugInMesh);

	//MFnDependencyNode fnSphere(oSphere, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	//fnSphere.setName("attractor", false, &status);
	//CHECK_MSTATUS_AND_RETURN_IT(status);

    //create the locator and connect it's matrix to the deformer node
	MObject oLocator = dagMod.createNode(MString("locator"), oSphereTrans,&status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MFnDependencyNode fnLoctor(oLocator,&status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	//rename the locator
	fnLoctor.setName("attractorLoc",false,&status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MPlug plugWorldMatrix = fnLoctor.findPlug("worldMatrix",false,&status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	status = plugWorldMatrix.selectAncestorLogicalIndex(0,plugWorldMatrix.attribute());
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MObject oThis = thisMObject();
	MPlug plugAttractorMatrix(oThis,aAttractorMatrix);
	
	status = dagMod.connect(plugWorldMatrix,plugAttractorMatrix);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	
	//create a outputplug to set the attractor sphere size
	MPlug plugSphereSize(oThis, aSphereSize);
	status = dagMod.connect(plugSphereSize, plugRadius);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}

MStatus AttractionDeformer::initialize()
{ 
	MStatus status;
	MFnMatrixAttribute mAttr;
	MFnNumericAttribute nAttr;
	MFnEnumAttribute eAttr;

	aAttractorMatrix = mAttr.create("attractorMatrix","attractorMatrix");
	addAttribute(aAttractorMatrix);
	attributeAffects(aAttractorMatrix,outputGeom);


	aFactor = nAttr.create("attractionFactor","attractionFactor",MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0f);
	nAttr.setMax(5.0f);
	nAttr.setDefault(0.0f);
	addAttribute(aFactor);
    attributeAffects(aFactor,outputGeom);

	aMaxDistance = nAttr.create("maxInfluence","maxInfluence",MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0f);
	nAttr.setMax(100.0f);
	nAttr.setDefault(1.0f);
	addAttribute(aMaxDistance);
	attributeAffects(aMaxDistance,outputGeom);

	aPinch = nAttr.create("pinch","pinch",MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0f);
	nAttr.setMax(1.0f);
	nAttr.setDefault(0.0f);
	addAttribute(aPinch);
	attributeAffects(aPinch,outputGeom);

	aMaxRepel = nAttr.create("maxRepelDistance", "maxRepelDistance", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0f);
	nAttr.setMax(100.0f);
	nAttr.setDefault(1.0f);
	addAttribute(aMaxRepel);
	attributeAffects(aMaxRepel, outputGeom);


	aAttractorType = eAttr.create("behavior","behavior");
	eAttr.addField("attract",0);
	eAttr.addField("repel",1);
	eAttr.setDefault(0);
	eAttr.setWritable(true);
	eAttr.setKeyable(true);
	addAttribute(aAttractorType);
	attributeAffects(aAttractorType,outputGeom);

	aFalloffType = eAttr.create("falloffType","falloffType");
	eAttr.addField("constant",0);
	eAttr.addField("linear",1);
	eAttr.addField("square",2);
	eAttr.setDefault(1);
	eAttr.setWritable(true);
	eAttr.setKeyable(true);
	addAttribute(aFalloffType);
	attributeAffects(aFalloffType,outputGeom);

	aSphereSize = nAttr.create("attractorSize", "attractorSize", MFnNumericData::kFloat);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0f);
	nAttr.setMax(2.0f);
	nAttr.setDefault(0.05f);
	addAttribute(aSphereSize);
	//attributeAffects(a, outputGeom);
	
	MGlobal::executeCommand( "makePaintable -attrType multiFloat -sm deformer attractionDeformer weights;" );

	return MS::kSuccess;
}

MStatus AttractionDeformer::deform( MDataBlock& data, MItGeometry& itGeo,
							 const MMatrix& localToWorldMatrix,unsigned int geomIndex)
{
	MStatus status;
	MMatrix attratorMatrix = data.inputValue(aAttractorMatrix).asMatrix();
	MMatrix attratorMatrixInverse = attratorMatrix.inverse();
	MMatrix worldToLocalMatrix = localToWorldMatrix.inverse();
	float factor = data.inputValue(aFactor).asFloat();
	float maxDistance = data.inputValue(aMaxDistance).asFloat();
	float maxRepel = data.inputValue(aMaxRepel).asFloat();
	short attractorType = data.inputValue(aAttractorType).asShort();
	short falloffType = data.inputValue(aFalloffType).asShort();
	float pinch = data.inputValue(aPinch).asFloat();

	//grab the input geometry data from the data block 
	MArrayDataHandle hInput = data.outputArrayValue(input, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	status = hInput.jumpToElement(geomIndex);
	MDataHandle hInputElement = hInput.outputValue(&status);
	MObject oInputGeom = hInputElement.child(inputGeom).asMesh();
	MFnMesh fnMesh(oInputGeom, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MFloatVectorArray normals; //get all the normals into an array
	fnMesh.getVertexNormals(false, normals);



	float env = data.inputValue( envelope ).asFloat();

	MPoint point;
	MVector pointToAttractor;
	MVector shift;
	float w;

	for(;!itGeo.isDone();itGeo.next())
	{
		point = itGeo.position();
		w = weightValue(data,geomIndex,itGeo.index());//paintable weight value

		point*= localToWorldMatrix;
		point*= attratorMatrixInverse; 
		pointToAttractor = MVector(point);
		MVector normal = normals[itGeo.index()];
		
		//get the sign
		float sign = 1.0;
		float ndotl = pointToAttractor*normal;
		if (ndotl != 0)
		{
			sign = (ndotl) / abs(ndotl);
		}

		MVector shiftDirection = (normal*sign*(1.0-pinch) + pointToAttractor.normal()*pinch).normal();
		shift = shiftDirection*shiftDistance(attractorType,falloffType,
			pointToAttractor,maxDistance,factor,maxRepel)*env*w;
		
		point+=shift;
		point*= attratorMatrix*worldToLocalMatrix;
		
        itGeo.setPosition(point);
	}
	return MS::kSuccess;
}

float AttractionDeformer::shiftDistance(short attractorType,short falloffType, 
		                      MVector pToA,float maxDist, float factor, float maxRepel)
{   //if overlaps with the attractor

	float weight;
	float distance = pToA.length();

	if (distance==0 || distance>maxDist)
	{
		return 0;
	}

	switch (falloffType)
	{
		case 0: weight = 1.0;
			break;
		case 1: weight = 1.0/distance;
			break;
		case 2: weight = 1.0/(distance*distance);
			break;
	}

	//clamp the output value
	if(attractorType == 0) //attraction
	{
		weight *= -1.0;
	}

	weight*=factor; //multiplay factor
	
	if(weight + distance < 0 )//attract to the attractor
	{
		weight = -distance;
	}

	if(weight> maxRepel)
	{
		weight = maxRepel;
	}
	

	return weight;//return the value
}

