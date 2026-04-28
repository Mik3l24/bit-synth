#include "MixChannelParameters.h"
#include "StructureEditor.h"

namespace ui
{
MixChannelParameters::MixChannelParameters(const ElementID _id, const SynthStateManager _state_manager)
    : SynthElement(_id, ElementCategory::SINK, _state_manager)
{
    level_slider.reset(new juce::Slider("phase_knob"));
    addAndMakeVisible(level_slider.get());
    level_slider->setRange(0, 1, 0);
    level_slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    level_slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);

    level_label.reset(new juce::Label("new label",
                                      TRANS("Level")));
    addAndMakeVisible(level_label.get());
    level_label->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    level_label->setJustificationType(juce::Justification::centred);
    level_label->setEditable(false, false, false);
    level_label->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    level_label->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    target.reset(new TargetConnector(createConnectionID(id, 0, SIGN_SINK)));
    addAndMakeVisible(target.get());
    target->addListener(this);
    target->setBounds(0, 40, 10, 10);

    {
        const juce::ValueTree tree = state_manager.parameters.state
                                     .getChildWithName(Name::SINKS)
                                     .getChildWithProperty(Name::ID, id);
        jassert(tree.isValid());
        const juce::String level_parameter_id = tree[Name::LEVEL].toString();
        level_attachment = std::make_unique<juce::SliderParameterAttachment>(
            *state_manager.parameters.getParameter(level_parameter_id), *level_slider);
    }

    setSize(96, 96);

    snapAndSavePosition();
}

MixChannelParameters::~MixChannelParameters()
{
    level_attachment = nullptr; // Must be destroyed before the slider it is attached to.
    level_slider = nullptr;
    level_label = nullptr;
    target = nullptr;
}

void MixChannelParameters::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff505050));
    {
        int x = 0, y = 0, width = proportionOfWidth(1.0000f), height = proportionOfHeight(1.0000f);
        juce::Colour fillColour = juce::Colour(0xffc4c4c4);
        juce::Colour strokeColour = juce::Colour(0xff787878);
        g.setColour(fillColour);
        g.fillRect(x, y, width, height);
        g.setColour(strokeColour);
        g.drawRect(x, y, width, height, 4);
    }
}

void MixChannelParameters::resized()
{
    level_slider->setBounds(proportionOfWidth(0.1667f), getHeight() - 96, 64, 72);
    level_label->setBounds(proportionOfWidth(0.1667f) + 64 / 2 - (48 / 2), (getHeight() - 96) + 64, 48, 16);
}

std::optional<std::pair<juce::Point<int>, juce::Point<int>>> MixChannelParameters::getConnectionPoints() const
{
    return target->getConnectionPoints();
}

void MixChannelParameters::mouseDown(const juce::MouseEvent& e)
{
    dragger.startDraggingComponent(this, e);
}

void MixChannelParameters::mouseDrag(const juce::MouseEvent& e)
{
    dragger.dragComponent(this, e, nullptr);
    if(e.mouseWasDraggedSinceMouseDown())
        repaintEditor();
}

void MixChannelParameters::mouseUp(const juce::MouseEvent&)
{
    snapAndSavePosition();
}

void MixChannelParameters::connectionMade(TargetConnector* connector, const ConnectionID source_id)
{
    jassert(connector == target.get());
    state_manager.setConnection(source_id, connector->id);
}
}
