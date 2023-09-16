#include <maya/MIOStream.h>
#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MFnTransform.h>
#include <maya/MVector.h>
#include <maya/MPlug.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MFnMatrixData.h>
#include <maya/MTime.h>
#include <maya/MAnimControl.h>

//コマンドのクラス MPxCommandから派生
class testSampleAttr : public MPxCommand
{
public:
	testSampleAttr() {};
	~testSampleAttr() override;
	MStatus	 doIt(const MArgList& args) override;
	static void* creator();

private:
};

testSampleAttr::~testSampleAttr() {}

void* testSampleAttr::creator()
{
	return new testSampleAttr();
}

//コマンドで実行する内容
MStatus testSampleAttr::doIt( const MArgList& )
{
	MTime currentTime = MAnimControl::currentTime();
	double timeInFrames = currentTime.as(MTime::kFilm);
	MStatus status;

	MSelectionList slist;
	MGlobal::getActiveSelectionList( slist );
	for ( unsigned int i = 0; i < slist.length(); i++ ) {

		MObject nodeObj;
		slist.getDependNode( i, nodeObj );

		if ( nodeObj.apiType() == MFn::kTransform ) {

			////////////////////////////////////////////////////////////////////
			//MFnTransformによるtranslate値の取得/設定
			MFnTransform transFn( nodeObj );

			//translate値の取得
			MVector  translateValue = transFn.getTranslation( MSpace::kObject );
			cout << "MFnTransform getTranslation: " << translateValue << endl;

			//translate値の設定
			transFn.setTranslation( MVector(2.0, 3.0, 4.0), MSpace::kObject );



			////////////////////////////////////////////////////////////////////
			//MFnDependencyNodeでMPlugからtranslateX値を取得/設定
			MFnDependencyNode depFn( nodeObj );

			////MPlugによるアトリビュート値の取得
			MPlug translateXPlug = depFn.findPlug( "translateX" );
			double translateXValue;
			translateXPlug.getValue( translateXValue );
			cout << "MPlug getValue translateX: " << translateXValue << endl;

			//MPlugによるアトリビュート値の設定
			translateXPlug.setValue( 3.0 );



			////////////////////////////////////////////////////////////////////
			//DisplayLocalAxisアトリビュートをオン
			MPlug displayLocalAxisPlug = depFn.findPlug( "displayLocalAxis", true );
			displayLocalAxisPlug.setValue( true );
			MPlug displayHandlePlug = depFn.findPlug("displayHandle", true);
			displayHandlePlug.setValue(true);

			////////////////////////////////////////////////////////////////////
			//MPlugによるトランスフォームマトリックス値の取得
			MFnDependencyNode depFn2( nodeObj );
			MPlug matrixPlug = depFn2.findPlug( "matrix" );
			MObject matrixObj;
			matrixPlug.getValue( matrixObj );
			cout<< "matrixObj.apiTypeStr(): " <<matrixObj.apiTypeStr()<<endl;
			MFnMatrixData matDataFn( matrixObj );
			MMatrix matrixValue = matDataFn.matrix();
			cout << "MPlug getValue matrix:" << matrixValue << endl;

			//MFnTransformによるマトリックス値の設定
			MFnTransform transFn2( nodeObj );
			MMatrix m;
			m[0][0] = 1.0;  m[0][1] = 0.0;  m[0][2] = 0.0;  m[0][3] = 0.0;
			m[1][0] = 0.0;  m[1][1] = 1.0;  m[1][2] = 0.0;  m[1][3] = 0.0;
			m[2][0] = 0.0;  m[2][1] = 0.0;  m[2][2] = 1.0;  m[2][3] = 0.0;
			m[3][0] = 2.0;  m[3][1] = 3.0;  m[3][2] = 4.0;  m[3][3] = 1.0;
			MTransformationMatrix trMat( m );
			transFn2.set( trMat );

		}
	}


	return status;
}

//プラグインロード時のイニシャライズ
MStatus initializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "1.0", "Any");

	status = plugin.registerCommand("testSampleAttr", testSampleAttr::creator);
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

	status = plugin.deregisterCommand("testSampleAttr");
	if (!status) {
		status.perror("deregisterCommandAttr");
		return status;
	}

	return status;
}
