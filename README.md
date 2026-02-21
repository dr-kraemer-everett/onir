# onir
onir: manage dials, switches, and servos over I2C (uses Arduino's Wire library)

# license

The name 'onir' may sound dreamy, but it's also short for "oh, nothing I'd recommend." I hope this software will be ready to use outside my lab someday. But for now I wouldn't advise putting this near your hardware.

# updates

2026-02-21:

Add IODevice and Control/Client, with demo sketches (remote device dial & screen).

(Ditching the ordinal commit labels since git doesn't number things that way.)

This year's least impressive bench demo: a dial and screen needing not just one board to run them, but now two! One side is running a Control looking for devices. If it finds one on channel 9, it reads the device's dial state, and writes the dial's value to the screen.

C++ makes this just awkward enough to be annoying. I've committed my share of semicolons (if mostly long ago) and still much of what I spend time thinking about isn't what I want the program to do, but how to make the type system accept it without littering the code with the kind of copy this to that / that back to this 'helper' functions I've a saddening fraction of production code comprise. (sometimes running on big metal, at least by the standards of the time.)

The bot (link updated) was of limited help here. The fact that it's a pattern-matching system works against it here. It found way the code is fairly simple at most levels perplexing, despite its frequent hectoring about clean layer boundaries.

It particularly kept trying to push the Wire code further up- and down-stack and through more of the logic. As currently written the client code only needs to know that there's something called a Wire that it can call with a particular phone number and either give or take a message. The device code doesn't know anything at all about I2C -- the sketch just directly tinkers with its public state member. I find the way it seemed to think the *lack* of confusing I2C code in the files was something requiring a lot of alarmed-sounding explanatory comments written to... someone? (My students aren't gonna ask why the telephone set isn't out on the desk.)

It was, however, fairly useful for writing pretty-print functions and other such utilities. And I half-expect that it will spot the pointer error crashing the demo immediately -- it's fairly good with that kind of thing.


2026-02-20:

11 & 12: Get rid of [(int)PinFunction::END] clutter (It's meaningless to students.)

I added a 'using' declaration to turn a bunch of visual clutter into a word, except where C++ made that difficult for me. (https://chatgpt.com/share/6998ac1d-c3e8-8012-9a14-e79c23442880)

9: Remove CharDisplay/CharDevice and associated demos; organize demos. 

8: Add a class called Display that splits a string across a handful of screens and keeps them refreshed.

I got the bot to write this yesterday from a description and some iteration. It seemed like the type of problem it should be best at: empty page, not much to integrate with upstream and nothing downstream. As usual it started with the omni-loop style. But it only took some gentle, non-directive nudging to get it to break out the most egregious repeated chunk, the logic to find the right letter to show.

Initially the core logic was maybe 1/3 edge-checks and guards against bad inputs. Sometimes if you simplify your expectations about behavor, a lot of that logic falls away like a dry chrysalis, and you can just say what you need to say. In this case, stipulating that the offset would loop around the message circularly had this effect: lots of vanishing guard conditions, higher signal:noise for students trying to figure out what's going on.

But modulo the usual difficulties with the robot refusing to read the code and stubbornly making up methods (honestly I've gone through worse with junior hires) the code worked almost out of the box. First win on my books for team robot overlords.

(https://chatgpt.com/share/699853fb-a1b8-8003-9d56-4e841b3d149c)


2026-02-19:

seventh commit: Duplicate ChannelSelector into Selector, with a script that uses it to select a channel for Screen.

Not much to say here. Did this by hand, gonna see if the machine can do anything useful with the next part.

The next obvious step would be to update the multi-display demo to use the Screen class, but I don't want to do that. There's a bunch of logic dealing with more than one screen in there that belongs at the library level. Then I'll make a new multiscreen demo and delete most of the existing examples.

Have to think a minute on the design. I'll probably let the robot take a crack at it -- it loves that kind of high level hand-wavy stuff. We'll see how the ideas are.

(no new link, updated the thread.)

sixth commit: Miscellaneous readability / cleanup / calligraphy (yes).

This was also around the time I tried again to stuff the Wire handler that lives in the device script into the class as a static method, and once again figured out there's no good way to do that without covering the entire class in static declarations and initializations that are meaningless noise to my students.

Instead, I have followed the example of Aesop's fox, and decided that the presence of this alarming code ("readBytes((byte*)&device.state"? looks bold.) at the top level of the script is not a bug, but a feature.

After all, the prospect of being in the position of an I2C device would be alarming! Imagine being given a phone and told you have to pick up whenever it rings, then and believe anything you are told. (*That* is how the device do...)

The alert reader will note I said no *good* way of smuggling information into the handler. Like last year when I wrote the initial version of this code, I did, however, think of a *bad* way. Last year I had to talk myself off of this particular ledge.

But this year I could get robotic assistance with my problem, and say to a bot, "hey, did'ja ever notice that the parameter with the message size is the same as the Uno's address table? because I sure as heck noticed that." Then I got to hear a big pile of algebra play back a vague, half-coherent echoing chorus chanting dire tones laying out the reasons this would not be the right path to go down. (https://chatgpt.com/share/699755fb-e7fc-8003-9667-4ed68952f0f1)

I found this to be a reassuring experience overall, since if you read though most of my chats with the bot you'll see it validating my various choices to a degree I find kind of alarming. But it turns out this mule can balk when the path ahead is nothing but rocks to the horizon.

2026-02-18:

fifth commit: Add screen and screen_device, meant as a replacement for char_display. I find it amusing that my AI coding assistant, faced with the task of adapting this code, more or less told me, "Looks like you got this one, I'll just watch from here."

in all fairness, there was plenty of cruft I could remove and still have the minimal local screen reading "onir" and the I2C client/device pair blinking the colon. I probably would have made the bot port most of it over, but if I had to chip it out with my parentheses by hand I saw the upside of letting it go.

Next step is to swap the key demos over to use screen (and drop the non-key demos), and delete the char_thing code.

(updated the chat thread on the server side. no new link.)

third/fourth commit: (dropped the initial one that was just the creation of the repo without the files.) I used ChatGPT (the 'Plus' version, from my personal account) to split up the dial code into a client and device side, and got the demos working again.

I'd describe the result as mixed. I could've written the code in as much time as it took to produce with the model. Then I would've spent those hours writing C code instead of relitigating with a robot all of the semantic and procedural decisions I previously made with readability as my main (or only!) constraint. As the model put it, "The assistant frequently tried to generalize or conventionalize the structure. The final version is better because those changes were rejected."

But the end result wasn't that different from plenty of other code collaboration/review processes I've been part of. Maybe I'd rather be reviewing and mutating autogenerated code into saying what I want than writing it from scratch using letters and parentheses? I've written lots of C, and I'm sure I'll write more soon. But arguing with robots is pretty new. And I'm hoping that having hashed through these points we can pick up steam with the next part.

(https://chatgpt.com/share/6995fae0-6750-8003-951d-a63edae4d822)

second commit: Remove stray root directory.
(technical assistance: https://chatgpt.com/share/6995e7e2-22b4-8003-9260-1ff04d0b619f)

2026-02-17:
first commit of onir, a simple I2C library for dials, switches, and (eventually) servos. I hope it will help my students practice using some components that are greatly abundant in ko lab.

working sketches; char_channel, multi_char_client, char_device, local_char_device, simple_char_client, local_dial_device, test_char_client

I'm uploading onir to github because there's enough of it that it seems worth stashing here, and because I'm going to ask some coding AIs to handle the TODO material in dial_encoder.h to see how they manage the task.

I haven't successfully used AI for programming so far. I tried giving one of my old graph-traversal questions to ChatGPT shortly after its public release and was impressed at how readily it spat out what looked like working Python and C++ solutions. But it seemed to get lost when I tried it with anything bigger, and to have very different instincts than me when it came to how to break things up. And I didn't have any programs to write that I couldn't quickly bang out by hand anyway, so I deferred.

But I keep hearing that recent improvements have been impressive. Here I have a chunk of code that should be enough to generalize from. And I have other things to get done this week. So I thought I'd give it another shot.


