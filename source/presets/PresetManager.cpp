#include "PresetManager.h"
#include "FactoryPresets.h"
#include "../parameters/ParameterIDs.h"
#include "../utils/Constants.h"

PresetManager::PresetManager (juce::AudioProcessorValueTreeState& vts)
    : apvts (vts)
{
    rebuildPresetList();
}

void PresetManager::rebuildPresetList()
{
    allPresets = FactoryPresets::getAll();
    auto userPresets = loadUserPresetsFromDisk();
    allPresets.insert (allPresets.end(), userPresets.begin(), userPresets.end());
}

std::vector<Preset> PresetManager::loadUserPresetsFromDisk() const
{
    std::vector<Preset> userPresets;
    auto dir = getUserPresetsDirectory();
    if (! dir.isDirectory()) return userPresets;

    for (auto& file : dir.findChildFiles (juce::File::findFiles, false, "*.xml"))
    {
        Preset p;
        p.name = file.getFileNameWithoutExtension();
        p.category = "User";
        p.isFactory = false;
        p.sourceFile = file;
        userPresets.push_back (std::move (p));
    }

    std::sort (userPresets.begin(), userPresets.end(),
        [] (const Preset& a, const Preset& b) { return a.name.compareIgnoreCase (b.name) < 0; });

    return userPresets;
}

juce::File PresetManager::getUserPresetsDirectory() const
{
    auto appData = juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory);
#if JUCE_MAC
    auto dir = appData.getChildFile ("Application Support").getChildFile ("Advanced Compressor").getChildFile ("User Presets");
#else
    auto dir = appData.getChildFile ("Advanced Compressor").getChildFile ("User Presets");
#endif
    return dir;
}

std::vector<juce::String> PresetManager::getCategories() const
{
    std::vector<juce::String> cats;
    for (auto& p : allPresets)
        if (std::find (cats.begin(), cats.end(), p.category) == cats.end())
            cats.push_back (p.category);
    return cats;
}

std::vector<const Preset*> PresetManager::getPresetsInCategory (const juce::String& category) const
{
    std::vector<const Preset*> result;
    for (auto& p : allPresets)
        if (p.category == category)
            result.push_back (&p);
    return result;
}

void PresetManager::applyPreset (int presetIndex)
{
    if (presetIndex < 0 || presetIndex >= (int) allPresets.size()) return;

    auto& preset = allPresets[(size_t) presetIndex];

    if (! preset.isFactory && preset.sourceFile.existsAsFile())
        applyUserPresetFromFile (preset.sourceFile);
    else
    {
        resetToDefaults();
        applyPresetData (preset.data);
    }

    currentIndex = presetIndex;
    notifyListeners();
}

void PresetManager::applyUserPresetFromFile (const juce::File& file)
{
    auto xml = juce::XmlDocument::parse (file);
    if (xml == nullptr) return;
    auto newState = juce::ValueTree::fromXml (*xml);
    if (newState.isValid())
        apvts.replaceState (newState);
}

void PresetManager::resetToDefaults()
{
    auto setParam = [this] (const char* id, float value)
    {
        if (auto* p = apvts.getParameter (id))
            p->setValueNotifyingHost (p->convertTo0to1 (value));
    };

    setParam (ParamIDs::kThreshold, Constants::kDefaultThreshold);
    setParam (ParamIDs::kRatio, Constants::kDefaultRatio);
    setParam (ParamIDs::kKnee, Constants::kDefaultKnee);
    setParam (ParamIDs::kRange, Constants::kDefaultRange);
    setParam (ParamIDs::kAttack, Constants::kDefaultAttack);
    setParam (ParamIDs::kRelease, Constants::kDefaultRelease);
    setParam (ParamIDs::kHold, Constants::kDefaultHold);
    setParam (ParamIDs::kMix, 100.0f);
    setParam (ParamIDs::kOutputGain, 0.0f);
    setParam (ParamIDs::kWetGain, 0.0f);
    setParam (ParamIDs::kInputGain, 0.0f);
}

void PresetManager::applyPresetData (const CompressorPresetData& data)
{
    auto setParam = [this] (const char* id, float value)
    {
        if (auto* p = apvts.getParameter (id))
            p->setValueNotifyingHost (p->convertTo0to1 (value));
    };

    setParam (ParamIDs::kThreshold, data.threshold);
    setParam (ParamIDs::kRatio, data.ratio);
    setParam (ParamIDs::kKnee, data.knee);
    setParam (ParamIDs::kRange, data.range);
    setParam (ParamIDs::kAttack, data.attackMs);
    setParam (ParamIDs::kRelease, data.releaseMs);
    setParam (ParamIDs::kHold, data.holdMs);

    if (auto* p = apvts.getParameter (ParamIDs::kCompStyle))
        p->setValueNotifyingHost (p->convertTo0to1 ((float) data.style));

    setParam (ParamIDs::kMix, data.mix);
    setParam (ParamIDs::kOutputGain, data.outputGain);
}

bool PresetManager::saveCurrentAsPreset (const juce::String& name)
{
    if (name.isEmpty()) return false;

    auto dir = getUserPresetsDirectory();
    dir.createDirectory();

    auto safeName = juce::File::createLegalFileName (name);
    auto file = dir.getChildFile (safeName + ".xml");

    auto state = apvts.copyState();
    auto xml = state.createXml();
    if (xml == nullptr) return false;

    if (! xml->writeTo (file)) return false;

    rebuildPresetList();

    for (int i = 0; i < (int) allPresets.size(); ++i)
    {
        if (! allPresets[(size_t) i].isFactory && allPresets[(size_t) i].name == safeName)
        {
            currentIndex = i;
            break;
        }
    }

    notifyListeners();
    return true;
}

bool PresetManager::deleteUserPreset (int presetIndex)
{
    if (presetIndex < 0 || presetIndex >= (int) allPresets.size()) return false;
    auto& preset = allPresets[(size_t) presetIndex];
    if (preset.isFactory) return false;
    if (! preset.sourceFile.deleteFile()) return false;

    if (currentIndex == presetIndex) currentIndex = -1;
    else if (currentIndex > presetIndex) --currentIndex;

    rebuildPresetList();
    notifyListeners();
    return true;
}

void PresetManager::rescanUserPresets()
{
    auto currentName = getCurrentPresetName();
    rebuildPresetList();
    currentIndex = -1;
    for (int i = 0; i < (int) allPresets.size(); ++i)
    {
        if (allPresets[(size_t) i].name == currentName)
        {
            currentIndex = i;
            break;
        }
    }
    notifyListeners();
}

void PresetManager::nextPreset()
{
    if (allPresets.empty()) return;
    int next = currentIndex + 1;
    if (next >= (int) allPresets.size()) next = 0;
    applyPreset (next);
}

void PresetManager::previousPreset()
{
    if (allPresets.empty()) return;
    int prev = currentIndex - 1;
    if (prev < 0) prev = (int) allPresets.size() - 1;
    applyPreset (prev);
}

juce::String PresetManager::getCurrentPresetName() const
{
    if (currentIndex >= 0 && currentIndex < (int) allPresets.size())
        return allPresets[(size_t) currentIndex].name;
    return "Init";
}

void PresetManager::notifyListeners()
{
    listeners.call ([] (Listener& l) { l.presetChanged(); });
}
