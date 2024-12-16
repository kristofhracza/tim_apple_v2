# Tim Apple v2
This is my [external cheat's](https://github.com/kristofhracza/tim_apple) internal version.				

As always, this is made for educational purposes only!


> [!NOTE]  
> Do not add issues without a comprehensive explanation and/or log. Refer to the [FAQ](#faq) section for more info!

![](screenshots/1.png)


# Compiling From Source
Open `tim_apple_v2.sln` in Visual Studio 2022, set build configuration to `Release | x64`.      
Press **Build solution** and you should receive `tim_apple_v2.dll` file.


# To-Do & Known Issues
**These MIGHT be implemented and/or fixed. It's dependent on the amount of time I have**
## Issues and Bugs
- The cheat crashes sometimes for reasons unknown
- Weapon icons are not mapped to weapon names 100% correctly
- Anti Aim shakes player model when it jitters
- Cursors disappears after the menu has been opened. *(imgui flags)*
- Logging isn't the best

## Feature Ideas
- Chams
	- Multiple materials
- Auto hitbox selector for aimbot -- *(it will choose the closest bone to the crosshair)*
- Key binding for all features
- Smoke radius
- Auto hit
- Better AA
- Hit marker
- Molotov colour modulation
- Map and sky colour modulation


# Features
## ESP
- Bounding box (with coloured edges and colour fill)
- Health bar
- Player name
- Weapon icons
- Dropped weapon icons
- Skeleton
- Joints
- Snap lines
- Distance

## Aim
- Normal aimbot (lock)
- Silent aimbot
- Adjustable FOV
- FOV circle
- Hitbox selector
- Smoothing
- Spotted check

### Misc
- Recoil control
- Trigger bot

## Movement
- Anti Aim with *jitter* and adjustable *yaw* and *pitch*
- Bunny hop

## Visuals
### Map Related
- Invisible smoke
- Coloured smoke

## Miscellaneous
- Anti flash


# FAQ
## Is This Safe To Use?
As far as the cheat goes; yes. However, I have gotten banned from multiple accounts. This however could be due to my injector, as it's well known and easily detected.

## What If It Crashes?
Debug it yourself -- **it will most likely be caused by outdated signatures**

## How To Inject?
I used *Extreme Injector* with **manual mapping**. You're welcomed to use any other injector, but make sure it uses manual map.


# Credits
Firstly, a HUGE thanks to the [UC](https://www.unknowncheats.me/forum/counter-strike-2-a/) community for all the help!
- For the absolute UNIT of a code structure and logic: [maecry/asphyxia](https://github.com/maecry/asphyxia-cs2)
- [ocornut/imgui](https://github.com/ocornut/imgui)
- [a2x/cs2-dumper](https://github.com/a2x/cs2-dumper)