Liquinth VST (c)2007 mumart@gmail.com

A relatively simple Java-based polysynth, using jVSTwRapper.
Java 6 or later is required.

To use the synth with other VST hosts, copy the following
files into your plugins directory:

liquinthvst-a30.jar
jVSTwRapper.ini
jVSTwRapper.dll
jVSTwRapper_bin.jar
jVSTsYstem_bin.jar

In the unlikely event you already have a plugin that uses jVSTwRapper,
you will need to edit the ini file to include support for both.

History:

a28:
First version to use jVSTwRapper

a29:
Added initial Swing GUI.
All sliders are mapped to MIDI controllers 20 onwards.

a30:
Fixed so that the synth can be used with or without GUI.
