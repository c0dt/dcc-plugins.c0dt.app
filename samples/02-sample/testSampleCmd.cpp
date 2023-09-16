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

//コマンドのクラス MPxCommandから派生
class testSample : public MPxCommand
{
public:
	testSample() {};
	~testSample() override;
	MStatus	 doIt(const MArgList& args) override;
	static void* creator();

private:
};

testSample::~testSample() {}

void* testSample::creator()
{
	return new testSample();
}

//コマンドで実行する内容
MStatus testSample::doIt(const MArgList&)
{
	MStatus status;
	// require OpenMayaAnim.lib to link
	//MTime currentTime = MAnimControl::currentTime();
	//double timeInFrames = currentTime.as(MTime::kFilm);

	auto now = std::chrono::system_clock::now();
	auto now_time_t = std::chrono::system_clock::to_time_t(now);

	cout << "---------- testSample ---------- Start " << std::ctime(&now_time_t) << endl;

	//選択中のオブジェクトの名前を出力
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
	cout << "---------- testSample ---------- End" << std::ctime(&now_time_t) << endl;
	return status;
}

//プラグインロード時のイニシャライズ
MStatus initializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "1.0", "Any");

	status = plugin.registerCommand("testSample", testSample::creator);
	if (!status) {
		status.perror("registerCommand");
		return status;
	}

	return status;
}

//プラグインアンロード時のアンイニシャライズ
MStatus uninitializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj);

	status = plugin.deregisterCommand("testSample");
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}

	return status;
}
