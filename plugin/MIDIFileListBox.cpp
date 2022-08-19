//
// Created by Andrei Gheorghe on 19.08.2022.
//

#include "MIDIFileListBox.h"
#include <iostream>
#include <utility>

MIDIFileListBox::MIDIFileListBox() {
    midiFilesDir = File::getSpecialLocation(File::userDocumentsDirectory)
            .getChildFile (ProjectInfo::companyName)
            .getChildFile("midi_files");

    std::cout << midiFilesDir.getFullPathName() << std::endl;
    midiFiles.add(midiFilesDir.getParentDirectory());
    midiFiles.addArray(midiFilesDir.findChildFiles(File::findDirectories, false, "*"));
    midiFiles.addArray(midiFilesDir.findChildFiles(File::findFiles, false, "*.mid"));
    settings->addChangeListener(this);
}

MIDIFileListBox::~MIDIFileListBox() {
    settings->removeChangeListener (this);
}

void MIDIFileListBox::listBoxItemClicked(int rowNumber, const MouseEvent &event) {
    if (event.mods.isPopupMenu())
    {
        juce::PopupMenu::Options options;
        juce::PopupMenu menu;
        menu.addItem ("Remove", [this, rowNumber]()
        {
            midiFiles.remove(rowNumber);
        });
        menu.showMenuAsync (options);
    }

    if (onSelectionChanged)
        onSelectionChanged (rowNumber);
}
void MIDIFileListBox::paintListBoxItem(int rowNumber, Graphics &g, int width, int height, bool rowIsSelected) {
    auto bounds = juce::Rectangle<int> (0, 0, width, height);
    if (rowIsSelected)
    {
        g.setColour (juce::Colours::grey);
        g.fillRect (bounds);
    }

    bounds = juce::Rectangle<int> (5, 2, width, height);

    g.setColour (juce::Colours::silver);

    g.drawFittedText (midiFiles[rowNumber] != midiFilesDir.getParentDirectory() ? midiFiles[rowNumber].getFileName() : "..", bounds, juce::Justification::centredLeft, 1);
}

int MIDIFileListBox::getNumRows() {
    return midiFilesDir.getNumberOfChildFiles(File::findFiles, "*.mid") + midiFilesDir.getNumberOfChildFiles(File::findDirectories, "*") + 1;
}

void MIDIFileListBox::changeListenerCallback(juce::ChangeBroadcaster *) {
    midiFilesDir = File(settings->settings.getProperty("path"));
    std::cout << midiFilesDir.getFullPathName() << std::endl;
    midiFiles = {};
    midiFiles.add(midiFilesDir.getParentDirectory());
    midiFiles.addArray(midiFilesDir.findChildFiles(File::findDirectories, false, "*"));
    midiFiles.addArray(midiFilesDir.findChildFiles(File::findFiles, false, "*.mid"));
    sendChangeMessage();
}
