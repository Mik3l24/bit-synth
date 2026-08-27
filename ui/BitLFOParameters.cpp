#include "BitLFOParameters.h"
#include "StructureEditor.h"

namespace ui
{

BitLFOParameters::BitLFOParameters(const ElementID _id, const SynthStateManager _state_manager)
    : SynthElement(_id, ElementCategory::GENERATOR, _state_manager)
{
    pw_slider = std::make_unique<juce::Slider>("pw_slider");
    addAndMakeVisible(pw_slider.get());
    pw_slider->setRange(-0.5, 0.5, 0);
    pw_slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    pw_slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);

    frequency_label.reset(new juce::Label("frequency_label",
                                      TRANS("Frequency")));
    addAndMakeVisible(frequency_label.get());
    frequency_label->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    frequency_label->setJustificationType(juce::Justification::centred);
    frequency_label->setEditable(false, false, false);
    frequency_label->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    frequency_label->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    frequency_label->setBounds(30, 80, 80, 16);

    frequency_slider = std::make_unique<juce::Slider>("frequency_slider");
    addAndMakeVisible(frequency_slider.get());
    frequency_slider->setRange(0.125, 4, 0.01);
    frequency_slider->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    frequency_slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);
    frequency_slider->setBounds(30, 10, 80, 80);

    phase_slider = std::make_unique<juce::Slider>("phase_knob");
    addAndMakeVisible(phase_slider.get());
    phase_slider->setRange(0, 2, 0);
    phase_slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    phase_slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);

    phase_label.reset(new juce::Label("phase_label",
                                      TRANS("Phase")));
    addAndMakeVisible(phase_label.get());
    phase_label->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    phase_label->setJustificationType(juce::Justification::centred);
    phase_label->setEditable(false, false, false);
    phase_label->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    phase_label->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    pw_label.reset(new juce::Label("pw_label",
                                   TRANS("Pulse Width")));
    addAndMakeVisible(pw_label.get());
    pw_label->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    pw_label->setJustificationType(juce::Justification::centred);
    pw_label->setEditable(false, false, false);
    pw_label->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    pw_label->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    source_connector = std::make_unique<SourceConnector>(createConnectionIDWithSign(id, 0, SIGN_GENERATOR));
    addAndMakeVisible(source_connector.get());
    internalPath1.startNewSubPath(72.0f, 16.0f);
    internalPath1.lineTo(64.0f, 32.0f);
    internalPath1.closeSubPath();

    setSize(136, 180);

    {
        const juce::ValueTree tree = state_manager.parameters.state
                .getChildWithName(Name::GENERATORS)
                .getChildWithProperty(Name::ID, id);
        jassert(tree.isValid());
        const juce::String ratio_parameter_id = tree[Name::FREQUENCY].toString();
        frequency_attachment = std::make_unique<juce::SliderParameterAttachment>(
            *state_manager.parameters.getParameter(ratio_parameter_id), *frequency_slider);
        const juce::String pw_parameter_id = tree[Name::PULSE_WIDTH].toString();
        pw_attachment = std::make_unique<juce::SliderParameterAttachment>(
            *state_manager.parameters.getParameter(pw_parameter_id), *pw_slider);
        const juce::String phase_parameter_id = tree[Name::STARTING_PHASE].toString();
        phase_attachment = std::make_unique<juce::SliderParameterAttachment>(
            *state_manager.parameters.getParameter(phase_parameter_id), *phase_slider);
    }

    snapAndSavePosition();
}

BitLFOParameters::~BitLFOParameters()
{
    // Attachments must be deleted before sliders.
    pw_attachment = nullptr;
    phase_attachment = nullptr;
    frequency_attachment = nullptr;

    pw_slider = nullptr;
    frequency_label = nullptr;
    phase_slider = nullptr;
    phase_label = nullptr;
    pw_label = nullptr;
    frequency_label = nullptr;
    source_connector = nullptr;
}

void BitLFOParameters::paint(juce::Graphics& g)
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
#if 0 // Numerator / Denumerator slash, no longer used.
    {
        float x = 0, y = 0;
        juce::Colour strokeColour = juce::Colour(0xff818d92);
        g.setColour(strokeColour);
        g.strokePath(internalPath1,
                     juce::PathStrokeType(2.700f, juce::PathStrokeType::mitered, juce::PathStrokeType::rounded),
                     juce::AffineTransform::translation(x, y));
    }
#endif
}

void BitLFOParameters::resized()
{
    pw_slider->setBounds(proportionOfWidth(0.0368f), getHeight() - 90, 64, 72);
    phase_slider->setBounds(proportionOfWidth(0.5000f), getHeight() - 90, 64, 72);
    phase_label->setBounds(proportionOfWidth(0.5000f) + 64 / 2 - (48 / 2), (getHeight() - 90) + 62, 48, 16);
    pw_label->setBounds(proportionOfWidth(0.0368f) + 64 / 2 - (64 / 2), (getHeight() - 90) + 62, 64, 17);
    //fine_slider->setBounds (proportionOfWidth (0.4706f), getHeight() - 132, 64, 32);
    source_connector->setBounds(getWidth() - 8, proportionOfHeight(0.5000f) - (10 / 2), 10, 10);
}

void BitLFOParameters::mouseDown(const juce::MouseEvent& e)
{
    dragger.startDraggingComponent(this, e);
}

void BitLFOParameters::mouseDrag(const juce::MouseEvent& e)
{
    dragger.dragComponent(this, e, nullptr);
    if(e.mouseWasDraggedSinceMouseDown())
        repaintEditor();

}

void BitLFOParameters::mouseUp(const juce::MouseEvent& e)
{
    snapAndSavePosition();
}

}
