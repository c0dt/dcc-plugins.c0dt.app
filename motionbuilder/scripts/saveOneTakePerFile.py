from pyfbsdk import *
import os

def saveOneTakePerFile(path):
    oApp = FBApplication()
    if not os.path.exists(path):
        os.makedirs(path)
    scene = FBSystem().Scene
    takes = scene.Takes
    takeCount = len(takes)
    for takeIndex in range(takeCount):
        motionFbxFile=os.path.join(path, takes[takeIndex].Name + ".fbx")
        opSave = FBFbxOptions(False)
        opSave.EmbedMedia = True
        opSave.BaseCameras = False
        opSave.CameraSwitcherSettings = False
        opSave.CurrentCameraSettings = False
        opSave.GlobalLightingSettings = False
        opSave.TransportSettings = False
        for pTakeIndex in range(takeCount):
            opSave.SetTakeSelect(pTakeIndex, pTakeIndex == takeIndex)
        oApp.FileSave(motionFbxFile, opSave)