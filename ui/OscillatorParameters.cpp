#include "OscillatorParameters.h"
#include "StructureEditor.h"

namespace ui
{

OscillatorParameters::OscillatorParameters(const ElementID _id, const SynthStateManager _state_manager)
    : SynthElement(_id, ElementCategory::GENERATOR, _state_manager)
{
    pw_slider = std::make_unique<juce::Slider>("pw_slider");
    addAndMakeVisible(pw_slider.get());
    pw_slider->setRange(-0.5, 0.5, 0);
    pw_slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    pw_slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);

    ratio_label.reset(new juce::Label("ratio_label",
                                      TRANS("Ratio")));
    addAndMakeVisible(ratio_label.get());
    ratio_label->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    ratio_label->setJustificationType(juce::Justification::centred);
    ratio_label->setEditable(false, false, false);
    ratio_label->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    ratio_label->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    ratio_label->setBounds(32, 32, 70, 16);

    ratio_slider = std::make_unique<juce::Slider>("ratio_slider");
    addAndMakeVisible(ratio_slider.get());
    ratio_slider->setRange(0.125, 4, 0.01);
    ratio_slider->setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    ratio_slider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    ratio_slider->setBounds(24, 16, 32 + 32, 16);

#if false
    numerator_slider = std::make_unique<juce::Slider>("numerator_slider");
    addAndMakeVisible(numerator_slider.get());
    numerator_slider->setRange(1, 64, 1);
    numerator_slider->setSliderStyle(juce::Slider::LinearBarVertical);
    numerator_slider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    numerator_slider->setSkewFactor(0.5);

    numerator_slider->setBounds(24, 16, 32, 16);
#endif

    phase_slider = std::make_unique<juce::Slider>("phase_knob");
    addAndMakeVisible(phase_slider.get());
    phase_slider->setRange(0, 2, 0);
    phase_slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    phase_slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 80, 20);

#if false
    denominator_slider = std::make_unique<juce::Slider>("denominator_slider");
    addAndMakeVisible(denominator_slider.get());
    denominator_slider->setRange(1, 64, 1);
    denominator_slider->setSliderStyle(juce::Slider::LinearBarVertical);
    denominator_slider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);
    denominator_slider->setSkewFactor(0.5);

    denominator_slider->setBounds(80, 16, 32, 16);
#endif

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

#if false
    transpose_slider = std::make_unique<juce::Slider>("transpose_slider");
    addAndMakeVisible(transpose_slider.get());
    transpose_slider->setRange(-36, 36, 1);
    transpose_slider->setSliderStyle(juce::Slider::LinearBar);
    transpose_slider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 80, 20);

    transpose_slider->setBounds(16, 56, 32, 16);

    transpose_label.reset(new juce::Label("transpose_label",
                                          TRANS("Transpose")));
    addAndMakeVisible(transpose_label.get());
    transpose_label->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    transpose_label->setJustificationType(juce::Justification::centred);
    transpose_label->setEditable(false, false, false);
    transpose_label->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    transpose_label->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    transpose_label->setBounds(0, 80, 70, 16);

    fine_slider = std::make_unique<juce::Slider>("fine_slider");
    addAndMakeVisible(fine_slider.get());
    fine_slider->setTooltip(TRANS("cents"));
    fine_slider->setRange(-100, 100, 1);
    fine_slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    fine_slider->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 40, 20);

    fine_label.reset(new juce::Label("fine_label",
                                     TRANS("Fine")));
    addAndMakeVisible(fine_label.get());
    fine_label->setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
    fine_label->setJustificationType(juce::Justification::centred);
    fine_label->setEditable(false, false, false);
    fine_label->setColour(juce::TextEditor::textColourId, juce::Colours::black);
    fine_label->setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    fine_label->setBounds(64, 80, 70, 16);

#endif

    source_connector = std::make_unique<SourceConnector>(createConnectionID(id, 0, SIGN_GENERATOR));
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
        const juce::String ratio_parameter_id = tree[Name::RATIO].toString();
        ratio_attachment = std::make_unique<juce::SliderParameterAttachment>(
            *state_manager.parameters.getParameter(ratio_parameter_id), *ratio_slider);
        const juce::String pw_parameter_id = tree[Name::PULSE_WIDTH].toString();
        pw_attachment = std::make_unique<juce::SliderParameterAttachment>(
            *state_manager.parameters.getParameter(pw_parameter_id), *pw_slider);
        const juce::String phase_parameter_id = tree[Name::STARTING_PHASE].toString();
        phase_attachment = std::make_unique<juce::SliderParameterAttachment>(
            *state_manager.parameters.getParameter(phase_parameter_id), *phase_slider);
    }

    snapAndSavePosition();
}

OscillatorParameters::~OscillatorParameters()
{
    // Attachments must be deleted before sliders.
    pw_attachment = nullptr;
    phase_attachment = nullptr;
    ratio_attachment = nullptr;

    pw_slider = nullptr;
    ratio_label = nullptr;
    numerator_slider = nullptr;
    phase_slider = nullptr;
    denominator_slider = nullptr;
    phase_label = nullptr;
    pw_label = nullptr;
    transpose_slider = nullptr;
    transpose_label = nullptr;
    fine_slider = nullptr;
    fine_label = nullptr;
    ratio_slider = nullptr;
    ratio_label = nullptr;
    source_connector = nullptr;
}

void OscillatorParameters::paint(juce::Graphics& g)
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

void OscillatorParameters::resized()
{
    pw_slider->setBounds(proportionOfWidth(0.0368f), getHeight() - 90, 64, 72);
    phase_slider->setBounds(proportionOfWidth(0.5000f), getHeight() - 90, 64, 72);
    phase_label->setBounds(proportionOfWidth(0.5000f) + 64 / 2 - (48 / 2), (getHeight() - 90) + 62, 48, 16);
    pw_label->setBounds(proportionOfWidth(0.0368f) + 64 / 2 - (64 / 2), (getHeight() - 90) + 62, 64, 17);
    //fine_slider->setBounds (proportionOfWidth (0.4706f), getHeight() - 132, 64, 32);
    source_connector->setBounds(getWidth() - 8, proportionOfHeight(0.5000f) - (10 / 2), 10, 10);
}

void OscillatorParameters::mouseDown(const juce::MouseEvent& e)
{
    dragger.startDraggingComponent(this, e);
}

void OscillatorParameters::mouseDrag(const juce::MouseEvent& e)
{
    dragger.dragComponent(this, e, nullptr);
    if(e.mouseWasDraggedSinceMouseDown())
        repaintEditor();

}

void OscillatorParameters::mouseUp(const juce::MouseEvent& e)
{
    snapAndSavePosition();
}

inline double OscillatorParameters::getRatio() const
{
    return numerator_slider->getValue() / denominator_slider->getValue();
}

}
