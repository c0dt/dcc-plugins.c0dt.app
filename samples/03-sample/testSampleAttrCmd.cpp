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

//�R�}���h�̃N���X MPxCommand����h��
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

//�R�}���h�Ŏ��s������e
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
			//MFnTransform�ɂ��translate�l�̎擾/�ݒ�
			MFnTransform transFn( nodeObj );

			//translate�l�̎擾
			MVector  translateValue = transFn.getTranslation( MSpace::kObject );
			cout << "MFnTransform getTranslation: " << translateValue << endl;

			//translate�l�̐ݒ�
			transFn.setTranslation( MVector(2.0, 3.0, 4.0), MSpace::kObject );



			////////////////////////////////////////////////////////////////////
			//MFnDependencyNode��MPlug����translateX�l���擾/�ݒ�
			MFnDependencyNode depFn( nodeObj );

			////MPlug�ɂ��A�g���r���[�g�l�̎擾
			MPlug translateXPlug = depFn.findPlug( "translateX" );
			double translateXValue;
			translateXPlug.getValue( translateXValue );
			cout << "MPlug getValue translateX: " << translateXValue << endl;

			//MPlug�ɂ��A�g���r���[�g�l�̐ݒ�
			translateXPlug.setValue( 3.0 );



			////////////////////////////////////////////////////////////////////
			//DisplayLocalAxis�A�g���r���[�g���I��
			MPlug displayLocalAxisPlug = depFn.findPlug( "displayLocalAxis", true );
			displayLocalAxisPlug.setValue( true );
			MPlug displayHandlePlug = depFn.findPlug("displayHandle", true);
			displayHandlePlug.setValue(true);

			////////////////////////////////////////////////////////////////////
			//MPlug�ɂ��g�����X�t�H�[���}�g���b�N�X�l�̎擾
			MFnDependencyNode depFn2( nodeObj );
			MPlug matrixPlug = depFn2.findPlug( "matrix" );
			MObject matrixObj;
			matrixPlug.getValue( matrixObj );
			cout<< "matrixObj.apiTypeStr(): " <<matrixObj.apiTypeStr()<<endl;
			MFnMatrixData matDataFn( matrixObj );
			MMatrix matrixValue = matDataFn.matrix();
			cout << "MPlug getValue matrix:" << matrixValue << endl;

			//MFnTransform�ɂ��}�g���b�N�X�l�̐ݒ�
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

//�v���O�C�����[�h���̃C�j�V�����C�Y
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

//�v���O�C���A�����[�h���̃A���C�j�V�����C�Y
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
