#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "ElementPicker.h"
#include "Gate.h"
#include "OscillatorParameters.h"
#include "MixChannelParameters.h"
#include "synth_management/SynthStateManager.h"

namespace ui
{

class StructureEditor:
    public juce::Component,
    public juce::DragAndDropTarget,
    public juce::DragAndDropContainer
{
public:
    explicit StructureEditor(const SynthStateManager& state_manager);
    ~StructureEditor() override;

    // Overrides
    bool isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails) override;
    void itemDropped(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails) override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void rebuildUI();

private: // Methods
    void addNewElement(const juce::Point<int> position, const ElementCategory element_type, const GateType gate_type = GateType::NONE)
    {
        const ElementID id = state_manager.addElementRep(element_type, gate_type);
        addElementComponent(id, position, element_type, gate_type);
    }

    void addElementComponent(ElementID id, juce::Point<int> position, ElementCategory element_type, GateType gate_type = GateType::NONE, bool position_is_center = true);

    [[nodiscard]] OscillatorParameters* findGeneratorByID(ElementID id) const;
    [[nodiscard]]                 Gate* findProcessorByID(ElementID id) const;
    [[nodiscard]] MixChannelParameters* findSinkByID(ElementID id) const;

    [[nodiscard]] SourceConnector* findSourceByID(ConnectionID id) const;


private: // Members
    SynthStateManager state_manager;

    std::vector<std::unique_ptr<OscillatorParameters>> osc_components;
    std::vector<std::unique_ptr<Gate>> processor_components;
    std::vector<std::unique_ptr<MixChannelParameters>> mix_components;

    std::unique_ptr<ElementPicker> picker;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StructureEditor)
};

}
