## A high-level object-oriented Game Boy Advance bitmap 3D software engine

**What's this?** A GBA MODE4 3D Software Engine - built from scratch.

This is (very) loosely based on David's 3D soft engine in C#/JS: https://www.davrous.com/2013/06/13/tutorial-series-learning-how-to-write-a-3d-soft-engine-from-scratch-in-c-typescript-or-javascript/

Engine blueprint: a stripped-down version of [https://github.com/wgroeneveld/gba-sprite-engine/](https://github.com/wgroeneveld/gba-sprite-engine/) combined with more _tonc_ library functions. 

### Show me the money!

Sure thing. 

#### **Demo 1**: without wires

![design](https://github.com/wgroeneveld/gba-bitmap-engine/blob/master/img/wireless.gif?raw=true)

Camera rotates `2/256` units on x and y each render cycle. 
You 'should' see a cube forming based on 8 vertices. It's a simple example to showcase what the `Mesh` class is about, and how `GBAEngine` handles projections.

60 FPS. Yay.

#### **Demo 1b**: with wires

![design](https://github.com/wgroeneveld/gba-bitmap-engine/blob/master/img/wired.gif?raw=true)

A lot less smooth. 20 FPS. I implemented Bresenham somewhat similar to David's approach, without any good results. 
MODE4's weird byte write problems are causing trouble. Time to consult _tonc_ and fix this:

![design](https://github.com/wgroeneveld/gba-bitmap-engine/blob/master/img/wired2.gif?raw=true)

30 FPS, winning 10 frames using `bmp8_line()`.

#### **Demo 2**: load actual vertex content

From Blender/Babylon based on [this](https://david.blob.core.windows.net/softengine3d/part3/index.html).

A JS script has been provided that generates the needed C++ code. It's not great, and not very performant.

![design](https://github.com/wgroeneveld/gba-bitmap-engine/blob/master/img/monkey.gif?raw=true)

2 FPS. Ouch!! This thing has 507 meshes and 968 faces. GLHF! The GBA CPU does not seem to be very happy with that. Like everyone indeed says:

> GBA Bitmap mode is not for gaming!

Use the `gba-sprite-engine` instead.

I am aware of countless optimization opportunities but even thinking about that makes me sleepy. 
This is a high-level C++ engine, meaning redundant stack objects could also cause problems. And I'm okay with that - it's a proof-of-concept!

#### **Demo 3**: rasterization

The `RasterizerRenderer` class draws triangles as 'fast' as possible, using horizontal scanlines. 
There is a fast way to lines into VRAM. I tried implementing Z-buffering, but the buffer was too big and too slow as z-coords also had to be interpolated... 

![design](https://github.com/wgroeneveld/gba-bitmap-engine/blob/master/img/raster.gif?raw=true)

At this point, I do not think it's that interesting to go on to texture mapping other than the fun of it. Even with a lot of haxx and tricks, the colored monkey won't ever spin at 30FPS... 

#### **Demo 3b**: with back-face culling

It did improve performance. I exported a few Babylon meshes, and the octahedron with 8 faces does run at 20FPS compared to 11FPS when back-face culling was implemented (that omits rendering certain faces if z < 0)

![design](https://github.com/wgroeneveld/gba-bitmap-engine/blob/master/img/octa.gif?raw=true)

Changing colors indicate certain triangles were not drawn (into the background). It does not help a lot with our monkey, alas. Too many vertices...

More examples of meshes (box, cylinder, octahedron, sphere, torus) included.  

### GBA-Specific problems  

**Fixed-point math** sums up things nicely. 

Numbers should be `.8f`, so lots of shifting is needed. Problem with `Sqrt()` BIOS functions is that they output `.8f` but require `.16f`?
So, calculating the length of a vector:

```C
inline FIXED length() const {
    FIXED toRoot = fxmul(v.x, v.x) + fxmul(v.y, v.y) + fxmul(v.z, v.z);
    return Sqrt(toRoot << 8);
}
```

Second problem, sin/cos are expensive so we use _tonc_'s lookup tables - but they also come with weird requirements. 

For instance, input is normally in radians. But we have fixed-point radians. And the lookup table is filled in `[1-512]` slices but actually requires input in `[1-FFFFh]`:

```C
INLINE FIXED fxrad2lut(FIXED rad) {
    int scale = fx2float(rad) / (2*M_PI / 512);
    return (scale << 6) * 2;
}

INLINE FIXED fxsin(FIXED fxrad) {
    if(fxrad == 0) return 0;
    FIXED theta = fxrad2lut(fxrad);
    FIXED sin = lu_sin(theta);
    return fx12Tofx8(sin);
}
```

Divisions are a bit of a mess, but converting them to fixed-point and going back is not going to help much. 
More details in `math.h`.

In any case, lots of rounding errors occur. It is luckily not a problem due to GBA's limited screen dimensions. 

**Limited iWRAM** size is another problem. Use `const` arrays as much as possible!