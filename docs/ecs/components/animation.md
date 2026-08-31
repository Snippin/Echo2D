# Animation

## Description

Animates the sprite assuming that the sprite is part of a texture atlas and will
adjust the current uv values based on the current frame.

## Properties

### `Frames (int)`

Number of frames in the animation.

### `FrameRate (int)`

Speed of the animation in frames per second.

### `FrameOffset (int)`

Offset position from start position. Assumes to be 0 if not set.

### `CurrentFrame (int)`

Frame that the animation is currently on.

### `StartTime (int)`

Defaults to the current ticks upon creation. Used in the frame calculations.

### `IsVertical (bool)`

Flag to determine direction of animation scroll.

### `IsLooped (bool)`

Flag to determine if animation is looped.

If true, animation will reset `CurrentFrame` to 0 once it reaches `Frames`.

If false, the user must call `Reset()` function to reset the animation.

## Functions

### `Reset()`

- brief: Resets `CurrentFrame` back to 0 and sets `StartTime` to current running
  ticks

## Constructors

### `Animation(int, int, int, bool, bool)`

- brief: Creates a new animation component
- params: Total frames, frame rate, frame offset, vertical scroll, looped
- return: Newly created `Animation`
