# Cell - Tools
This section contains some additional tools for developers.

### SteamOS toolchain
Cross-compile toolchain mainly intended for use with SteamOS' runtime SDK.  
Requires grabbing the `com.valvesoftware.SteamRuntime.Sdk-amd64,i386-sniper-sysroot.tar.gz` tarball from [Valve's image repository](https://repo.steampowered.com/steamrt-images-sniper/snapshots).

Compilation can be performed as such, this script is written in PowerShell and was meant for Windows.  
This build was done using SDK version `0.20240321.82093`. You may need to adjust paths and symlinks (e.g linking `/usr/lib` to `/lib` and such), depending on your platform.

You will have to install the Linux target for rustc on Windows, this can be done as such:
```ps1
rustup target add x86_64-unknown-linux-gnu
```
The toolchain automatically picks this triple.

```ps1
# Run this script within the cloned project root
# I assume you're also using the ThirdParty folder in the project root for random garbage storage

$STEAMOS_ROOT="$PWD/ThirdParty/steamos-sniper-sysroot"

cmake `
    -S $PWD `
    -B "$PWD/build-steamos" `
    -G Ninja `
    "-DCMAKE_BUILD_TYPE=Release" `
    "-DSTEAMOS_SYSROOT=$STEAMOS_ROOT" `
    "-DCMAKE_TOOLCHAIN_FILE=$PWD/Tools/SteamOS.toolchain" `
    "-Dwayland_LIBRARY=$STEAMOS_ROOT/lib/x86_64-linux-gnu/libwayland-client.so.0.3.0" `
    "-Dxkbcommon_LIBRARY=$STEAMOS_ROOT/lib/x86_64-linux-gnu/libxkbcommon.so.0.0.0" `
    "-DPulseAudio_LIBRARY=$STEAMOS_ROOT/lib/x86_64-linux-gnu/libpulse.so.0.23.0" `
    "-DVulkan_LIBRARY=$STEAMOS_ROOT/lib/x86_64-linux-gnu/libvulkan.so.1.3.239" `
    "-Dspirv-link_PROGRAM=$env:VULKAN_SDK/Bin/spirv-link.exe" # CMake will grab spirv-link from the SDK if you don't specify it yourself!
```

## Note: While this allows to run Cell on SteamOS, at this point, gamescope does not support Wayland clients yet. You can only run Cell within a Plasma Wayland session.
