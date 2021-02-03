/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WhiteNoiseSynthPluginExAudioProcessor::WhiteNoiseSynthPluginExAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    m_soundPlay = false;
}

WhiteNoiseSynthPluginExAudioProcessor::~WhiteNoiseSynthPluginExAudioProcessor()
{
}

//==============================================================================
const juce::String WhiteNoiseSynthPluginExAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WhiteNoiseSynthPluginExAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WhiteNoiseSynthPluginExAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WhiteNoiseSynthPluginExAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WhiteNoiseSynthPluginExAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WhiteNoiseSynthPluginExAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WhiteNoiseSynthPluginExAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WhiteNoiseSynthPluginExAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WhiteNoiseSynthPluginExAudioProcessor::getProgramName (int index)
{
    return {};
}

void WhiteNoiseSynthPluginExAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WhiteNoiseSynthPluginExAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void WhiteNoiseSynthPluginExAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WhiteNoiseSynthPluginExAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WhiteNoiseSynthPluginExAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::MidiMessage message (0x0);
	juce::MidiBuffer::Iterator i (midiMessages);
	int messageFrameRelativeTothisProcess;
	while (i.getNextEvent (message, messageFrameRelativeTothisProcess))
	{
		// here you typically use message
		if (message.isNoteOn()){
            m_soundPlay = true;
		}
		if (message.isNoteOff()){
		    m_soundPlay = false;
		}
		
	}
	
	if(m_soundPlay==false){
	    return;
	}
	
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample){
            channelData[sample] = random.nextFloat() * 0.25f - 0.125f;
        }
    }
}

//==============================================================================
bool WhiteNoiseSynthPluginExAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WhiteNoiseSynthPluginExAudioProcessor::createEditor()
{
    return new WhiteNoiseSynthPluginExAudioProcessorEditor (*this);
}

//==============================================================================
void WhiteNoiseSynthPluginExAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WhiteNoiseSynthPluginExAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WhiteNoiseSynthPluginExAudioProcessor();
}
