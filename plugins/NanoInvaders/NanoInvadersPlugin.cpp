/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2015 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "DistrhoPlugin.hpp"
#include "DistrhoPluginInfo.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

/**
  Plugin to show how to get some basic information sent to the UI.
 */
class NanoInvadersPlugin : public Plugin
{
public:
    NanoInvadersPlugin()
        : Plugin(kParameterCount, 0, 0)
    {
    }

protected:
    const char *getLabel() const override
    {
        return "NanoInvaders";
    }

    /**
      Get an extensive comment/description about the plugin.
    */
    const char *getDescription() const override
    {
        return "Protect your host from the aliens";
    }

    /**
      Get the plugin author/maker.
    */
    const char *getMaker() const override
    {
        return "Clearly Broken Software";
    }

    /**
      Get the plugin homepage.
    */
    const char *getHomePage() const override
    {
        return "https://github.com/clearly-broken-software/nanoinvaders";
    }

    const char *getLicense() const override
    {
        return "GPLv3";
    }

    uint32_t getVersion() const override
    {
        return d_version(1, 0, 0);
    }

    int64_t getUniqueId() const override
    {
        return d_cconst('n', 'n', 'I', 'v');
    }

    void initParameter(uint32_t index, Parameter &parameter) override
    {
        parameter.hints = kParameterIsAutomable | kParameterIsOutput;
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
    }

    float getParameterValue(uint32_t index) const override
    {
        return 1.0f;
    }

    void setParameterValue(uint32_t, float) override
    {
        // this is only called for input parameters, which we have none of.
    }
    void run(const float **inputs, float **outputs, uint32_t frames,
             const MidiEvent *midiEvents, uint32_t midiEventCount)
    {

        for (uint i = 0; i < frames; i++)
        {
            outputs[0][i] = 0.0f;
            outputs[1][i] = 0.0f;
        }
    }

private:
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NanoInvadersPlugin)
};

/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

Plugin *createPlugin()
{
    return new NanoInvadersPlugin();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
