import maya.cmds as cmds
import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx
import maya.cmds as cmds
import maya.mel as mel
from PySide2 import QtWidgets, QtGui, QtCore

kPluginCmdName = "c0dtCmdTemplate"

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
    main_window = {o.objectName(): o for o in QtWidgets.QApplication.topLevelWidgets()}["MayaWindow"]

    # Create main dialog
    dialog = QtWidgets.QDialog(parent=main_window)
    dialog.setWindowTitle("UI Controls Demo")
    dialog.setObjectName("uiDemoWindow")
    dialog.setFixedSize(300, 500)

    # Create layout for the dialog
    layout = QtWidgets.QVBoxLayout(dialog)

    # Add QLabel
    label = QtWidgets.QLabel("Sample QLabel")
    layout.addWidget(label)

    # Add QLineEdit
    line_edit = QtWidgets.QLineEdit()
    line_edit.setPlaceholderText("Sample QLineEdit")
    layout.addWidget(line_edit)

    # Add QPushButton
    button = QtWidgets.QPushButton("Sample QPushButton")
    button.clicked.connect(lambda: mel.eval('polySphere;'))
    layout.addWidget(button)

    button = QtWidgets.QPushButton("Native MEL HelloWorld")
    button.clicked.connect(lambda: mel.eval('helloWorld;'))
    layout.addWidget(button)

    # Add QComboBox
    combo_box = QtWidgets.QComboBox()
    combo_box.addItems(["Option 1", "Option 2", "Option 3"])
    layout.addWidget(combo_box)

    # Add QCheckBox
    checkbox = QtWidgets.QCheckBox("Sample QCheckBox")
    layout.addWidget(checkbox)

    # Add QRadioButton
    radio_button1 = QtWidgets.QRadioButton("Radio Button 1")
    radio_button2 = QtWidgets.QRadioButton("Radio Button 2")
    layout.addWidget(radio_button1)
    layout.addWidget(radio_button2)

    # Add QSlider
    slider = QtWidgets.QSlider(QtCore.Qt.Horizontal)
    layout.addWidget(slider)

    # Add QSpinBox
    spin_box = QtWidgets.QSpinBox()
    layout.addWidget(spin_box)

    # Add QTextEdit
    text_edit = QtWidgets.QTextEdit()
    layout.addWidget(text_edit)

    # Add QDateEdit
    date_edit = QtWidgets.QDateEdit()
    layout.addWidget(date_edit)

    # Add QTimeEdit
    time_edit = QtWidgets.QTimeEdit()
    layout.addWidget(time_edit)

    # Show the dialog
    dialog.show()