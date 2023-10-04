#include <maya/MIOStream.h>
#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MMatrix.h>
#include <maya/MFnIkHandle.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MObject.h>
#include <maya/MFnTransform.h>
#include <iostream>
#include <chrono>
#include <ctime>

class c0dtScaffoldIKControl : public MPxCommand
{
public:
	c0dtScaffoldIKControl() {};
	~c0dtScaffoldIKControl() override;
	MStatus	 doIt(const MArgList& args) override;
	static void* creator();

private:
	bool isSelectionJointChain();
	MStatus createIKHandle(MDagPath& startJointPath, MDagPath& endJointPath, MFnIkHandle& outIkHandle);
	MStatus createControlCircle(const MFnIkHandle& ikHandle, MObject& outCircleObj);
};

c0dtScaffoldIKControl::~c0dtScaffoldIKControl() {}

void* c0dtScaffoldIKControl::creator()
{
	return new c0dtScaffoldIKControl();
}

bool c0dtScaffoldIKControl::isSelectionJointChain() // Added this function definition
{
	MSelectionList selList;
	MGlobal::getActiveSelectionList(selList);

	MItSelectionList selIter(selList, MFn::kJoint);
	MObject prevJoint;

	for (; !selIter.isDone(); selIter.next()) {
		MObject currentJoint;
		selIter.getDependNode(currentJoint);

		if (prevJoint.isNull()) {
			prevJoint = currentJoint;
			continue;
		}

		MFnDagNode fnPrevJoint(prevJoint);
		if (fnPrevJoint.child(0) != currentJoint) {
			return false;
		}

		prevJoint = currentJoint;
	}

	return true;
}

MStatus c0dtScaffoldIKControl::createIKHandle(MDagPath& startJointPath, MDagPath& endJointPath, MFnIkHandle& outIkHandle) {
	MStatus status;
	outIkHandle.create(startJointPath, endJointPath, &status);
	outIkHandle.setSolver(MString("ikRPsolver"));
	return status;
}

MStatus c0dtScaffoldIKControl::createControlCircle(const MFnIkHandle& ikHandle, MObject& outCircleObj) {
	MStatus status;
	MStringArray resultArray;
	status = MGlobal::executeCommand("circle -normal 0 1 0 -radius 10", resultArray);
	if (status != MS::kSuccess || resultArray.length() == 0) return status;

	MSelectionList listResult;
	listResult.add(resultArray[0]);
	listResult.getDependNode(0, outCircleObj);

	MMatrix ikHandleMatrix = ikHandle.transformationMatrix();
	MFnTransform fnCircleTransform(outCircleObj);
	fnCircleTransform.set(ikHandleMatrix);
	return status;
}


MStatus c0dtScaffoldIKControl::doIt(const MArgList&)
{
	MStatus status = MS::kFailure;

	if (!isSelectionJointChain()) {
		MGlobal::displayWarning("The selected joints do NOT form a chain.");
		return MS::kFailure;
	}

	MSelectionList selList;
	MGlobal::getActiveSelectionList(selList);

	if (selList.length() < 2) {
		MGlobal::displayError("Please select at least two joints.");
		return MS::kFailure;
	}

	MDagPath startJointPath, endJointPath;
	selList.getDagPath(0, startJointPath);
	selList.getDagPath(selList.length() - 1, endJointPath);

	MFnIkHandle fnIkHandle;
	status = createIKHandle(startJointPath, endJointPath, fnIkHandle);
	if (status != MS::kSuccess) {
		MGlobal::displayError("Failed to create IK handle.");
		return status;
	}

	MObject circleObj;
	status = createControlCircle(fnIkHandle, circleObj);
	if (status != MS::kSuccess) {
		MGlobal::displayError("Failed to create NURBS circle.");
		return status;
	}

	status = MGlobal::executeCommand("parent " + fnIkHandle.name() + " " + MFnDagNode(circleObj).name());
	if (status != MS::kSuccess) {
		MGlobal::displayError("Failed to parent IK handle to NURBS circle.");
		return status;
	}

	return MS::kSuccess;
}

MStatus initializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "1.0", "Any");

	status = plugin.registerCommand("c0dtScaffoldIKControl", c0dtScaffoldIKControl::creator);
	if (!status) {
		status.perror("registerCommand");
		return status;
	}

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj);

	status = plugin.deregisterCommand("c0dtScaffoldIKControl");
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}

	return status;
}
