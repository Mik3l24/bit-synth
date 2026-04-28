#include "Gate.h"

#include "Theme.h"
#include "StructureEditor.h"


namespace ui {

Gate::Gate(const ElementID _id, const GateType _type, const SynthStateManager _state_manager)
    : SynthElement(_id, ElementCategory::PROCESSOR, _state_manager), type(_type)
{
    source = std::make_unique<SourceConnector>(createConnectionID(id, 0, SIGN_PROCESSOR));
    addAndMakeVisible(source.get());
    source->setName("source");

    source->setCentrePosition(86, 50);

    target0 = std::make_unique<TargetConnector>(createConnectionID(id, 0, SIGN_PROCESSOR));
    addAndMakeVisible(target0.get());
    target0->setName("target0");
    target0->addListener(this);

    if(type != GateType::NOT)
    {
        target0->setCentrePosition(13, 41);

        target1 = std::make_unique<TargetConnector>(createConnectionID(id, 1, SIGN_PROCESSOR));
        addAndMakeVisible(target1.get());
        target1->setName("target1");
        target1->addListener(this);

        target1->setCentrePosition(13, 59);
    }
    else // if(type == GateType::NOT)
    {
        target0->setCentrePosition(13, 50);
        target1 = nullptr;
    }

    switch(type) // Body
    {
    case GateType::AND:
        bodyPath.startNewSubPath(30.0f, 30.0f);
        bodyPath.lineTo(54.0f, 30.0f);
        bodyPath.quadraticTo(72.0f, 30.0f, 72.0f, 50.0f);
        bodyPath.quadraticTo(72.0f, 70.0f, 54.0f, 70.0f);
        bodyPath.lineTo(30.0f, 70.0f);
        bodyPath.closeSubPath();
        break;
    case GateType::XOR:
        // The additional curve
        bodyPath.startNewSubPath(19.0f, 69.0f);
        bodyPath.quadraticTo(28.0f, 63.0f, 28.0f, 49.0f);
        bodyPath.quadraticTo(28.0f, 37.0f, 20.0f, 30.0f);
    case GateType::OR:
        // The common _or body
        bodyPath.startNewSubPath(45.0f, 29.0f);
        bodyPath.quadraticTo(64.0f, 29.0f, 70.0f, 50.0f);
        bodyPath.quadraticTo(63.0f, 70.0f, 46.0f, 70.0f);
        bodyPath.lineTo(28.0f, 70.0f);
        bodyPath.quadraticTo(36.0f, 63.0f, 36.0f, 49.0f);
        bodyPath.quadraticTo(36.0f, 37.0f, 29.0f, 29.0f);
        bodyPath.closeSubPath();
        break;
    case GateType::NOT:
        bodyPath.startNewSubPath(28.0f, 33.0f);
        bodyPath.lineTo(63.0f, 50.0f);
        bodyPath.lineTo(28.0f, 67.0f);
        bodyPath.closeSubPath();
        break;
    default:
        // Crossed out
        bodyPath.startNewSubPath(30.0f, 30.0f);
        bodyPath.lineTo(70.0f, 70.0f);
        bodyPath.closeSubPath();
        bodyPath.startNewSubPath(30.0f, 70.0f);
        bodyPath.lineTo(70.0f, 30.0f);
        bodyPath.closeSubPath();
        break;
    }

    switch(type) // Target tips
    {
    case GateType::AND:
    case GateType::XOR:
        // target1 tip
        connectorsPath.setUsingNonZeroWinding(false);
        connectorsPath.startNewSubPath(13.0f, 59.0f);
        connectorsPath.lineTo(27.0f, 59.0f);
        connectorsPath.closeSubPath();

        // target0 tip
        connectorsPath.setUsingNonZeroWinding(false);
        connectorsPath.startNewSubPath(13.0f, 41.0f);
        connectorsPath.lineTo(27.0f, 41.0f);
        connectorsPath.closeSubPath();
        break;
    case GateType::OR: // OR tips are slightly longer
        connectorsPath.setUsingNonZeroWinding(false);
        connectorsPath.startNewSubPath(13.0f, 59.0f);
        connectorsPath.lineTo(34.0f, 59.0f);
        connectorsPath.closeSubPath();

        connectorsPath.setUsingNonZeroWinding(false);
        connectorsPath.startNewSubPath(13.0f, 41.0f);
        connectorsPath.lineTo(34.0f, 41.0f);
        connectorsPath.closeSubPath();
        break;
    case GateType::NOT:
        connectorsPath.setUsingNonZeroWinding(false);
        connectorsPath.startNewSubPath(13.0f, 50.0f);
        connectorsPath.lineTo(26.0f, 50.0f);
        connectorsPath.closeSubPath();
        break;
    default:
        break;
    }

    // Source tip
    connectorsPath.startNewSubPath(72.0f, 50.0f);
    connectorsPath.lineTo(86.0f, 50.0f);
    connectorsPath.closeSubPath();


    setSize(100, 100);

    snapAndSavePosition();
}

Gate::~Gate()
{
    source = nullptr;
    target0 = nullptr;
    target1 = nullptr;
}

//==============================================================================
void Gate::paint(juce::Graphics& g)
{
    const juce::Colour strokeColour = Theme::getStructureLogicForeground();
    const juce::Colour fillColour = Theme::getStructureBackground();
    const float strokeThickness = Theme::getStructureLogicStrokeThickness();
    const float connectionThickness = Theme::getStructureConnectionStrokeThickness();


    g.setColour(strokeColour);

    g.strokePath(bodyPath,
                 juce::PathStrokeType(strokeThickness, juce::PathStrokeType::JointStyle::curved,
                                      juce::PathStrokeType::EndCapStyle::rounded),
                 juce::AffineTransform::translation(0, 0));
    g.strokePath(connectorsPath,
                 juce::PathStrokeType(connectionThickness, juce::PathStrokeType::JointStyle::curved,
                                      juce::PathStrokeType::EndCapStyle::rounded),
                 juce::AffineTransform::translation(0, 0));

    if(type == GateType::NOT)
    {
        constexpr float x = 61.0f, y = 45.0f, width = 10.0f, height = 10.0f;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
        g.setColour(strokeColour);
        g.drawEllipse(x, y, width, height, 3.500f);
    }
}

void Gate::resized() {}

void Gate::connectionMade(TargetConnector* connector, const ConnectionID source_id)
{
    state_manager.setConnection(source_id, connector->id);
}

std::optional<std::pair<juce::Point<int>, juce::Point<int>>> Gate::getConnectionPoints(unsigned int target) const
{
    if(target == 0)
        return target0->getConnectionPoints();
    else if(target == 1 && type != GateType::NOT)
        return target1->getConnectionPoints();
    else
        return std::nullopt;
}

void Gate::mouseDown(const juce::MouseEvent& e)
{
    dragger.startDraggingComponent(this, e);
}

void Gate::mouseDrag(const juce::MouseEvent& e)
{
    dragger.dragComponent(this, e, nullptr);
    if(e.mouseWasDraggedSinceMouseDown())
        repaintEditor();
}

void Gate::mouseUp(const juce::MouseEvent& e)
{
    snapAndSavePosition();
}

}
