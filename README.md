# DiscordGameSDK

[![release](https://img.shields.io/github/release/TeamCyberless/DiscordGameSDK.svg)](https://github.com/TeamCyberless/DiscordGameSDK/releases)
[![issues](https://img.shields.io/github/issues/TeamCyberless/DiscordGameSDK.svg)](https://github.com/TeamCyberless/DiscordGameSDK/issues)

An unofficial Unreal Engine 5 plugin that integrates Discord's GameSDK.

## How to install
 1. Download and copy the repository to your project `[YOUR_PROJECT_NAME]/Plugins/` folder.
 2. Download [Discord GameSDK](https://discord.com/developers/docs/game-sdk/sdk-starter-guide).
 3. At `[YOUR_PROJECT_NAME]/Plugins/DiscordGameSDK/Source/ThirdParty/DiscordGameSDKLibrary/`, create an `Include` folder and copy `.h` files of the `cpp/` folder to this folder from Discord GameSDK zip file.
 4. Follow the steps below for each OS
 5. Build your UE4 project
 6. Launch the editor, and enable this plugin.

#### Windows
 - At `[YOUR_PROJECT_NAME]/Plugins/DiscordGameSDK/Source/ThirdParty/DiscordGameSDKLibrary/`, create a `Win64` folder
 - Copy `lib/x86_64/discord_game_sdk.dll` and `lib/x86_64/discord_game_sdk.dll.lib` from Discord GameSDK zip file to the `Win64` folder

#### Mac
- At `[YOUR_PROJECT_NAME]/Plugins/DiscordGameSDK/Source/ThirdParty/DiscordGameSDKLibrary/`, create a `Mac` folder
- Copy `lib/x86_64/discord_game_sdk.dylib` and `lib/x86_64/discord_game_sdk.bundle` from Discord GameSDK zip file to the `Mac` folder

#### Linux
- At `[YOUR_PROJECT_NAME]/Plugins/DiscordGameSDK/Source/ThirdParty/DiscordGameSDKLibrary/`, create a `Linux` folder
- Inside, create another folder `x86_64-unknown-linux-gnu`
- Copy `lib/x86_64/discord_game_sdk.so` from Discord GameSDK zip file to the `Linux/x86_64-unknown-linux-gnu` folder

## License
Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt or copy at https://opensource.org/licenses/MIT)

## How to contribute
### GitHub website
The most efficient way to help and contribute to this wrapper project is to
use the tools provided by GitHub:
- please fill bug reports and feature requests here: https://github.com/TeamCyberless/DiscordGameSDK/issues
- fork the repository, make some small changes and submit them with independent pull-requests

### Contact
- You can use the [Unreal Engine forums](https://forums.unrealengine.com/).
- You can also join our [Discord server](https://discord.gg/RtKCBfQYfj).

### Coding Style Guidelines
The source code follow the Unreal Engine official [Coding Standard](https://docs.unrealengine.com/latest/INT/Programming/Development/CodingStandard/index.html):
- CamelCase naming convention, with a prefix letter to differentiate classes ('F'), interfaces ('I'), templates ('T')
- files (.cpp/.h) are named like the class they contains
- Doxygen comments, documentation is located with declaration, on headers
- Use portable common features of C++11 like nullptr, auto, range based for, override keyword
- Braces on their own line
- Tabs to indent code, with a width of 4 characters

## See also
[Discord GameSDK Documentation](https://discord.com/developers/docs/game-sdk/sdk-starter-guide)

[Unreal Engine Third Party Documentation](https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/BuildTools/UnrealBuildTool/ThirdPartyLibraries/)
