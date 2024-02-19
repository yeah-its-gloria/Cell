# SPDX-FileCopyrightText: Copyright 2023-2024 Gloria G.
# SPDX-License-Identifier: BSD-2-Clause

include(FindPackageHandleStandardArgs)

cmake_host_system_information(RESULT WINDOWS_SDK_ROOT QUERY WINDOWS_REGISTRY "HKLM/SOFTWARE/Microsoft/Windows Kits/Installed Roots" VALUE "KitsRoot10")
cmake_host_system_information(RESULT WINDOWS_SDK_VERSIONS QUERY WINDOWS_REGISTRY "HKLM/SOFTWARE/Microsoft/Windows Kits/Installed Roots" SUBKEYS)
list(GET WINDOWS_SDK_VERSIONS 0 WINDOWS_SDK_VERSION)

mark_as_advanced(WINDOWS_SDK_VERSION WINDOWS_SDK_VERSIONS WINDOWS_SDK_ROOT)
