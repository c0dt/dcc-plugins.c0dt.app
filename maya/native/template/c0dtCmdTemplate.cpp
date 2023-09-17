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

class c0dtCmdTemplate : public MPxCommand
{
public:
	c0dtCmdTemplate() {};
	~c0dtCmdTemplate() override;
	MStatus	 doIt(const MArgList& args) override;
	static void* creator();

private:
};

c0dtCmdTemplate::~c0dtCmdTemplate() {}

void* c0dtCmdTemplate::creator()
{
	return new c0dtCmdTemplate();
}


MStatus c0dtCmdTemplate::doIt(const MArgList&)
{
	MStatus status;

	//****************************************************************************************************
	// require OpenMayaAnim.lib to link
	//MTime currentTime = MAnimControl::currentTime();
	//double timeInFrames = currentTime.as(MTime::kFilm);

	auto now = std::chrono::system_clock::now();
	auto now_time_t = std::chrono::system_clock::to_time_t(now);

	cout << "---------- c0dtCmdTemplate ---------- Start " << std::ctime(&now_time_t) << endl;

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
	cout << "---------- c0dtCmdTemplate ---------- End" << std::ctime(&now_time_t) << endl;
	//****************************************************************************************************


	return status;
}

MStatus initializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "1.0", "Any");

	status = plugin.registerCommand("c0dtCmdTemplate", c0dtCmdTemplate::creator);
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

	status = plugin.deregisterCommand("c0dtCmdTemplate");
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}

	return status;
}
