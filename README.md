# jork

## Build Instructions

- Install [`vcpkg`](https://vcpkg.io/en/getting-started.html)
- Open the folder in Visual Studio
- Add this to the `windows-base` configuration in `CMakePresets.json`:
```
"toolchainFile": "PATH/TO/vcpkg/scripts/buildsystems/vcpkg.cmake",
```
- Hit `Build > Build All`
