// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "GameCubeAdapter/Report.hh"
#include "Result.hh"

#include <Cell/IO/USB.hh>

namespace Runtime::Devices {

class GameCubeAdapter : public Cell::Object {
public:
    // Tries to find an uninitialized GameCube Controller and initializes it.
    CELL_FUNCTION_INTERNAL static Cell::Wrapped<GameCubeAdapter*, Result> Find();

    // Destructs the GameCube Controller instance.
    CELL_FUNCTION_INTERNAL ~GameCubeAdapter();

    // Polls the GameCube Controller.
    CELL_FUNCTION_INTERNAL Result Poll();

    // Returns the report for the given controller.
    CELL_INLINE GameCubeControllerReport GetReportForIndex(const uint8_t index) {
        CELL_ASSERT(index < 4);
        return this->reports[index];
    }

private:
    CELL_INLINE GameCubeAdapter(Cell::IO::USB* CELL_NONNULL device) : device(device) { }

    Cell::IO::USB* device;
    GameCubeControllerReport reports[4];
};

}
