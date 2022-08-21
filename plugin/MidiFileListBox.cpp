//
// Created by Andrei Gheorghe on 19.08.2022.
//

#include "MidiFileListBox.h"
#include <iostream>
#include <utility>

MidiFileListBox::MidiFileListBox() {
    midiFilesDir = File::getSpecialLocation(File::userDocumentsDirectory)
            .getChildFile (ProjectInfo::companyName)
            .getChildFile("midi_files");

    std::cout << midiFilesDir.getFullPathName() << std::endl;
    midiFiles.add(midiFilesDir.getParentDirectory());
    midiFiles.addArray(midiFilesDir.findChildFiles(File::findDirectories, false, "*"));
    midiFiles.addArray(midiFilesDir.findChildFiles(File::findFiles, false, "*.mid"));
    settings->addChangeListener(this);
}

MidiFileListBox::~MidiFileListBox() {
    settings->removeChangeListener (this);
}

void MidiFileListBox::listBoxItemDoubleClicked(int rowNumber, const MouseEvent &event) {
    if (onDoubleClick)
        onDoubleClick (midiFiles[rowNumber]);
}
void MidiFileListBox::paintListBoxItem(int rowNumber, Graphics &g, int width, int height, bool rowIsSelected) {
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

int MidiFileListBox::getNumRows() {
    return midiFilesDir.getNumberOfChildFiles(File::findFiles, searchText + "*.mid") + midiFilesDir.getNumberOfChildFiles(File::findDirectories, searchText+  "*") + 1;
}

void MidiFileListBox::changeListenerCallback(juce::ChangeBroadcaster *) {
    midiFilesDir = File(settings->settings.getProperty("path"));
    searchText = settings->settings.getProperty("text").toString();
    std::cout << midiFilesDir.getFullPathName() << std::endl;
    midiFiles = {};
    midiFiles.add(midiFilesDir.getParentDirectory());
    midiFiles.addArray(midiFilesDir.findChildFiles(File::findDirectories, false, searchText + "*"));
    midiFiles.addArray(midiFilesDir.findChildFiles(File::findFiles, false, searchText + "*.mid"));
    ChangeBroadcaster::sendChangeMessage();
}

void MidiFileListBox::listBoxItemClicked(int rowNumber, const MouseEvent &event) {
    if (event.mods.isPopupMenu())
    {
        juce::PopupMenu::Options options;
        juce::PopupMenu menu;
        menu.addItem ("Remove", [this, rowNumber]()
        {
            File file = midiFiles[rowNumber];
            file.isDirectory() ? file.deleteRecursively() : file.deleteFile();
            midiFiles.remove(rowNumber);
        });

        menu.showMenuAsync (options);
    }

    File file = midiFiles[rowNumber];
    if (onSelectionChanged && !file.isDirectory() && event.getNumberOfClicks() > 1)
        onSelectionChanged (midiFiles[rowNumber]);
}

void MidiFileListBox::labelTextChanged(juce::Label *labelThatHasChanged) {
    std::cout << "value changed: " << labelThatHasChanged->getText() << std::endl;
    searchText = labelThatHasChanged->getText();
    if (update)
        update(searchText);
}
