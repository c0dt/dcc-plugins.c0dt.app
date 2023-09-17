import maya.cmds as cmds
import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx
import maya.cmds as cmds
import maya.mel as mel
from PySide2 import QtWidgets, QtGui, QtCore
from PySide2.QtCore import Qt
from PySide2.QtWidgets import (
    QApplication,
    QCheckBox,
    QComboBox,
    QDateEdit,
    QDateTimeEdit,
    QDial,
    QDoubleSpinBox,
    QFontComboBox,
    QLabel,
    QLCDNumber,
    QLineEdit,
    QMainWindow,
    QProgressBar,
    QPushButton,
    QRadioButton,
    QSlider,
    QSpinBox,
    QTimeEdit,
    QVBoxLayout,
    QWidget,
)

kPluginCmdName = "c0dtHelloWorldUI"

# ================ PLUGIN SYS COMMON =========================
# Command
class CreateSphereCmd(OpenMayaMPx.MPxCommand):
    def __init__(self):
        OpenMayaMPx.MPxCommand.__init__(self)

    def doIt(self, args):
        createPluginUI()

# Creator
def cmdCreator():
    return OpenMayaMPx.asMPxPtr(CreateSphereCmd())

# Initialize the script plug-in
def initializePlugin(mobject):
    mplugin = OpenMayaMPx.MFnPlugin(mobject)
    try:
        mplugin.registerCommand(kPluginCmdName, cmdCreator)
    except:
        sys.stderr.write("Failed to register command: %s\n" % kPluginCmdName)
        raise

# Uninitialize the script plug-in
def uninitializePlugin(mobject):
    mplugin = OpenMayaMPx.MFnPlugin(mobject)
    try:
        mplugin.deregisterCommand(kPluginCmdName)
    except:
        sys.stderr.write("Failed to unregister command: %s\n" % kPluginCmdName)

# ================ PLUGIN SYS COMMON =========================

def createPluginUI():
    # Check if the window exists and if so, delete
    if cmds.window("uiDemoWindow", exists=True):
        cmds.deleteUI("uiDemoWindow", window=True)

    # Convert Maya's main window to QWidget for parenting
    main_window = {o.objectName(): o for o in QApplication.topLevelWidgets()}["MayaWindow"]

    # Create main dialog
    dialog = QtWidgets.QDialog(parent=main_window)
    dialog.setWindowTitle("UI Controls Demo")
    dialog.setObjectName("uiDemoWindow")
    dialog.setFixedSize(300, 500)

    # Create layout for the dialog
    layout = QtWidgets.QVBoxLayout(dialog)
    
    # Create Menu Bar
    menu_bar = QtWidgets.QMenuBar(dialog)
    fileMenu = menu_bar.addMenu('File')
    # menu.addAction('Create Sphere', self.createSphere)
    layout.addWidget(menu_bar)

    openMenu = QtWidgets.QMenu("Open")
    # openMenu.addAction("help")

    exitAction = QtWidgets.QAction("Exit", main_window)
    exitAction.setShortcut("Ctrl+G")
    exitAction.triggered.connect(main_window.close)

    # menuBar = dialog.menuBar()
    fileMenu = menu_bar.addMenu("File")
    fileMenu.addMenu(openMenu)
    fileMenu.addAction(exitAction)

    # Create Toolbar
    toolbar = QtWidgets.QToolBar(dialog)
    # toolbar.addAction('Create Sphere', self.createSphere)
    layout.addWidget(toolbar)


    widgets = [
            QCheckBox,
            QComboBox,
            QDateEdit,
            QDateTimeEdit,
            QDial,
            QDoubleSpinBox,
            QFontComboBox,
            QLCDNumber,
            QLabel,
            QLineEdit,
            QProgressBar,
            QPushButton,
            QRadioButton,
            QSlider,
            QSpinBox,
            QTimeEdit,
        ]

    for w in widgets:
        instance = w()
        if w == QDial:
            # Just test print the dial value
            instance.valueChanged.connect(lambda x: print("Dial value = %i" % (x)))
        layout.addWidget(instance)

    # Status Bar
    status_bar = QtWidgets.QStatusBar(dialog)
    status_bar.showMessage("Ready")
    layout.addWidget(status_bar)

    # Show the dialog
    dialog.show()