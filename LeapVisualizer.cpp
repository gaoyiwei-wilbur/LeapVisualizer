/******************************************************************************\
 * Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
 * Leap Motion proprietary and confidential. Not for distribution.              *
 * Use subject to the terms of the Leap Motion SDK Agreement available at       *
 * https://developer.leapmotion.com/sdk_agreement, or another agreement         *
 * between Leap Motion and you, your company or other organization.             *
 \******************************************************************************/

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <cstring>

#include <vtkSmartPointer.h>
#include <vtkLineSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballActor.h>

#include "Leap.h"

using namespace Leap;

class SampleListener : public Listener {
public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    virtual void onDeviceChange(const Controller&);
    virtual void onServiceConnect(const Controller&);
    virtual void onServiceDisconnect(const Controller&);
    
private:
};

const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
//const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

void SampleListener::onInit(const Controller& controller) {
    std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
    std::cout << "Connected" << std::endl;
    //controller.enableGesture(Gesture::TYPE_CIRCLE);
    //controller.enableGesture(Gesture::TYPE_KEY_TAP);
    //controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
    //controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller) {
    // Note: not dispatched when running in a debugger.
    std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
    std::cout << "Exited" << std::endl;
}

double FPStart[5][4][3];
double FPEnd[5][4][3];

//define LineSource start
vtkSmartPointer<vtkLineSource> ThumbLine1 =
vtkSmartPointer<vtkLineSource>::New();
vtkSmartPointer<vtkLineSource> ThumbLine2 =
vtkSmartPointer<vtkLineSource>::New();
vtkSmartPointer<vtkLineSource> ThumbLine3 =
vtkSmartPointer<vtkLineSource>::New();
vtkSmartPointer<vtkLineSource> ThumbLine4 =
vtkSmartPointer<vtkLineSource>::New();

vtkSmartPointer<vtkLineSource> IndexLine1 =
vtkSmartPointer<vtkLineSource>::New();
vtkSmartPointer<vtkLineSource> IndexLine2 =
vtkSmartPointer<vtkLineSource>::New();
vtkSmartPointer<vtkLineSource> IndexLine3 =
vtkSmartPointer<vtkLineSource>::New();
vtkSmartPointer<vtkLineSource> IndexLine4 =
vtkSmartPointer<vtkLineSource>::New();

vtkSmartPointer<vtkLineSource> MiddleLine1 =
vtkSmartPointer<vtkLineSource>::New();
vtkSmartPointer<vtkLineSource> MiddleLine2 =
vtkSmartPointer<vtkLineSource>::New();
vtkSmartPointer<vtkLineSource> MiddleLine3 =
vtkSmartPointer<vtkLineSource>::New();
vtkSmartPointer<vtkLineSource> MiddleLine4 =
vtkSmartPointer<vtkLineSource>::New();

vtkSmartPointer<vtkLineSource> RingLine1 =
vtkSmartPointer<vtkLineSource>::New();
vtkSmartPointer<vtkLineSource> RingLine2 =
vtkSmartPointer<vtkLineSource>::New();
vtkSmartPointer<vtkLineSource> RingLine3 =
vtkSmartPointer<vtkLineSource>::New();
vtkSmartPointer<vtkLineSource> RingLine4 =
vtkSmartPointer<vtkLineSource>::New();

vtkSmartPointer<vtkLineSource> PinkyLine1 =
vtkSmartPointer<vtkLineSource>::New();
vtkSmartPointer<vtkLineSource> PinkyLine2 =
vtkSmartPointer<vtkLineSource>::New();
vtkSmartPointer<vtkLineSource> PinkyLine3 =
vtkSmartPointer<vtkLineSource>::New();
vtkSmartPointer<vtkLineSource> PinkyLine4 =
vtkSmartPointer<vtkLineSource>::New();
//define LinSource end

vtkSmartPointer<vtkRenderWindow> renderWindow =
vtkSmartPointer<vtkRenderWindow>::New();
vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
vtkSmartPointer<vtkRenderWindowInteractor>::New();

void SampleListener::onFrame(const Controller& controller) {
    // Get the most recent frame
    const Frame frame = controller.frame();
    
    HandList hands = frame.hands();
    for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
        // Get the first hand
        const Hand hand = *hl;
        std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
        //std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
        //<< ", palm position: " << hand.palmPosition() << std::endl;
        
        // Get fingers
        int fingerName = 0; //refer to 5 fingers: 1->Thumb; 2->Index; 3->Middle; 4->Ring; 5->Pinky
        const FingerList fingers = hand.fingers();
        for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
            const Finger finger = *fl;
            //std::cout << std::string(4, ' ') <<  fingerNames[finger.type()]
            //<< " finger, id: " << finger.id()
            //<< ", length: " << finger.length()
            //<< "mm, width: " << finger.width() << std::endl;
            
            // Get finger bones
            for (int b = 0; b < 4; ++b) {
                Bone::Type boneType = static_cast<Bone::Type>(b);
                Bone bone = finger.bone(boneType);
                //std::cout << std::string(6, ' ') <<  boneNames[boneType]
                //<< " bone, start: " << bone.prevJoint()
                //<< ", end: " << bone.nextJoint()
                //<< ", direction: " << bone.direction() << std::endl;
                
                FPStart[fingerName][b][0] = bone.prevJoint().x;
                FPStart[fingerName][b][1] = bone.prevJoint().y;
                FPStart[fingerName][b][2] = bone.prevJoint().z;
                FPEnd[fingerName][b][0] = bone.nextJoint().x;
                FPEnd[fingerName][b][1] = bone.nextJoint().y;
                FPEnd[fingerName][b][2] = bone.nextJoint().z;
            }
            ++fingerName;
        }
    }
    
    //set Line Points start
    ThumbLine1->SetPoint1(FPStart[0][0]);
    ThumbLine1->SetPoint2(FPEnd[0][0]);
    ThumbLine1->Update();
    ThumbLine2->SetPoint1(FPStart[0][1]);
    ThumbLine2->SetPoint2(FPEnd[0][1]);
    ThumbLine2->Update();
    ThumbLine3->SetPoint1(FPStart[0][2]);
    ThumbLine3->SetPoint2(FPEnd[0][2]);
    ThumbLine3->Update();
    ThumbLine4->SetPoint1(FPStart[0][3]);
    ThumbLine4->SetPoint2(FPEnd[0][3]);
    ThumbLine4->Update();
    
    IndexLine1->SetPoint1(FPStart[1][0]);
    IndexLine1->SetPoint2(FPEnd[1][0]);
    IndexLine1->Update();
    IndexLine2->SetPoint1(FPStart[1][1]);
    IndexLine2->SetPoint2(FPEnd[1][1]);
    IndexLine2->Update();
    IndexLine3->SetPoint1(FPStart[1][2]);
    IndexLine3->SetPoint2(FPEnd[1][2]);
    IndexLine3->Update();
    IndexLine4->SetPoint1(FPStart[1][3]);
    IndexLine4->SetPoint2(FPEnd[1][3]);
    IndexLine4->Update();
    
    MiddleLine1->SetPoint1(FPStart[2][0]);
    MiddleLine1->SetPoint2(FPEnd[2][0]);
    MiddleLine1->Update();
    MiddleLine2->SetPoint1(FPStart[2][1]);
    MiddleLine2->SetPoint2(FPEnd[2][1]);
    MiddleLine2->Update();
    MiddleLine3->SetPoint1(FPStart[2][2]);
    MiddleLine3->SetPoint2(FPEnd[2][2]);
    MiddleLine3->Update();
    MiddleLine4->SetPoint1(FPStart[2][3]);
    MiddleLine4->SetPoint2(FPEnd[2][3]);
    MiddleLine4->Update();
    
    RingLine1->SetPoint1(FPStart[3][0]);
    RingLine1->SetPoint2(FPEnd[3][0]);
    RingLine1->Update();
    RingLine2->SetPoint1(FPStart[3][1]);
    RingLine2->SetPoint2(FPEnd[3][1]);
    RingLine2->Update();
    RingLine3->SetPoint1(FPStart[3][2]);
    RingLine3->SetPoint2(FPEnd[3][2]);
    RingLine3->Update();
    RingLine4->SetPoint1(FPStart[3][3]);
    RingLine4->SetPoint2(FPEnd[3][3]);
    RingLine4->Update();
    
    PinkyLine1->SetPoint1(FPStart[4][0]);
    PinkyLine1->SetPoint2(FPEnd[4][0]);
    PinkyLine1->Update();
    PinkyLine2->SetPoint1(FPStart[4][1]);
    PinkyLine2->SetPoint2(FPEnd[4][1]);
    PinkyLine2->Update();
    PinkyLine3->SetPoint1(FPStart[4][2]);
    PinkyLine3->SetPoint2(FPEnd[4][2]);
    PinkyLine3->Update();
    PinkyLine4->SetPoint1(FPStart[4][3]);
    PinkyLine4->SetPoint2(FPEnd[4][3]);
    PinkyLine4->Update();
    //set Line Points end
    
    renderWindow->Render();
}


void SampleListener::onFocusGained(const Controller& controller) {
    std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
    std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
    std::cout << "Device Changed" << std::endl;
    const DeviceList devices = controller.devices();
    
    for (int i = 0; i < devices.count(); ++i) {
        std::cout << "id: " << devices[i].toString() << std::endl;
        std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
    }
}

void SampleListener::onServiceConnect(const Controller& controller) {
    std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
    std::cout << "Service Disconnected" << std::endl;
}

int main(int argc, char** argv) {
    // Create a sample listener and controller
    SampleListener listener;
    Controller controller;
    
    // Have the sample listener receive events from the controller
    controller.addListener(listener);
    
    if (argc > 1 && strcmp(argv[1], "--bg") == 0)
        controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);
    
    //Visualize
    
    //add mapper
    vtkSmartPointer<vtkPolyDataMapper> Thumbmapper1 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Thumbmapper1->SetInputConnection(ThumbLine1->GetOutputPort());
    vtkSmartPointer<vtkActor> Thumbactor1 =
    vtkSmartPointer<vtkActor>::New();
    Thumbactor1->SetMapper(Thumbmapper1);
    Thumbactor1->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Thumbmapper2 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Thumbmapper2->SetInputConnection(ThumbLine2->GetOutputPort());
    vtkSmartPointer<vtkActor> Thumbactor2 =
    vtkSmartPointer<vtkActor>::New();
    Thumbactor2->SetMapper(Thumbmapper2);
    Thumbactor2->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Thumbmapper3 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Thumbmapper3->SetInputConnection(ThumbLine3->GetOutputPort());
    vtkSmartPointer<vtkActor> Thumbactor3 =
    vtkSmartPointer<vtkActor>::New();
    Thumbactor3->SetMapper(Thumbmapper3);
    Thumbactor3->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Thumbmapper4 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Thumbmapper4->SetInputConnection(ThumbLine4->GetOutputPort());
    vtkSmartPointer<vtkActor> Thumbactor4 =
    vtkSmartPointer<vtkActor>::New();
    Thumbactor4->SetMapper(Thumbmapper4);
    Thumbactor4->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Indexmapper1 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Indexmapper1->SetInputConnection(IndexLine1->GetOutputPort());
    vtkSmartPointer<vtkActor> Indexactor1 =
    vtkSmartPointer<vtkActor>::New();
    Indexactor1->SetMapper(Indexmapper1);
    Indexactor1->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Indexmapper2 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Indexmapper2->SetInputConnection(IndexLine2->GetOutputPort());
    vtkSmartPointer<vtkActor> Indexactor2 =
    vtkSmartPointer<vtkActor>::New();
    Indexactor2->SetMapper(Indexmapper2);
    Indexactor2->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Indexmapper3 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Indexmapper3->SetInputConnection(IndexLine3->GetOutputPort());
    vtkSmartPointer<vtkActor> Indexactor3 =
    vtkSmartPointer<vtkActor>::New();
    Indexactor3->SetMapper(Indexmapper3);
    Indexactor3->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Indexmapper4 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Indexmapper4->SetInputConnection(IndexLine4->GetOutputPort());
    vtkSmartPointer<vtkActor> Indexactor4 =
    vtkSmartPointer<vtkActor>::New();
    Indexactor4->SetMapper(Indexmapper4);
    Indexactor4->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Middlemapper1 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Middlemapper1->SetInputConnection(MiddleLine1->GetOutputPort());
    vtkSmartPointer<vtkActor> Middleactor1 =
    vtkSmartPointer<vtkActor>::New();
    Middleactor1->SetMapper(Middlemapper1);
    Middleactor1->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Middlemapper2 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Middlemapper2->SetInputConnection(MiddleLine2->GetOutputPort());
    vtkSmartPointer<vtkActor> Middleactor2 =
    vtkSmartPointer<vtkActor>::New();
    Middleactor2->SetMapper(Middlemapper2);
    Middleactor2->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Middlemapper3 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Middlemapper3->SetInputConnection(MiddleLine3->GetOutputPort());
    vtkSmartPointer<vtkActor> Middleactor3 =
    vtkSmartPointer<vtkActor>::New();
    Middleactor3->SetMapper(Middlemapper3);
    Middleactor3->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Middlemapper4 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Middlemapper4->SetInputConnection(MiddleLine4->GetOutputPort());
    vtkSmartPointer<vtkActor> Middleactor4 =
    vtkSmartPointer<vtkActor>::New();
    Middleactor4->SetMapper(Middlemapper4);
    Middleactor4->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Ringmapper1 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Ringmapper1->SetInputConnection(RingLine1->GetOutputPort());
    vtkSmartPointer<vtkActor> Ringactor1 =
    vtkSmartPointer<vtkActor>::New();
    Ringactor1->SetMapper(Ringmapper1);
    Ringactor1->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Ringmapper2 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Ringmapper2->SetInputConnection(RingLine2->GetOutputPort());
    vtkSmartPointer<vtkActor> Ringactor2 =
    vtkSmartPointer<vtkActor>::New();
    Ringactor2->SetMapper(Ringmapper2);
    Ringactor2->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Ringmapper3 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Ringmapper3->SetInputConnection(RingLine3->GetOutputPort());
    vtkSmartPointer<vtkActor> Ringactor3 =
    vtkSmartPointer<vtkActor>::New();
    Ringactor3->SetMapper(Ringmapper3);
    Ringactor3->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Ringmapper4 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Ringmapper4->SetInputConnection(RingLine4->GetOutputPort());
    vtkSmartPointer<vtkActor> Ringactor4 =
    vtkSmartPointer<vtkActor>::New();
    Ringactor4->SetMapper(Ringmapper4);
    Ringactor4->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Pinkymapper1 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Pinkymapper1->SetInputConnection(PinkyLine1->GetOutputPort());
    vtkSmartPointer<vtkActor> Pinkyactor1 =
    vtkSmartPointer<vtkActor>::New();
    Pinkyactor1->SetMapper(Pinkymapper1);
    Pinkyactor1->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Pinkymapper2 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Pinkymapper2->SetInputConnection(PinkyLine2->GetOutputPort());
    vtkSmartPointer<vtkActor> Pinkyactor2 =
    vtkSmartPointer<vtkActor>::New();
    Pinkyactor2->SetMapper(Pinkymapper2);
    Pinkyactor2->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Pinkymapper3 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Pinkymapper3->SetInputConnection(PinkyLine3->GetOutputPort());
    vtkSmartPointer<vtkActor> Pinkyactor3 =
    vtkSmartPointer<vtkActor>::New();
    Pinkyactor3->SetMapper(Pinkymapper3);
    Pinkyactor3->GetProperty()->SetLineWidth(4);
    
    vtkSmartPointer<vtkPolyDataMapper> Pinkymapper4 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
    Pinkymapper4->SetInputConnection(PinkyLine4->GetOutputPort());
    vtkSmartPointer<vtkActor> Pinkyactor4 =
    vtkSmartPointer<vtkActor>::New();
    Pinkyactor4->SetMapper(Pinkymapper4);
    Pinkyactor4->GetProperty()->SetLineWidth(4);
    
    //render
    vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
    
    renderWindow->AddRenderer(renderer);
    renderWindowInteractor->SetRenderWindow(renderWindow);
    
    //add actor
    renderer->AddActor(Thumbactor1);
    renderer->AddActor(Thumbactor2);
    renderer->AddActor(Thumbactor3);
    renderer->AddActor(Thumbactor4);
    renderer->AddActor(Indexactor1);
    renderer->AddActor(Indexactor2);
    renderer->AddActor(Indexactor3);
    renderer->AddActor(Indexactor4);
    renderer->AddActor(Middleactor1);
    renderer->AddActor(Middleactor2);
    renderer->AddActor(Middleactor3);
    renderer->AddActor(Middleactor4);
    renderer->AddActor(Ringactor1);
    renderer->AddActor(Ringactor2);
    renderer->AddActor(Ringactor3);
    renderer->AddActor(Ringactor4);
    renderer->AddActor(Pinkyactor1);
    renderer->AddActor(Pinkyactor2);
    renderer->AddActor(Pinkyactor3);
    renderer->AddActor(Pinkyactor4);
    
    renderWindow->Render();
    renderWindowInteractor->Start();//should not change camera angle too much, may cause error
    
    // Keep this process running until Enter is pressed
    //std::cin.get();
    
    // Remove the sample listener when done
    controller.removeListener(listener);
    
    return 0;
}
