// SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <Cell/String.hh>
#include <Cell/Audio/Result.hh>
#include <Cell/Collection/List.hh>

namespace Cell::Audio {

// Types of supported formats.
enum class FormatType : uint8_t {
    // 32 bit PCM stored in floating point.
    Float32PCM,

    // 16 bit PCM stored as signed integers.
    S16PCM
};

// Contains relevant info for a device.
struct DeviceInfo {
    CELL_FUNCTION ~DeviceInfo();

    // Identifying info.
    // The contained data varies by subsystem, e.g WASAPI uses IDs encoded as wide strings.
    void* identifier;

    // The presentable name of the device, as shown in e.g a sound device UI.
    String name;
};

// Describes a bare audio format.
struct Format {
    FormatType type;
    uint8_t channels;

    // In Hertz.
    uint32_t rate;
};

// Calculates a single sample's byte size for the given format.
CELL_FUNCTION_INTERNAL inline constexpr uint32_t GetSampleByteSize(const Format& format) {
    const uint32_t bits = [&format]{
        switch (format.type) {
        case FormatType::Float32PCM: {
            return 32;
            break;
        }

        case FormatType::S16PCM: {
            return 16;
            break;
        }

        default:  {
            CELL_UNREACHABLE;
        }
        }
    }();

    return (format.channels * bits) / 8;
}

// Base audio subsystem interface.
class ISubsystem : public NoCopyObject {
public:
    CELL_FUNCTION virtual ~ISubsystem() = default;

    // Sets up a renderer with the given format. The given renderer will always output to the currently selected device.
    virtual Wrapped<class IRenderer*, Result> CreateRenderer(const Format& format) = 0;

    // Sets up a renderer with the given format. This allows choosing a specific device.
    virtual Wrapped<class IRenderer*, Result> CreateRenderer(const DeviceInfo& info, const Format& format) = 0;

    // Creates a loopback capturer for the given rendering device, with the selected format.
    virtual Wrapped<class ICapturer*, Result> CreateLoopback(const DeviceInfo& info, const Format& format) = 0;

    // Returns a list of all currently available renderers.
    virtual Wrapped<Collection::List<DeviceInfo>, Result> DiscoverAvailableRenderers() = 0;
};

// Sets up the most suitable subsystem for the current platform.
CELL_FUNCTION Wrapped<ISubsystem*, Result> CreateSubsystem(const String& title);

}
