from pyfbsdk import *

def moveTakesBackToZero():
    scene = FBSystem().Scene
    takes = scene.Takes
    for take in takes:
        time_span = take.LocalTimeSpan
        start_frame = time_span.GetStart().GetFrame()
        end_frame = time_span.GetStop().GetFrame()
        take.OffsetAnimation(FBTime(0, 0, 0, -start_frame))

        # Set the start end the end frame of the take
        take.LocalTimeSpan = FBTimeSpan(FBTime(0,0,0,0), FBTime(0,0,0,end_frame-start_frame))
    
    # Refresh the scene to reflect changes.
    scene.Evaluate()

moveTakesBackToZero()