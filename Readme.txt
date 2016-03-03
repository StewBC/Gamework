1. INTRODUCTION

Gamework is a C++ Arcade Game Framework for SFML.  I was using SFML 2.3.2 while
developing this framework.

The goal of Gamework is to create a starting point for making an arcade game as
they were made in the 80's.  The framework is organized in a few namespaces and
the files in the namespaces are in folders under src.  These are:
a. WC 
b. Gamework
c. plist
d. Examples

The library provides a small amount of abstraction over SFML in certain areas
and also provide some functionality that makes making games easier.  There's a
built-in flow that "ticks over" and is set up for 1 or more players to take
turns playing the game.  There's a high-score table system, a front-end system
which also has a small state machine and there's a layer that abstracts
keyboard and joystick input into a bit-mask.

In general, things in the WC namespace will likely be used as-is and things in
the Gamework namespace will be heavily modified for the specific game being
made.

The plist namespace is for loading resource meta-data from a specific type of
XML file, called a plist.

The Examples are used by the Gamework (mostly the FrontEnd) to render effects
or show the high scores, etc.

2. NAMESPACES 
The namespaces and their implementations are:

a. WC 
The WC namespace is where the graphics and controller abstractions are
found.  The following files implement the WC namespace:

a.1 Render.<c|h>pp
	This file has the implementation of the Render class which sub-classes the
	sf::RenderWindow class.  The Render class primarily has a resource manager
	(m_resourceManager which it may or may not "own", i.e. will or won't free
	when destroyed) and an array of array's called m_renderQueue.  The render
	queues actually hold a small container which points at sprites to be
	rendered.  These queues are filled every frame and then cleaned out at the
	end of the frame when rendering happens.
a.2 SpriteResourceManager.<c|h>pp
	Keeps track of resources loaded.  These resources are really textures and
	animations.  The Textures are Gamework::SpriteTextures and the animations
	are	Gamework::SpriteAnimaion's.
a.3 SpriteTexture.<c|h>pp
	The SpriteTexture class has a name (usually the name of the file that
	contained the texture), an sf::Texture pointer and an array of rectangles. 
	The Rectangles (there's always at least one) frame the "sprites" inside the
	texture.  The texture can thus be a complete image or it is a sprite page
	with multiple images (perhaps an animation) all in one texture.
a.4. SpriteAnimation.<c|h>pp
	A SpriteAnimation has a name (name of the Animation), a pointer to a
	SpriteTexture and an array of indices.  The indices are indexes into the
	rectangle frames in the SpriteAnimation.  These indices indicate which
	frames, and in what order, make up the named animation.
a.5 AnimatedSprite.<c|h>pp
	The AnimatedSprite class is a class that does the actual work of animating
	a sprite.  A sprite can be set to animate the frames in an animation
	forwards or backwards, to loop the animation or to play the animation
	forwards and backwards in a ping-pong fashion.  The animation is also set
	to play back at some fixed speed.
a.6 InputManager.<c|h>pp
	The input manager takes an InputKeyMap or InputJoyMap and maps specified
	keys or joystick actions into a single bitmask that represent actions being
	on or off.  In other words, everything is turned into digital inputs, even
	analog joystick ranges.  Joystick support is not well tested but it does
	work with a XBox 360 joystick and should work with most joysticks.  In this
	implementation, the dead zone is set at a value of 50, so 50% through the
	range of 0 to 100 that SFML sets for joystick ranges.
a.7 EffectsMgs.<c|h>pp
	The effects manager is a framework to create fire and forget effects.  An
	effect is told for how long it should run and at what point, optionally,
	the effect should start fading.  The effects manager will then run the
	effect as specified and will clean it up when done.

b. Gamework
The Gamework namespace contains elements of the game that is specific to the
implementation of the game.  The following files are in the Gamework
namespace:
b.1 Globals.<h|h>pp
	There are a few global pointers, such as gGame, gRenderer, gEffectsMgr and
	gInputMgr through which these components can be accessed from anywhere.
b.2 Game.<c|h>pp
	The main state machine controlling the flow of the game.  This also sets up
	the input mappings for the input manager, etc.  It's the heart of the game
	and will be massively expanded when creating a game.  Even though it's one
	file now, this will become many files in a game.
b.3 FrontEnd.<c|h>pp
	The state machine that controls the pre & post game information. In other
	words, everything that is shown or that the user interacts with while not
	playing flows through this module.  As with Game, this will probably also
	expand to several files.
b.4 LevelManager.<c|h>pp
	This is the most basic shell.  A stub really for where level loading will
	happen.
b.5 HighScoreTable.<c|h>pp
	Keeps track of a table of high scores.  Holds scores and names or initials.
	Scores can be loaded from disk or saved to disk.

c. plist
This namespace really exists only because I chose to specify resources in a
plist file format. Plist files are OS X type files for containers.  Since there
isn't, as far as I know, a nice equivalent for Windows, I implemented a plist
type file handler.  This is built on rapidxml.hpp (Copyright (C) 2006, 2009
Marcin Kalicinski).

The plist format is thus an XML format but you have array's, dictionaries,
strings and integers.  See the included res/sprites.plist.  The high-scores
are saved in this format as well in the included code - in other words, plists
can be read and written.  It's not nearly as nice as it is under OS X, but it
works.

The files in this namespace are only PlistHelper.<c|h>pp, and rapidxml.hpp

d. Examples
These files use the other namespaces to make an actual project.  This project
has a few front end screens, an input configuration screen and not much more.
d.1 ExConfigInput.<c|h>pp
	Keyboard and Joystick configuration screen.
d.2 ExExplosion.<c|h>pp
	Makes an explosion effect - as soon on the Demo Attract screen
d.3 ExGlobals.<c|h>pp
	Lots of global variables such as the high scores, the colours, etc.
d.4 ExHighScreen.<c|h>pp
	The display of the high scores (today's best and all time best).
	The High Score display screen uses a second Render class to cache the
	sprite primitives.  This allows the renderer to run about 1.6x faster in
	a test I did.  Even when not using the cache, the high-score screen still
	ran at several hundred frames per second on 2008 laptop hardware.  However,
	there's an example of how to do that there.
d.5 ExInitials.<c|h>pp
	Reads initials from the user.  Used when the user gets a high-score.
d.6 ExWessels.<c|h>pp
	An effect that shows the Wessels sprite in random locations and colours.
	This is seen on the Title Screen.

3. main.cpp
Currently there's a src/main.cpp that is set up to create a game that ticks
over. A full-blown game probably won't have much more than what's here now as
far as a main and main loop goes.

4. Keys
Pressing 1 starts a 1-player game, and 2 starts a 2-player game.  Since there
is no game, a random high-score is generated and if the score is high enough to
be on a high-score table, there's an opportunity to enter 3 initials.

F1 key will bring up the Input Config screen.  I think the Joystick part is
buggy but I suspect it won't be too tough to fix.  I don't normally have a 
joystick connected to my machine so I haven't taken this very far.

5. Render::Print
The Render::Print function is worth a mention.  It has the following signature:
sf::Vector2f Print(const sf::String& aString, const char* format, ...);

The "aString" is what will be shown and the format is how and where to show
that string.  The following explains the possible format characters:
Format:
a  				| PrintAttr* argument read
c  				| sf::Color* argument read
d<t|m|b> 		| Text Anchor Top, Mid or Bottom
f  				| FontTexture* argument read
g<x|y|*> 		| Goto x (argument read) or y(argument read) or x and y (2
					arguments read)
l  				| Render Layer argument read
p<a<x|y|*>|b|c<x|y|*>|r|t>+<;|\0> 
				| Position text a = around x, y or both; b = bottom; 
					c = centered in x, y or both; r = right; t=top. End with
					';' or and of 	string
r<x|y|*> 		| Goto (as ratio of window) x (argument read) or y (argument
					read) or x and y (2 arguments read)
s  				| Scale argument read
t 				| Text Wrap at window end ON
v				| Viewport - restrict drawing to the rectangle in w (clip)
w				| sf::FloatRect* argument read

An example might best explain:
Print("Hello World", "r*cpax;sdm", 0.5f, 0.2f, &Color::Green, 1.f);

Print Hello World at a ratio of 50% into the screen in x, 20% in Y.  Use the
Color Green.  Position around x (center the Hello world around the 50% where X
is mark) - ";" ends the position sub-string.  Anchor the text in the center. If
the characters have different heights, they will be aligned around there center
in Y, they will not be top aligned (dt) not bottom aligned (db).

The PrintAttr* array contains scaled and colour information.  The array must
have the same number of entries as there are characters in the string and each
character will be printed using the matching information from the PrintAttr
array.

There are many other examples in the code.  Also note that you can give
parameters that compete and don't make sense.  There's not a lot of error
checking so it's really up to the user to "get it right".

6. Render commands
In Render::PrintXY there's an example of how to use "render commands".  These
commands are put into the render queues and affect all the following sprites
until the command is popped off of the render queue.  Using this system, a 
sprite primitive or set of sprite primitives can be moved, rotated or scaled.

It is used in the PrintXY function to do text layout.  A command is put on the
render queue and then the text is printed.  Once the text is also in the queue
the extents are known.  The command sprite is then updated to move or
translate the string to the desired position - for example centered in an axis
or around a point, or to the top or bottom or right justified.  The pop
command is then put on the queue and at render time, the string shows up in
the desired place.  Commands can be stacked (several put onto the queue) and
that works fine.  All they do is affect a Matrix that SFML will, in essence,
apply to the current transformation matrix (CTM).

7. cmake
I am using cmake to create the build files.  I have used cmake to make the
build files for Windows "Visual Studio 14 2015 Win64", OS X ("Unix Makefile" &
"Xcode") and Linux ("Unix Makefile").  The framework compiles and runs on all
of these. I am currently using cmake 3.5.0-rc1.  

The way I have it set up is with the SFML installed near the project for
Windows and OS X but editing the CMakeLists.txt file will be necessary -
especially to set the SFML_ROOT variable.  On Linux, after installing with
"sudo apt-get install libsfml-dev" all worked very well and no editing of the
CMakeLists.txt file should be necessary.

The files in the res/ folder must end up in the same folder as the executable
as that's the only place where Gamework will search for resources.  Because of
this, it's best to set Visual Studio's Working Directory to the following, so
a debug session will find the resources to load:
$(SolutionDir)$(Configuration)\

With cmake installed, the easiest way to use it with Gamework is to go
into the Gamework folder and do the following:
7.a OS X and Linux
	mkdir UnixExe
	cd UnixExe
	cmake -G "Unix Makefiles" ../
	make

7.b Windows
	md WinExe
	cd WinExe
	cmake -G "Visual Studio 14 2015 Win64" ..\
	start Gamework.sln

The folder names (UnixExe or WinExe) are arbitrary.  I make them Linux, Mac
and Windows.

For windows, and not VC++ 2015, then type cmake -G to see what to use for the
correct version of VC++

8. Final Thoughts before a thank you
As I have just finished this framework, I have not made a game using this
framework so there may be bugs and other issues.  The framework is very similar
to the one I created when I made the OS X Defender so I know it works well for
the task.

The framework doesn't have Audio support - I will add that if the SFML support
isn't sufficient.

I had not programmed in C++ in a long time, neither had I used STL before this
so there may be some odd things afoot in this code, but it all seems okay.

9. Thank you
SFML is an absolutely awesome library that I found to be very well thought out
and incredibly easy to use.  I must confess that I didn't work through samples
or even read the Docs before I started Gamework.  If I had, I may have done
things differently.  I made Gamework for the joy of making something, no other
reason and I had a lot of fun doing this and none of this would have happened
had it not been for SFML.  Thanks for making it.

I also found rapidxml.hpp really easy to use and I am again very thankful that
people will, in essence, "give away" such high quality software.

Thank you
Stefan Wessels
2 March 2016
