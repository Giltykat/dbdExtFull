# dbdExtFull

A comprehensive external modification framework for Dead by Daylight (Win64) featuring ESP visualization, gameplay modifications, and an ImGui-based interface.

## ⚠️ Disclaimer

This project is for **educational purposes only**. Using game modifications in multiplayer games may violate the game's Terms of Service and can result in account bans. The authors are not responsible for any misuse of this software.

## Features

### ESP (Extra Sensory Perception)
- **Survivor ESP** - Highlight and track survivor locations
- **Killer ESP** - Visualize killer position and movement
- **Generator ESP** - Show generator locations and status
- **Pallet ESP** - Mark pallet positions
- **Window ESP** - Highlight vaultable windows
- **Hook ESP** - Display meat hook locations
- **Customizable Colors** - Configure ESP colors per entity type
- **Corner Boxes** - Optional corner-style bounding boxes
- **Distance Display** - Show distance to entities
- **Name Tags** - Display entity names

### Gameplay Modifications
- **FOV Changer** - Modify field of view
- **Aspect Ratio Changer** - Custom aspect ratio support
- **Auto Skill Check** - Automatic skill check handling
- **Speed Hack** - Adjustable movement speed modifier
- **Infinite Lunge** - Remove lunge cooldown restrictions
- **No Cooldown** - Remove ability cooldowns
- **Instant Bell** - Instant shrine bell interaction
- **Auto Dead Hard** - Automatic Dead Hard activation

### Visual Features
- **Crosshair** - Custom crosshair overlay
- **Aura ESP** - Enhanced aura visualization
- **ImGui Menu** - In-game configuration menu (Toggle with INSERT key)

## Project Structure

```
dbdExtFull/
├── Core/
│   ├── Globals.h      # Global variables and configuration
│   ├── Offsets.h      # Game memory offsets
│   ├── Driver.h       # Kernel driver communication
│   ├── Renderer.h     # DirectX 11 rendering setup
│   └── Utils.h        # Utility functions
├── Features/
│   ├── ESP.h          # ESP/draw functionality
│   ├── Exploits.h     # Gameplay modifications
│   └── Workers.h      # Background update loops
├── Menu/
│   └── Menu.h         # ImGui menu implementation
├── imgui/             # Dear ImGui library
├── main.cpp           # Application entry point
└── dbdExtFull.vcxproj # Visual Studio project file
```

## Requirements

- **Operating System**: Windows 10/11 (x64)
- **Compiler**: MSVC (Visual Studio 2019 or later recommended)
- **Dependencies**:
  - DirectX 11 SDK
  - Windows SDK
  - A compatible kernel driver for memory access

## Supported Platforms

- Steam (`DeadByDaylight-Win64-Shipping.exe`)
- Epic Games Store (`DeadByDaylight-EGS-Shipping.exe`)
- Xbox/MS Store (`DeadByDaylight-WinGDK-Shipping.exe`)

## Building

### Prerequisites

1. Install Visual Studio 2019 or later with C++ development tools
2. Ensure Windows SDK and DirectX 11 are installed
3. Obtain a compatible kernel driver for memory access

### Compilation Steps

1. Open `dbdExtFull.slnx` in Visual Studio
2. Set build configuration to **Release | x64**
3. Build the solution (`Ctrl+Shift+B`)
4. The compiled executable will be in the `x64/Release/` directory

### Manual Build (Command Line)

```bash
cl /EHsc /O2 /I. /link d3d11.lib dxgi.lib dwmapi.lib user32.lib gdi32.lib shell32.lib main.cpp
```

## Usage

1. **Launch the application** after starting Dead by Daylight
2. The overlay will automatically attach to the game window
3. Press **INSERT** to toggle the configuration menu
4. Press **F7** or **END** to exit the application

### Configuration

All settings can be adjusted through the in-game menu:
- ESP toggles and colors
- Exploit activation
- Visual enhancements
- Key bindings

## Technical Details

### Memory Access

The project uses a kernel-level driver for reading and writing process memory. This approach provides:
- Direct physical memory access
- Bypass of standard memory protection mechanisms
- High-performance memory operations

### Rendering

- **DirectX 11** overlay rendering
- **Dear ImGui** for UI components
- Transparent window overlay technique
- Real-time window synchronization with game

### Entity Detection

- Scans the game's actor array via `GWorld` pointer
- Identifies entities by class name patterns
- Updates entity list in background thread
- Maintains entity state across frames

## Key Controls

| Key | Action |
|-----|--------|
| INSERT | Toggle menu |
| F7 | Exit application |
| END | Exit application |

## Configuration Options

### ESP Settings
- `g_espSurvivors` - Enable survivor ESP
- `g_espKillers` - Enable killer ESP
- `g_espGenerators` - Enable generator ESP
- `g_drawEspCornerBox` - Use corner boxes instead of full boxes
- `g_drawLines` - Draw lines to entities
- `g_drawDistance` - Show distance text
- `g_drawName` - Display entity names

### Exploit Settings
- `g_autoSkillcheck` - Auto-complete skill checks
- `g_speedHack` - Enable speed modification
- `g_infiniteLunge` - Remove lunge limitations
- `g_noCooldown` - Remove ability cooldowns
- `g_instantBell` - Instant bell interaction

### Visual Settings
- `g_fovChanger` - Enable FOV modification
- `g_aspectRatioChanger` - Enable custom aspect ratio
- `g_drawCrosshair` - Show custom crosshair
- `g_auraEsp` - Enhanced aura visualization

## Troubleshooting

### Common Issues

1. **Application doesn't start**
   - Ensure Dead by Daylight is running
   - Verify kernel driver is properly installed
   - Run as Administrator

2. **ESP not showing**
   - Check if correct platform offsets are detected
   - Verify game version compatibility
   - Ensure ESP options are enabled in menu

3. **Menu not appearing**
   - Press INSERT key
   - Check if overlay window is created
   - Verify DirectX 11 initialization

4. **Crashes on startup**
   - Update offsets for current game version
   - Check driver compatibility
   - Disable antivirus temporarily (may flag driver)

## Updating Offsets

Game updates frequently change memory offsets. To update:

1. Use a pattern scanner tool (e.g., Cheat Engine, ReClass)
2. Find new addresses for:
   - `GWorld` pointer
   - Key class properties
3. Update values in `Core/Offsets.h`

## License

This project is provided as-is for educational purposes. No warranty is expressed or implied.

## Contributing

Contributions should focus on:
- Code optimization
- Bug fixes
- Documentation improvements
- New non-disruptive features

## Acknowledgments

- [Dear ImGui](https://github.com/ocornut/imgui) - Immediate mode GUI library
- Unreal Engine community for reverse engineering resources
- Dead by Daylight modding community

---

**Note**: Always respect game developers' terms of service and other players' experience. Use responsibly and at your own risk.
