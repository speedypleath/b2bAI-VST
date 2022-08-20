//
// Created by Andrei Gheorghe on 19.08.2022.
//

#include "JuceHeader.h"
#pragma once

class MidiFileListBox: public ListBoxModel,
                       public Label::Listener,
                       public ChangeBroadcaster,
                       public ChangeListener {
private:
    File midiFilesDir;
    Array<File> midiFiles;
    foleys::SharedApplicationSettings settings;
    String searchText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiFileListBox)
public:
    MidiFileListBox();
    ~MidiFileListBox() override;

    void listBoxItemClicked (int rowNumber, const juce::MouseEvent& event) override;
    void listBoxItemDoubleClicked(int row, const juce::MouseEvent &) override;
    void paintListBoxItem (int rowNumber, juce::Graphics &g, int width, int height, bool rowIsSelected) override;

    void labelTextChanged(juce::Label *labelThatHasChanged) override;
    int getNumRows() override;
    void changeListenerCallback (juce::ChangeBroadcaster*) override;
    std::function<void(File file)> onSelectionChanged;
    std::function<void(File file)> onDoubleClick;
    std::function<void(String text)> update;
};
