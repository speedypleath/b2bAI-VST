//
// Created by Andrei Gheorghe on 19.08.2022.
//

#include "MIDIFileListBox.h"

MIDIFileListBox::MIDIFileListBox() {
    settings->addChangeListener(this);
}

MIDIFileListBox::~MIDIFileListBox() {
    settings->removeChangeListener(this);
}

void MIDIFileListBox::listBoxItemClicked(int rowNumber, const MouseEvent &event) {
    if (onSelectionChanged)
        onSelectionChanged (rowNumber);
}

void MIDIFileListBox::changeListenerCallback(juce::ChangeBroadcaster *) {
        files = settings->settings.getOrCreateChildWithName ("midi_files", nullptr);
        sendChangeMessage();
}

void MIDIFileListBox::paintListBoxItem(int rowNumber, Graphics &g, int width, int height, bool rowIsSelected) {
    auto bounds = juce::Rectangle<int> (0, 0, width, height);
        if (rowIsSelected)
        {
            g.setColour (juce::Colours::grey);
            g.fillRect (bounds);
        }

        g.setColour (juce::Colours::silver);
        g.drawFittedText (files.getChild (rowNumber).getProperty ("name", "foo").toString(), bounds, juce::Justification::centredLeft, 1);
}

void MIDIFileListBox::setPresetsNode (ValueTree node) {
    files = node;
    sendChangeMessage();
}

int MIDIFileListBox::getNumRows() {
    return files.getNumChildren();
}
