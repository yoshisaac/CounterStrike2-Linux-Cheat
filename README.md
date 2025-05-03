# How to compile
### Cloning Repository
Clone the repo using `git`, and change your directory into the root of the project.  

```console
$ git clone https://github.com/yoshisaac/CounterStrike2-Linux-Cheat.git  
$ cd CounterStrike2-Linux-Cheat/
```

### Installing dependencies
Install the necessary packages.  

Debian/Ubuntu:
``` console
$ sudo apt install mesa-common-dev libx11-dev libxfixes-dev libxcomposite-dev gcc g++ make qt6-base-dev
```
  
Arch Linux:
``` console
$ sudo pacman -S base-devel xorg-fonts-misc qt6-base
```

Fedora: (only tested on Fedora 42)
``` console
$ sudo dnf install g++ libX11-devel libXtst-devel libXcomposite-devel qt6-qtbase-devel
```

### Compiling
Build the program from source.  

```console
$ make
...
```

### Running

```console
$ sudo ./cs2-hack
```

# Features
- ESP
  * Box
  * Health
  * Name
  * Skeleton
  * Head dot
  * Ears
  * Snap lines
- Visuals
  * Crosshair
  * Crosshair recoil
  * Sniper crosshair
- Aimbot
  * FOV
  * Recoil control
  * Auto shoot
  
![Screenshot of esp full](https://i.imgur.com/y9Nq2Tg.png)
![Screenshot of esp ideal](https://i.imgur.com/lBKVnd6.png)
![Screenshot of menu esp](https://i.imgur.com/NtTMRDW.png)
![Screenshot of menu aimbot](https://i.imgur.com/FYB7jJf.png)

# Compatibility
This hack strictly uses X11 as of right now. To make the ESP overlay window be as convient to use as possible, in the least hacky manor, I've used lower level X11 attributes, etc.  
If you use a Wayland session for your Desktop Environment, then XWayland and the Desktop's compositor should handle everything just fine.  
  
If you are using something like Hyperland, then the composition done has no good XWayland support for features like double buffering. Therefor, the hack does not run correctly.  
  
Using [glfw](https://github.com/glfw/glfw) would fix this, but making an overlay which has complete feature parody would be much harder. Something to be done in the future.  

# TODO
- native Wayland
