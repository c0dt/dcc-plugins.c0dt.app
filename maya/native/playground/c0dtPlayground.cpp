#include <maya/MIOStream.h>
#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include <maya/MSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <iostream>
#include <chrono>
#include <ctime>

class c0dtPlayground : public MPxCommand
{
public:
	c0dtPlayground() {};
	~c0dtPlayground() override;
	MStatus	 doIt(const MArgList& args) override;
	static void* creator();

private:
};

c0dtPlayground::~c0dtPlayground() {}

void* c0dtPlayground::creator()
{
	return new c0dtPlayground();
}


MStatus c0dtPlayground::doIt(const MArgList&)
{
	MStatus status;

	//****************************************************************************************************
	// require OpenMayaAnim.lib to link
	//MTime currentTime = MAnimControl::currentTime();
	//double timeInFrames = currentTime.as(MTime::kFilm);

	auto now = std::chrono::system_clock::now();
	auto now_time_t = std::chrono::system_clock::to_time_t(now);

	cout << "---------- c0dtPlayground ---------- Start " << std::ctime(&now_time_t) << endl;

	MSelectionList slist;
	MGlobal::getActiveSelectionList( slist );
	for (unsigned int i = 0; i < slist.length(); i++ ) {
		MDagPath dagPath;
		slist.getDagPath(i, dagPath );
		cout << dagPath.fullPathName() << endl;
		appendToResult( dagPath.fullPathName() );
	}

	now = std::chrono::system_clock::now();
	now_time_t = std::chrono::system_clock::to_time_t(now);
	cout << "---------- c0dtPlayground ---------- End" << std::ctime(&now_time_t) << endl;
	//****************************************************************************************************


	return status;
}

MStatus initializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "1.0", "Any");

	status = plugin.registerCommand("c0dtPlayground", c0dtPlayground::creator);
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

	status = plugin.deregisterCommand("c0dtPlayground");
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}

	return status;
}
